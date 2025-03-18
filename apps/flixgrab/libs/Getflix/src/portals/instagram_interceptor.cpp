#include "instagram_interceptor.h"
#include "get_utils.h"
#include <iostream>


InstagramInterceptor::InstagramInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p) : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {
}

InstagramInterceptor::~InstagramInterceptor() {
    qDebug() << "Interceptor: ~InstagramInterceptor";
}

void InstagramInterceptor::getToken() {
    if (csrf_token_.isEmpty()) {
        auto cookies = nam_->cookieJar()->cookiesForUrl(base_url_);
        foreach(const auto & c, cookies) {
            if (c.name() == "csrftoken") {
                csrf_token_ = c.value();
                break;
            }
        }
    }
}

void InstagramInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {
    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());
    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;

    if (!signed_in_)
        return;

    info.setHttpHeader("User-Agent", USER_AGENT);

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame
        && media_url_.isEmpty()) {

        base_url_ = qstr;
        getToken();

        QMetaObject::invokeMethod(this, "interceptHTML", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    }
}

void InstagramInterceptor::interceptHTML(const QUrl& url) {
    qDebug() << "html: " << url;

    if (html_ != nullptr
        || !signed_in_)
        return;

    html_ = new WebContent(this);
    initRequest(html_);

    QObject::connect(html_, &WebContent::finished, this, [this]() {
		auto content = html_->data().toUtf8();
        if (content.isEmpty()) {
            emit error(EStatusCode::EmptyData);
            return;
        }

#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("instagram_" + QString::number(ms) + ".html");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(content);
            file.close();
        }
#endif
        if (content.isEmpty()) {
            emit error(EStatusCode::EmptyData);
            return;
        }

        if (content.contains("EmbeddedMedia")) {
            QString html;
            QRegExp expression = QRegExp("\"canonical\"\\s*href=\"([^\"]+)[\"]", Qt::CaseInsensitive);  
            if (expression.indexIn(content) != -1) {
                QString canonical_url = expression.cap(1);
                if (!canonical_url.isEmpty())
                    QMetaObject::invokeMethod(this, "requestHTML", Qt::QueuedConnection, Q_ARG(const QUrl&, canonical_url));
            }
        }
        else {
            QString media_id;
            QRegExp expression = QRegExp("\"media_id\":\"([^\"]+)[\"]", Qt::CaseInsensitive);
            if (expression.indexIn(content) != -1) {
                media_id = expression.cap(1);
                if (!media_id.isEmpty()) {
                    
                    expression = QRegExp("\"X-IG-App-ID\":\"([^\"]+)[\"]", Qt::CaseInsensitive);
                    if (expression.indexIn(content) != -1)
                        app_id_ = expression.cap(1);

                    expression = QRegExp("\"claim\":\"([^\"]+)[\"]", Qt::CaseInsensitive);
                    if (expression.indexIn(content) != -1)
                        claim_ = expression.cap(1);

                    getToken();

                    if (!app_id_.isEmpty()
                        && !claim_.isEmpty()
                        && !csrf_token_.isEmpty()) {

                        media_url_ = QString("https://i.instagram.com/api/v1/media/%1/info/").arg(media_id);
                        QMetaObject::invokeMethod(this, "requestMedia", Qt::QueuedConnection, Q_ARG(const QUrl&, media_url_));
                    }
                }
            }

            if (media_id.isEmpty())
                emit error(EStatusCode::VideoUnavailable);
        }

        html_ = nullptr;

    }, Qt::QueuedConnection);

    getToken();

    html_->add_header("csrftoken", csrf_token_);
    html_->add_header("Referer", base_url_);
    
    html_->reset();
    html_->fetch(url);
}

void InstagramInterceptor::requestMedia(const QUrl& url) {
    qDebug() << "requestMedia base_url_" << base_url_ << ", app_id_: " << app_id_ << ", claim_: " << claim_;

    if (app_id_.isEmpty()
        ||claim_.isEmpty()
        || media_ != nullptr)
        return;

    media_ = new WebContent(this);
    initRequest(media_);

    QObject::connect(media_, &WebContent::finished, this, [this]() {
        const auto& content = media_->data().toUtf8();
#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("media_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(content);
            file.close();
        }
#endif
        if (content.isEmpty()) {
            emit error(EStatusCode::EmptyData);
            return;
        }

        QList<QString> image_urls;
        QList<QString> video_urls;
        const auto doc = QJsonDocument::fromJson(content).object();
        const auto items = doc.value("items").toArray();
        if (!items.isEmpty()) {
            int images_count = 0;
            int video_count = 0;
            for (const auto item : items) {

                const auto user = item.toObject()["user"].toObject();
                if (!user.isEmpty()) {
                    QString title;
                    QString username = user.value("username").toString();
                    if (!username.isEmpty()) {
                        title = username;
                    }
                    else {
                        QString full_name = user.value("full_name").toString();
                        if (!full_name.isEmpty())
                            title = full_name;
                    }

                    video_info_.video_title = normalizeTitle(title);

                }

                const auto caption = item.toObject()["caption"].toObject();
                if (!caption.isEmpty()) {
                    QString text = caption.value("text").toString();
                    if (!text.isEmpty()) {
                        formatText(text);
                        text = normalizeTitle(text);
                        if (!text.isEmpty())
                            video_info_.video_title = text;
                    }
                }

                const auto video_subtitles_uri = item.toObject()["video_subtitles_uri"].toString();
                if (!video_subtitles_uri.isEmpty()) {
                    SubtitleTrackInfo subtitle_track;
                    subtitle_track.language_tag = "en";
                    subtitle_track.profile = SubtitleTrackInfo::SubtitleProfile::SRT;
                    subtitle_track.urls.append(video_subtitles_uri);
                    normalizeTrack(subtitle_track);
                    
                    tracks_info_.subtitle_tracks << subtitle_track;
                }

                const auto image_versions2 = item.toObject()["image_versions2"].toObject();
                if (!image_versions2.isEmpty()) {
                    const auto candidates = image_versions2["candidates"].toArray();
                    if (!candidates.isEmpty()) 
                        getImageTracks(candidates, images_count, image_urls);
                }

                const auto carousel_media = item.toObject()["carousel_media"].toArray();
                if (!carousel_media.isEmpty()) {
                    for (const auto carousel : carousel_media) {
                        const auto image_versions2 = carousel["image_versions2"].toObject();
                        if (!image_versions2.isEmpty()) {
                            const auto candidates = image_versions2["candidates"].toArray();
                            if (!candidates.isEmpty())
                                getImageTracks(candidates, images_count, image_urls);
                        }

                        bool has_audio = carousel.toObject()["has_audio"].toBool();
                        double duration = carousel.toObject()["video_duration"].toDouble();
                        video_info_.duration = int(duration);

                        const auto video_versions = carousel.toObject()["video_versions"].toArray();
                        if (!video_versions.isEmpty())
                            getVideoTracks(video_versions, video_count, has_audio, video_urls);
                    }
                }

                bool has_audio = item.toObject()["has_audio"].toBool();
                double duration = item.toObject()["video_duration"].toDouble();
                video_info_.duration = int(duration);

                const auto video_versions = item.toObject()["video_versions"].toArray();
                if (!video_versions.isEmpty())
                    getVideoTracks(video_versions, video_count, has_audio, video_urls);
            }
        }

        media_ = nullptr;

        if (!video_info_.video_title.isEmpty()
            && (!tracks_info_.image_tracks.isEmpty() || !tracks_info_.video_tracks.isEmpty()))
            emit metadataIntercepted(QUrl());

    }, Qt::QueuedConnection);

    getToken();

    media_->add_header("x-csrftoken", csrf_token_);
    media_->add_header("x-ig-app-id", app_id_);
    media_->add_header("x-ig-www-claim", claim_);
    
    media_->reset();
    media_->fetch(url);
}

void InstagramInterceptor::getImageTracks(const QJsonArray& candidates, int& images_count, QList<QString>& image_urls) {

    for (const auto candidate : candidates) {
        QString url = candidate.toObject()["url"].toString();

        if (!url.isEmpty()
            && !image_urls.contains(url)) {

            image_urls << url;

            ImageTrackInfo image_track;
            image_track.image_number = images_count;
            image_track.urls << url;

            int width = candidate.toObject()["width"].toInt();
            if (width > 0)
                image_track.width = width;

            int height = candidate.toObject()["height"].toInt();
            if (height > 0)
                image_track.height = height;

            if (video_info_.artwork_url.isEmpty())
                video_info_.artwork_url = url;

            normalizeTrack(image_track);
            normalizeImageTrack(image_track);
            tracks_info_.image_tracks << image_track;
        }
    }

    images_count++;
}

void InstagramInterceptor::getVideoTracks(const QJsonArray& video_versions, int& video_count,
    const bool has_audio, QList<QString>& video_urls) {

    for (const auto video_version : video_versions) {
        QString url = video_version.toObject()["url"].toString();

        if (!url.isEmpty()
            && !video_urls.contains(url)) {

            video_urls << url;

            VideoTrackInfo video_track;
            video_track.urls << url;

            int width = video_version.toObject()["width"].toInt();
            if (width > 0)
                video_track.width = width;

            int height = video_version.toObject()["height"].toInt();
            if (height > 0)
                video_track.height = height;

            bool next_video = false;
            if (video_count > 0
                && height > 0) {
                for (auto& track : tracks_info_.video_tracks) {
                    if (track.height == video_track.height) {
                        track.urls << url;
                        track.is_chunked = true;
                        next_video = true;
                        break;
                    }
                }
            }

            if (!next_video) {
                normalizeTrack(video_track);
                normalizeVideoTrack(video_track);
                tracks_info_.video_tracks.append(video_track);

                if (has_audio) {
                    AudioTrackInfo audio_track;

                    audio_track.from_video_stream = true;
                    audio_track.eval_type = ManualEvaluation::UnrecAudio;
                    audio_track.urls << url;
                    normalizeTrack(audio_track);
                    normalizeAudioTrack(audio_track);
                    tracks_info_.audio_tracks.append(audio_track);
                }
            }
        }
    }

    video_count++;
}

void InstagramInterceptor::initialize() {
    qDebug() << "InstagramInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };
}

void InstagramInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}

void InstagramInterceptor::startPage(bool downgrade_to_sd) {
    Q_UNUSED(downgrade_to_sd);

    qDebug() << "start page ...";
    start_time_ = std::chrono::system_clock::now();
    invalidate();
    app_id_.clear();
    claim_.clear();
    base_url_.clear();
    media_url_.clear();
    media_ = nullptr;
    html_ = nullptr;
}

void InstagramInterceptor::finishPage(bool ok) {
    Q_UNUSED(ok);
}

void InstagramInterceptor::formatText(QString& title) {
    if (!title.isEmpty()) {
        title = title.split("Instagram: ").last();
        title = title.split("Instagram post: ").last();
        title = title.split("(Tags)").first();
        title = title.split(" -Tags").first();
        title = title.split("Tags").first();
        title = title.split("#").first();
        title = title.remove(QRegExp("[<>:\"\/\\|?*']"));
        if (title == "Instagram")
            title.clear();

        if (!title.isEmpty()
            && title.at(0).unicode() == 8220)                      // “ -> 8220 unicode number
            title = title.mid(1);
        if (!title.isEmpty()
            && title.at(0).unicode() == 171)                       // « -> 171 unicode number
            title = title.mid(1);
        if (!title.isEmpty()
            && title.at(title.count() - 1).unicode() == 8221)      // ” -> 8221 unicode number
            title = title.mid(0, title.count() - 1);
        if (!title.isEmpty()
            && title.at(title.count() - 1).unicode() == 187)       // » -> 187 unicode number
            title = title.mid(0, title.count() - 1);
    }
}