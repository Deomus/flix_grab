#include "web_content.h"

#include <utils/qs_utils.h>
#include <utils/zlib_utils.h>


WebContent::WebContent(QObject* parent /*= nullptr*/) : QObject(parent) {
    initModification();
}

WebContent::~WebContent() {
    qDebug() << "~WebContent";
    abort();
	waitForFinished();
}

void WebContent::fetch(const QUrl& url) {
    valid_ = (url_ == url);
    url_ = url;
    urs_str_ = url.toString();
    QMetaObject::invokeMethod(this, "fetchInternal", Qt::QueuedConnection);
}

void WebContent::post(QUrl const& url, const QByteArray& post_data, const char* content_type, int method) {
    method_ = method; // 1 = post, 2 = put
    valid_ = (url_ == url);
    url_ = url;
    urs_str_ = url.toString();
    post_data_ = post_data;
    content_type_ = content_type;
    QMetaObject::invokeMethod(this, "fetchInternal", Qt::QueuedConnection);
}

Status WebContent::modifyContent() {
    Status status;
    return status;
}

bool WebContent::isModifyAsync() const {
    return false;
}

void WebContent::waitForFinished() {
	if (watcher_.isRunning()) {
		watcher_.waitForFinished();
	}
}

Q_INVOKABLE void WebContent::fetchInternal() {
    if (!isValid()) {
        qDebug() << "fetchInternal " << (post_data_.isEmpty() ? "GET " : std::string("POST ") + std::to_string(post_data_.size()) + " bytes ").c_str() << url_;

        if (!nam_)
            nam_ = new QNetworkAccessManager(this);

        QNetworkRequest         request(url_);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);

        foreach (const auto& h, custom_headers_)
            request.setRawHeader(h.first.toLocal8Bit(), h.second.toLocal8Bit());

        fetching_ = true;

        if (method_ == 0) {// get
            reply_ = nam_->get(request);
        }
        else { // post, put
            if (content_type_.isEmpty())
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            else
                request.setHeader(QNetworkRequest::ContentTypeHeader, content_type_);

            if (method_ == 2)
                reply_ = nam_->put(request, post_data_);
            else
                reply_ = nam_->post(request, post_data_);

            post_data_.clear();
        }
        
        QObject::connect(reply_, &QNetworkReply::finished, this, [=]() {
            data_.clear();
            fetching_ = false;
            if (aborted_) {
                valid_ = status_ = { EStatusCode::Aborted, "" };
                emit finished();
            }
            else if (reply_->error() == QNetworkReply::NoError) {
                unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
                qDebug() << "Content '" << url_ << "' downloaded size: " << reply_->size() << "' ms: " << ms;
                raw_data_ = reply_->readAll();
//#ifdef _DEBUG
                //QFile   file("raw_data_" + QString::number(ms) + ".js");
                //if (file.open(QIODevice::ReadWrite)) {
                //    file.write(raw_data_);
                //    file.close();
                //}
//#endif    
                bool unpacking_required = false;
                foreach (auto const&h, custom_headers_) {
                    if (h.second.contains("gzip")) {
                        unpacking_required = true;
                        break;
                    }
                }

                if (unpacking_required) {
                    std::wstring data;
                    if (zlib::decompressData(raw_data_.data(), raw_data_.size(), data) == 0)
                        data_ = WS2Q(data);
                }

                if (data_.isEmpty()
                    && !raw_data_.isEmpty())
                    data_ = raw_data_;

                // extract cookies from Set-Cookie:
                QVariant cookieVar = reply_->header(QNetworkRequest::SetCookieHeader);
                if (cookieVar.isValid()) {
                    QList<QNetworkCookie> cookies = cookieVar.value<QList<QNetworkCookie> >();
                    foreach(QNetworkCookie cookie, cookies) {
                        qDebug() << "cookie set:" << cookie.name() << "=" << cookie.value() << "; " << cookie.domain();
                        nam_->cookieJar()->insertCookie(cookie);
                    }
                }

                runModification();
            }
            else {
                qDebug() << "Content '" << url_ << "' reply_ error: " << reply_->errorString();
                valid_ = status_ = { EStatusCode::NetError, reply_->errorString() + ", " + reply_->url().toEncoded() };
                emit finished();
            }
            reply_->deleteLater();
        }, Qt::QueuedConnection);

    }
    else
        emit finished();
}

void WebContent::initModification() {
    QObject::connect(&watcher_, &QFutureWatcher<Status>::finished, this, [this]() {
        valid_ = status_ = watcher_.result();

        emit finished();
    }, Qt::QueuedConnection);
}

void WebContent::runModification() {
    if (isModifyAsync()) {
        QFuture<Status> future = QtConcurrent::run(this, &WebContent::modifyContent);
        watcher_.setFuture(future);
    }
    else {
        valid_ = status_ = modifyContent();
        emit finished();
    }
}

void WebContent::add_header(const QString& name, const QString& val) {
    custom_headers_[name] = val;
}