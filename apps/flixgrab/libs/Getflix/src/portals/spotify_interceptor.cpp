#include "spotify_interceptor.h"

#include <ctime>
#include <iostream>

#include <recode_op.h>

#include "get_utils.h"
#include "cdm_keys_loader.h"
#include "cdm_magickeys_client.h"


SpotifyInterceptor::SpotifyInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p) : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {

    Q_INIT_RESOURCE(scripts);
}

SpotifyInterceptor::~SpotifyInterceptor() {
    qDebug() << "Interceptor: ~SpotifyInterceptor";
}

void SpotifyInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {

    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    qDebug() << ">>>>>> " << QString::fromLatin1(info.requestMethod()) << ": " << qstr;
    
    info.setHttpHeader("User-Agent", USER_AGENT);
    
    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame)
        base_url_ = qstr;
    else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {
#ifdef _DEBUG 
        //QMetaObject::invokeMethod(this, "dumpOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif
        if (qstr.contains("/web-player.", Qt::CaseInsensitive)) {
            qDebug() << "Interceptor Script: " << qstr; // QFileInfo(qstr).fileName();
            QMetaObject::invokeMethod(this, "interceptVendorScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }
    }
    else {
        if (!base_url_.contains("/playlist/", Qt::CaseInsensitive)) { //single audio
            if (qstr.contains("?access_token=", Qt::CaseInsensitive)) {
                if (access_token_.isEmpty()) {
                    auto pos = qstr.indexOf("?access_token=") + 14;
                    access_token_ = "Bearer " + qstr.mid(pos);

                    qDebug() << "access_token: " << access_token_;
                    QString licenseUrl = "https://gew-spclient.spotify.com/melody/v1/license_url?keysystem=com.widevine.alpha&mediatype=audio&sdk_name=harmony&sdk_version=4.16.0";
                    QMetaObject::invokeMethod(this, "LoadLicenseUrl", Qt::QueuedConnection, Q_ARG(const QUrl&, licenseUrl));
                }
            }
            else if (qstr.contains("/license_url?", Qt::CaseInsensitive)
                && !access_token_.isEmpty()
                && wv_server_url_.isEmpty()
                && !cdm_url_->isValid()) {

                qDebug() << "LoadLicenseUrl... ";
                QMetaObject::invokeMethod(this, "LoadLicenseUrl", Qt::QueuedConnection, Q_ARG(const QUrl&, url)); //https://gew-spclient.spotify.com/melody/v1/license_url?keysystem=com.widevine.alpha&mediatype=audio&sdk_name=harmony&sdk_version=4.16.0
            }
        }
        else { //playlist
            if (qstr.contains("?access_token=", Qt::CaseInsensitive)) {
                if (access_token_.isEmpty()) {
                    auto pos = qstr.indexOf("?access_token=") + 14;
                    access_token_ = "Bearer " + qstr.mid(pos);
                }
            }
            else if (qstr.contains("/playlists/", Qt::CaseInsensitive)) {
                if (!access_token_.isEmpty()) {
                    qstr = qstr.split("?").first();
                    qstr.append("/tracks?offset=0&limit=100&additional_types=track%2Cepisode&market=from_token");
                    url.setUrl(qstr);

                    qDebug() << "Interceptor requestPlaylist: " << qstr;
                    QMetaObject::invokeMethod(this, "requestPlaylist", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
                    emit error(Status(EStatusCode::StopPage)); // stop all activity on the page
                }
            }
        }
    }
}

Q_INVOKABLE void   SpotifyInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    if (confName == "config") {
#ifdef _DEBUG
        QFile file("spotify-config_config.json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(jsonStr.toUtf8());
            file.close();
        }
#endif

        auto config = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
        if (access_token_.isEmpty())
            access_token_ = "Bearer " + config.value("accessToken").toString();
        //playlist_->set_authorization(access_token_);
        //state_->set_authorization(access_token_);

        // todo:
        //request.setRawHeader("spotify-app-version", "1.1.60.436.gdf2196c1");
        //request.setRawHeader("app-platform", "WebPlayer");
        //request.setRawHeader("accept-language", "ru");

        //todo:
        //const QString playListUrl = "https://api.spotify.com/v1/playlists/37i9dQZF1DX3rxVfibe1L0/tracks?offset=0&limit=100&additional_types=track%2Cepisode&market=RU";
        //QMetaObject::invokeMethod(this, "interceptPlaylist", Qt::QueuedConnection, Q_ARG(const QUrl&, playListUrl));
    }
    else if (confName == "newContentConfig:") { // deviceId
        qDebug() << " deviceId: " << jsonStr;
        //QString postData = "{\"seq_num\":1621342217415,\"state_ref\":{\"state_machine_id\":\"ChS6VXf66rCaRmxRNGRqoyGtEjGP4A\",\"state_id\":\"52354fc933f7f464e9a2883160f7c040\",\"paused\":true},\"sub_state\":{\"playback_speed\":0,\"position\":1837,\"duration\":172244,\"stream_time\":0,\"media_type\":\"AUDIO\",\"bitrate\":256000},\"debug_source\":\"before_track_load\"}";
        //// todo:
        //state_->post("https://gew-spclient.spotify.com/track-playback/v1/devices/"+ jsonStr.trimmed() +"/state", 
        //    postData.toUtf8(), 
        //    "application/json",
        //    2);        
    }
    else if (confName == "Final player config:") {
        // state machine:

#ifdef _DEBUG
        QFile file("Final player config.json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(jsonStr.toUtf8());
            file.close();
        }
#endif
        QString base_url = base_url_;
        if (base_url.isEmpty()
            && page_)
            base_url = page_->url().toEncoded();

        QString video_id = base_url.split("/").last().split("?").first();
        qDebug() << "base_url_: " << base_url << " video_id: " << video_id;

        if (base_url.contains("/playlist/", Qt::CaseInsensitive) //playlist case
            || video_id.isEmpty()
            || !tracks_info_.audio_tracks.isEmpty())
            return; // already loaded

        const auto jsonObj = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
        const auto items = jsonObj.value("tracks").toArray();
        if (!items.isEmpty()) {
            for (const auto item : items) {
                const auto metadata = item.toObject()["metadata"].toObject();

                QString uri = metadata.value("uri").toString();
                QString linked_from_uri = metadata.value("linked_from_uri").toString();
                QString context_uri = metadata.value("context_uri").toString();
                if ((uri.contains(video_id) || linked_from_uri.contains(video_id))
                    && context_uri.contains(video_id)) {

                    video_info_.duration = metadata.value("duration").toInt() / 1000;
                    //const auto group_name = metadata.value("group_name").toString();
                    video_info_.video_title = normalizeTitle(metadata.value("name").toString());
                    video_info_.enable_metadata = true;
                    video_info_.metadata_title = video_info_.video_title;
                    video_info_.metadata_comment = metadata.value("context_description").toString();

                    const auto authors = metadata.value("authors").toArray();
                    if (!authors.isEmpty()) {
                        for (const auto auth : authors) {
                            const auto authObj = auth.toObject();
                            QString author = authObj["name"].toString();
                            if (!author.isEmpty())
                                video_info_.metadata_artist.append(author).append(",");
                        }
                        int pos = video_info_.metadata_artist.lastIndexOf(QChar(','));
                        video_info_.metadata_artist = video_info_.metadata_artist.left(pos);
                    }

                    const auto images = metadata.value("images").toArray();
                    if (!images.isEmpty()) {
                        int min_width = 0;
                        int width = 0;
                        for (const auto img : images) {
                            const auto imgObj = img.toObject();
                            int w = imgObj["width"].toInt();
                            QString url = imgObj["url"].toString();

                            if (w > width) {
                                video_info_.artwork_url = url;
                                video_info_.metadata_artwork_url = video_info_.artwork_url;
                                width = w;
                            }
                        }
                    }
                    //if (!images.isEmpty()) {
                    //    int best_width = 0;
                    //    int best_width_low_quality = 0;

                    //    QList<int> widths;
                    //    for (const auto img : images) {
                    //        const auto imgObj = img.toObject();
                    //        widths << imgObj["width"].toInt();
                    //    }
                    //    if (!widths.isEmpty()) {
                    //        std::sort(widths.begin(), widths.end(), [](const int i, const int j) {
                    //            return i < j;
                    //        });

                    //        for (const auto width : widths) {
                    //            if (width <= 300)
                    //                best_width = width;
                    //            if (best_width_low_quality <= 280)
                    //                best_width_low_quality = width;
                    //        }
                    //    }

                    //    if (best_width > 0) {
                    //        for (const auto img : images) {
                    //            const auto imgObj = img.toObject();
                    //            int width = imgObj["width"].toInt();
                    //            if (best_width == width) {
                    //                video_info_.artwork_url = imgObj["url"].toString();
                    //                video_info_.artwork_width = width;
                    //                video_info_.metadata_artwork_url = video_info_.artwork_url;
                    //                qDebug() << "best_width" << best_width << " artwork_url: " << video_info_.artwork_url << " artwork_width: " << video_info_.artwork_width;
                    //            }
                    //        }
                    //    }
                    //    else {
                    //        const auto imgObj = images[0].toObject();
                    //        video_info_.artwork_url = imgObj["url"].toString();
                    //        video_info_.metadata_artwork_url = video_info_.artwork_url;
                    //        video_info_.artwork_width = imgObj["width"].toInt();
                    //        qDebug() << " artwork_url: " << video_info_.artwork_url << " artwork_width: " << video_info_.artwork_width;
                    //    }
                    //}

                    const auto manifest = item.toObject()["manifest"].toObject();
                    const auto file_ids_mp4 = manifest["file_ids_mp4"].toArray();
                    if (!file_ids_mp4.isEmpty()) {
                        tracks_count_ = file_ids_mp4.size();
                        qDebug() << "video_title: "<< video_info_.video_title << " duration: " << video_info_.duration << " sec., tracks: " << tracks_count_
                            << " metadata_comment: " << video_info_.metadata_comment << " metadata_artist: " << video_info_.metadata_artist
                            << " metadata_artwork_url: " << video_info_.metadata_artwork_url;

                        for (const auto file : file_ids_mp4) {
                            const auto fileObj = file.toObject();
                            const auto file_id = fileObj.value("file_id").toString();
                            const auto bitrate = fileObj.value("bitrate").toInt();
                            qDebug() << "file_id: " << file_id << " bitrate: " << bitrate;
                            loadTrackUrls(file_id, bitrate);
                        }

                    }
                }
            }
            //QString licenseUrl = "https://gew-spclient.spotify.com/melody/v1/license_url?keysystem=com.widevine.alpha&mediatype=audio&sdk_name=harmony&sdk_version=4.16.0"; //TODO
            //LoadLicenseUrl(licenseUrl);
            // all done have fun
            emit error(Status(EStatusCode::StopPage)); // stop all activity on the page
        }
    }
}

void SpotifyInterceptor::loadTrackUrls(const QString& file_id, int bitrate) {
    QString url = "https://gew-spclient.spotify.com/storage-resolve/v2/files/audio/interactive/11/";
    url += file_id;
    url += "?version=10000000&product=9&platform=39&alt=json";
    qDebug() << "loadUrls for file_id: " << file_id;

    auto webRequest = new WebContent(this);
    webRequest->setNetworkAccess(nam_);
    QObject::connect(webRequest, &WebContent::finished, this, [this, webRequest, bitrate, file_id] {
        if (webRequest->status().code() == EStatusCode::Success
            && tracks_count_ > 0) {
                
            AudioTrackInfo t;
            t.is_encrypted = true;
            t.bitrate = bitrate;
            const auto data = webRequest->data();

#ifdef _DEBUG
            QFile   file("track_" + file_id + ".json");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(data.toUtf8());
                file.close();
            }
#endif
            const auto jsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
            const auto items = jsonObj.value("cdnurl").toArray();
                
            if (!items.isEmpty()) {
                for (const auto url : items)
                    t.urls.push_back(url.toString());

                if (!t.urls.empty()) {
                    normalizeTrack(t);
                    normalizeAudioTrack(t);
                    tracks_info_.audio_tracks.push_back(t);
                    LoadKeysForFileId(file_id, t.urls[0]);
                    qDebug() << bitrate << " bps | file_id: " << file_id << "| url:" << t.urls[0];
                }
            }
        }
    });

    qDebug() << " authorization: " << access_token_;
    webRequest->add_header("authorization", access_token_);
    webRequest->fetch(url);
}

Q_INVOKABLE void SpotifyInterceptor::requestPlaylist(const QUrl& url) {
    qDebug() << "requestPlaylist access_token_: " << access_token_;

    if (access_token_.isEmpty()
        || playlist_ != nullptr)
        return;

    playlist_ = new WebContent(this);
    initRequest(playlist_);

    QObject::connect(playlist_, &WebContent::finished, this, [=]() {
        const auto& content = playlist_->data().toUtf8();
#ifdef _DEBUG
        QFile   file("spotify-playlist.json");
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

        const auto ps = QJsonDocument::fromJson(content).object();
        const auto items = ps.value("items").toArray();
        if (!items.isEmpty()) {
            QList<QString> urls;
            QString playlist;

            for (const auto item : items) {
                auto sharing_info = item.toObject()["sharing_info"].toObject();
                auto url = sharing_info.value("share_url").toString(); // https://open.spotify.com/track/5ivNZHQQddQve6Hnp52hNn?si=_AlFnnq6S2W7f4MO0B1SQQ&dl_branch=1

                qDebug() << "playlist item url: " << url;

                if (!url.isEmpty()
                    && !urls.contains(url)) {

                    playlist.append(url + "|");
                    urls << url;
                }
            }

            if (!playlist.isEmpty())
                emit loadPlaylist(playlist);

            playlist_ = nullptr;
        }
    }, Qt::QueuedConnection);

    playlist_->add_header("authorization", access_token_);
    playlist_->fetch(url);
}

Q_INVOKABLE void SpotifyInterceptor::dumpOrigScript(const QUrl& url) {
    if (!url.isLocalFile()) {
        auto script = new WebContent(this);
        script->setNetworkAccess(nam_);
        QObject::connect(script, &WebContent::finished, this, [=]() {
            //if (script->status() == Status.OK) {
                QString qstr = url.toEncoded();
                QFile   file(".\\spotify_js\\" + QFileInfo(qstr).fileName());
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0); // remove old content
                    file.write(script->data().toLocal8Bit());
                }
            //}
        });

#ifdef _DEBUG
        all_scripts_.push_back(script);
#endif
        script->fetch(url);
    }
}

void SpotifyInterceptor::PlayButtonClick() {
    const QString code = R"(
    var buttonFound = false; 
        console.log('searching for Play button...');
        setInterval(searchplaybutton, 1000);
        function searchplaybutton() {
        if(buttonFound)
            return;
        //var d=document.querySelector('div.player-controls__buttons>button');
        var d=document.querySelector('button[data-testid="play-button"]');
        if(d){
            console.log(' found divs: ' + d);
            if('function' == typeof d.click){
                //buttonFound=true;
                d.click();
                console.log('play clicked!');
                return;
            }
        }
        console.log('not found Play button');
        })";
    //qDebug() << "run script searching for Play button ...";
    page_->runJavaScript(code);
}

class VendorWebPlayerScript : public WebContent {
public:
    VendorWebPlayerScript(QObject* p = nullptr) : WebContent(p) {}
protected:

    Status  modifyContent() override {
        qDebug() << "Interceptor: vendorscript injection...";
#if _DEBUG
        QFile   jf("spotify-vendor.js");
        if (jf.open(QIODevice::ReadWrite)) {
            jf.resize(0);
            QTextStream(&jf) << mutableData() << endl;
            jf.close();
        }
#endif

        mutableData().insert(0, "function dumptxt(x){console.log(x);return !0;}dumptxt('run vendorscript!');");
        /*{
            auto size_bkup = mutableData().size();
            QRegExp reg("_parseDescriptor\\((\\w+)\\)\\s*\\{", Qt::CaseInsensitive);
            auto i = reg.indexIn(mutableData());
            if (i >= 0) {
                auto txt = reg.capturedTexts();
                if (txt.length() >= 1) {
                    mutableData().insert(i + txt[0].length(), "dumptxt('newContentConfig: ' + e.getId());");
                }
            }
            auto ok = size_bkup != data().size();
            qDebug() << "Injected  vendorscript.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << i;
        }*/
        /*{
            auto size_bkup = mutableData().size();
            QRegExp reg("this\\._localDeviceId\\=(\\w+)\\.id", Qt::CaseInsensitive);
            auto i = reg.indexIn(mutableData());
            if (i >= 0) {
                auto txt = reg.capturedTexts();
                if (txt.length() >= 1) {
                    mutableData().insert(i, "dumptxt('newContentConfig: ' + e.id),");
                }
            }
            auto ok = size_bkup != data().size();
            qDebug() << "Injected  vendorscript.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << i;
        }*/
        {
            //setStateMachine(e)
            //_handleStateUpdateResponse
            auto size_bkup = mutableData().size();
            QRegExp reg("setStateMachine\\(e\\)\\{", Qt::CaseInsensitive);
            auto i = reg.indexIn(mutableData());
            if (i >= 0) {
                auto txt = reg.capturedTexts();
                if (txt.length() >= 1) {
                    mutableData().insert(i + txt[0].length(), "dumptxt('Final player config: ' + JSON.stringify(e));");
                }
            }
            auto ok = size_bkup != data().size();
            qDebug() << "Injected  vendorscript.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << i;
        }
        //{
        //    // authenticate(e){
        //    auto size_bkup = mutableData().size();
        //    QRegExp reg("_parseProvidedToken\\(e\\)\\{", Qt::CaseInsensitive);
        //    auto i = reg.indexIn(mutableData());
        //    if (i >= 0) {
        //        auto txt = reg.capturedTexts();
        //        if (txt.length() >= 1) {
        //            auto pos = mutableData().indexOf("return this._lastToken");
        //            if(pos > 0)
        //                mutableData().insert(pos, "dumptxt('Player config parameter: ' + JSON.stringify(t));");
        //        }
        //    }
        //    auto ok = size_bkup != data().size();
        //    qDebug() << "Injected  vendorscript.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << i;
        //}

#if _DEBUG
        QFile   jfi("spotify-vendor-injected.js");
        if (jfi.open(QIODevice::ReadWrite)) {
            jfi.resize(0);
            QTextStream(&jfi) << mutableData() << endl;
            jfi.close();
        }
#endif
        return EStatusCode::Success;
    }

    bool    isModifyAsync() const override {
        return false;
    }
};

void SpotifyInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);

    qDebug() << "start page ...";
    start_time_ = std::chrono::system_clock::now();

    invalidate();
    vendor_script_->reset();
    playlist_ = nullptr;
    cdm_url_->reset();

    cdm_license_requests_.clear();
#ifdef _DEBUG
    all_scripts_.clear();
#endif
    base_url_.clear();
    wv_server_url_.clear();
    access_token_.clear();

    tracks_count_ = 0;

#ifdef DECRYPTSERVICE
    keys_loaders_.clear();
    host_primise_id_ = -1;
#endif
}

void SpotifyInterceptor::initialize() {
    qDebug() << "SpotifyInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
            });
    };

    vendor_script_ = new VendorWebPlayerScript(this);
    initContent(vendor_script_);

    //state_ = new WebContent(this);
    //initContent(state_);
    //QObject::connect(state_, &WebContent::finished, this, [=]() {
    //    qDebug() << "state loaded."; 
    //    parseState(state_->data(), state_->url());
    //}, Qt::QueuedConnection);

    cdm_url_ = new WebContent(this);
    initContent(cdm_url_);

    //track_ = new WebContent(this);
    //initContent(track_);
    //QObject::connect(track_, &WebContent::finished, this, [=]() {
    //    qDebug() << "track_ downloaded."; 
    //    
    //    QFile   file("track.mp4");
    //    if (file.open(QIODevice::ReadWrite)) {
    //        file.resize(0);
    //        file.write(track_->raw_data());
    //        file.close();
    //    }        

    //}, Qt::QueuedConnection);
}

Q_INVOKABLE void SpotifyInterceptor::interceptVendorScript(const QUrl& url) {
    Q_UNUSED(url);

    emit showWebEngine(ERequestType::showminimized);

    vendor_script_->fetch(url);
    scripts_.addAsync(vendor_script_);
    scripts_.run(page_);
    if (wv_server_url_.isEmpty())
        PlayButtonClick();
    emit hideWebEngine(ERequestType::unknown);
}

//QString SpotifyInterceptor::parseState(const QString& data, const QUrl& url) {
////#ifdef _DEBUG
//    QFile   file("spotify-state.json");
//    if (file.open(QIODevice::ReadWrite)) {
//        file.resize(0);
//        file.write(data.toUtf8());
//        file.close();
//    }
////#endif
//
//    const auto ps = QJsonDocument::fromJson(data.toUtf8()).object();
//    //const auto device = ps.value("device").toObject();
//    return "";
//}

void SpotifyInterceptor::LoadLicenseUrl(const QUrl& wvGetUrl) {
    //emit error(Status(Status::StopPage)); // stop all activity on the page
    if (cdm_url_->url().isEmpty()) {
        QObject::connect(cdm_url_, &WebContent::finished, this, [=]() {
            if (wv_server_url_.isEmpty()) {
                qDebug() << "get url for wv: " << cdm_url_->data();
                const auto r = QJsonDocument::fromJson(cdm_url_->data().toUtf8()).object();
                wv_server_url_ = "https://gew-spclient.spotify.com/" + r.value("uri").toString();
                qDebug() << "wv_server_url: " << wv_server_url_;
            }
            //LoadKeys();
        }, Qt::QueuedConnection);

        cdm_url_->add_header("authorization", access_token_);
        cdm_url_->fetch(wvGetUrl); // https://gew-spclient.spotify.com/melody/v1/license_url?keysystem=com.widevine.alpha&mediatype=audio&sdk_name=harmony&sdk_version=4.16.0
    }
}

void SpotifyInterceptor::LoadKeysForFileId(const QString& fileId, const QString& track_url) {
    QString manifest = "https://seektables.scdn.co/seektable/" + fileId + ".json";

    QPointer<WebContent> manifestRequest = new WebContent(this);
    manifestRequest->setNetworkAccess(nam_);
    QObject::connect(manifestRequest, &WebContent::finished, this, [=]() {
        if (!manifestRequest->status())
            emit error(manifestRequest->status());
#ifdef _DEBUG
        QFile   file("spotify-seektable.json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(manifestRequest->data().toUtf8());
            file.close();
        }
#endif
        auto jsonStr = QJsonDocument::fromJson(manifestRequest->data().toUtf8()).object();
        auto pssh_widevine = jsonStr.value("pssh_widevine").toString();
        if (pssh_widevine.isEmpty())
            pssh_widevine = jsonStr.value("pssh").toString();

        if (!pssh_widevine.isEmpty()) {
            qDebug() << "pssh_widevine: " << pssh_widevine;

#ifdef PRIVATE_KEYS_DECRYPTOR
            for (auto& a : tracks_info_.audio_tracks)
                if (track_url == a.urls[0]) {
                    a.cench_pssh = pssh_widevine; // bind pssh with track
                    qDebug() << a.bitrate << ": " << a.cench_pssh;
                }
            
            loadPrivateKeys(pssh_widevine);
#endif
            const auto r = QByteArray::fromBase64(pssh_widevine.toUtf8());
            std::vector<uint8_t> init_data(r.begin(), r.end());
            LoadKeysWithPssh(init_data);
            //auto str = hexStr(init_data.data(), init_data.size());
        }
        else {
            //pssh_widevine = jsonStr.value("pssh").toString();
            qDebug() << "Load pssh from track url ...";
            LoadKeys(track_url);
        }
    });

    manifestRequest->add_header("authorization", access_token_);
    manifestRequest->fetch(manifest);
    cdm_license_requests_.push_back(manifestRequest);
}

void SpotifyInterceptor::LoadKeys(const QString& url) {
#ifdef DECRYPTSERVICE
    if (!wv_server_url_.isEmpty()) {
        auto recode_op = std::make_shared<recode::RecodeOp>(url.toUtf8().constData(), "", nullptr);
        auto init_data = recode_op->GetInitData();

        //auto str = hexStr(init_data.data(), init_data.size());
        qDebug() << "init_data extracted " << init_data.size() << " bytes: " << url.toUtf8();
        LoadKeysWithPssh(init_data);
    }
#endif
}

void SpotifyInterceptor::LoadKeysWithPssh(const std::vector<uint8_t> init_data) {
#ifdef DECRYPTSERVICE
    if (!wv_server_url_.isEmpty()) {
        //if (keys_loader_ == nullptr)

        auto keys_loader = new CdmKeysLoader();
        keys_loaders_.push_back(keys_loader);

        QObject::connect(keys_loader, &CdmKeysLoader::keysLoaded, this, [this]() {
            InterlockedDecrement(&tracks_count_);
            onKeysLoaded();
        });

        QPointer<WebContent> cdmLicense = new WebContent(this);
        cdmLicense->setNetworkAccess(nam_);
        QObject::connect(cdmLicense, &WebContent::finished, this, [=]() {
            if (!cdmLicense->status())
                emit error(cdmLicense->status());
        });

        cdmLicense->add_header("authorization", access_token_);
        cdm_license_requests_.push_back(cdmLicense);
        keys_loader->initCDMSession(init_data, wv_server_url_, cdmLicense, &host_primise_id_);
        //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
#endif
}

void SpotifyInterceptor::loadPrivateKeys(const QString pssh) {
    auto key_client = new CdmMagicKeysClient();
    key_client_.push_back(key_client);

    //qDebug() << "load magicKeys ... " << pssh << "; access_token_: " << access_token_;
    ::InterlockedIncrement(&private_keys_count_);

    QObject::connect(key_client, &CdmMagicKeysClient::keysObtained, this, [this, pssh](const QList<QString>& kidKeys) {
        tracks_info_.setPrivateKeys(kidKeys, pssh);
        emit keysObtained(kidKeys, pssh);
        ::InterlockedDecrement(&private_keys_count_);
        onKeysLoaded();
    });

    QObject::connect(key_client, &CdmMagicKeysClient::error, this, [this](const Status& err) {
        qDebug() << "magicKeyServer error: " << err;
        ::InterlockedDecrement(&private_keys_count_);
        onKeysLoaded();
    });

    auto cdmLicensePrivate = new WebContent(this);
    cdmLicensePrivate->setNetworkAccess(networkAccess());
    cdmLicensePrivate->add_header("authorization", access_token_);
    cdmLicensePrivate_.push_back(cdmLicensePrivate);

    key_client->initCDMSession(pssh, wv_server_url_, cdmLicensePrivate, video_info_.video_title, 1024);
}

void SpotifyInterceptor::onKeysLoaded() {
    if (tracks_count_ <= 0 && private_keys_count_ <= 0) {
        qDebug() << "all bitrates intercepted";
        emit metadataIntercepted(this->url());
    }
    else
        qDebug() << "bitrates to go: " << tracks_count_ << ", private keys to go: " << private_keys_count_;
}

void SpotifyInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}
