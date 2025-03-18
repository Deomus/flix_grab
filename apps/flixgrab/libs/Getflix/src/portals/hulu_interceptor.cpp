#include "hulu_interceptor.h"

#include <ctime>

#include <decrypsis.h>
#include <recode_op.h>
#include <languages.h>

#include "get_utils.h"
#include "utils/qs_utils.h"
#include "cdm_magickeys_client.h"


//#define LOGIN_HULU_ALTERNATIVE
/*
1) extract eabId from given link. -> create url_deeplink -> fetch url_deeplink -> get eab.

2) Read json-config from html tag "__NEXT_DATA__". Extract "subscription". Construct content_config.

3) Interceptor APP -> Inject "eab" and "content_config" into app.js -> Run app.js.

4) Intercept Console log "Player config parameter". -> Parse widevine server url, and stream_url.

5) Interceptor Entity -> GET manifest -> parse_manifest -> Parse video_title,artwork.

6) Stop loading page.

7) GET stream_url -> Parse MPD -> CreateSessionAndGenerateRequest.

8) OnSessionMessage -> POST license request to widevine server.
*/


HuluInterceptor::HuluInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {
    Q_INIT_RESOURCE(scripts);
}

HuluInterceptor::~HuluInterceptor() {
    qDebug() << "Interceptor: ~HuluInterceptor";
}

void HuluInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {
    if (!wv_server_.isEmpty())
        info.block(true);
    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto m = QString::fromLatin1(info.requestMethod());

    info.setHttpHeader("User-Agent", USER_AGENT);

    if (qstr.contains("consumer.krxd.net", Qt::CaseInsensitive)
        || qstr.contains("doppler.hulu.com", Qt::CaseInsensitive)
        || qstr.contains("artwork")) {
        info.block(true);
        return;
    }
    if (m == "POST" &&
        (qstr.contains("event", Qt::CaseInsensitive) ||
            qstr.contains("jserrors", Qt::CaseInsensitive))) {
        info.block(true);
        //qDebug() << "block logging activity";
        return;
    }

    qDebug() << "Request url:" << m << ": " << qstr;

#ifdef LOGIN_HULU_ALTERNATIVE
    if (qstr.contains("login")) {
        qDebug() << "AUTH generate_csrf_value ...";
        // via HU: https://www.hulu.com/api/3.0/generate_csrf_value?for_hoth=true&path=/v2/web/password/authenticate
        // via FB: https://www.hulu.com/api/3.0/generate_csrf_value?for_hoth=true&path=/v2/web/facebook/authenticate
        generate_csrf_->fetch(QUrl("https://www.hulu.com/api/3.0/generate_csrf_value?for_hoth=true&path=/v2/web/password/authenticate"));
        //generate_csrf_->fetch(QUrl("https://www.hulu.com/api/3.0/generate_csrf_value?&for_hoth=true&path=/v1/web/profiles/switch"));
        emit error(Status(Status::StopPage));
    }
#endif

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
        base_url_ = qstr;
    }
    else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {
        qDebug() << "###### " << QString::fromLatin1(info.requestMethod()) << ": " << qstr;
#ifdef _DEBUG 
        //QMetaObject::invokeMethod(this, "interceptOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif
        if (qstr.contains("auth", Qt::CaseInsensitive) && qstr.contains("main-", Qt::CaseInsensitive) && !url.isLocalFile()) {
            //qDebug() << "Interceptor Script: " << qstr; // QFileInfo(qstr).fileName();
            //QMetaObject::invokeMethod(this, "interceptMainScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            //info.block(true);
        }
        //if (qstr.contains("main-84dd3589f7d337e4070f.js", Qt::CaseInsensitive) ) {
            //qDebug() << "Interceptor Script: " << qstr; // QFileInfo(qstr).fileName();
            //QMetaObject::invokeMethod(this, "interceptMainScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            //info.block(true);
        //} 
        if (qstr.contains("/app.js", Qt::CaseInsensitive) && !url.isLocalFile()) {
            // "https://player.hulu.com/site/dash/305715-site-curiosity/js/app.js"
            qDebug() << "Interceptor APP Script: " << qstr; // QFileInfo(qstr).fileName();
            QMetaObject::invokeMethod(this, "interceptAppScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }
        if (qstr.contains("load_player_build.js", Qt::CaseInsensitive) && !url.isLocalFile()) {
            qDebug() << "Interceptor Script: " << qstr; // QFileInfo(qstr).fileName();
            //QMetaObject::invokeMethod(this, "interceptLoaderScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            //info.block(true);
        }
        if (qstr.contains("/index.js", Qt::CaseInsensitive) && !url.isLocalFile()) {
            // "https://player.hulu.com/site/dash/305715-site-curiosity/js/app.js"
            qDebug() << "Interceptor INDEX Script: " << qstr; // QFileInfo(qstr).fileName();
            //QMetaObject::invokeMethod(this, "interceptIndexScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            //info.block(true);
        }
    }
    if (signed_in_) {
        if (qstr.contains("discover", Qt::CaseInsensitive)
            && qstr.contains("/content/", Qt::CaseInsensitive)
            && qstr.contains("/entity?", Qt::CaseInsensitive)) {
            qDebug() << "Interceptor Manifest: " << qstr; // https://discover.hulu.com/content/v3/entity?
            QMetaObject::invokeMethod(this, "interceptManifest", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        }
        else if (qstr.contains("/playlist", Qt::CaseInsensitive)
            && m == "POST"
            && signed_in_) {
            qDebug() << "Interceptor Playlist: " << qstr; // https://play.hulu.com/v6/playlist
            QMetaObject::invokeMethod(this, "interceptPlaylist", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        }
    }

    /*if (qstr.contains(".mpd", Qt::CaseInsensitive) && tracks_info_.video_tracks.length() == 0)
    {
        parseMPD(url);
    }*/
}

class AppHuluScript : public WebContent {
public:
    AppHuluScript(const QString& config, const QString& eab, QObject* p = nullptr) : WebContent(p), config_(config), eab_(eab) {}

protected:
    const QString& config_;
    const QString& eab_;

    Status  modifyContent() override {
        if (eab_.isEmpty()) {
            qDebug() << "ERROR: eab is empty.";
            //return Status::ParseError;
        }
        qDebug() << "Interceptor: app injection. eab: " << eab_ << "; config: " << config_;
        auto size_bkup = mutableData().size();

#ifdef _DEBUG
        QFile   file("app_hulu_script.js");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(mutableData().toLocal8Bit());
            file.close();
        }
#endif

        bool ok;
        // new Ke("hulu:client:playback:metadata:entity:error:invalid:eab_unavailable"); e='EAB::eabId::entityId::assetId';if(false)return qe.error = _t33,
        int j;
        auto i = data().indexOf("error:invalid:eab_unavailable");
        if (i > 0)
        {
            j = data().indexOf("return", i); // insert before return
            if (j > 0)
                mutableData().insert(j, "e='" + eab_ + "';if(false) ");
        }
        //mutableData().replace("e='EAB::eabId::entityId::assetId", "e='" + eab_, Qt::CaseSensitivity::CaseSensitive);
        ok = size_bkup != data().size();
        qDebug() << "Injected  eab in app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << j;

        //throw Ze(_e21, "hulu:client:playback:playlist:error:invalid-essential"), _e21 }   console.log("Player config parameter: " + JSON.stringify(e));
        size_bkup = data().size();
        //i = data().indexOf("error:invalid-essential");
        //if (i > 0)
        //{
        //    j = data().indexOf("}", i); // insert after }
        //    if (j > 0)
        //        mutableData().insert(j+1, " console.log('Player config parameter: ' + JSON.stringify(e));");
        //}
        i = data().indexOf("_processMetadata=");
        if (i > 0)
        {
            j = data().indexOf("{", i); // insert after {
            if (j > 0)
                mutableData().insert(j + 1, " console.log('Player config parameter: ' + JSON.stringify(e));");
        }

        ok = size_bkup != data().size();
        qDebug() << "Injected  log in app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << j;

        size_bkup = data().size();
        // window.DashPlayer.getSitePlayer=Ed ;console.log('app done.');const c_play=document.getElementById('dash-player-container');var config_intercept=JSON.parse('" + config_ + "'); var a_play=window.DashPlayer.getSitePlayer(c_play, config_intercept);a_play.playVideo();
        i = data().indexOf("window.DashPlayer.getSitePlayer=");
        if (i < 0)
            i = data().indexOf("window.DashPlayer.getSitePlayer");
        if (i > 0)
        {
            j = data().indexOf("}", i); // insert before }
            if (j > 0)
                mutableData().insert(j, ";console.log('app done.');const c_play=document.getElementById('dash-player-container');var config_intercept=JSON.parse('" + config_ + "'); var a_play=window.DashPlayer.getSitePlayer(c_play, config_intercept);a_play.playVideo(); ");
        }

        ok = size_bkup != data().size();
        qDebug() << "Injected conf in app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << j;
        //InjPlaylistLog(); // output playlist to log
        //InjDevCapLog(); // output Device Cap to log
        //InjDevCap(); // inject Device capabilities

        mutableData().replace(QString("robustness:\"\""), QString("robustness:\"HW_SECURE_DECODE\""));

#ifdef _DEBUG 
        QFile   jf("app_hulu_script_injected.js");
        if (jf.open(QIODevice::ReadWrite)) {
            jf.resize(0);
            QTextStream(&jf) << mutableData() << endl;
            jf.close();
        }
#endif

        return EStatusCode::Success;
    }

    //void InjPlaylistLog() {
    //    const auto size_bkup = data().size();
    //    int j = 0, i = data().indexOf("function _actualLoadAndDecryptPlaylist(e");
    //    if (i > 0) {
    //        j = data().indexOf("{", i); // insert after {
    //        if (j > 0)
    //            mutableData().insert(j + 1, "console.log('playlist:' + JSON.stringify(e)); ");
    //    }
    //    auto ok = size_bkup != data().size();
    //    qDebug() << "Injected pl log in app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << j;
    //}

    //void InjDevCapLog() {
    //    const auto size_bkup = data().size();
    //    int j = 0, i = data().indexOf("return dn.deviceCapabilities.");
    //    if (i > 0)
    //        mutableData().insert(j = i, "console.log('deviceCapabilities:' + JSON.stringify(dn.deviceCapabilities)); ");
    //    auto ok = size_bkup != data().size();
    //    qDebug() << "Injected DevCapLog in app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << j;
    //}

    //void InjDevCap() {
    //    const auto size_bkup = data().size();
    //    QRegExp reg("var (\\w+)=dn\\.deviceCapabilities\\.h264", Qt::CaseInsensitive);
    //    int j = 0, i = reg.indexIn(data());
    //    if (i >= 0) {
    //        auto txt = reg.capturedTexts();
    //        j = data().indexOf(";", i);
    //        if (j > 0 && txt.length() >= 1) {
    //            auto v = txt[1]; // the name of the variable containing deviceCapabilities h264
    //            mutableData().insert(j + 1, v + "={width:1920,height:1080,bitrate:11000000,framerate:60,profile:'HIGH',level:42};");
    //        }
    //        // it doesn't make sense to use higher resolution then 720p
    //        mutableData().replace(QString("1024:1280"), QString("1280:1280")); //1920
    //        mutableData().replace(QString("546:720"), QString("720:720")); // 1080

    //        //auto txt = reg.capturedTexts();
    //        //if (txt.length() >= 1) {
    //        //    auto v = txt[1]; // the name of the variable containing response
    //        //    j = data().indexOf(";", i);
    //        //    if (j > 0)
    //        //        mutableData().insert(j + 1, v + ".width=s=1920;u.height=l=1080;u.framerate=60;");
    //        //}
    //    }
    //    auto ok = size_bkup != data().size();
    //    qDebug() << "Injected DevCap in app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << j;
    //}

    bool    isModifyAsync() const override {
        return false;
    }
};

#ifdef _DEBUG 
Q_INVOKABLE void HuluInterceptor::interceptOrigScript(const QUrl& url)
{
    if (!url.isLocalFile()) {
        auto script = new WebContent(this);
        script->setNetworkAccess(nam_);
        QObject::connect(script, &WebContent::finished, this, [=]() {
            //if (script->status() == Status.OK)
            {
                QString qstr = url.toEncoded();
                QFile   file(".\\hulu_js\\" + QFileInfo(qstr).fileName());
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0); // remove old content
                    file.write(script->data().toLocal8Bit());
                }
            }
            });
        all_scripts_.push_back(script);
        script->fetch(url);
    }
}
#endif

Q_INVOKABLE void        HuluInterceptor::interceptMainScript(const QUrl& url) {
    Q_UNUSED(url);
    //main_script_->fetch(QUrl("qrc:///scripts/hulu/main-auth.js"));
    //main_script_->fetch(QUrl("qrc:///scripts/hulu/main-84dd3589f7d337e4070f.js"));
    //scripts_.addAsync(main_script_);
    //scripts_.run(page_);
}
Q_INVOKABLE void HuluInterceptor::interceptAppScript(const QUrl& url) {
    Q_UNUSED(url);
    app_script_->fetch(QUrl(url));
    //app_script_->fetch(QUrl("qrc:///scripts/hulu/app_v5.js"));
    scripts_.addAsync(app_script_);
    scripts_.run(page_);

    //qDebug() << "run " << app_script_->url();
    //page_->runJavaScript(app_script_->data());
    //qDebug() << "run done " << app_script_->url();
}

Q_INVOKABLE void HuluInterceptor::interceptLoaderScript(const QUrl& url) {
    Q_UNUSED(url);
    loader_script_->fetch(QUrl("qrc:///scripts/hulu/load_player_build.js"));
    scripts_.addAsync(loader_script_);
    scripts_.run(page_);
}

Q_INVOKABLE void HuluInterceptor::interceptIndexScript(const QUrl& url) {
    Q_UNUSED(url);
    //index_script_->fetch(QUrl(url));
    index_script_->fetch(QUrl("qrc:///scripts/hulu/index.js"));
    scripts_.addAsync(index_script_);
    scripts_.run(page_);
}

Q_INVOKABLE void   HuluInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    //if (ready_for_metadata_)
    //    return;
    qDebug() << "interceptConfig " << confName << " " << content_config_;

#ifdef _DEBUG
    QFile   jf("hulu-" + confName.left(14).trimmed() + ".json");
    if (jf.open(QIODevice::ReadWrite)) {
        jf.resize(0);
        QTextStream(&jf) << jsonStr << endl;
        jf.close();
    }
#endif
    if (confName == "props:") {
        if (!subscription_json_.isEmpty())
            return;
        //qDebug() << confName << " " << jsonStr.mid(0, 300);
        auto root = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
        auto props = root.value("props").toObject();
        auto initialState = props.value("initialState").toObject();
        auto pm = initialState.value("profile_manager").toObject();
        auto subscription = pm.value("subscription").toObject();
        //subscription_json_ = pm.value("subscription").toString();
        QJsonDocument docSub(subscription);
        subscription_json_ = docSub.toJson(QJsonDocument::Compact);
        qDebug() << "subscription: " << subscription_json_;
        // insert 'subscription' into content_config_
    // todo: read computerGuid from "guid" cookie.
        content_config_ = "{\"computerGuid\":\"1D8F5CABE269B7CC974A45EA0224F780\",\"latitude\":-1,\"longitude\":-1,\"isNonSub\":false,\"liveGuideEnabled\":true,\"isForcedNonSub\":false,\"siteAPI\":{}}";
        int pos = content_config_.lastIndexOf(QChar('}'));
        if (pos > 0) {
            content_config_.insert(pos, ",\"subscription\":" + subscription_json_);
            qDebug() << "content_config: " << content_config_;
        }
    }
    else if (confName == "Content config parameter:") {
        content_config_ = jsonStr;
        //app_script_->fetch(QUrl("qrc:///scripts/hulu/app_v4.js"));

        qDebug() << confName << " " << content_config_;
    }
    else if (confName == "Player config parameter:") {
        parse_playlist(jsonStr);
    }
}

Q_INVOKABLE void HuluInterceptor::interceptPlaylist(const QUrl& url) {
    QUrlQuery query_(url);
    QString guid = "FF4D9231AE9BA4870C4F6FA6C7A9C025"; // just in case..
    auto cookies = nam_->cookieJar()->cookiesForUrl(QUrl("https://www.hulu.com"));
    foreach(const auto & c, cookies)
        if (c.name() == "guid") {
            guid = c.value();
            break;
        }

    QString sdata = "{\"device_identifier\":\""
        + guid
        + "\",\"deejay_device_id\":214,\"version\":1,\"all_cdn\":true,\"content_eab_id\":\""
        + eab_
        + "\",\"region\":\"US\",\"xlink_support\":false,\"device_ad_id\":\"89BC5B5F-69D5-847E-EC7C-68C8C1F66936\",\"limit_ad_tracking\":false,\"ignore_kids_block\":false,\"language\":\"en\",\"guid\":\""
        + guid
        + "\",\"rv\":126634,\"kv\":469735,\"unencrypted\":true,\"include_t2_revenue_beacon\":\"1\",\"include_t2_rev_beacon\":true,\"include_t2_adinteraction_beacon\":true,\"cp_session_id\":\"24110B37-B450-321C-CF17-DB2EA26EC79E\",\"interface_version\":\"1.14.3\",\"network_mode\":\"wifi\",\"is_html5_player\":true,\"support_js\":true,\"support_vpaid\":true,\"play_intent\":\"resume\",\"playback\":{\"version\":2,\"video\":{\"codecs\":{\"values\":[{\"type\":\"H264\",\"width\":1920,\"height\":1080,\"framerate\":60,\"level\":\"4.2\",\"profile\":\"HIGH\"}],\"selection_mode\":\"ONE\"}},\"audio\":{\"codecs\":{\"values\":[{\"type\":\"AAC\"}],\"selection_mode\":\"ONE\"}},\"drm\":{\"values\":[{\"type\":\"WIDEVINE\",\"version\":\"MODULAR\",\"security_level\":\"L3\"},{\"type\":\"PLAYREADY\",\"version\":\"V2\",\"security_level\":\"SL2000\"}],\"selection_mode\":\"ALL\"},\"manifest\":{\"type\":\"DASH\",\"https\":true,\"multiple_cdns\":true,\"patch_updates\":true,\"hulu_types\":true,\"live_dai\":true,\"multiple_periods\":false,\"xlink\":false,\"secondary_audio\":true,\"unified_asset_signaling\":false,\"live_fragment_delay\":3},\"segments\":{\"values\":[{\"type\":\"FMP4\",\"encryption\":{\"mode\":\"CENC\",\"type\":\"CENC\"},\"https\":true}],\"selection_mode\":\"ONE\"}}}";

    playlist_->post(QUrl(url), sdata.toUtf8(), "application/json");
    // see parse_playlist
}

Q_INVOKABLE void HuluInterceptor::interceptManifest(const QUrl& url) {
    QUrlQuery query_(url);
    manifest_->fetch(QUrl(url));
    // see parse_manifest
}


Q_INVOKABLE void HuluInterceptor::interceptCollection(const QUrl& url) {
    collection_->fetch(QUrl(url));
    // see parse_manifest
}

Q_INVOKABLE void HuluInterceptor::interceptDeeplink(const QString& eabId) {
    url_deeplink_ = "https://discover.hulu.com/content/v5/deeplink/playback?namespace=entity&id=" + eabId + "&schema=0&referralHost=production";
    deeplink_->reset();
    deeplink_->fetch(url_deeplink_);
    // see parse_deeplink
}

void HuluInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    qDebug() << "start page ...";
    start_time_ = std::chrono::system_clock::now();
    invalidate();
    index_script_->reset();
    app_script_->reset();
    loader_script_->reset();
    //main_script_->reset();
    base_url_.clear();
    subscription_json_.clear();
    wv_server_.clear();
    eab_.clear();
    eabId_.clear();
    cdm_initializing_ = false;
    cdm_session_.clear();
    cdmLicense_->reset();
    cdmLicense2_->reset();
    manifest_->reset();
    playlist_->reset();
    collection_->reset();
    deeplink_->reset();
    url_deeplink_.clear();
    artwork_cache_->reset();
#ifdef LOGIN_HULU_ALTERNATIVE
    generate_csrf_->reset();
    auth_->reset();
#endif

    video_info_.playlist.clear();
    eab_extract();

    is_tracks_ready_ = false;
    is_video_info_ready_ = false;
    is_cdm_license_ready_ = false;
}

void HuluInterceptor::eab_extract() {
    auto qstr = page_->requestedUrl().toString();
    {
        foreach(const auto & pref, eab_source_) {
            if (qstr.contains(pref, Qt::CaseInsensitive)) {
                auto i = qstr.indexOf(pref);
                if (i > 0) {
                    eabId_ = qstr.mid(i + pref.length(), 36); // "2c4fca1a-406a-417d-b143-dd0c20a26d61"
                    qDebug() << "eabId: " << eabId_;
                    QMetaObject::invokeMethod(this, "interceptDeeplink", Qt::QueuedConnection, Q_ARG(const QString&, eabId_));
                }
                break;
            }
        }
        if (eabId_.isEmpty()) {
            if (qstr.contains("entity_id=", Qt::CaseInsensitive)) {
                auto i = qstr.indexOf("entity_id=");
                if (i > 0) {
                    eabId_ = qstr.mid(i + strlen("entity_id="), 36); // "2c4fca1a-406a-417d-b143-dd0c20a26d61"
                    qDebug() << "eabId: " << eabId_;
                    QMetaObject::invokeMethod(this, "interceptDeeplink", Qt::QueuedConnection, Q_ARG(const QString&, eabId_));
                }
            }
        }
    }
}

void HuluInterceptor::initialize() {
    qDebug() << "HuluInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
            });
    };

    //main_script_ = new WebContent(this);
    //initContent(main_script_);
    //main_script_->fetch(QUrl("qrc:///scripts/hulu/main-84dd3589f7d337e4070f.js"));

    app_script_ = new AppHuluScript(content_config_, eab_, this);
    initContent(app_script_);
    //#ifdef _DEBUG
    //    QObject::connect(app_script_, &WebContent::finished, this, [=]() {
    //        QFile   file("hulu_app_new.js");
    //        if (file.open(QIODevice::ReadWrite)) {
    //            file.resize(0);
    //            file.write(app_script_->data().toLocal8Bit());
    //        }}
    //    , Qt::QueuedConnection);
    //#endif

    loader_script_ = new WebContent(this);
    initContent(loader_script_);
#ifdef _DEBUG
    /*    QObject::connect(loader_script_ , &WebContent::finished, this, [=]() {
            QFile   file("hulu_loader_new.js");
            if (file.open(QIODevice::ReadWrite)) {
                file.write(loader_script_ ->data().toLocal8Bit());
            }}
        , Qt::QueuedConnection);*/
#endif
    index_script_ = new WebContent(this);
    initContent(index_script_);
#ifdef _DEBUG
    /*        QObject::connect(index_script_, &WebContent::finished, this, [=]() {
                QFile   file("hulu_index.js");
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0);
                    file.write(index_script_->data().toLocal8Bit());
                }}
            , Qt::QueuedConnection);*/
#endif
    artwork_cache_ = new WebContent(this);
    artwork_cache_->setNetworkAccess(nam_);

    // two license senders:
    cdmLicense_ = new WebContent(this);
    initContent(cdmLicense_);
    cdmLicense_->setNetworkAccess(NULL); // no cookies for license request
    QObject::connect(cdmLicense_, &WebContent::finished, this, [=]() {
        HuluInterceptor::onLoadResponseFromPRS(cdmLicense_);
    }, Qt::QueuedConnection);

    cdmLicense2_ = new WebContent(this);
    cdmLicense2_->setNetworkAccess(NULL); // no cookies for license request
    initContent(cdmLicense2_);
    QObject::connect(cdmLicense2_, &WebContent::finished, this, [=]() {
        HuluInterceptor::onLoadResponseFromPRS(cdmLicense2_);
    }, Qt::QueuedConnection);

    manifest_ = new WebContent(this);
    initContent(manifest_);
    QObject::connect(manifest_, &WebContent::finished, this, [=]() {
        parse_manifest(manifest_->data());
    }, Qt::QueuedConnection);

    playlist_ = new WebContent(this);
    initContent(playlist_);
    QObject::connect(playlist_, &WebContent::finished, this, [=]() {
        parse_playlist(playlist_->data());
    }, Qt::QueuedConnection);

    collection_ = new WebContent(this);
    initContent(collection_);
    QObject::connect(collection_, &WebContent::finished, this, [=]() {
        parse_collection(collection_->data());
    }, Qt::QueuedConnection);

    deeplink_ = new WebContent(this);
    //initContent(deeplink_);
    deeplink_->setNetworkAccess(nam_);
    QObject::connect(deeplink_, &WebContent::finished, this, [=]() {
        parse_deeplink(deeplink_->data());
        }, Qt::QueuedConnection);

#ifdef LOGIN_HULU_ALTERNATIVE
    auth_ = new WebContent(this);
    initContent(auth_);
    QObject::connect(auth_, &WebContent::finished, this, [=]() {
        if (!generate_csrf_->isValid() || !generate_csrf_->status())
            return;
        qDebug() << "AUTH done.";
        emit authenticated(QUrl("https://www.hulu.com/watch/" + eabId_));
        }
    , Qt::QueuedConnection);

    generate_csrf_ = new WebContent(this);
    initContent(generate_csrf_);
    QObject::connect(generate_csrf_, &WebContent::finished, this, [=]() {
        if (!generate_csrf_->isValid() || !generate_csrf_->status())
            return;
        QString auth_data = "csrf=";

        auto cookies = nam_.cookieJar()->cookiesForUrl(QUrl("https://www.hulu.com"));
        foreach(const auto& c, cookies)
            if (c.name() == "_tcv") {
                auth_data += c.value(); // csrf = _tcv;
                //nam_.cookieJar()->deleteCookie(c);
                break;
            }
        auth_data += "&user_email=freegrabapp%40gmail.com&password=P%40ssw0rd";
        qDebug() << "AUTH authenticate: " << auth_data;

        auth_->post(QUrl("https://auth.hulu.com/v2/web/password/authenticate"), auth_data.toLocal8Bit());
    }, Qt::QueuedConnection);
#endif
}

std::string HuluInterceptor::hexStr(const unsigned char* data, int len) {
    static const char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

void HuluInterceptor::parseMPD(const QUrl& url) {
    QFileInfo   mpd_info = url.path();
    QString content;
    qDebug() << "mpd url: " << url;

    if (tracks_info_.video_tracks.length() != 0
        || !downloadToString(url.toEncoded(), content)
        || !mpd_.read(content)) {
        qDebug() << "Failed to load mpd ";
        return;
    }

    qDebug() << "Parsed Manifest " << mpd_info.fileName();

    QList<int> video_bitrates;
    auto tracks = mpd_.get_tracks();
    if (video_info_.duration == 0
        && tracks.duration > 0)
        video_info_.duration = tracks.duration;

#ifdef PRIVATE_KEYS_DECRYPTOR
    tracks_info_.pssh = tracks.pssh;
    loadPrivateKeys(tracks_info_.pssh);
#endif

    for each (auto & v in tracks.video_tracks) {
        auto ti = v;
        ti.is_encrypted = true;

        ti.contains_audio = false;
        if (!video_bitrates.contains(ti.bitrate) || ti.bitrate == 0) {
            video_bitrates << ti.bitrate;
            normalizeTrack(ti);
            normalizeVideoTrack(ti);
            tracks_info_.video_tracks.push_back(ti);
        }
        for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
            qDebug() << "video (" << ti.size / 1024 / 1024 << " MB, " << video_info_.duration << " sec.): " << *o;
    }

    QList<int> audio_bitrates;
    for each (auto & v in tracks.audio_tracks) {
        auto ti = v;
        ti.is_encrypted = true;

        if (!audio_bitrates.contains(ti.bitrate) || ti.bitrate == 0) {
            audio_bitrates << ti.bitrate;
            normalizeTrack(ti);
            normalizeAudioTrack(ti);
            tracks_info_.audio_tracks.push_back(ti);
        }
#if _DEBUG
        for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
            qDebug() << "audio (" << ti.size / 1024 / 1024 << " MB): " << *o;
#endif
    }

    if (!tracks_info_.video_tracks.isEmpty())
        setFlag(kTracksReady);
    
    initCDMSession();
    
}

void HuluInterceptor::loadPrivateKeys(const QString pssh)
{
    qDebug() << "load magicKeys ...";
    cdmLicensePrivate_ = new WebContent(this);
    cdmLicensePrivate_->setNetworkAccess(NULL); // no cookies for license request

    key_client_ = new CdmMagicKeysClient();
    
    QObject::connect(key_client_, &CdmMagicKeysClient::keysObtained, this, [this, pssh](const QList<QString>& kidKeys) {
        tracks_info_.setPrivateKeys(kidKeys, pssh);
        emit keysObtained(kidKeys, pssh);
        setFlag(kPrivateKeysReady_);
    });
    
    QObject::connect(key_client_, &CdmMagicKeysClient::error, this, [this](const Status& err) {
        qDebug() << "magicKeyServer error: " << err;
        setFlag(kPrivateKeysReady_); // failed to get private keys, use cdm for decryption
    });

    key_client_->initCDMSession(pssh, wv_server_, cdmLicensePrivate_, video_info_.video_title, 2);
}

void HuluInterceptor::parseSubtitles(const QJsonObject& transcripts) {
    foreach(const QString & key, transcripts.keys()) {
        if (key == "webvtt") {
            QJsonObject langs = transcripts[key].toObject();
            foreach(const QString & lang, langs.keys()) {
                QString url = langs.value(lang).toString();
                QString lang_tag = lang.toLower();
                if (!url.isEmpty()) {
                    SubtitleTrackInfo track;
                    track.language_tag = SS2Q(normalizeLangTag(QS2S(lang_tag)));
                    track.profile = SubtitleTrackInfo::WEBVTT;
                    track.urls.push_back(url);
                    //normalizeTrack(track);
                    tracks_info_.subtitle_tracks.push_back(track);
                }
            }
        }
    }
}

void HuluInterceptor::parse_manifest(const QString& jsonStr) {
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("hulu-manifest_" + QString::number(ms) + ".json");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        QTextStream(&file) << jsonStr << endl;
        file.close();
    }
#endif

    auto root = QJsonDocument::fromJson(jsonStr.toUtf8()).object();

    auto type = root.value("_type").toString();
    if (type == "hub") {                                       // movie ?
        /*video_info_.video_title = normalizeTitle(root.value("name").toString());
        auto details = root.value("details").toObject();
        auto entity = details.value("entity").toObject();
        auto name = entity.value("name").toString();
        //"type": "movie",
        // "description":
        // "artwork":
        video_info_.video_title = normalizeTitle(name);
        video_info_.duration = entity.value("duration").toInt();*/
    }
    else if (type == "collection") {
        auto items = root.value("items").toArray();
        if (!items.isEmpty()) {
            for (const auto item : items) {
                auto type = item.toObject()["_type"].toString();

                video_info_.video_title = normalizeTitle(item.toObject()["name"].toString());
                video_info_.metadata_title = video_info_.video_title;
                video_info_.duration = item.toObject()["duration"].toInt();
                video_info_.metadata_comment = normalizeTitle(item.toObject()["description"].toString());
                video_info_.metadata_year = item.toObject()["premiere_date"].toString().split("-").first();

                if (type == "episode") {
                    auto bundle = item.toObject()["bundle"].toObject();
                    if (!bundle.isEmpty()) {
                        auto duration = bundle["duration"].toInt();
                        if (duration > 0)
                            video_info_.duration = duration;
                    }

                    QString title = item.toObject()["series_name"].toString();

                    int season_num = item.toObject()["season"].toString().toInt();
                    QString season_number = QString::number(season_num);
                    if (season_num < 10)
                        season_number = "0" + season_number;

                    int episode_num = item.toObject()["number"].toString().toInt();
                    QString episode_number = QString::number(episode_num);
                    if (episode_num < 10)
                        episode_number = "0" + episode_number;

                    QString movie_name = item.toObject()["name"].toString();
                    QString album_name = item.toObject()["series_name"].toString();

                    video_info_.metadata_title = movie_name;
                    video_info_.metadata_album = normalizeTitle(album_name);
                    video_info_.metadata_track = QString::number(episode_num) + "/" + QString::number(season_num);

                    if (season_num > 0
                        && episode_num > 0)
                        title = "[S" + season_number + ".E" + episode_number + "] " + album_name + " - " + movie_name;

                    video_info_.video_title = normalizeTitle(title);
                    auto artwork = item.toObject()["series_artwork"].toObject();
                    auto program_tile = artwork.value("program.tile").toObject();
                    video_info_.artwork_url = program_tile.value("path").toString() + "&operations=%5B%7B\"resize\":\"360x360%7Cmax\"%7D,%7B\"format\":\"jpeg\"%7D%5D";

                    //parse playlist
                    QString series_url = item.toObject()["href"].toString();
                    if (!series_url.isEmpty()
                        && video_info_.playlist.isEmpty()) {
                        QUrl url_col;
                        url_col.setQuery(query_);
                        QString url = series_url.split("?").first() + "/season/" + QString::number(season_num) + "?" + series_url.split("?").last() + "&limit=999";
                        url_col.setUrl(url);
                        QMetaObject::invokeMethod(this, "interceptCollection", Qt::QueuedConnection, Q_ARG(const QUrl&, url_col));
                    }
                }
                else if (type == "movie") {
                    video_info_.video_title = normalizeTitle(item.toObject()["name"].toString());

                    auto artwork = item.toObject()["artwork"].toObject();
                    auto program_tile = artwork.value("program.tile").toObject();
                    video_info_.artwork_url = program_tile.value("path").toString() + "&operations=%5B%7B\"resize\":\"360x360%7Cmax\"%7D,%7B\"format\":\"jpeg\"%7D%5D";
                }

                if (!video_info_.artwork_url.isEmpty())
                    artwork_cache_->fetch(video_info_.artwork_url); // Need to cache image.  
            }
        }

        if (!video_info_.video_title.isEmpty())
            setFlag(kVideoInfoReady_);
    }
    qDebug() << "video_title: " << video_info_.video_title << ". Artwork: " << video_info_.artwork_url;
}

void HuluInterceptor::parse_playlist(const QString& jsonStr)
{
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("hulu-playlist_" + QString::number(ms) + ".json");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(jsonStr.toUtf8());
        file.close();
    }
#endif
    if (!jsonStr.isEmpty()) {
         emit error(Status(EStatusCode::StopPage)); // stop loading page. Now we have everything.

        auto playerConfig = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
        auto video_metadata = playerConfig.value("video_metadata").toObject();
        auto length = video_metadata.value("length").toInt(); // ms
        video_info_.duration = length / 1000;

        wv_server_ = playerConfig.value("dash_wv_server").toString();
        if (wv_server_.isEmpty())
            wv_server_ = playerConfig.value("wv_server").toString();
        auto mpdUrl = playerConfig.value("stream_url").toString();
        parseMPD(mpdUrl);

        auto transcripts = playerConfig.value("transcripts_urls").toObject();
        if (!transcripts.isEmpty())
            parseSubtitles(transcripts);

        std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time_);
        qDebug() << "PAGE PARSING TIME(sec.): " << duration.count();
    }
}

void HuluInterceptor::parse_collection(const QString& jsonStr) {
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("hulu-collection_" + QString::number(ms) + ".json");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        QTextStream(&file) << jsonStr << endl;
        file.close();
    }
#endif

    auto root = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
    auto items = root.value("items").toArray();
    if (!items.isEmpty()) {
        QString playlist;
        QList<QString> ids;
        QString base = base_url_.mid(0, 1 + base_url_.lastIndexOf("/"));
        QString cur_id = base_url_.split("/").last();
        int season_number = -1;
        QList<QPair<QString, QString>> episodes;
        for (const auto item : items) {
            QString id = item.toObject()["id"].toString();
            if (!id.isEmpty()
                && !ids.contains(id)) {
                ids << id;

                QString name = item.toObject().value("name").toString();
                QString ep_title = normalizeTitle(name, 40);

                if (season_number < 0)
                    season_number = item.toObject().value("season").toString().toInt();

                QString ep_duration;
                auto bundle = item.toObject().value("bundle").toObject();
                if (!bundle.isEmpty()) {
                    int duration = bundle.value("duration").toInt();
                    ep_duration = QDateTime::fromTime_t(duration).toUTC().toString(duration < 3600 ? "mm:ss" : "hh:mm:ss");
                }

                QString ep_url = base + id;
                episodes.push_back(QPair(ep_title + " " + ep_duration, ep_url));
            }
        }

        if (!episodes.isEmpty())
            video_info_.playlist.push_back(QPair(season_number, episodes));
    }
}

void HuluInterceptor::parse_deeplink(const QString& jsonStr) {
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("hulu-deeplink-playback_" + QString::number(ms) + ".json");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        QTextStream(&file) << jsonStr << endl;
        file.close();
    }
#endif
    auto root = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
    eab_ = root.value("eab_id").toString(); //EAB::2c4fca1a-406a-417d-b143-dd0c20a26d61::61530815::94865852
    auto href = root.value("href").toString();
    auto href_type = root.value("href_type").toString();
    qDebug() << "eab: " << eab_ << ". href_type: " << href_type << ". href: " << href;

    //Manual manifest request:
    //QUrl manifest_url = "https://discover.hulu.com/content/v3/entity?language=en&device_context_id=1&eab_ids=" + eab_ + "&schema=4&referral_host=www.hulu.com";
    //manifest_->fetch(manifest_url);
}

void HuluInterceptor::initCDMSession() {
#ifdef DECRYPTSERVICE
    if (cdm_initializing_)
        return;

    cdmLicense_->invalidate();
    cdmLicense2_->invalidate();

    auto v = tracks_info_.video_tracks.constFirst(); // all tracks has the same keys
    auto recode_op = std::make_shared<recode::RecodeOp>(v.urls[0].toUtf8().constData(), "", nullptr);
    auto init_data = recode_op->GetInitData();
    auto str = hexStr(init_data.data(), init_data.size());
    qDebug() << "init_data extracted " << "(" << v.width << "x" << v.height << ") from: " << v.urls[0].toUtf8() << " | " << str.c_str();

    InstallDecrypsis();
    DecryptCreateSessionAndGenerateRequest(init_data.data(), init_data.size(), (SessionMessageCallback*)this, false);
    cdm_initializing_ = true;
#endif
}

void HuluInterceptor::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size) {
    Q_UNUSED(promise_id);
    Q_UNUSED(session_id);
    Q_UNUSED(session_id_size);
    qDebug() << "interceptor: OnResolveNewSessionPromise ...";
}

void HuluInterceptor::OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size) {
    auto end = std::chrono::system_clock::now();
    //std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    qDebug() << "OnSessionMessage. size:" << message_size; // << ": " << (byte)message[0] << (byte)message[1]; // << "" << std::ctime(&end_time);

    QPointer<WebContent> sender;
    if (cdmLicense_
        && !cdmLicense_->isValid())
        sender = cdmLicense_;
    else if (cdmLicense2_
        && !cdmLicense2_->isValid())
        sender = cdmLicense2_; //second request

    if (wv_server_.isEmpty())
        qDebug() << "ERROR: wv_server_ is not obtained!";

    cdm_session_ = QByteArray(session_id, session_id_size);
    QByteArray qbChallenge(message, (int)message_size); // "\b\u0004";
    QUrl url = wv_server_; // "https://play.hulu.com/license/v1/dash_wv_license/61430820?signature=1586395444_b9ec11ec2001d353c387481ed83e389815060a32";
    qDebug() << "send License Request to server: " << qbChallenge.size() << /*<< qbChallenge <<*/ " >> " << url.toString();
    sender->post(url, qbChallenge, "application/octet-stream");     // ! hulu license request
}

void HuluInterceptor::onLoadResponseFromPRS(QPointer<WebContent>& content) {
#ifdef DECRYPTSERVICE
    try {
        qDebug() << "licenseResponse size: " << content->raw_data().size(); // << " bytes: " << QString(content->raw_data());
        if (content->data().size() == 0)
            return;
        DecryptUpdateSession(cdm_session_.constData(), cdm_session_.size(), content->raw_data(), content->raw_data().size());
        setFlag(kCDMLicenseReady_);
    }
    catch (...) {
        qDebug() << "error loading From PRS.";
    }
#endif
}

void HuluInterceptor::setFlag(EInterceptFlags flag) {
    if (!is_tracks_ready_)
        is_tracks_ready_ = checkInterceptFlag(flag, kTracksReady);
    if (!is_video_info_ready_)
        is_video_info_ready_ = checkInterceptFlag(flag, kVideoInfoReady_);
    if (!is_cdm_license_ready_)
        is_cdm_license_ready_ = checkInterceptFlag(flag, kCDMLicenseReady_);
#ifdef PRIVATE_KEYS_DECRYPTOR
    if (!is_private_keys_ready_)
        is_private_keys_ready_ = checkInterceptFlag(flag, kPrivateKeysReady_);
#else
    is_private_keys_ready_ = true;
#endif
        ;
    if (is_tracks_ready_
        && is_video_info_ready_
        && is_cdm_license_ready_
        && is_private_keys_ready_)
        emit metadataIntercepted(this->url());
}

void HuluInterceptor::OnSessionKeysChange() {
    qDebug() << "got keys.";
}

void HuluInterceptor::OnRejectPromise() {
    qDebug() << "License request rejected.";
}