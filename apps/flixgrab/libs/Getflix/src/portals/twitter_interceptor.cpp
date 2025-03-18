#include "twitter_interceptor.h"

#include "get_utils.h"


TwitterInterceptor::TwitterInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {
    //Q_INIT_RESOURCE(scripts);
}

TwitterInterceptor::~TwitterInterceptor() {
    qDebug() << "Interceptor: ~TwitterInterceptor";
}

void TwitterInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {

    info.setHttpHeader("User-Agent", USER_AGENT);

    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());
    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
        base_url_ = qstr;
        QMetaObject::invokeMethod(this, "requestGuestToken", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    }
    //else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {
    //    if (qstr.contains("main.a", Qt::CaseInsensitive)) {
    //        QMetaObject::invokeMethod(this, "requestMain", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    //    }
    //}
    else if (qstr.contains("TweetDetail", Qt::CaseInsensitive)) {
        QMetaObject::invokeMethod(this, "requestMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    }
}

void TwitterInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}

void TwitterInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    qDebug() << "start page ...";
    invalidate();
    base_url_.clear();
    guest_token_.clear();
    metadata_ = nullptr;
    guestTokendata_ = nullptr;
    main_script_ = nullptr;
}

void TwitterInterceptor::finishPage(bool ok) {
    Q_UNUSED(ok);
}

void TwitterInterceptor::initialize() {
    qDebug() << "TwitterInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };
}

void TwitterInterceptor::requestGuestToken(const QUrl& url) {
    Q_UNUSED(url);

    if (!guest_token_.isEmpty())
        return;

    guestTokendata_ = new WebContent(this);
    initRequest(guestTokendata_);
    qDebug() << "guestTokendata";

    QObject::connect(guestTokendata_, &WebContent::finished, this, [this]() {
        const auto& content = guestTokendata_->data().toUtf8();
#ifdef _DEBUG
        QFile   file("activatedata.json");
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

        QJsonDocument doc = QJsonDocument::fromJson(content);
        QJsonObject json_obj = doc.object();
        if (!json_obj.isEmpty())
            guest_token_ = json_obj.value("guest_token").toString().toLocal8Bit();

        guestTokendata_ = nullptr;

    }, Qt::QueuedConnection);

    QString post_data;
    guestTokendata_->reset();
    guestTokendata_->add_header("authorization", bearer_.toLocal8Bit());
    guestTokendata_->post(token_url_, post_data.toLocal8Bit());
}

void TwitterInterceptor::requestMetadata(const QUrl& url) {
    if (guest_token_.isEmpty() 
        || metadata_ != nullptr)
        return;

    metadata_ = new WebContent(this);
    initRequest(metadata_);
    qDebug() << "metadata: " << url;

    QObject::connect(metadata_, &WebContent::finished, this, [this]() {
        const auto& content = metadata_->data().toUtf8();
#ifdef _DEBUG
        QFile   file("jsondata.json");
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

        QJsonDocument doc = QJsonDocument::fromJson(content);
        if (!doc.isNull()) {
            QString video_id = base_url_.split("/").last().split("?").first();
            QStringList urls;
            QJsonObject data = doc.object()["data"].toObject();
            if (!data.isEmpty()) {
                QJsonObject threaded_conversation_with_injections = data["threaded_conversation_with_injections"].toObject();
                if (!threaded_conversation_with_injections.isEmpty()) {
                    QJsonArray instructions = threaded_conversation_with_injections["instructions"].toArray();
                    foreach(const QJsonValue & instruction, instructions) {
                        if (instruction.toObject()["type"].toString() == "TimelineAddEntries") {
                            QJsonArray entries = instruction["entries"].toArray();
                            foreach(const QJsonValue & entrie, entries) {
                                if (entrie["entryId"].toString().contains("tweet-" + video_id)) {
                                    QJsonObject content = entrie["content"].toObject();
                                    if (!content.isEmpty()) {
                                        QJsonObject itemContent = content["itemContent"].toObject();
                                        if (!itemContent.isEmpty()) {
                                            QJsonObject tweet_results = itemContent["tweet_results"].toObject();
                                            if (!tweet_results.isEmpty()) {
                                                QJsonObject result = tweet_results["result"].toObject();
                                                if (!result.isEmpty()) {
                                                    QJsonObject core = result["core"].toObject();
                                                    if (!core.isEmpty()) {
                                                        QJsonObject user_results = core["user_results"].toObject();
                                                        if (!user_results.isEmpty()) {
                                                            QJsonObject result = user_results["result"].toObject();
                                                            if (!result.isEmpty()) {
                                                                QJsonObject legacy = result["legacy"].toObject();
                                                                if (!legacy.isEmpty())
                                                                    video_info_.artwork_url = legacy["profile_banner_url"].toString();
                                                            }
                                                        }
                                                    }

                                                    QJsonObject card = result["card"].toObject();
                                                    if (!card.isEmpty()) {
                                                        QJsonObject legacy = card["legacy"].toObject();
                                                        if (!legacy.isEmpty()) {
                                                            QJsonArray binding_values = legacy["binding_values"].toArray();
                                                            foreach(const QJsonValue & binding_value, binding_values) {
                                                                if (binding_value.toObject()["key"].toString() == "amplify_url_vmap") {
                                                                    QJsonObject value = binding_value["value"].toObject();
                                                                    if (!value.isEmpty()) {
                                                                        QString string_value = value["string_value"].toString();
                                                                        if (!string_value.isEmpty()) {
                                                                            loadVmap(string_value, video_info_, tracks_info_);
                                                                            break;
                                                                        }
                                                                    }
                                                                }
                                                                if (binding_value.toObject()["key"].toString() == "player_stream_url") {
                                                                    QJsonObject value = binding_value["value"].toObject();
                                                                    if (!value.isEmpty()) {
                                                                        QString string_value = value["string_value"].toString();
                                                                        if (!string_value.isEmpty()) {
                                                                            VideoTrackInfo video_track;
                                                                            video_track.urls.push_back(string_value);

                                                                            normalizeTrack(video_track);
                                                                            tracks_info_.calculateTrackSize(video_track, video_info_);
                                                                            normalizeVideoTrack(video_track);
                                                                            tracks_info_.video_tracks.append(video_track);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    QJsonObject legacy = result["legacy"].toObject();
                                                    if (!legacy.isEmpty()) {
                                                        video_info_.video_title = normalizeTitle(legacy["full_text"].toString());
                                                        QJsonObject extended_entities = legacy["extended_entities"].toObject();
                                                        if (!extended_entities.isEmpty()) {
                                                            QJsonArray medias = extended_entities["media"].toArray();
                                                            foreach(const QJsonValue & media, medias) {
                                                                video_info_.artwork_url = media["media_url_https"].toString();
                                                                QJsonObject info = media["video_info"].toObject();
                                                                if (!info.isEmpty()) {
                                                                    video_info_.duration = info["duration_millis"].toInt() / 1000;
                                                                    QJsonArray variants = info["variants"].toArray();
                                                                    
                                                                    foreach(const QJsonValue & variant, variants) {
                                                                        if (variant["content_type"].toString().contains("video")) {
                                                                            QString url = variant["url"].toString();

                                                                            if (!url.isEmpty()
                                                                                && !urls.contains(url)) {
                                                                                urls << url;

                                                                                VideoTrackInfo video_track;
                                                                                video_track.urls.push_back(url);
                                                                                video_track.bitrate = variant["bitrate"].toInt();

                                                                                QRegExp widthRe("(\\d+)x");
                                                                                QRegExp heightRe("x(\\d+)");

                                                                                int pos = widthRe.indexIn(url);
                                                                                if (pos > -1)
                                                                                    video_track.width = widthRe.cap(1).toInt();

                                                                                pos = heightRe.indexIn(url);
                                                                                if (pos > -1)
                                                                                    video_track.height = heightRe.cap(1).toInt();

                                                                                normalizeTrack(video_track);
                                                                                tracks_info_.calculateTrackSize(video_track, video_info_);
                                                                                normalizeVideoTrack(video_track);
                                                                                tracks_info_.video_tracks.append(video_track);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                if (!video_info_.video_title.isEmpty()
                                                                    && !tracks_info_.video_tracks.isEmpty())
                                                                    break;
                                                            }
                                                        }
                                                    }


                                                }
                                            }
                                        }
                                    }
                                }
                                if (!video_info_.video_title.isEmpty()
                                    && !tracks_info_.video_tracks.isEmpty())
                                    break;
                            }
                        }
                        if (!video_info_.video_title.isEmpty()
                            && !tracks_info_.video_tracks.isEmpty())
                            break;
                    }
                }
            }
        }

        metadata_ = nullptr;

        if (!video_info_.video_title.isEmpty()
            && !tracks_info_.video_tracks.isEmpty())
            emit metadataIntercepted(QUrl());

    }, Qt::QueuedConnection);


    metadata_->add_header("authorization", bearer_.toLocal8Bit());
    metadata_->add_header("referer", base_url_);
    //metadata_->add_header("x-csrf-token", "9943da5042c33b37e813868a65063837");
    metadata_->add_header("x-guest-token", guest_token_);
    metadata_->reset();
    metadata_->fetch(url);
}

void TwitterInterceptor::loadVmap(const QString& url, VideoInfo& video_info, TracksInfo& tracks_info) {
    QString data;
    if (downloadToString(url, data)) {

        if (!data.isEmpty()) {//parse vmap xml
            QXmlStreamReader xml(data);
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.tokenType() == QXmlStreamReader::StartElement) {
                    if (xml.name() == "videoVariant") {  //video quality in block - <videoVariant>
                        QXmlStreamAttributes attributes = xml.attributes();
                        if (attributes.hasAttribute("url")) {
                            QString url = QUrl::fromPercentEncoding(attributes.value("url").toString().toLocal8Bit()); //url in block - prcent encoding
                            QString video_re_str = "^https://video.twimg.com/.*\\.m3u8.*";
                            if (QRegExp(video_re_str).exactMatch(url)) {
                                QString m3u8data;
                                if (tracks_info.video_tracks.isEmpty() && downloadToString(url, m3u8data))
                                    parseM3U8(m3u8data, url, tracks_info);
                            }
                            else {
                                VideoTrackInfo video_track;
                                video_track.urls.push_back(url);

                                if (attributes.hasAttribute("bit_rate"))
                                    video_track.bitrate = attributes.value("bit_rate").toInt();

                                QRegExp widthRe("(\\d+)x");
                                QRegExp heightRe("x(\\d+)");

                                int pos = widthRe.indexIn(url);
                                if (pos > -1)
                                    video_track.width = widthRe.cap(1).toInt();

                                pos = heightRe.indexIn(url);
                                if (pos > -1)
                                    video_track.height = heightRe.cap(1).toInt();

                                normalizeTrack(video_track);
                                tracks_info.calculateTrackSize(video_track, video_info);
                                normalizeVideoTrack(video_track);
                                tracks_info.video_tracks.append(video_track);
                            }
                        }
                    }
                }
            }
        }
    }
}

void TwitterInterceptor::parseM3U8(const QString & data, const QString & url, TracksInfo & tracks_info) {
    QRegExp stream1Re("^#EXT-X-STREAM-INF:.*$");
    QRegExp bandWidhtRe("BANDWIDTH=(\\d+),");
    QRegExp widthRe("RESOLUTION=(\\d+)x");
    QRegExp heightRe("x(\\d+),");

    QStringList stringList = data.split("\n");
    bool urlString = false;
    foreach(QString str, stringList) {
        if (stream1Re.exactMatch(str)) {
            tracks_info.video_tracks.push_back(VideoTrackInfo());

            int pos = bandWidhtRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().bitrate = bandWidhtRe.cap(1).toInt();

            pos = widthRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().width = widthRe.cap(1).toInt();

            pos = heightRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().height = heightRe.cap(1).toInt();

            normalizeTrack(tracks_info.video_tracks.back());
            normalizeVideoTrack(tracks_info.video_tracks.back());

            urlString = true;
            continue;
        }
        if (urlString) {
            QString keyword = str.mid(1).split("/").first();
            QString url_base = url.split("/" + keyword).first();
            QString m3u8_url = url_base + str;

            QString m3u8_data;
            if (downloadToString(m3u8_url, m3u8_data)
                && !m3u8_data.isEmpty()) {

                QStringList stringList = m3u8_data.split("\n");
                foreach(QString str, stringList) {
                    if (str.contains(keyword)) {
                        tracks_info.video_tracks.back().urls.push_back(url_base + str);
                        tracks_info.video_tracks.back().is_chunked = true;
                    }
                }
            }

            urlString = false;
        }
    }
}

void TwitterInterceptor::requestMain(const QUrl& url) {
    main_script_ = new WebContent(this);
    initRequest(main_script_);
    qDebug() << "main_script: " << url;

    QObject::connect(main_script_, &WebContent::finished, this, [this]() {
        const auto& content = main_script_->data().toUtf8();
#ifdef _DEBUG
        QFile   file("main_script.js");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(content);
            file.close();
        }
#endif

        main_script_ = nullptr;

        }, Qt::QueuedConnection);

    //Q_UNUSED(url);
    main_script_->fetch(url);
    scripts_.addAsync(main_script_);
    scripts_.run(page_);
}
//
//Q_INVOKABLE void TwitterInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
//    qDebug() << "interceptConfig confName: " << confName;
//#ifdef _DEBUG
//    QString file_name = QString("%1/%2/%3").arg(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).arg("FreeGrabApp").arg("apple_" + confName + ".json");
//    QFile file(file_name);
//    if (file.open(QIODevice::ReadWrite)) {
//        file.resize(0);
//        file.write(jsonStr.toUtf8());
//        file.close();
//    }
//#endif
//}