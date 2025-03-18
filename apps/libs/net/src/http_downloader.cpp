#include <net/http_downloader.h>
#include <QNetworkRequest>

using namespace net;
#define MAX_REDIRECTION                 (16)

HttpDownloader::HttpDownloader(QString const& path, QObject* parent)
    : QObject(parent),
    file_(path) {
    request_.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request_.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request_.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
    request_.setMaximumRedirectsAllowed(MAX_REDIRECTION);
    request_.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
}

HttpDownloader::~HttpDownloader() {
    if (reply_)
        reply_->deleteLater();
}

int HttpDownloader::httpStatus() const {
    if (reply_)
        return reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    return 0;
}

QString HttpDownloader::errorString() const {
    if (reply_)
        return reply_->errorString();

    return QString();
}

bool HttpDownloader::isOk() const {
    return !reply_ || reply_->error() == QNetworkReply::NoError;
}

bool HttpDownloader::isDownloading() const {
    return reply_ && file_.isOpen() && reply_->isRunning();
}

bool HttpDownloader::isFinished() const {
    return reply_ && reply_->isFinished();
}


void HttpDownloader::download(  QUrl const& url, 
                            FileContent content /*= Continue*/, 
                            const QByteArray& post_data /*= NULL*/) {
    if (!isDownloading()) {
        request_.setUrl(url);
        qDebug() << "Checking URL=" << url.toString();

        QIODevice::OpenMode mode = content == Truncate ?
            QIODevice::ReadWrite | QIODevice::Truncate : QIODevice::ReadWrite | QIODevice::Append;

        if (file_.open(mode)) {
            file_start_pos_ = content == Append ? file_.size() : 0;
            progress_ = content != Append ? file_.size() : 0;
            post_data_ = post_data;
            //connectReply(nam_.head(request_));
            continueDownload();
        }
    }
}

void HttpDownloader::pause() {
    qDebug() << "pause() = " << progress();
    if (reply_) {
        reply_->disconnect();
        reply_->abort();
        //    m_pFile->write( m_pReply->readAll());
        file_.flush();

        progress_ = progress();
        current_progress_ = 0;
    }
}

void HttpDownloader::resume() {
    qDebug() << "resume() = " << progress();
    continueDownload();
}

qint64 HttpDownloader::progress() const {
    return current_progress_ + progress_;
}

qint64 HttpDownloader::size() const {
    return current_size_ >= 0 ? current_size_ + progress_ : current_size_;
}

qint64 HttpDownloader::chunkSize() const {
    return chunk_size_;
}

void HttpDownloader::setChunkSize(qint64 size /*= 0*/) {
    chunk_size_ = size;
}

void HttpDownloader::dataProgress(qint64 received, qint64 total) {
    if (!file_.isOpen())
        return;

    if (total != 0) {

        if (current_progress_ == 0) {
            if (!reply_->hasRawHeader("Accept-Ranges") ||
                !QString(reply_->rawHeader("Accept-Ranges")).contains("bytes", Qt::CaseInsensitive)) {
                //Ranges not Accepted;
                file_.seek(file_start_pos_);
                current_size_ = total;
            }
            else {
                QString content_range = reply_->rawHeader("Content-Range");
                bool ok = false;
                qint64 length = content_range.mid(content_range.lastIndexOf('/') + 1).toLongLong(&ok);
                current_size_ = ok ? length : -1;
                qDebug() << "Content-Range " << content_range;
            }

            //size_ = reply_->header(QNetworkRequest::ContentLengthHeader).toLongLong();
            qDebug() << "Content Length: " << reply_->header(QNetworkRequest::ContentLengthHeader).toLongLong() << " Content range: " << current_size_;
        }

        //Update current progress on chunk/download
        current_progress_ = received;
        //current_progress_ += reply_->bytesAvailable();

        if (timer_.elapsed() > 5)
            speed_ = reply_->bytesAvailable() * 1000 / timer_.elapsed();

        timer_.restart();

        qDebug() << "Download Progress: Received=" << progress_ + current_progress_ << ": Total=" << progress_ + total << ": Write bytes=" << reply_->bytesAvailable() << ": Speed=" << speed_ / 1024.0 << "Kb/s";

        file_.write(reply_->readAll());
        emit progress(progress(), size());
    }
}

void HttpDownloader::handleFinishedReply()
{
    int http_status = reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qint64 size = reply_->header(QNetworkRequest::ContentLengthHeader).toLongLong();

    qDebug() << "Http Status: " << http_status;

    if (http_status < 200 ||
        http_status > 206 ||
        reply_->error() != QNetworkReply::NoError) {
        qDebug() << "Network error:" << reply_->errorString();
        emit error();
    }

    if (http_status == 206 &&
        requested_size_ &&
        requested_size_ == size)
        continueDownload();
    else {
        file_.close();
        emit finished();
        qDebug() << "Download Finished!";
    }
}

void HttpDownloader::continueDownload() {
    //Check If We Can Continue Download;
    progress_ = progress();
    requested_size_ = current_progress_ = 0;
    qint64 target_bytes = chunk_size_ > 0 ? progress_ + chunk_size_ : 0;

    QString range = QString("bytes=%1-").arg(progress_);

    if (target_bytes > 0) {
        range += QString::number(target_bytes);
        requested_size_ = target_bytes + 1 - progress_;
    }

    qDebug() << "Range: " << range;
    //Setup range header;
    request_.setRawHeader("Range", range.toLatin1());

    if (reply_)
        reply_->deleteLater();

    //Continuing after pausing
    if (post_data_.isEmpty()) {
        request_.setHeader(QNetworkRequest::ContentTypeHeader, QVariant());
        reply_ = nam_.get(request_);
    }
    else {
        request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        reply_ = nam_.post(request_, post_data_);
        post_data_.clear();
    }

    QObject::connect(reply_, &QNetworkReply::finished, this, &HttpDownloader::handleFinishedReply);
    QObject::connect(reply_, &QNetworkReply::downloadProgress, this, &HttpDownloader::dataProgress);

    timer_.restart();
}

