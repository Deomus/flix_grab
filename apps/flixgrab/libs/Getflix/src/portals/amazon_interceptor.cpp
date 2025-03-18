#include "amazon_interceptor.h"

#include <ctime>

#include <QtWebEngineWidgets>

#include <decrypsis.h>
#include <recode_op.h>

#include "get_utils.h"
#include "mpd_parser_amazon.h"


AmazonInterceptor::AmazonInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , page_(page) {

    Q_INIT_RESOURCE(scripts);
}

AmazonInterceptor::~AmazonInterceptor() {
    qDebug() << "Interceptor: ~AmazonInterceptor";
}

bool AmazonInterceptor::isClassic(const QString& marketplaceId) {
    return
        /*t.DE = */"A1PA6795UKMFR9" == marketplaceId ||
        /*t.JP = */"A1VC38T7YXB528" == marketplaceId ||
        /*t.UK = */"A1F83G8C2ARO7P" == marketplaceId ||
        /*t.US = */"ATVPDKIKX0DER" == marketplaceId || 
        marketplaceId.isEmpty();

    /*
    ROE_EU = "A3K6Y4MI8GDYMT",
    t.ROW_EU = "A2MFUE2XK8ZSSY",
    t.ROW_FE = "A15PK738MTQHSO",
    t.ROW_NA = "ART4WZ8MWBX2Y",
    t.DE = "A1PA6795UKMFR9",
    t.JP = "A1VC38T7YXB528",
    t.UK = "A1F83G8C2ARO7P",
    t.US = "ATVPDKIKX0DER"
    */
}

bool AmazonInterceptor::parReady(const QString& name, const QString& value) {
    if (!query_.hasQueryItem(name)) {
        qDebug() << "parReady: name" << name << "; value" << value;
        if (value.isEmpty())
            return false;
        query_.addQueryItem(name, value);
    }
    return true;
}

void AmazonInterceptor::interceptMetadata() {
    emit hideWebEngine(ERequestType::profile);

    setOtherQueryParams();

    res_req_url_.setQuery(query_);

    qDebug() << "interceptMetadata:" << res_req_url_;
    this->fetch(res_req_url_);
    emit error(Status(EStatusCode::StopPage)); // stop all activity on the page
}

void AmazonInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
    if (ready_for_metadata_ 
        && !res_req_url_.isEmpty()) {
        info.block(true);
        //qDebug() << "interceptRequest return" << " ready_for_metadata_: " << ready_for_metadata_ << " res_req_url_: " << res_req_url_;
        return; // no need qtweb from here
    }

    info.setHttpHeader("User-Agent", USER_AGENT);

    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());
    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
        if (qstr.contains("cookieprefs?ref_=portal_banner_all"))
            cookies_accepted_ = true;

        base_url_ = qstr;

        if (qstr.contains("primevideo"))
            res_req_url_.setHost("atv-ps.primevideo.com");
    }
    else if (signed_in_
        && info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeXhr) {
        qDebug() << "$$$$$$ " << qstr;

        if (qstr.contains("/cdp/usage/Clickstream")) {
            // url depends on region: atv-ps, atv-ps-eu, atv-ps-fe.
            // https://atv-ps.amazon.com/cdp/usage/Clickstream?deviceID=null&deviceTypeID=AOAGZA014O5RE&gascEnabled=false&marketplaceID=ATVPDKIKX0DER&uxLocale=en_US&firmware=DVWebNode
            auto i = qstr.indexOf("/cdp/usage/Clickstream", Qt::CaseInsensitive);
            res_req_url_ = qstr.mid(0, i) + "/cdp/catalog/GetPlaybackResources";

            auto tmp = QUrlQuery(url);
            if (tmp.hasQueryItem("marketplaceID"))
                marketplaceId_ = tmp.queryItemValue("marketplaceID");

            qDebug() << "res_req_url_: " << res_req_url_;
        }
        else if (qstr.contains("/action-impressions/") // https://fls-na.amazon.com/1/action-impressions/1/OE/aiv-web-player/action/HTML5Player
            && !ready_for_metadata_) {
            
            ready_for_metadata_ = true;

            query_ = QUrlQuery();

            auto tmp = QUrlQuery(url);
            if (tmp.hasQueryItem("instance"))
                clientId_ = tmp.queryItemValue("instance");
            parReady("clientId", clientId_);

            if (marketplaceId_.isEmpty()
                && tmp.hasQueryItem("marketplaceID"))
                marketplaceId_ = tmp.queryItemValue("marketplaceId");

            parReady("marketplaceID", marketplaceId_);

            QString asin = getAsin(base_url_);

            if (asin.size() < 20
                && !base_url_.contains("primevideo")) { //asin.size() == 26 -> trailer)
                asin_ = asin;
                parReady("asin", asin_);
            }

            if (!asin_.isEmpty()) {
                parReady("asin", asin_);
                qDebug() << "interceptMetadata /action-impressions/ marketplaceID: " << marketplaceId_ << "; asin: " << asin_ << "; clientId_: " << clientId_;
                interceptMetadata();
            }
        }
        /*else if (qstr.contains("GetPlaybackResources")
         && !qstr.contains("GetPlaybackResourcesTime")
         && !ready_for_metadata_) {
         res_req_url_ = url;
         ready_for_metadata_ = true;

         query_ = QUrlQuery(url);
         setOtherQueryParams();
         res_req_url_.setQuery(query_);

         qDebug() << "GetPlaybackResources:" << qstr;
         this->fetch(res_req_url_);
         //QMetaObject::invokeMethod(this, "interceptMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
         emit error(Status(EStatusCode::StopPage)); // stop all activity on the page
        }*/
    }
    else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript) {
        qDebug() << "###### " << qstr;
#ifdef _DUMP_ORIGINAL 
        QMetaObject::invokeMethod(this, "interceptOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif

        //if (false && qstr.contains("WebLoader", Qt::CaseInsensitive) && !url.isLocalFile()) {
        //    qDebug() << "Interceptor Script: " << qstr; // QFileInfo(qstr).fileName();
        //    QMetaObject::invokeMethod(this, "interceptWebLoaderScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        //    info.block(true);
        //}
    }

    /*if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript) {
    if (qstr.contains("ATVWebPlayer.js", Qt::CaseInsensitive) && !url.isLocalFile()) {
        qDebug() << "Interceptor Script " << qstr; //  QFileInfo(qstr).fileName();
    }
    //else if (qstr.contains("ClientSideMetrics", Qt::CaseInsensitive) && !url.isLocalFile())
    //{
    //  //qDebug() << "Interceptor: Script blocked: " << qstr;
    //  //info.block(true);
    //}
    }
    else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeXhr) {
        if (qstr.contains("aiv-web-player/1/OE", Qt::CaseInsensitive)
            //|| qstr.contains("Events/AV", Qt::CaseInsensitive)
             //|| qstr.contains("ReportEvent", Qt::CaseInsensitive)
            ) {
            //qDebug() << "Interceptor: blocked.";
            //info.block(true); // Block all logging activity
        }
    }

    if (qstr.contains("GetPlaybackResources", Qt::CaseInsensitive)
        && qstr.contains("PlaybackUrls", Qt::CaseInsensitive)) {
        //Found Video Description Url;
        //qDebug() << "Interceptor: Content Metadata: " << qstr;
        ///info.block(true);
        //QUrl url (qstr.replace("deviceVideoQualityOverride=SD", "deviceVideoQualityOverride=HD"));
        //if(!qstr.contains("deviceVideoQualityOverride"))
        //  QMetaObject::invokeMethod(this, "interceptMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    }
    else if (qstr.contains("GetPlaybackResources", Qt::CaseInsensitive)
        && qstr.contains("Widevine2License", Qt::CaseInsensitive)) {
        //desiredResources=Widevine2License
        //qDebug() << "Interceptor: Request Widevine2License "; // << qstr;
        //QMetaObject::invokeMethod(this, "interceptWidevine2License", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    }*/
}

void AmazonInterceptor::invalidate() {
    scripts_.clear();
    reset();
    video_info_ = {};
    tracks_info_ = {};
    mpd_url_.clear();

    audioLangAvail_.clear();
    audioLangLoaded_.clear();

    res_req_url_.clear();
    widevine_2_license_query_.clear();
    cdm_initializing_ = false;

    if (cdmLicense_)
        cdmLicense_->invalidate();
}


void AmazonInterceptor::setTrackInfo(const TracksInfo& t, const QString& track_base_url) {
    if (tracks_info_.key_id.length() == 0)
        tracks_info_.key_id = t.key_id;

    if (video_info_.duration == 0
        && t.duration > 0)
        video_info_.duration = t.duration;

    auto p = track_base_url.lastIndexOf("/");
    auto base_url = track_base_url.mid(0, p);

    if (tracks_info_.video_tracks.length() == 0) {
        foreach(const auto& v, t.video_tracks) {
            if (video_info_.video_title.isEmpty())
                break;

            auto ti = v;
            ti.contains_audio = false; // todo: ??
            // update url
            for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                *o = base_url + "/" + *o + "?amznDtid=" + deviceTypeID_;

            normalizeTrack(ti);
            normalizeVideoTrack(ti);
            tracks_info_.video_tracks.push_back(ti);
#if _DEBUG
            for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                qDebug() << "video (size: " << ti.size / 1024 / 1024 << " MB); bitrate: " << ti.bitrate << "; height: " << ti.height << ": " << *o;
#endif
        }
    }
    
    bool track_exists = false;
    foreach (const auto& a, t.audio_tracks) {
        if (video_info_.video_title.isEmpty())
            break;
        if (audioLangLoaded_.indexOf(a.audioTrackId) < 0)
            audioLangLoaded_.push_back(a.audioTrackId);

        auto ti = a;
        // update url
        for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
            *o = base_url + "/" + *o + "?amznDtid=" + deviceTypeID_;

        normalizeTrack(ti);
        normalizeAudioTrack(ti);

        track_exists = false;
        for (const auto& track : tracks_info_.audio_tracks) {
            if (track.language_tag == ti.language_tag
                && track.bitrate == ti.bitrate
                && track.size == ti.size
                && track.profile == ti.profile
                && track.is_assistive == ti.is_assistive)
                track_exists = true;
        }

        if (!track_exists)
            tracks_info_.audio_tracks.push_back(ti);
#if _DEBUG
        for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
            qDebug() << "audio audioTrackId: " << ti.audioTrackId << " language_tag: " << ti.language_tag << " bitrate: " << ti.bitrate << " size: " << ti.size << " profile: " << ti.profile << " : " << *o;
#endif
    }
}

const QString AmazonInterceptor::getNextAvailAudio() {
    foreach(const auto& t, audioLangAvail_)
        if (audioLangLoaded_.indexOf(t) < 0)
            return t;

    return QString();
}

bool AmazonInterceptor::isTrackReady() {
    bool tracks_ready = (audioLangAvail_.count() <= audioLangLoaded_.length() && !tracks_info_.video_tracks.isEmpty());
    qDebug() << "isTrackReady: " << tracks_ready << "; audioLangAvail_: " << audioLangAvail_.count() << "; audioLangLoaded_: " << audioLangLoaded_.length() ;

    return tracks_ready;
}

void AmazonInterceptor::parseMPD(/*const QString& url*/) {
    if (!mpd_url_.isEmpty()) {
        qDebug() << "Parsing MPD " << mpd_url_ << " ...";
        QString                     manifest;
        MpdParserAmazon             mpd_;
        if (downloadToString(mpd_url_, manifest)
            && isValid()
            && mpd_.read(manifest)) { // MPD contains video and audio info
            if (!isValid())
                return; // page was stopped
            
            qDebug() << "Parsed, pssh: " << mpd_.get_tracks().pssh << " | url:" << mpd_url_;
            setTrackInfo(mpd_.get_tracks(), mpd_url_);
        }
    }
}

void AmazonInterceptor::startPage(bool forced_reload_page) {
    qDebug() << "startPage " << (forced_reload_page ? "DowngradeToSd" : "") << " ..."; //HD content not allowed.Forcing restart for SD asset
    scripts_.clear();
    invalidate();
    res_req_url_.clear();
    widevine_2_license_query_.clear();

    query_.removeQueryItem("userWatchSessionId"); // this is enough when downgrading
    if (!forced_reload_page)
        query_.clear(); // new url
    ready_for_metadata_ = false;
    cdm_initializing_ = false;
    forced_reload_page_ = forced_reload_page;
    if (cdmLicense_)
        cdmLicense_->reset();
    webloader_script_->reset();
    html_->reset();
    asin_.clear();
    marketplaceId_.clear();
    mpd_url_.clear();
    season_urls_.clear();
    seasons_count_ = 0;
    is_tracks_ready_ = false;
    is_video_info_ready_ = false;
}

//Q_INVOKABLE void AmazonInterceptor::interceptWebLoaderScript(const QUrl& url) {
//    //webloader_script_->fetch(url);
//     
//    Q_UNUSED(url);
//    webloader_script_->fetch(QUrl("qrc:///scripts/amazon/WebLoader.js"));
//    scripts_.addAsync(webloader_script_);
//    scripts_.run(page_);
//}

#ifdef _DUMP_ORIGINAL
Q_INVOKABLE void AmazonInterceptor::interceptOrigScript(const QUrl& url) {
    if (!url.isLocalFile()) {
        auto script = new WebContent(this);
        script->setNetworkAccess(networkAccess());
        QObject::connect(script, &WebContent::finished, this, [=]() {
            //if (script->status() == Status.OK)
            {
                auto idx = script->data().indexOf("has been blocked", 0, Qt::CaseInsensitive);
                QString qstr = url.toEncoded();
                auto fileName = ".\\amazon_js\\" + QFileInfo(qstr).fileName();
                int i = 0;
                while (QFile::exists(fileName))
                    fileName = ".\\amazon_js\\" + QString(++i) + QFileInfo(qstr).fileName();
                auto file = QFile(fileName);
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0); // remove old content
                    file.write(script->data().toLocal8Bit());
                }
                if (idx >= 0)
                    qDebug() << "FOUND string in " << qstr;
            }
            });
        script->fetch(url);
    }
}
#endif

const QString AmazonInterceptor::getAsin(const QString& url) {
    // extract asin from url:
    auto detFound = false;
    foreach (const auto& p, url.split("/")) {
        if (detFound)
            return p.split("?").first();
        detFound = p.compare("detail", Qt::CaseInsensitive) == 0;
    }
}

void AmazonInterceptor::setOtherQueryParams() {
    if (marketplaceId_.isEmpty()
        && query_.hasQueryItem("marketplaceID"))
        marketplaceId_ = query_.queryItemValue("marketplaceID");
    parReady("gascEnabled", isClassic(marketplaceId_) ? "false" : "true"); // gascEnabled: u.isClassic(this.marketplaceId) ? "false" : "true",
    parReady("marketplaceID", marketplaceId_);
    parReady("consumptionType", "Streaming");
    parReady("operatingSystemName", "Windows");
    parReady("operatingSystemVersion", "10.0");
    parReady("videoMaterialType", "Feature");
    parReady("playerType", "html5");
    parReady("firmware", "1");
    parReady("deviceDrmOverride", "CENC");
    parReady("deviceStreamingTechnologyOverride", "DASH");
    parReady("deviceTypeID", deviceTypeID_);
    parReady("userWatchSessionId", QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces));
    parReady("deviceID", "null"); // QUuid::createUuid().toString()
    // parReady("uxLocale", "en_US"); // ??

    parReady("displayWidth", "2880");
    parReady("displayHeight", "1800");
    // parReady("supportsVariableAspectRatio", "false"); // ?

    if (forced_reload_page_)
        parReady("deviceVideoQualityOverride", "SD"); //HD content not allowed.Forcing restart for SD asset

    widevine_2_license_query_ = query_;
    widevine_2_license_query_.addQueryItem("desiredResources", "Widevine2License");
    widevine_2_license_query_.addQueryItem("resourceUsage", "ImmediateConsumption");

    query_.addQueryItem("desiredResources", "PlaybackUrls%2CCatalogMetadata%2CForcedNarratives%2CSubtitlePresets%2CSubtitleUrls%2CPlaybackSettings");
    query_.addQueryItem("resourceUsage", "CacheResources"); //?
    query_.addQueryItem("deviceProtocolOverride", "Https");
    query_.addQueryItem("supportedDRMKeyScheme", "DUAL_KEY");
    query_.addQueryItem("liveManifestType", "live%2Caccumulating"); // live,accumulating
    query_.addQueryItem("deviceBitrateAdaptationsOverride", "CVBR%2CCBR");  // CVBR,CBR
    query_.addQueryItem("titleDecorationScheme", "primary-content");
    query_.addQueryItem("subtitleFormat", "TTMLv2");
    query_.addQueryItem("languageFeature", "MLFv2");
    query_.addQueryItem("playbackSettingsFormatVersion", "1.0.0");
    query_.addQueryItem("playerAttributes", "%7B%22frameRate%22%3A%22HFR%22%7D"); //  {"frameRate": "HFR"}
    //query_.addQueryItem("playerAttributes", "%7B%22middlewareName%22%3A%22Firefox%22%2C%22middlewareVersion%22%3A%2288.0.1%22%2C%22nativeApplicationName%22%3A%22Firefox%22%2C%22nativeApplicationVersion%22%3A%2288.0.1%22%2C%22supportedAudioCodecs%22%3A%22AAC%2CAC3%22%2C%22frameRate%22%3A%22HFR%22%2C%22H264.codecLevel%22%3A%224.2%22%2C%22H265.codecLevel%22%3A%220.0%22%7D"); //  {"middlewareName":"Firefox","middlewareVersion":"88.0.1","nativeApplicationName":"Firefox","nativeApplicationVersion":"88.0.1","supportedAudioCodecs":"AAC,AC3","frameRate":"HFR","H264.codecLevel":"4.2","H265.codecLevel":"0.0"}
    //query_.addQueryItem("deviceApplicationName", "Firefox");
    //query_.addQueryItem("deviceHdrFormatsOverride", "Hdr10");
}

Q_INVOKABLE void AmazonInterceptor::interceptConfig(const QString& asin, const QString& marketplaceID) { //amazon prime case and amazon.com trailser case
    qDebug() << "amazon interceptConfig asin_:" << asin_ << "; asin: " << asin << "; marketplaceID: " << marketplaceID;

    if (!marketplaceID.isEmpty()) {
        marketplaceId_ = marketplaceID;
        parReady("marketplaceID", marketplaceId_);
    }

    if (!asin.isEmpty()) {
        asin_ = asin;
        parReady("asin", asin_);

        if (ready_for_metadata_) {
            qDebug() << "interceptMetadata interceptConfig marketplaceID: " << marketplaceId_ << "; asin: " << asin_ << "; clientId_: " << clientId_;;
            interceptMetadata();
        }
    }
//    if (ready_for_metadata_)
//        return;
//#ifdef _DEBUG
//    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
//    QFile   file("amz-" + QString::number(ms) + ".json");
//    if (file.open(QIODevice::ReadWrite)) {
//        file.resize(0);
//        QTextStream(&file) << jsonStr << endl;
//        file.close();
//    }
//#endif
//    auto playerConfig = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
//
//    auto partnerServiceEndpoint = playerConfig.value("partnerServiceEndpoint").toString(); // "https://atv-ps-eu.primevideo.com"
//    if (!partnerServiceEndpoint.isEmpty())
//        res_req_url_ = partnerServiceEndpoint + "/cdp/catalog/GetPlaybackResources?";
//
//    bool par_ready = true;
//    par_ready &= parReady("sessionId", playerConfig.value("sessionId").toString());
//    par_ready &= parReady("uxLocale", confName == "Player config parameter:" ? playerConfig.value("locale").toString() : QString()); // en_US
//    par_ready &= parReady("clientId", playerConfig.value("clientId").toString());
//    par_ready &= parReady("marketplaceID", playerConfig.value("marketplaceId").toString());
//    par_ready &= parReady("deviceID", playerConfig.value("deviceId").toString());
//    par_ready &= parReady("customerId", playerConfig.value("customerId").toString());
//    par_ready &= parReady("asin", playerConfig.value("asin").toString()); //  "amzn1.dv.gti.4eb7121f-5804-fe53-95c0-d650f7128588"
//
//    auto configStore = playerConfig.value("configurationStore").toObject();
//    if (!configStore.isEmpty())
//        par_ready &= parReady("userWatchSessionId", configStore.value("value").toObject().value("userWatchSessionId").toString());
//    //auto primitiveSessionId = configStoreVal.value("primitiveSessionId").toString();
//
//    qDebug() << "intercepted Config " << (par_ready ? "Ready: " : "Not ready: ") << query_.toString();
//
//    if (ready_for_metadata_ = (par_ready && !res_req_url_.isEmpty())) {
//        emit error(Status(EStatusCode::StopPage)); // stop all activity on the page
//
//        qDebug() << "Ready to query metadata";
//        // common parameters for both widevine and urls:
//        const auto marketplaceId = query_.queryItemValue("marketplaceID");
//        parReady("gascEnabled", isClassic(marketplaceId) ? "false" : "true"); // gascEnabled: u.isClassic(this.marketplaceId) ? "false" : "true",
//        parReady("consumptionType", "Streaming");
//        parReady("operatingSystemName", "Windows");
//        parReady("operatingSystemVersion", "10.0");
//        parReady("videoMaterialType", "Feature");
//        parReady("playerType", "html5");
//        parReady("firmware", "1");
//        parReady("deviceDrmOverride", "CENC");
//        parReady("deviceStreamingTechnologyOverride", "DASH");
//        parReady("deviceTypeID", deviceTypeID_);
//        if (forced_reload_page_)
//            parReady("deviceVideoQualityOverride", "SD"); //HD content not allowed.Forcing restart for SD asset
//
//        if (!isValid()) {
//            begin_metadata_ = std::chrono::steady_clock::now();
//            QUrl url_l = res_req_url_;
//            QUrlQuery q(url_l);
//            setDeviceTypeId(q.queryItemValue("deviceTypeID"));
//
//            QUrlQuery query = query_;
//            query.addQueryItem("desiredResources", "Widevine2License");
//            query.addQueryItem("resourceUsage", "ImmediateConsumption"); //?
//            widevine_2_license_query_ = query;
//
//            query = query_;
//            query.addQueryItem("desiredResources", "PlaybackUrls%2CCatalogMetadata%2CForcedNarratives%2CSubtitlePresets%2CSubtitleUrls%2CPlaybackSettings");
//            query.addQueryItem("resourceUsage", "CacheResources"); //?
//            query.addQueryItem("deviceProtocolOverride", "Https");
//            query.addQueryItem("supportedDRMKeyScheme", "DUAL_KEY");
//            query.addQueryItem("liveManifestType", "live%2Caccumulating"); // live,accumulating
//            query.addQueryItem("deviceBitrateAdaptationsOverride", "CVBR%2CCBR");  // CVBR,CBR
//            query.addQueryItem("titleDecorationScheme", "primary-content");
//            query.addQueryItem("subtitleFormat", "TTMLv2");
//            query.addQueryItem("languageFeature", "MLFv2");
//            query.addQueryItem("playbackSettingsFormatVersion", "1.0.0");
//            query.addQueryItem("playerAttributes", "%7B%22frameRate%22%3A%22HFR%22%7D"); //  {"frameRate": "HFR"}
//
//            url_l.setQuery(query);
//            QMetaObject::invokeMethod(this, "interceptMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, url_l));
//        }
//    }
}
//
//Q_INVOKABLE void AmazonInterceptor::interceptMetadata(const QUrl& url) {
//    if (!isValid()) {
//        begin_metadata_ = std::chrono::steady_clock::now();
//        qDebug() << "Start quering metadata ...";
//        QUrl url_l = url;
//        QUrlQuery q(url_l);
//        setDeviceTypeId(q.queryItemValue("deviceTypeID"));
//
//        QUrlQuery query;
//        query.addQueryItem("asin", q.queryItemValue("asin"));
//        query.addQueryItem("consumptionType", "Streaming");
//        query.addQueryItem("desiredResources", "Widevine2License");
//        query.addQueryItem("deviceID", q.queryItemValue("deviceID"));
//        query.addQueryItem("deviceTypeID", "AOAGZA014O5RE"); // e.SILVERLIGHT_PC = "A63V4FRV3YUP9", e.SILVERLIGHT_MAC = "A2G17C9GWLWFKO", e.HTML5 = "AOAGZA014O5RE"
//        query.addQueryItem("firmware", "1");
//        query.addQueryItem("gascEnabled", q.queryItemValue("gascEnabled"));
//        query.addQueryItem("marketplaceID", q.queryItemValue("marketplaceID"));
//        query.addQueryItem("resourceUsage", "ImmediateConsumption"); //?
//        query.addQueryItem("userWatchSessionId", q.queryItemValue("userWatchSessionId"));
//        query.addQueryItem("videoMaterialType", "Feature");
//        query.addQueryItem("playerType", "html5");
//        query.addQueryItem("clientId", q.queryItemValue("clientId"));
//        query.addQueryItem("operatingSystemName", "Windows");
//        query.addQueryItem("operatingSystemVersion", q.queryItemValue("operatingSystemVersion"));
//        if (forced_reload_page_)
//            query.addQueryItem("deviceVideoQualityOverride", "SD"); //HD content not allowed.Forcing restart for SD asset
//        query.addQueryItem("deviceDrmOverride", "CENC");
//        query.addQueryItem("deviceStreamingTechnologyOverride", "DASH");
//        widevine_2_license_query_ = query;
//
//        query.clear();
//        query.addQueryItem("asin", q.queryItemValue("asin"));
//        query.addQueryItem("consumptionType", "Streaming");
//        query.addQueryItem("desiredResources", "PlaybackUrls%2CCatalogMetadata%2CForcedNarratives%2CSubtitlePresets%2CSubtitleUrls%2CPlaybackSettings");
//        query.addQueryItem("deviceID", q.queryItemValue("deviceID"));
//        query.addQueryItem("deviceTypeID", "AOAGZA014O5RE"); // e.SILVERLIGHT_PC = "A63V4FRV3YUP9", e.SILVERLIGHT_MAC = "A2G17C9GWLWFKO", e.HTML5 = "AOAGZA014O5RE"
//        query.addQueryItem("firmware", "1");
//        query.addQueryItem("gascEnabled", q.queryItemValue("gascEnabled"));
//        query.addQueryItem("marketplaceID", q.queryItemValue("marketplaceID"));
//        query.addQueryItem("resourceUsage", q.queryItemValue("resourceUsage")); //?
//        query.addQueryItem("userWatchSessionId", q.queryItemValue("userWatchSessionId"));
//        query.addQueryItem("videoMaterialType", "Feature");
//        query.addQueryItem("playerType", "html5");
//        query.addQueryItem("clientId", q.queryItemValue("clientId"));
//        query.addQueryItem("operatingSystemName", "Windows");
//        query.addQueryItem("operatingSystemVersion", q.queryItemValue("operatingSystemVersion"));
//        if (forced_reload_page_)
//            query.addQueryItem("deviceVideoQualityOverride", "SD"); //HD content not allowed.Forcing restart for SD asset
//        query.addQueryItem("deviceDrmOverride", "CENC");
//        query.addQueryItem("deviceStreamingTechnologyOverride", "DASH");
//        query.addQueryItem("deviceProtocolOverride", "Https");
//        query.addQueryItem("supportedDRMKeyScheme", "DUAL_KEY");
//        query.addQueryItem("liveManifestType", "live%2Caccumulating"); // live,accumulating
//        query.addQueryItem("deviceBitrateAdaptationsOverride", "CVBR%2CCBR");  // CVBR,CBR
//        query.addQueryItem("titleDecorationScheme", "primary-content");
//        query.addQueryItem("subtitleFormat", "TTMLv2");
//        query.addQueryItem("languageFeature", "MLFv2");
//        query.addQueryItem("uxLocale", q.queryItemValue("uxLocale"));
//        query.addQueryItem("playbackSettingsFormatVersion", "1.0.0");
//        query.addQueryItem("playerAttributes", "%7B%22frameRate%22%3A%22HFR%22%7D"); //  {"frameRate": "HFR"}
//
//        url_l.setQuery(query);
//        fetch(url_l);
//    }
//}

Q_INVOKABLE void AmazonInterceptor::interceptHTML(const QUrl& url) {
    html_->fetch(url);
}

Q_INVOKABLE void AmazonInterceptor::interceptSeason(const QUrl& url) {
    QPointer<WebContent> season = new WebContent(this);
    season->setNetworkAccess(networkAccess());
    QObject::connect(season, &WebContent::finished, this, [=]() {
        qDebug() << "season downloaded:" << season->url() << " status:" << season->status();
        
        if (!season->status())
            emit error(season->status());
        else
            parseSeason(season->data());
    }, Qt::QueuedConnection);

    season->add_header("Referer", base_url_);
    QUrl url_many_episodes = url;
    QUrlQuery query_many_episodes;
    query_many_episodes.addQueryItem("episodeListSize", "999");
    url_many_episodes.setQuery(query_many_episodes);
    season->fetch(url_many_episodes);
}

//Q_INVOKABLE void AmazonInterceptor::loadWidevine2License(const QUrl& url, const QString& post_data) {
//    Widevine2LicenseUrl_ = url;
//}

/*
Q_INVOKABLE void AmazonInterceptor::interceptDeferScript(const QUrl& url) {

    WebContent* script = nullptr;
    for (auto s : defer_scripts_) {
        if (s->url() == url) {
            script = s;
            break;
        }
    }

    if (script == nullptr) {
        script = new WebContent(this);
        script->setNetworkAccess(&nam_);
        QObject::connect(script, &WebContent::finished, this, [=]() {
            if (!script->status())
                emit error(script->status());
        });
        defer_scripts_.push_back(script);
    }
    script->fetch(url);
    scripts_.addDefer(script);
}*/

void AmazonInterceptor::initialize() {

    std::cout << "AmazonInterceptor::initialize ..." << std::endl;
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(networkAccess());
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };

#ifdef _DEBUG
    QObject::connect(webloader_script_, &WebContent::finished, this, [=]() {
        QFile   file("webloader_script.js");
        if (file.open(QIODevice::ReadWrite)) {
            file.write(webloader_script_->data().toLocal8Bit());
            file.close();
        }
    }, Qt::QueuedConnection);
#endif
    
    initContent(this);
    QObject::connect(this, &WebContent::finished, this, [=]() {
        try {
            if (isValid()) {
                //auto url = metadata_->url().toEncoded(); // video_info().base_url;
                //auto mpd_url = metadata_->video_info().base_url;
                //if (mpd_url.size() > 0)
                parseMPD(/*mpd_url_*/);

                if (!cdm_initializing_ 
                    && !tracks_info_.video_tracks.isEmpty()) {
#ifdef DECRYPTSERVICE
                    //initCDMSession();
                    auto v = tracks_info_.video_tracks.constFirst(); // all tracks has the same keys
                    if (!v.urls.empty()) {
                        auto recode_op = std::make_shared<recode::RecodeOp>(v.urls[0].toUtf8().constData(), "", nullptr);
                        auto init_data = recode_op->GetInitData();
                        auto str = hexStr(init_data.data(), init_data.size());
                        if (init_data.size() == 0)
                            qDebug() << "Failed to extract init_data from: " << v.urls[0].toUtf8();
                        else
                            qDebug() << "init_data extracted " << "(" << v.width << "x" << v.height << ") from: " << v.urls[0].toUtf8() << " | " << str.c_str() << " | pssh:" << tracks_info_.pssh;
                        cdm_initializing_ = true;
                        QUrl lic_url = res_req_url_;
                        lic_url.setQuery(widevine_2_license_query_);

                        keys_loader_ = new AmazonKeysLoader(this);
                        QObject::connect(keys_loader_, &AmazonKeysLoader::error, [=](const Status& s) {emit error(s); });

                        keys_loader_->initCDMSession(init_data, lic_url, cdmLicense_);
                    }
#endif
                }

                if (isTrackReady()) {
                    std::chrono::steady_clock::time_point end_metadata = std::chrono::steady_clock::now();
                    qDebug() << "Metadata interception takes, s: " << std::chrono::duration_cast<std::chrono::seconds>(end_metadata - begin_metadata_).count();
                    setFlag(kTracksReady);
                }
                else {//load next audio
                    QUrl url_a = url();
                    QUrlQuery query(url_a);
                    //auto audioTrackId = query.queryItemValue("audioTrackId");
                    auto nextAudio = getNextAvailAudio();
                    if (!url_a.isEmpty()
                        && !nextAudio.isEmpty()) {
                        qDebug() << "load next audio: " << nextAudio;
                        query.removeAllQueryItems("audioTrackId");
                        query.addQueryItem("audioTrackId", nextAudio);
                        //query.addQueryItem("Origin", "https://www.amazon.com");
                        //query.addQueryItem("Referer", "https://www.amazon.com/");
                        //query.removeAllQueryItems("deviceTypeID");
                        //query.addQueryItem("deviceTypeID", "A63V4FRV3YUP9");
                        //query.removeAllQueryItems("desiredResources");
                        //query.addQueryItem("desiredResources", "PlaybackUrls"); // only urls needed for audio
                        url_a.setQuery(query.query());
                        //qDebug() << "new url: " << url_a;
                        auto gascEnabled = isClassic(marketplaceId_) ? "false" : "true";
                        if (asin_.isEmpty())
                            asin_ = getAsin(base_url_);
                        auto userWatchSessionId = QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
                        QString post_data = QString("{deviceID=null&deviceTypeID=%1&gascEnabled=%2&marketplaceID=%3&firmware=1&playerType=html5&operatingSystemName=Windows&operatingSystemVersion=10.0&asin=%4&consumptionType=Streaming&desiredResources=PlaybackUrls&videoMaterialType=Feature&userWatchSessionId=%5&displayWidth=2880&displayHeight=1800&deviceStreamingTechnologyOverride=DASH&deviceDrmOverride=CENC&deviceVideoCodecOverride=H264&deviceVideoQualityOverride=UHD&deviceBitrateAdaptationsOverride=AC3%2CCVBR%2CCBR&audioTrackId=%6}").arg(deviceTypeID_).arg(gascEnabled).arg(marketplaceId_).arg(asin_).arg(userWatchSessionId).arg(nextAudio);
                        reset();
                        this->post(url_a, post_data.toLocal8Bit());
                    }
                }
            }
        }
        catch (...) {
            qDebug() << "error loading metadata.";
        }
    });

    webloader_script_ = new WebContent(this);
    initContent(webloader_script_);

    //keys_loader_ = new AmazonKeysLoader();

    html_ = new WebContent(this);
    initContent(html_);
    QObject::connect(html_, &WebContent::finished, this, [=]() {
        if (season_urls_.isEmpty()) {
            parseHTML(html_->data());
            qDebug() << "season_urls_: " << season_urls_;
            if (season_urls_.isEmpty())
                setFlag(kVideoInfoReady_);
            else {
                for (const auto& season_url : season_urls_)
                    QMetaObject::invokeMethod(this, "interceptSeason", Qt::QueuedConnection, Q_ARG(const QUrl&, QUrl(season_url)));
            }
        }
    }, Qt::QueuedConnection);

    // two license senders:
    cdmLicense_ = new WebContent(this);
    initContent(cdmLicense_);
}

const void AmazonInterceptor::parseHTML(const QString& data) {
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("main_" + QString::number(ms) + ".html");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(data.toUtf8());
        file.close();
    }
#endif

    QRegularExpression re_url("<script type=\"text/template\">(.+)</script>");
    QRegularExpressionMatchIterator i = re_url.globalMatch(data);
    
    QString base_url;
    bool base = false;
    while (i.hasNext()) {
        QRegularExpressionMatch match_url = i.next();
        QString json_str = match_url.captured(1);
        if (json_str.contains("enableMarinTracking")) {
            auto json_doc = QJsonDocument::fromJson(json_str.toUtf8()).object();
            const auto& props = json_doc.value("props").toObject();
            if (!props.isEmpty()) {
                const auto& state = props.value("state").toObject();
                if (!state.isEmpty()) {
                    const auto& seasons = state.value("seasons").toObject();
                    if (!seasons.isEmpty()) {
                        foreach(const auto& key, seasons.keys()) {
                            const auto& it = seasons.value(key);
                            const auto& items = it.toArray();
                            if (!items.isEmpty()) {
                                for (const auto& item : items) {
                                    const auto& seasonLink = item.toObject().value("seasonLink").toString();
                                    if (!seasonLink.isEmpty()) {
                                        if (!base) {
                                            base = true;
                                            QList<QString> list = seasonLink.split("/");
                                            list.removeAll("");
                                            if (!list.isEmpty()) {
                                                base_url = base_url_.split(list.first()).first();
                                                base_url = base_url.mid(0, base_url.lastIndexOf("/"));
                                            }
                                        }
                                        QString season_url = base_url + seasonLink;

                                        qDebug() << "season_url:" << season_url;
                                        if (!season_urls_.contains(season_url)) {
                                            season_urls_.push_back(season_url);
                                            seasons_count_++;
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
    }
}

void AmazonInterceptor::parseSeason(const QString& data) {
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("season_" + QString::number(seasons_count_) + "_" + QString::number(ms) + ".html");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(data.toUtf8());
        file.close();
    }
#endif

    QRegularExpression re_url("<script type=\"text/template\">(.+)</script>");
    QRegularExpressionMatchIterator i = re_url.globalMatch(data);

    bool base = false;
    QString base_url; //https://www.primevideo.com/region/eu/detail/0FJVGFFHMUJL3SJJUISIAIEQUR/ref=atv_dp_btf_el_prime_sd_tv_resume_t1ADAAAAAA0wg0?autoplay=1&t=0
    QStringList gtis;
    QList<QPair<QString, QString>> episodes;
    int season_number = -1;
    QList<QPair<int, QPair<QString, QString>>> episodes_to_sort;

    while (i.hasNext()) {
        QRegularExpressionMatch match_url = i.next();
        QString json_str = match_url.captured(1);
        if (json_str.contains("homeRegion")
            && !json_str.contains("enableMarinTracking")) {
            auto json_doc = QJsonDocument::fromJson(json_str.toUtf8()).object();
            const auto& props = json_doc.value("props").toObject();
            if (!props.isEmpty()) {
                const auto& state = props.value("state").toObject();
                if (!state.isEmpty()) {
                    const auto& self = state.value("self").toObject();
                    if (!self.isEmpty()) {
                        foreach(const auto& key, self.keys()) {
                            const auto& it = self.value(key);
                            const auto& gti = it.toObject().value("gti").toString();
                            if (!gtis.contains(gti)) {
                                gtis.push_back(gti);
                                const auto& link = it.toObject().value("link").toString();
                                const auto& sequenceNumber = it.toObject().value("sequenceNumber").toInt();
                                if (!base) {
                                    base = true;
                                    QList<QString> list = link.split("/");
                                    list.removeAll("");
                                    if (!list.isEmpty()) {
                                        base_url = base_url_.split(list.first()).first();
                                        base_url = base_url.mid(0, base_url.lastIndexOf("/"));
                                    }
                                }
                                QString playlist_url = base_url + link;
                                qDebug() << "playlist_url:" << playlist_url;
                                episodes.push_back(qMakePair(gti, playlist_url));
                            }
                        }
                    }

                    const auto& detail = state.value("detail").toObject();
                    if (!detail.isEmpty()) {
                        const auto& details = detail.value("detail").toObject();
                        if (!details.isEmpty()) {
                            foreach(const auto& key, details.keys()) {
                                const auto& it = details.value(key);
                                const auto& title = it.toObject().value("title").toString();
                                const auto& duration = it.toObject().value("duration").toInt();
                                const auto& episodeNumber = it.toObject().value("episodeNumber").toInt();
                                QString video_duration = QDateTime::fromSecsSinceEpoch(duration).toUTC().toString(duration < 3600 ? "mm:ss" : "hh:mm:ss");
                                if (season_number < 0)
                                    season_number = it.toObject().value("seasonNumber").toInt();
                                for (auto& ep : episodes) {
                                    if (ep.first == key
                                        || ep.second.contains(key)) {
                                        QString ep_title = normalizeTitle(QTextDocumentFragment::fromHtml(title).toPlainText(), 40);
                                        ep.first = ep_title + " " + video_duration;
                                        episodes_to_sort.push_back(qMakePair(episodeNumber, ep));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }

    if (!episodes_to_sort.isEmpty()) {
        std::sort(episodes_to_sort.begin(), episodes_to_sort.end(), [](const QPair<int, QPair<QString, QString>>& t1, const QPair<int, QPair<QString, QString>>& t2)->bool {
            return (t1.first < t2.first);
        });

        episodes.clear();
        for (auto& ep : episodes_to_sort)
            episodes.push_back(ep.second);

        video_info_.playlist.push_back(qMakePair(season_number, episodes));
    }

    if (--seasons_count_ <= 0) {
        std::sort(video_info_.playlist.begin(), video_info_.playlist.end(), [](const QPair<int, QList<QPair<QString, QString>>>& t1, const QPair<int, QList<QPair<QString, QString>>>& t2)->bool {
            return (t1.first < t2.first);
        });

        setFlag(kVideoInfoReady_);
    }
}

void AmazonInterceptor::setFlag(EInterceptFlags flag) {
    if (!is_tracks_ready_)
        is_tracks_ready_ = checkInterceptFlag(flag, kTracksReady);
    if (!is_video_info_ready_)
        is_video_info_ready_ = checkInterceptFlag(flag, kVideoInfoReady_);
    if (is_tracks_ready_ && is_video_info_ready_) {
        ready_for_metadata_ = false;
        emit metadataIntercepted(this->url());
    }
}

//
//void AmazonInterceptor::OnSessionKeysChange(const char* session_id, uint32_t session_id_size) {
//    qDebug() << "got OutputRestrictedKey.";
//    cdmLicense_->reset();
//    cdmLicense2_->reset();
//
//    DecryptCreateSessionAndGenerateRequest(init_data_.data(), init_data_.size(), (SessionMessageCallback*)this, false);
//    cdm_initializing_ = true;
//}