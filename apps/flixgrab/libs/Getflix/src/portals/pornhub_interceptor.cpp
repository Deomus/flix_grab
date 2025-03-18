#include "pornhub_interceptor.h"

#include <iostream>

#include "get_utils.h"


PornhubInterceptor::PornhubInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p) 
    : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {

    Q_INIT_RESOURCE(scripts);
}

PornhubInterceptor::~PornhubInterceptor() {
    qDebug() << "Interceptor: ~PornhubInterceptor";
}

void PornhubInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    info.setHttpHeader("User-Agent", USER_AGENT);

    qDebug() << "####### Request url: " << url.fileName() << " " << QString::fromLatin1(info.requestMethod()) << " " << qstr << "signed_in_: " << signed_in_;
    
    if (signed_in_) {
        if (qstr.contains("master.m3u8", Qt::CaseInsensitive)
            && !qstr.contains("etahub.com", Qt::CaseInsensitive)) {
            //Found Video Description Url;
            qDebug() << "Metadata intercepted: " << qstr;
            QMetaObject::invokeMethod(this, "requestMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }

        if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
            QMetaObject::invokeMethod(this, "requestHTML", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        }
        else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript) {
            //Block all logging activity, to hide downloading process
            if (qstr.contains("ads.trafficjunky", Qt::CaseInsensitive)
                || qstr.contains("ads_test", Qt::CaseInsensitive)
                || qstr.contains("analytics", Qt::CaseInsensitive)
                || qstr.contains("googletagmanager", Qt::CaseInsensitive)) {
                info.block(true);
            }
            else if (qstr.contains("loader.min.js", Qt::CaseInsensitive)
                && !url.isLocalFile()) {
                qDebug() << "Interceptor: Core Script " << QFileInfo(qstr).fileName();

                QMetaObject::invokeMethod(this, "interceptLoaderScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
                info.block(true); //Block loading of original script;
            }
            else {
                qDebug() << "Interceptor: Script passed: " << qstr;
            }
        }
        else {
            //info.block(true);
        }
    }
}

Q_INVOKABLE void PornhubInterceptor::interceptLoaderScript(const QUrl& url) {
    //Q_UNUSED(url);
    //webloader_script_->fetch(QUrl("qrc:///scripts/pornhub/loader.js"));
    webloader_script_->fetch(url);
    scripts_.addAsync(webloader_script_);
    scripts_.run(page_);
}

void PornhubInterceptor::requestMetadata(const QUrl& url) {
    metadata_->fetch(url);
}

void PornhubInterceptor::requestHTML(const QUrl& url) {
    html_->fetch(url);
}

Status PornhubInterceptor::parseRedirectedHTML(const QString& html, VideoInfo& video_info) {
    Status status;

    QRegExp expression = QRegExp("og:title\" content=\"([^\"]+)[\"]");
    expression.setMinimal(true);

    QString title;
    //get title
    if (expression.indexIn(html) != -1)
        title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();

    //get title from twitter
    if (title.isEmpty()) {
        expression = QRegExp("twitter:title\" content=\"([^\"]+)[\"]");
        if (expression.indexIn(html) != -1)
            title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();
    }

    //get title from <title>
    if (title.isEmpty()) {
        expression = QRegExp("<title>(.*)</title>");
        if (expression.indexIn(html) != -1) {
            title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();
            title = title.split("\"").first();
            title.remove(" - Pornhub.com");
        }
    }

    //obtain basic info from web page
    if (!title.isEmpty()) {
        video_info.video_title = normalizeTitle(title);

        //get thumbnail url
        expression = QRegExp("og:image\" content=\"([^\"]+)[\"]");

        if (expression.indexIn(html) != -1)
            video_info.artwork_url = expression.cap(1);

        //embed url case
        if (video_info.artwork_url.isEmpty()) {
            expression = QRegExp("image_url\":\"([^\"]+)[\"]");
            if (expression.indexIn(html) != -1)
                video_info.artwork_url = expression.cap(1).replace("\\/", "/");
        }

        //get duration in seconds
        expression = QRegExp("video:duration\" content=\"([^\"]+)[\"]");
        if (expression.indexIn(html) != -1) {
            QString duration = expression.cap(1);
            video_info.duration = duration.toInt();
        }
        else {
            expression = QRegExp("video_duration\":\"([^\"]+)[\"]");
            if (expression.indexIn(html) != -1) {
                QString duration = expression.cap(1);
                video_info.duration = duration.toInt();
            }
        }
    }
    else {
        status.update(EStatusCode::ParseError, "Could not retrieve video title");
    }
    return status;
}

void PornhubInterceptor::initialize() {
    qDebug() << "PornhubInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };

    webloader_script_ = new WebContent(this);
    initContent(webloader_script_);
#ifdef _DEBUG
//    QObject::connect(webloader_script_, &WebContent::finished, this, [=]() {
//        QFile   file("Loader.js");
//        if (file.open(QIODevice::ReadWrite)) {
//            file.resize(0);
//            QTextStream(&file) << webloader_script_->data().toLocal8Bit() << endl;
//            file.close();
//        }
//    }, Qt::QueuedConnection);
#endif

    metadata_ = new WebContent(this);
    initContent(metadata_);
    QObject::connect(metadata_, &WebContent::finished, this, [=]() {
        const auto& content = metadata_->data().toUtf8();

        if (content.isEmpty()
            || !content.contains("#EXTM3U")) {
            //emit error(Status::VideoUnavailable);
            return;
        }

#ifdef _DEBUG
        QFile   file("pornhub-metadata.m3u8");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(content);
            file.close();
        }
#endif
        parseMetadata(content);

    }, Qt::QueuedConnection);


    html_ = new WebContent(this);
    initContent(html_);
    QObject::connect(html_, &WebContent::finished, this, [=]() {
        const auto& content = html_->data().toUtf8();
        
        if (content.isEmpty()) {
            //emit error(Status::VideoUnavailable);
            return;
        }

#ifdef _DEBUG
        QFile   file("pornhub.html");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(content);
            file.close();
        }
#endif
        parseHTML(content);

    }, Qt::QueuedConnection);
}

void PornhubInterceptor::parseMetadata(const QString& content) {
    QRegExp stream1Re("^#EXT-X-STREAM-INF:.*$");
    QRegExp stream2Re("^#EXTINF:.*$");
    QRegExp bandWidhtRe("BANDWIDTH=(\\d+),");
    QRegExp widthRe("RESOLUTION=(\\d+)x");
    QRegExp heightRe("x(\\d+),");
    QRegExp framerateRe("RATE=(.*),");

    QStringList stringList = QString(content).split("\n");
    bool urlString = false;
    for (const auto& str : stringList) {
        if (stream1Re.exactMatch(str)
            || stream2Re.exactMatch(str)) {
            tracks_info_.video_tracks.push_back(VideoTrackInfo());
            tracks_info_.video_tracks.back().size = 10000;
            tracks_info_.video_tracks.back().is_chunked = true;

            int pos = bandWidhtRe.indexIn(str);
            if (pos > -1)
                tracks_info_.video_tracks.back().bitrate = bandWidhtRe.cap(1).toInt();

            pos = widthRe.indexIn(str);
            if (pos > -1)
                tracks_info_.video_tracks.back().width = widthRe.cap(1).toInt();

            pos = heightRe.indexIn(str);
            if (pos > -1)
                tracks_info_.video_tracks.back().height = heightRe.cap(1).toInt();

            pos = framerateRe.indexIn(str);
            if (pos > -1) {
                QString framerate = framerateRe.cap(1).split(",").first().split("=").last();
                if (framerate.contains("."))
                    framerate = framerate.split(".").first();

                tracks_info_.video_tracks.back().framerate = framerate.toInt();
            }

            normalizeTrack(tracks_info_.video_tracks.back());
            normalizeVideoTrack(tracks_info_.video_tracks.back());

            urlString = true;
            continue;
        }
        if (urlString) {
            QString url_base = metadata_->url().toEncoded();
            url_base = url_base.split("master.m3u8").first();
            QString m3u8_url = url_base + str;
            
            if (!tracks_info_.video_tracks.isEmpty()) {
                tracks_count_++;
                QMetaObject::invokeMethod(this, "requestM3U8", Qt::QueuedConnection, Q_ARG(const QUrl&, m3u8_url));
            }
            
            urlString = false;
        }
    }
}

void PornhubInterceptor::requestM3U8(const QUrl& url) {
    QPointer<WebContent> m3u8_data = new WebContent(this);
    initRequest(m3u8_data);
    qDebug() << "m3u8_url: " << url;

    QObject::connect(m3u8_data, &WebContent::finished, this, [=]() {
        const QString& data = m3u8_data->data();

        if (!data.isEmpty() && data.contains("#EXTM3U")) {
#ifdef _DEBUG
            QFile   file("data_" + QString::number(i) + ".m3u8");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(data.toUtf8());
                file.close();
            }
            i++;
#endif
            if (!tracks_info_.video_tracks.isEmpty()) {
                QString url_base = metadata_->url().toEncoded();
                url_base = url_base.split("master.m3u8").first();
                QStringList stringList = data.split("\n");
                QRegExp expression = QRegExp("-f([^-]+)[-]", Qt::CaseInsensitive);
                foreach(QString str, stringList) {
                    if (str.contains(".ts")) {
                        if (expression.indexIn(str) != -1) {
                            int number = expression.cap(1).toInt() - 1;
                            if (number >= 0)
                                tracks_info_.video_tracks[number].urls.push_back(url_base + str);
                        }
                    }
                }
            }
        }

        if (--tracks_count_ <= 0) {
            QMutableListIterator<VideoTrackInfo> it(tracks_info_.video_tracks);
            while (it.hasNext()) {
                if (it.next().urls.isEmpty())
                    it.remove();
            }

            qDebug() << "Parsed tracks: videos " << tracks_info_.video_tracks.size() << " audios " << tracks_info_.audio_tracks.size() << " images " << tracks_info_.image_tracks.size();
            
            if (!tracks_info_.video_tracks.isEmpty())
                setFlag(kTracksReady);
        }
    });

    m3u8_data->fetch(url);
}

void PornhubInterceptor::parseHTML(const QString& content) {
    Status status;

    QString html = content;

    if (html.contains("video is private")) {
        status.update(EStatusCode::PrivateVideo);
    }
    else if (html.contains("\"geoBlocked\"")) {
        status.update(EStatusCode::VideoBlockedInLocation);
    }
    else if (html.contains("premium/login")) {
        if (html.contains("pornhub.com/view_video")) {
            QString redirect_url;
            QRegExp expression = QRegExp("premiumUrl\":\"([^\"]+)[\"]");
            expression.setMinimal(true);

            //get redirect when premiumUrl exists
            if (expression.indexIn(html) != -1) {
                redirect_url = expression.cap(1);
                redirect_url.replace("\\/", "/");
            }
            //get redirect when trying get premiumUrl from nonpremium (premiumUrl doest exists)
            if (redirect_url.isEmpty()) {
                expression = QRegExp("canonical\" href=\"([^\"]+)[\"]");
                if (expression.indexIn(content) != -1) {
                    redirect_url = expression.cap(1);
                    redirect_url.replace("pornhub.com", "pornhubpremium.com");
                }
            }
            if (!redirect_url.isEmpty()) {
                QString data;
                if (downloadToString(redirect_url, data) && !data.isEmpty())
                    html = data;
            }
            else {
                status.update(EStatusCode::ParseError, "Could not retrieve redirect url");
            }
        }
        else {
            status.update(EStatusCode::ParseError);  //"login required"
        }
    }

    if (status)
        status = parseRedirectedHTML(html, video_info_);

    qDebug() << "video_info_.video_title: " << video_info_.video_title;

    if (status) {
        if (!video_info_.video_title.isEmpty())
            setFlag(kVideoInfoReady_);
    }
    else {
        emit error(status);
    }
}


void PornhubInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}

void PornhubInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    qDebug() << "start page ...";
    invalidate();
    tracks_count_ = 0;
    is_tracks_ready_ = false;
    is_video_info_ready_ = false;
    metadata_->reset();
    html_->reset();
    webloader_script_->reset();
}

void PornhubInterceptor::setFlag(EInterceptFlags flag) {
    if (!is_video_info_ready_)
        is_video_info_ready_ = checkInterceptFlag(flag, kVideoInfoReady_);
    if (!is_tracks_ready_)
        is_tracks_ready_ = checkInterceptFlag(flag, kTracksReady);
    
    if (is_tracks_ready_
        && is_video_info_ready_) {
        emit metadataIntercepted(this->url());
        invalidate();
    }
}
