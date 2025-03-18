#include "applemusic_interceptor.h"
#include "get_utils.h"
#include <iostream>
#include <recode_op.h>

#include "mpd_parser_amazon.h"


AppleMusicInterceptor::AppleMusicInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam)
{
    setSignedIn(false); // todo:
    Q_INIT_RESOURCE(scripts);
}

AppleMusicInterceptor::~AppleMusicInterceptor() {
    qDebug() << "Interceptor: ~AppleMusicInterceptor";
}

void AppleMusicInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {
    /*if (!accessToken_.isEmpty()) {
        info.block(true);
        return;
    }*/

    info.setHttpHeader("User-Agent", USER_AGENT);

    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());
    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {    // script intercept
#ifdef _DEBUG 
        //QMetaObject::invokeMethod(this, "dumpOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif
    }
    else {  // data intercept  
        if (qstr.contains("amp-api.music.apple.com") && qstr.contains("catalog") && qstr.contains("include") && qstr.contains("artists")) {
            QMetaObject::invokeMethod(this, "requestMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        }
        else if (qstr.contains("i=") && qstr.contains("album") && mediaId_.isEmpty()) {
            parseMediaId(qstr);
        }
    }
}

Q_INVOKABLE void AppleMusicInterceptor::dumpOrigScript(const QUrl& url) {
#ifdef _DEBUG 
    if (!url.isLocalFile()) {
        auto script = new WebContent(this);
        script->setNetworkAccess(nam_);
        QObject::connect(script, &WebContent::finished, this, [=]() {
            //if (script->status() == Status.OK)
            {
                QString qstr = url.toEncoded();
                QFile   file(".\\applemusic_js\\" + QFileInfo(qstr).fileName());
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

void AppleMusicInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}

//QString getJsonVal(const QString& jsonStr, const QString& name) {
//    auto pos = jsonStr.indexOf("\"" + name + "\":"); // "\"accessToken\":"
//    if (pos > 0 && (pos = jsonStr.indexOf("\"", 3 + pos + name.size())) > 0) {
//        auto pos2 = jsonStr.indexOf("\"", pos + 1);
//        return jsonStr.mid(pos + 1, pos2 - pos - 1);
//    }
//
//    return QString();
//}

Q_INVOKABLE void AppleMusicInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    qDebug() << "interceptConfig confName: " << confName;
#ifdef _DEBUG
    QString file_name = QString("%1/%2/%3")
        .arg(QStandardPaths::writableLocation(QStandardPaths::DataLocation))
        .arg("FreeGrabApp").arg("apple_" + confName + ".json");
    //QString file_name = "apple_" + confName + ".json";
    QFile file(file_name);
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(jsonStr.toUtf8());
        file.close();
    }
#endif

    if (confName == "token" && !jsonStr.isEmpty() && token_.isEmpty()) {
        token_ = "Bearer " + jsonStr;
        qDebug() << token_;
    }
}

void AppleMusicInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    qDebug() << "start page ...";
    invalidate();

    media_user_token_.clear();
    token_.clear();
    trackUrl_.clear();
    mediaId_.clear();
    host_primise_id_ = -1;
    //cdm_license_requests_.clear();
//#ifdef DECRYPTSERVICE
//    keys_loaders_.clear();
//#endif
    manifest_ = nullptr;
    metadata_ = nullptr;
}

void AppleMusicInterceptor::finishPage(bool ok) {
    Q_UNUSED(ok);
}

void AppleMusicInterceptor::initialize() {
    qDebug() << "AppleMusicInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };

}
void AppleMusicInterceptor::parseMediaId(const QUrl& url)
{
    QUrlQuery query(url);
    mediaId_ = query.queryItemValue("i"); // ?i=1552019511
}

void AppleMusicInterceptor::requestMetadata(const QUrl& url) {
    if (token_.isEmpty())
        return;

    metadata_ = new WebContent(this);
    initRequest(metadata_);
    qDebug() << "metadata: " << url;

    QObject::connect(metadata_, &WebContent::finished, this, [this]() {
        const auto& content = metadata_->data().toUtf8();
#ifdef _DEBUG
        QFile   file("apple-meta.json");
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
        
        const auto data = r.value("data").toArray();
        if (!data.isEmpty()) {
            for (const auto m : data) {
                auto type = m.toObject()["type"].toString();
                if (type == "albums") {
                    auto attributes = m.toObject()["attributes"].toObject();
                    auto relationships = m.toObject()["relationships"].toObject();
                    
                    video_info_.metadata_album = m.toObject()["name"].toString();
                    if (!attributes.empty()) {
                        auto artwork = attributes["artwork"].toObject();
                        auto awurl = artwork["url"].toString();
                        // https://is1-ssl.mzstatic.com/image/thumb/Music115/v4/3d/f0/00/3df0007f-715d-fc36-e997-5805f01e535d/18558.jpg/{w}x{h}bb.{f}
                        awurl = awurl.replace("{w}", "200").replace("{h}", "200").replace("{f}", "webp");
                        // https://is1-ssl.mzstatic.com/image/thumb/Music115/v4/3d/f0/00/3df0007f-715d-fc36-e997-5805f01e535d/18558.jpg/200x200bb.webp
                        video_info_.artwork_url = awurl;
                        video_info_.metadata_artwork_url = video_info_.artwork_url;
                        video_info_.metadata_artist = attributes["artistName"].toString();
                    }
                    if (!relationships.isEmpty()) {
                        auto tracks = relationships["tracks"].toObject();
                        auto tracks_data = tracks["data"].toArray();
                        for (const auto t : tracks_data) {
                            auto track_id = t.toObject()["id"].toString();
                            if (track_id == mediaId_) {
                                auto t_type = m.toObject()["type"].toString(); // songs
                                auto t_attributes = m.toObject()["attributes"].toObject(); 
                                if (!t_attributes.isEmpty()) {
                                    video_info_.video_title = normalizeTitle(t_attributes.value("name").toString());
                                    video_info_.duration = tracks_info_.duration = t_attributes.value("durationInMillis").toInt() / 1000;
                                    auto trackNumber = t_attributes.value("trackNumber").toInt();
                                }
                            }
                        }
                    }

                    video_info_.enable_metadata = true;
                    video_info_.metadata_title = video_info_.video_title;

                    qDebug() << "title: " << video_info_.video_title << ", " << video_info_.duration << " sec. " << mediaId_;

                    //QMetaObject::invokeMethod(this, "requestManifest", Qt::QueuedConnection, Q_ARG(const QUrl&, QUrl()));
                    break;
                }
            }
        }

        if (!video_info_.video_title.isEmpty() && !tracks_info_.audio_tracks.isEmpty())
            emit metadataIntercepted(QUrl());

        metadata_ = nullptr;

        }, Qt::QueuedConnection);
    
    auto cookies = nam_->cookieJar()->cookiesForUrl(QUrl(".mediaauth.apple.com"));
    foreach(const auto & c, cookies)
        if (c.name() == "media-user-token") {
            media_user_token_ = c.value(); 
            break;
        }
    qDebug() << "media_user_token: " << media_user_token_;
    metadata_->add_header("authorization", token_);
    metadata_->add_header("media-user-token", media_user_token_);
    metadata_->reset();
    metadata_->fetch(url);
}

void AppleMusicInterceptor::loadKeys(const QString url) {
#ifdef DECRYPTSERVICE
    auto recode_op = std::make_shared<recode::RecodeOp>(url.toUtf8().constData(), "", nullptr);
    auto init_data = recode_op->GetInitData();

    //auto str = hexStr(init_data.data(), init_data.size());
    qDebug() << "init_data extracted " << init_data.size() << " bytes: " << url.toUtf8();
    //loadKeysWithPssh(init_data);
#endif
}


QString AppleMusicInterceptor::postChallenge(QByteArray& challenge) {
    qDebug() << "postChallenge: " << challenge;
    /*QString challengeBase64 = QString(challenge.toBase64()); // .replace("/", "%2F").replace("+", "%2B").replace("-", "%2D").replace("=", "%3D");
    QString tmp = "{\"DrmType\":\"WIDEVINE\",\"licenseChallenge\":\"" + challengeBase64 +"\",\"customerId\":\"" + customerId_ +"\",\"deviceToken\":{\"deviceTypeId\":\"A16ZV8BU3SN1N3\",\"deviceId\":\"" + deviceId_ + "\"},\"appInfo\":{\"musicAgent\":\"Maestro/1.0 WebCP/1.0.7627.0 (4f69-40d4-WebC-b765-c06ea)\"},\"Authorization\":\"Bearer " + accessToken_ + "\"}";
    challenge = tmp.toLocal8Bit();*/

    return "application/json";
}

QByteArray AppleMusicInterceptor::extractLicense(const QByteArray& response) {
    qDebug() << "License response: " << response.constData();
    const auto r = QJsonDocument::fromJson(response).object();
    const auto license = r.value("license").toString();
    qDebug() << "extractLicense: " << license;
    return QByteArray::fromBase64(license.toLocal8Bit());
}

void AppleMusicInterceptor::onKeysLoaded() {
    /*qDebug() << "onKeysLoaded tracks_count_: " << tracks_count_;
    if (--tracks_count_ <= 0) {
        //std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time);
        qDebug() << "all bitrates intercepted" << ". PAGE PARSING TIME(sec.): "; // << duration.count();
        emit metadataIntercepted(QUrl());
    }
    else {
        qDebug() << "bitrates to go: " << tracks_count_;
    }*/
}

