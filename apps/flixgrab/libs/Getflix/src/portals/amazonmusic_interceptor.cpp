#include "amazonmusic_interceptor.h"
#include "get_utils.h"
#include <iostream>
#include <recode_op.h>

#include "mpd_parser_amazon.h"

AmazonMusicInterceptor::AmazonMusicInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {

    Q_INIT_RESOURCE(scripts);
}

AmazonMusicInterceptor::~AmazonMusicInterceptor() {
    qDebug() << "Interceptor: ~AmazonMusicInterceptor";
}

void AmazonMusicInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {
    if (!accessToken_.isEmpty()) {
        info.block(true);
        return;
    }

    info.setHttpHeader("User-Agent", USER_AGENT);

    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());
    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;
    
    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame)
        base_url_ = qstr;

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {
#ifdef _DEBUG 
        //QMetaObject::invokeMethod(this, "dumpOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif
    }
    else {  // data intercept  
        if (qstr.contains("/showHome?", Qt::CaseInsensitive)) {
            // https://na.mesk.skill.music.a2z.com/api/showHome?deeplink=%7B%22interface%22%3A%22DeeplinkInterface.v1_0.DeeplinkClientInformation%22%2C%22deeplink%22%3A%22%2Falbums%2FB091V7R59M%3FtrackAsin%3DB091V7JHXT%22%7D
            //manifest_url_ = url;
            //QMetaObject::invokeMethod(this, "requestManifest", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            //info.block(true);
        }
        else if (qstr.contains("&trackAsin=", Qt::CaseInsensitive)) {
            // parse urls 
            trackUrl_ = qstr;

            if (trackAsin_.isEmpty()) {
                auto startPos = qstr.indexOf("&trackAsin=") + QString("&trackAsin=").size();
                auto endPos = qstr.indexOf("&", startPos); // &trackAsin=B091V7JHXT
                trackAsin_ = qstr.mid(startPos, endPos - startPos);
            }

            if (albumAsin_.isEmpty()) {
                auto startPos = qstr.indexOf("/albums/") + QString("/albums/").size();
                auto endPos = qstr.indexOf("?", startPos); // /albums/B091V7R59M?
                albumAsin_ = qstr.mid(startPos, endPos- startPos);// B091V7R59M
            }

            qDebug() << "trackAsin: " << trackAsin_ << ", albumAsin: " << albumAsin_ << ", trackUrl: " << trackUrl_;
        }
    }
}

Q_INVOKABLE void AmazonMusicInterceptor::dumpOrigScript(const QUrl& url) {
#ifdef _DEBUG 
    if (!url.isLocalFile()) {
        auto script = new WebContent(this);
        script->setNetworkAccess(nam_);
        QObject::connect(script, &WebContent::finished, this, [=]() {
            //if (script->status() == Status.OK)
            {
                QString qstr = url.toEncoded();
                QFile   file(".\\amusic_js\\" + QFileInfo(qstr).fileName());
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0); // remove old content
                    file.write(script->data().toLocal8Bit());
                }
            }
        });

        all_scripts_.push_back(script);
        script->fetch(url);
    }
#endif
}

void AmazonMusicInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}

void AmazonMusicInterceptor::requestShowHome(const QUrl& url) {
    qDebug() << "requestShowHome base_url_" << base_url_<< ", token_:" << token_ << ", accessToken_: " << accessToken_;

    if (token_.isEmpty()
        || accessToken_.isEmpty()
        || showhome_ != nullptr
        /*|| (base_url_.contains("trackAsin") && metadata_ != nullptr)*/)
        return;

    showhome_ = new WebContent(this);
    initRequest(showhome_);

    QObject::connect(showhome_, &WebContent::finished, this, [this]() {
        const auto& content = showhome_->data().toUtf8();
#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("amusic-showhome" + QString::number(ms) + ".json");
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

        const auto doc = QJsonDocument::fromJson(content).object();
        const auto methods = doc.value("methods").toArray();
        QString base_id = base_url_.split("/").last().split("?").first();
        QString playlist;

        if (!methods.isEmpty()) {
            for (const auto m : methods) {
                const auto interface = m.toObject()["interface"].toString();

                //if (base_url_.contains("trackAsin")) { //not playlist item
                //    if (interface == "VideoPlayerAuthenticationInterface.v1_0.SetVideoPlayerTokenMethod") {
                //        const auto video_token = m.toObject()["token"].toString();
                //        requestMetadata(video_token);
                //        //break;
                //    }
                //}
                //else {
                    if (interface == "TemplateListInterface.v1_0.CreateAndBindTemplateMethod") {
                        const auto template_data = m.toObject()["template"].toObject();
                        if (!template_data.isEmpty()) {
                            const auto widgets = template_data["widgets"].toArray();
                            if (!widgets.isEmpty()) {
                                for (const auto w : widgets) {
                                    const auto items = w.toObject()["items"].toArray();
                                    if (!items.isEmpty()) {
                                        QList<QString> url_parts;
                                        for (const auto i : items) {
                                            const auto primaryLink = i["primaryLink"].toObject();
                                            if (!primaryLink.isEmpty()) {
                                                QString url_part = primaryLink.value("deeplink").toString();
                                                if (!url_parts.contains(url_part)) {
                                                    url_parts << url_part;
                                                    QString trackAsin = url_part.split("trackAsin=").last();
                                                    if (!trackAsin.contains("/")) { //check if not playlist or video item
                                                        url_part = url_part.split("?").first() + "?";
                                                        QString url = "https://music.amazon.com" + url_part + "marketplaceId=" + marketplaceId_ + "&musicTerritory=" + musicTerritory_ + "&trackAsin=" + trackAsin; //base_url_.mid(0, base_url_.lastIndexOf(base_id) - 1) + url_part;
                                                        qDebug() << "playlist item url: " << url;
                                                        
                                                        //if (base_url_.contains("trackAsin") //single item
                                                        //    && !base_url_.contains(trackAsin))
                                                        //    video_info_.playlist.append(url + "|");

                                                        playlist.append(url + "|");
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                //}
            }
        }
        showhome_ = nullptr;


        if (base_url_.contains("trackAsin")) {  //single item
            QUrl meta_url = QString("https://na.web.skill.music.a2z.com/api/playCatalogAlbum?at=") + trackAsin_ + "&id=" + albumAsin_ + "&userHash=" + QUrl::toPercentEncoding("{\"level\":\"UNLIMITED_MEMBER\"}");
            QMetaObject::invokeMethod(this, "requestMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, meta_url));
        }
        else {                                  //playlist
            if (!base_url_.contains("https://music.amazon.com/?")
                && !playlist.isEmpty())
                emit loadPlaylist(playlist);
        }

    }, Qt::QueuedConnection);

    qDebug() << "showhome: " << url << "sessionId: " << sessionId_ << ", deviceId:" << deviceId_ << ", accessToken: " << accessToken_ << ", csrf.token: " << token_
        << ", timestamp_: " << timestamp_ << ", rndNonce_: " << rndNonce_ << ", customerId_: " << customerId_;

    showhome_->add_header("authority", "na.web.skill.music.a2z.com");
    showhome_->add_header("path", "/api/showHome");
    showhome_->add_header("accept", "*/*");
    showhome_->add_header("accept-encoding", "gzip, deflate, br");
    showhome_->add_header("accept-language", "q=0.9,en-US;q=0.8,en;q=0.7");
    showhome_->add_header("cache-control", "no-cache");
    showhome_->add_header("content-type", "text/plain;charset=UTF-8");
    showhome_->add_header("origin", "https://music.amazon.com");
    showhome_->add_header("pragma", "no-cache");
    showhome_->add_header("referer", "https://music.amazon.com/");
    showhome_->add_header("x-amzn-affiliate-tags", "");
    showhome_->add_header("x-amzn-application-version", "1.0.9053.0");
    showhome_->add_header("x-amzn-authentication", "{\"interface\":\"ClientAuthenticationInterface.v1_0.ClientTokenElement\",\"accessToken\":\"" + accessToken_ + "\"}");
    showhome_->add_header("x-amzn-csrf", "{\"interface\":\"CSRFInterface.v1_0.CSRFHeaderElement\",\"token\":\"" + token_ + "\",\"timestamp\":\"" + timestamp_ + "\",\"rndNonce\":\"" + rndNonce_ + "\"}");
    showhome_->add_header("x-amzn-device-family", "WebPlayer");
    showhome_->add_header("x-amzn-device-height", "1080");
    showhome_->add_header("x-amzn-device-id", deviceId_);
    showhome_->add_header("x-amzn-device-language", "en_US");
    showhome_->add_header("x-amzn-device-model", "WEBPLAYER");
    showhome_->add_header("x-amzn-device-time-zone", "Europe/London");
    showhome_->add_header("x-amzn-device-width", "1920");
    showhome_->add_header("x-amzn-music-domain", "music.amazon.com");
    showhome_->add_header("x-amzn-os-version", "1.0");
    showhome_->add_header("x-amzn-page-url", base_url_);
    showhome_->add_header("x-amzn-ref-marker", "");
    showhome_->add_header("x-amzn-referer", "");
    showhome_->add_header("x-amzn-request-id", QUuid::createUuid().toString());
    showhome_->add_header("x-amzn-session-id", sessionId_);
    showhome_->add_header("x-amzn-timestamp", timestamp_);
    showhome_->add_header("x-amzn-user-agent", USER_AGENT);
    showhome_->add_header("x-amzn-video-player-token", "");
    showhome_->add_header("x-amzn-weblab-id-overrides", "");
    showhome_->reset(); 
    
    //if (base_url_.contains("trackAsin")) {
    //    showhome_->fetch(url);
    //}
    //else {
        QString deeplink = base_url_.split("music.amazon.com").last().split("?").first();
        QString post_data = "{\"deeplink\":\"{\\\"interface\\\":\\\"DeeplinkInterface.v1_0.DeeplinkClientInformation\\\",\\\"deeplink\\\":\\\"" + deeplink + "\\\"}\"}";
        qDebug() << "post_data: " << post_data;
        showhome_->post(url, post_data.toLocal8Bit());
    //}
}

void AmazonMusicInterceptor::requestMetadata(const QUrl& url/*, const QString& video_token*/) {
    qDebug() << "requestMetadata metadata token_: " << token_ << " accessToken_: " << accessToken_ << " video_info_.video_title: " << video_info_.video_title;

    if (token_.isEmpty()
        || accessToken_.isEmpty()
        || metadata_ != nullptr
        || !video_info_.video_title.isEmpty())
        return;

    metadata_ = new WebContent(this);
    initRequest(metadata_);

    QObject::connect(metadata_, &WebContent::finished, this, [this]() {
        const auto& content = metadata_->data().toUtf8();
#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("amusic-meta" + QString::number(ms) + ".json");
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

        const auto r = QJsonDocument::fromJson(content).object();
        // "methods" -> "interface":"PlaybackInterface.v1_0.SetMediaMethod" -> "metadata"

        const auto methods = r.value("methods").toArray();
        if (!methods.isEmpty()) {
            for (const auto m : methods) {
                auto interface = m.toObject()["interface"].toString();
                if (interface == "PlaybackInterface.v1_0.SetMediaMethod") {
                    auto metadata = m.toObject()["metadata"].toObject();
                    mediaId_ = metadata.value("mediaId").toString();
                    video_info_.video_title = normalizeTitle(metadata.value("title").toString());
                    video_info_.enable_metadata = true;
                    video_info_.artwork_url = metadata.value("artwork").toString();
                    video_info_.metadata_artwork_url = video_info_.artwork_url;
                    video_info_.duration = tracks_info_.duration = metadata.value("durationSeconds").toInt();

                    video_info_.metadata_title = video_info_.video_title;
                    video_info_.metadata_artist = metadata.value("artistName").toString();
                    video_info_.metadata_album = metadata.value("albumName").toString();

                    qDebug() << "title: " << video_info_.video_title << ", " << video_info_.duration << " sec. " << mediaId_;

                    QMetaObject::invokeMethod(this, "requestManifest", Qt::QueuedConnection, Q_ARG(const QUrl&, QUrl()));
                    break;
                }
            }
        }

        if (!video_info_.video_title.isEmpty() && !tracks_info_.audio_tracks.isEmpty())
            emit metadataIntercepted(QUrl());
        
        metadata_ = nullptr;

    }, Qt::QueuedConnection);

    metadata_->add_header("authority", "na.mesk.skill.music.a2z.com");
    metadata_->add_header("path", "/api/playCatalogAlbum");
    metadata_->add_header("accept", "*/*");
    metadata_->add_header("accept-encoding", "gzip, deflate, br");
    metadata_->add_header("accept-language", "q=0.9,en-US;q=0.8,en;q=0.7");
    metadata_->add_header("origin", "https://music.amazon.com");
    metadata_->add_header("referer", "https://music.amazon.com/");
    metadata_->add_header("user-agent", USER_AGENT);
    metadata_->add_header("x-amzn-affiliate-tags", "");
    metadata_->add_header("x-amzn-application-version", "1.0.9053.0");
    metadata_->add_header("x-amzn-authentication", "{\"interface\":\"ClientAuthenticationInterface.v1_0.ClientTokenElement\",\"accessToken\":\"" + accessToken_ + "\"}");
    metadata_->add_header("x-amzn-csrf", "{\"interface\":\"CSRFInterface.v1_0.CSRFHeaderElement\",\"token\":\"" + token_ + "\",\"timestamp\":\"" + timestamp_ + "\",\"rndNonce\":\"" + rndNonce_ + "\"}");
    metadata_->add_header("x-amzn-device-family", "WebPlayer");
    metadata_->add_header("x-amzn-device-height", "1080");
    metadata_->add_header("x-amzn-device-id", deviceId_);
    metadata_->add_header("x-amzn-device-language", "en_US");
    metadata_->add_header("x-amzn-device-model", "WEBPLAYER");
    metadata_->add_header("x-amzn-device-time-zone", "Europe/London");
    metadata_->add_header("x-amzn-device-width", "1920");
    metadata_->add_header("x-amzn-music-domain", "music.amazon.com");
    metadata_->add_header("x-amzn-os-version", "1.0");
    metadata_->add_header("x-amzn-page-url", trackUrl_);
    metadata_->add_header("x-amzn-ref-marker", "");
    metadata_->add_header("x-amzn-referer", "");
    metadata_->add_header("x-amzn-request-id", QUuid::createUuid().toString());
    metadata_->add_header("x-amzn-session-id", sessionId_);
    metadata_->add_header("x-amzn-timestamp", timestamp_);
    metadata_->add_header("x-amzn-user-agent", USER_AGENT);
    //metadata_->add_header("x-amzn-video-player-token", "{\"interface\":\"VideoPlaybackInterface.v1_0.VideoPlaybackHeaderElement\",\"token\":\"" + video_token + "\",\"expirationMS\":}");
    metadata_->add_header("x-amzn-weblab-id-overrides", "");
    metadata_->reset();
    QString post_data = "{\"id\":\"" + albumAsin_ + "\",\"userHash\":\"{\\\"level\\\":\\\"HD_MEMBER\\\"}\"}";
    metadata_->post(url, post_data.toLocal8Bit());
}

void AmazonMusicInterceptor::setTrackInfo(const TracksInfo& t) {
        if (video_info_.duration == 0
        && t.duration > 0)
        video_info_.duration = t.duration;

    for each (auto & a in t.audio_tracks) {
        if (video_info_.video_title.isEmpty())
            break;
        auto ti = a;

        if (ti.size == 0) {
            if (ti.is_chunked)
                ti.size = t.audio_tracks.size() * 10;
        }

        normalizeTrack(ti);
        normalizeAudioTrack(ti);
        tracks_info_.audio_tracks.push_back(ti);
    }
}

void AmazonMusicInterceptor::requestManifest(const QUrl&) {
    if (token_.isEmpty()
        || accessToken_.isEmpty()
        || manifest_ != nullptr)
        return;

    manifest_ = new WebContent(this);
    initRequest(manifest_);
    
    QObject::connect(manifest_, &WebContent::finished, this, [this]() {
        const auto& content = manifest_->data().toUtf8();
        const auto r = QJsonDocument::fromJson(content).object();
        
        const auto contentResponseList = r.value("contentResponseList").toArray();
        const auto contentItem = contentResponseList.begin();
        if (contentItem == contentResponseList.end())
            return;
        
        const auto manifest = contentItem->toObject().value("manifest").toString();
        
#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file2("amusic-manifest" + QString::number(ms) + ".xml");
        if (file2.open(QIODevice::ReadWrite)) {
            file2.resize(0);
            file2.write(manifest.toUtf8());
            file2.close();
        }
#endif
        
        MpdParserAmazon mpd;
        mpd.read(manifest);
        setTrackInfo(mpd.get_tracks());
        
        //for each (auto & v in mpd.get_tracks().audio_tracks) {
        //    auto ti = v;
        //    ti.duration = video_info_.duration; // (sec.) 
        //    tracks_info_.audio_tracks.push_back(ti);
        //    
        //    //for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
        //    qDebug() << "audio (" << ti.urls.size() << " urls) " << ti.bitrate << " bps, " << ti.codec << ", " << (ti.urls.size()>0? ti.urls.first(): "") << ", " << ti.;
        //    
        //}
        QString tmp_pssh;
        for each (auto &t in tracks_info_.audio_tracks) {
            qDebug() << " tracks: " << t.bitrate << ", urls: " << t.urls.size() << ", cench_pssh: " << t.cench_pssh << (tmp_pssh != t.cench_pssh? "Load keys ...": "");
            if (tmp_pssh != t.cench_pssh) {
                tmp_pssh = t.cench_pssh;
                tracks_count_++; // indeed count of keys
                auto d = QByteArray::fromBase64(t.cench_pssh.toUtf8());
                std::vector<uint8_t> init_data(d.begin(), d.end());
                loadKeysWithPssh(init_data);
            }
        }

        emit metadataIntercepted(QUrl());

    }, Qt::QueuedConnection);

    QString post_data = "{\"customerId\":\"" + customerId_ + "\",\"deviceToken\":{\"deviceTypeId\":\"A16ZV8BU3SN1N3\",\"deviceId\":\""+ deviceId_ +"\"},\
        \"appMetadata\":{\"https\":\"true\"},\"clientMetadata\":{\"clientId\":\"WebCP\",\"clientRequestId\":\"c9aa2740-eef2-11eb-b16c-e955b2b458e5\"},\
        \"contentIdList\":[{\"identifier\":\"" + mediaId_ + "\",\"identifierType\":\"ASIN\"}],\"bitrateTypeList\":[\"LOW\",\"MEDIUM\",\"HIGH\"],\
        \"musicDashVersionList\":[\"V1\",\"V2\"],\"appInfo\":{\"musicAgent\":\"Maestro/1.0 WebCP/1.0.7627.0 (6cdf-d4b0-WebC-3ebb-66b12)\"}}}";
    
    QUrl url = "https://music.amazon.com/NA/api/dmls/";
    qDebug() << "manifest: " << url;

    manifest_->add_header("Authorization", "Bearer " + accessToken_);
    manifest_->add_header("csrf-token", token_);
    manifest_->add_header("csrf-ts", timestamp_);
    manifest_->add_header("csrf-rnd", rndNonce_);
    manifest_->add_header("X-Amz-Target", "com.amazon.digitalmusiclocator.DigitalMusicLocatorServiceExternal.getDashManifestsV2");
    manifest_->add_header("User-Agent", USER_AGENT);
    manifest_->add_header("Content-Encoding", "amz-1.0");
    manifest_->add_header("Accept", "application/json, text/plain, */*");
    manifest_->post(url, post_data.toLocal8Bit(), "application/json");
}


QString getJsonVal(const QString& jsonStr, const QString& name) {
    auto pos = jsonStr.indexOf("\"" + name + "\":"); // "\"accessToken\":"
    if (pos > 0 && (pos = jsonStr.indexOf("\"", 3 + pos + name.size())) > 0) {
        auto pos2 = jsonStr.indexOf("\"", pos + 1);
        return jsonStr.mid(pos + 1, pos2 - pos - 1);
    }

    return QString();
}

Q_INVOKABLE void AmazonMusicInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    qDebug() << "interceptConfig confName: " << confName;
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QString file_name = confName + QString::number(ms) + ".json";
    QFile file(file_name);
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(jsonStr.toUtf8());
        file.close();
    }
#endif


    if (confName == "config") {
        accessToken_ = getJsonVal(jsonStr, "accessToken");
        token_ = getJsonVal(jsonStr, "token");
        timestamp_ = getJsonVal(jsonStr, "ts");
        rndNonce_ = getJsonVal(jsonStr, "rnd");
        sessionId_ = getJsonVal(jsonStr, "sessionId");
        deviceId_ = getJsonVal(jsonStr, "deviceId");
        customerId_ = getJsonVal(jsonStr, "customerId");
        marketplaceId_ = getJsonVal(jsonStr, "marketplaceId");
        musicTerritory_ = getJsonVal(jsonStr, "musicTerritory");
        //const auto jsonObj = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
        //const auto appConfig = jsonObj["appConfig"].toObject();
        qDebug() << "base_url_: " << base_url_ << " sessionId: " << sessionId_ << ", deviceId:" << deviceId_ << ", accessToken: "<< accessToken_ << ", csrf.token: " << token_ << ", timestamp_: " << timestamp_ << ", rndNonce_: " << rndNonce_ << ", customerId_: " << customerId_;
        if (accessToken_.isEmpty()) {
            emit loginRequested(QString()); // not signed in
        }
        else {
            QUrl showhomeurl = "https://na.web.skill.music.a2z.com/api/showHome";
            QMetaObject::invokeMethod(this, "requestShowHome", Qt::QueuedConnection, Q_ARG(const QUrl&, showhomeurl));
            //emit error(Status(Status::StopPage)); // stop all activity on the page
        }
    }
}

void AmazonMusicInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    qDebug() << "start page ...";
    if (!forced_reload_page)
        invalidate();
    accessToken_.clear();
    token_.clear();
    timestamp_.clear();
    rndNonce_.clear();
    sessionId_.clear();
    deviceId_.clear();
    customerId_.clear();
    marketplaceId_.clear();
    musicTerritory_.clear();
    trackUrl_.clear();
    trackAsin_.clear();
    albumAsin_.clear();
    mediaId_.clear();
    tracks_count_ = 0;
    host_primise_id_ = -1;
    //cdm_license_requests_.clear();
//#ifdef DECRYPTSERVICE
//    keys_loaders_.clear();
//#endif
    manifest_ = nullptr;
    metadata_ = nullptr;
    showhome_ = nullptr;
}

void AmazonMusicInterceptor::finishPage(bool ok) {
    Q_UNUSED(ok);
}

void AmazonMusicInterceptor::initialize() {
    qDebug() << "AmazonMusicInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };

}

void AmazonMusicInterceptor::loadKeys(const QString url) {
#ifdef DECRYPTSERVICE
    auto recode_op = std::make_shared<recode::RecodeOp>(url.toUtf8().constData(), "", nullptr);
    auto init_data = recode_op->GetInitData();

    //auto str = hexStr(init_data.data(), init_data.size());
    qDebug() << "init_data extracted " << init_data.size() << " bytes: " << url.toUtf8();
    loadKeysWithPssh(init_data);
#endif
}

void AmazonMusicInterceptor::loadKeysWithPssh(const std::vector<uint8_t> init_data) {
#ifdef DECRYPTSERVICE
    auto keys_loader = new AmazonMusicKeysLoader();
    //keys_loaders_.push_back(keys_loader);

    QObject::connect(keys_loader, &AmazonMusicKeysLoader::keysLoaded, this, &AmazonMusicInterceptor::onKeysLoaded);
    QObject::connect(keys_loader, &AmazonMusicKeysLoader::getPostData, this, &AmazonMusicInterceptor::postChallenge);
    QObject::connect(keys_loader, &AmazonMusicKeysLoader::extractLicense, this, &AmazonMusicInterceptor::extractLicense);

    QPointer<WebContent> cdmLicense = new WebContent(this);
    cdmLicense->setNetworkAccess(nam_);
    QObject::connect(cdmLicense, &WebContent::finished, this, [=]() {
        if (!cdmLicense->status())
            emit error(cdmLicense->status());
    });

    cdmLicense->add_header("authorization", "Bearer " + accessToken_);
    cdmLicense->add_header("csrf-token", token_);
    cdmLicense->add_header("csrf-ts", timestamp_);
    cdmLicense->add_header("csrf-rnd", rndNonce_);
    cdmLicense->add_header("X-Amz-Target", "com.amazon.digitalmusiclocator.DigitalMusicLocatorServiceExternal.getLicenseForPlaybackV2");
    cdmLicense->add_header("X-Requested-With", "XMLHttpRequest");
    cdmLicense->add_header("Content-Encoding", "amz-1.0");

    qDebug() << "cdmLicense args: accessToken_" << accessToken_ << " token_: " << token_ << " timestamp_: " << timestamp_ << " rndNonce_: " << rndNonce_;

    //cdm_license_requests_.push_back(cdmLicense);

    QString wv_server_url = "https://music.amazon.com/NA/api/dmls/";
    keys_loader->initCDMSession(init_data, wv_server_url, cdmLicense, &host_primise_id_);
#endif
}

QString AmazonMusicInterceptor::postChallenge(QByteArray& challenge) {
    qDebug() << "postChallenge: " << challenge;
    QString challengeBase64 = QString(challenge.toBase64()); // .replace("/", "%2F").replace("+", "%2B").replace("-", "%2D").replace("=", "%3D");
    QString tmp = "{\"DrmType\":\"WIDEVINE\",\"licenseChallenge\":\"" + challengeBase64 +"\",\"customerId\":\"" + customerId_ +"\",\"deviceToken\":{\"deviceTypeId\":\"A16ZV8BU3SN1N3\",\"deviceId\":\"" + deviceId_ + "\"},\"appInfo\":{\"musicAgent\":\"Maestro/1.0 WebCP/1.0.7627.0 (4f69-40d4-WebC-b765-c06ea)\"},\"Authorization\":\"Bearer " + accessToken_ + "\"}";
    challenge = tmp.toLocal8Bit();

    return "application/json";
}

QByteArray AmazonMusicInterceptor::extractLicense(const QByteArray& response) {
    qDebug() << "License response: " << response.constData();
    const auto r = QJsonDocument::fromJson(response).object();
    const auto license = r.value("license").toString();
    qDebug() << "extractLicense: " << license;
    return QByteArray::fromBase64(license.toLocal8Bit());
}

void AmazonMusicInterceptor::onKeysLoaded() {
    qDebug() << "onKeysLoaded tracks_count_: " << tracks_count_;
    if (--tracks_count_ <= 0) {
        //std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time);
        qDebug() << "all bitrates intercepted" << ". PAGE PARSING TIME(sec.): "; // << duration.count();
        emit metadataIntercepted(QUrl());
    }
    else {
        qDebug() << "bitrates to go: " << tracks_count_;
    }
}

