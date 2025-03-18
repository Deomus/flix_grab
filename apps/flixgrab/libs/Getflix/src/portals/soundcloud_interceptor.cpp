#include "soundcloud_interceptor.h"

#include "get_utils.h"


SoundcloudInterceptor::SoundcloudInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , nam_(nam) {
    qDebug() << "Interceptor";
}

SoundcloudInterceptor::~SoundcloudInterceptor() {
    qDebug() << "Interceptor: ~SoundcloudInterceptor";
}

void SoundcloudInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    qDebug() << "-> " << QString::fromLatin1(info.requestMethod()) << ": " << qstr;
    info.setHttpHeader("User-Agent", USER_AGENT);

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
        qDebug() << "Interceptor: interceptMetadata url:" << info.requestUrl().toEncoded();
        QMetaObject::invokeMethod(this, "interceptHTML", Qt::QueuedConnection, Q_ARG(const QUrl&, info.requestUrl()));
        //info.block(true);
    }

    if (qstr.contains("playlist.m3u8")) {
        qDebug() << "Interceptor: interceptM3U8data url:" << info.requestUrl().toEncoded();
        QMetaObject::invokeMethod(this, "interceptM3U8", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        //info.block(true);
    }
}

void SoundcloudInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    scripts_.clear();
    reset();
    tracks_info_.audio_tracks.clear();
    tracks_info_.all_audio_tracks.clear();
    tracks_info_.audio_languages.clear();
    tracks_info_.all_audio_languages.clear();
    tracks_info_.video_tracks.clear();
    tracks_info_.image_tracks.clear();
    tracks_info_.subtitle_tracks.clear();
    tracks_info_.req_decrypt = false;
    video_info_ = {};
    tracks_info_ = {};

    html_->reset();
    m3u8_data_->reset();
    deleteCatalogCache();
}

void SoundcloudInterceptor::deleteCatalogCache() {
    qDebug() << "--> ParserBase - deleteCatalogCache!";

    const static QString local = "/Local/";
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    int index = path.lastIndexOf(local);
    if (index > -1)
    {
        QString post = path.right(path.size() - index);
        path = path.left(index + local.size());

        post = post.right(post.size() - local.size());
        int index = post.indexOf('/');

        if (index > -1)
            post = post.left(index);

        QDir dir(path);
        if (dir.exists()) {
            dir.setPath(path + post);
            dir.removeRecursively();
        }
    }
}


void SoundcloudInterceptor::initialize() {
    qDebug() << "SoundcloudInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
            });
    };

    m3u8_data_ = new WebContent(this);
    initContent(m3u8_data_);
    QObject::connect(m3u8_data_, &WebContent::finished, this, [=]() {
        parseM3U8(m3u8_data_->data().toLocal8Bit());
        
        //QJsonDocument jsonResponse = QJsonDocument::fromJson(m3u8_data_->data().toLocal8Bit());
        //QJsonObject jsonObject = jsonResponse.object();
        //QString url = jsonObject["url"].toString();

        //QString data;
        //if (downloadToString(url, data) 
        //    && !data.isEmpty())
        //    parseM3U8(data);

    }, Qt::QueuedConnection);

    html_ = new WebContent(this);
    initContent(html_);
    QObject::connect(html_, &WebContent::finished, this, [=]() {
        parseHTML(html_->data().toLocal8Bit());
    }, Qt::QueuedConnection);
}

void SoundcloudInterceptor::parseM3U8(const QString& m3u8_data) {
    QList<QString> urls;
    auto data = m3u8_data.split("\n");
    if (!data.isEmpty()) {
        for (QList<QString>::iterator it = data.begin(); it != data.end(); it++) {
            if (!it->startsWith("#")
                && !it->isEmpty()) {
                urls << *it;
            }
        }
    }

    if (!urls.isEmpty()) {
        AudioTrackInfo audio_track;
        //audio_track.bitrate = bitrate;
        //audio_track.channels = channels;
        //audio_track.size = size;
        audio_track.urls << urls;
        audio_track.is_chunked = true;
        audio_track.profile = AudioProfile::MP3;

        normalizeTrack(audio_track);
        normalizeAudioTrack(audio_track);
        tracks_info_.audio_tracks.append(audio_track);
    }
    setFlag(kTracksReady);
}

void SoundcloudInterceptor::parseHTML(const QString& html) {
    //get title
    QString title;
    QRegExp expression = QRegExp("\"og:title\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);  
    if (expression.indexIn(html) != -1)
        title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();
    if (title.isEmpty()) {
        expression = QRegExp("<title>(.+)[</title>]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1)
            title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText().split(" | ").first();
    }

    if (!title.isEmpty()) {
        video_info_.video_title = normalizeTitle(title);
        video_info_.enable_metadata = true;
        //get duration
        expression = QRegExp("\"duration\":([^,]+)[,]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1)
            video_info_.duration = expression.cap(1).toInt() / 1000;
        //get artwork
        expression = QRegExp("\"og:image\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1)
            video_info_.artwork_url = expression.cap(1);
        if (video_info_.artwork_url.isEmpty()) {
            expression = QRegExp("\"twitter:image\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
            if (expression.indexIn(html) != -1)
                video_info_.artwork_url = expression.cap(1);
        }
        video_info_.metadata_artwork_url = video_info_.artwork_url;
        setFlag(kVideoInfoReady);
    }
}

void SoundcloudInterceptor::setFlag(EInterceptFlags flag) {
    if (!is_tracks_ready_)
        is_tracks_ready_ = checkInterceptFlag(flag, kTracksReady);
    if (!is_video_info_ready_)
        is_video_info_ready_ = checkInterceptFlag(flag, kVideoInfoReady);
    if (is_tracks_ready_ && is_video_info_ready_)
        emit metadataIntercepted(this->url());
}
