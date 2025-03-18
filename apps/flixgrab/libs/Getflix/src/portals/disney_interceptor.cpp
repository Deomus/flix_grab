#include "disney_interceptor.h"

#include <iostream>

#include <languages.h>
#include <recode_op.h>

#ifdef DECRYPTSERVICE
    #include "cdm_keys_loader.h"
#endif
#include "get_utils.h"
#include "utils/qs_utils.h"
#include "cdm_magickeys_client.h"


static AudioProfile     disneyAudioProfile(const QString& profile_str) {
    const QList<QString>
        profile_strs = { "atmos", "eac", "aac" };
    const AudioProfile
        profile_ids[] = { AudioProfile::DolbyDigitalPlusAtmos, AudioProfile::DolbyDigitalPlus, AudioProfile::AAC };
    int index = profile_strs.indexOf(profile_str);
    return index >= 0 ? profile_ids[index] : AudioProfile::UnknownProfile;
}

DisneyPlsInterceptor::DisneyPlsInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p)
    : WebPortal(page, nam, p)
    , page_(page) {
    Q_INIT_RESOURCE(scripts);
}

DisneyPlsInterceptor::~DisneyPlsInterceptor() {
    qDebug() << "Interceptor: ~DisneyPlsInterceptor";
}

void DisneyPlsInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {
    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());

    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;

    info.setHttpHeader("User-Agent", USER_AGENT);

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
        base_url_ = qstr;

        video_info_.artwork_url.clear();
        video_info_.video_title.clear();
        tracks_info_.audio_tracks.clear();
        tracks_info_.video_tracks.clear();
        tracks_info_.subtitle_tracks.clear();

        is_tracks_ready_ = false;
        is_video_info_ready_ = false;

        //manifest_->reset();
        //dmcVideos_->reset();
    }
    else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {
        // script intercept
#ifdef _DEBUG 
        //QMetaObject::invokeMethod(this, "dumpOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif
        if (qstr.contains("/app.", Qt::CaseInsensitive) && qstr.endsWith(".js", Qt::CaseInsensitive) && !url.isLocalFile()) {
            qDebug() << "Interceptor app Script: " << qstr; // QFileInfo(qstr).fileName();
            QMetaObject::invokeMethod(this, "interceptAppScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }
        else if (qstr.contains("/vendor.", Qt::CaseInsensitive) && qstr.endsWith(".js", Qt::CaseInsensitive) && !url.isLocalFile()) {
            qDebug() << "Interceptor vendor Script: " << qstr; // QFileInfo(qstr).fileName();
            QMetaObject::invokeMethod(this, "interceptVendorScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }
    }
    else { //  data intercept
        /* Doesn't work, "Host requires authentication"
        if (qstr.contains("search-api-disney.svcs.dssott.com", Qt::CaseInsensitive)
            && qstr.contains("DmcVideos", Qt::CaseInsensitive))
        {
            qDebug() << "Interceptor DmcVideos: " << qstr;
            //QMetaObject::invokeMethod(this, "interceptDmcVideos", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        }
        else if (qstr.contains("global.edge.bamgrid.com/media", Qt::CaseInsensitive)
            && qstr.contains("scenarios", Qt::CaseInsensitive))
        {
            qDebug() << "Interceptor Manifest: " << qstr;
            //QMetaObject::invokeMethod(this, "interceptManifest", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        }
        */
        //interceptPlaylist
        //if (qstr.contains("sanalytics.disneyplus.com", Qt::CaseInsensitive)) {
        //    emit loginRequested(qstr);
        //}
        //if (qstr.contains("/scenarios/", Qt::CaseInsensitive)) {
        //    qDebug() << "Interceptor scenarios: " << qstr;
        //    //QMetaObject::invokeMethod(this, "interceptScenario", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        //}
        /*else*/ 
        if (playlist_->url().isEmpty()
            && qstr.contains(".m3u8", Qt::CaseInsensitive)
            && qstr.count(".m3u8", Qt::CaseInsensitive) == 1) {

            qDebug() << "Interceptor playlist: " << qstr;

            if (!access_token_.isEmpty()
                && base_m3u8_url_.isEmpty()
                && !qstr.contains("/unenc-", Qt::CaseInsensitive)) {
                base_m3u8_url_ = qstr.mid(0, 1 + qstr.lastIndexOf('/'));
                qDebug() << "Base m3u8 url: " << base_m3u8_url_;
                QMetaObject::invokeMethod(this, "interceptPlaylist", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
                emit error(Status(EStatusCode::StopPage)); // stop all activity on the page
            }
        }
        else if (qstr.contains("/DmcSeriesBundle/", Qt::CaseInsensitive)
            && season_url_part_.isEmpty()) {
            
            qstr.replace("/DmcSeriesBundle/", "/DmcEpisodes/");
            season_url_part_ = qstr.split("/encodedSeriesId/").first() + "/seasonId/";
            qDebug() << "season_url_part_: " << season_url_part_;
        }
    }
}

void DisneyPlsInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
    });
}
//
//Q_INVOKABLE void DisneyPlsInterceptor::interceptDmcVideos(const QUrl& url) {
//    dmcVideos_->fetch(QUrl(url));
//}
//
//Q_INVOKABLE void DisneyPlsInterceptor::interceptManifest(const QUrl& url) {
//    manifest_->fetch(QUrl(url));
//    // see parse_manifest
//}

Q_INVOKABLE void   DisneyPlsInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    if (confName == "fgapp_access_token:") {
        access_token_ = jsonStr;
        return;
    }

    auto config = QJsonDocument::fromJson(jsonStr.toUtf8()).object();

    if (config.isEmpty())
        return;

    auto data = config.value("data").toObject();
    if (!data.isEmpty()) {
        auto key = data.constBegin().key();
        if (!key.isEmpty()) {

#ifdef _DEBUG
            QFile   jf("DisneyP-" + key + "_" + base_url_.split("/").last() + ".json");
            if (jf.open(QIODevice::ReadWrite)) {
                jf.resize(0);
                QTextStream(&jf) << jsonStr << Qt::endl;
                jf.close();
            }
#endif

            if (key == "ContinueWatchingSet")
                emit hideWebEngine(ERequestType::profile);

            qDebug() << "interceptConfig key: " << key << ";" << " confName: " << confName;

            if (key.contains("DmcSeriesBundle")
                && season_ids_.isEmpty())
                parseSeasons(data);

            if (key.contains("DmcVideo"))
                parseDmcVideo(data);
        }
    }
}
// not used now
//Q_INVOKABLE void DisneyPlsInterceptor::interceptScenario(const QUrl& url) {
//    if (scenario_->url().isEmpty())
//        scenario_->fetch(url);
//}

Q_INVOKABLE void DisneyPlsInterceptor::interceptPlaylist(const QUrl& url) {
    if (playlist_->url().isEmpty())
        playlist_->fetch(url);
}

Q_INVOKABLE void DisneyPlsInterceptor::requestSeason(const QUrl& url, const QString& season_id) {
    qDebug() << "requestSeason access_token_: " << access_token_;

    if (access_token_.isEmpty())
        return;

    auto season = new WebContent(this);
    initRequest(season);
    qDebug() << "metadata: " << url;

    QObject::connect(season, &WebContent::finished, this, [this, season, season_id]() {
        parseEpisodes(season_id, season->data());
    }, Qt::QueuedConnection);

    season->add_header("Authorization", access_token_);
    season->reset();
    season->fetch(url);
}

Q_INVOKABLE void DisneyPlsInterceptor::dumpOrigScript(const QUrl& url) {
    if (!url.isLocalFile()) {
        auto script = new WebContent(this);
        script->setNetworkAccess(networkAccess());
        QObject::connect(script, &WebContent::finished, this, [=]() {
            //if (script->status() == Status.OK) {
                QString qstr = url.toEncoded();
                QFile   file(".\\disney_js\\" + QFileInfo(qstr).fileName());
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

class AppDisneyScript : public WebContent {
public:
    AppDisneyScript(QObject* p = nullptr) : WebContent(p) {}
protected:
    Status  modifyContent() override {
        qDebug() << "Interceptor: app injection...";
        
#ifdef _DEBUG
        QFile   file("app.js");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(mutableData().toLocal8Bit());
            file.close();
        }
#endif
        auto size_bkup = mutableData().size();
        QRegularExpression regexp("return\\s*\\{\\s*payload:\\s*\\{\\s*request:\\s*\\w+,\\s*response:\\s*(\\w+)\\s*\\}", QRegularExpression::CaseInsensitiveOption);
        auto i = regexp.globalMatch(mutableData());
        int pos = -1;
        if (i.hasNext()) {
            auto txt = i.next();
            if (txt.capturedLength() >= 1) {
                pos = txt.capturedStart();
                if (pos != -1) {
                    auto response = txt.captured(1); // the name of the variable containing response
                    mutableData().insert(pos, "console.log(\"newContentConfig: \" + JSON.stringify(" + response + "));");
                }
            }
        }
        //example
        //console.log("newContentConfig: " + JSON.stringify(t));
        //return {
        //    payload: {
        //        request: n,
        //        response : t
        //    },

        //example2
        //{return{ payload: {request:n,response : t}

        auto ok = size_bkup != data().size();
        qDebug() << "Injected  app.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << pos;

#ifdef _DEBUG
        QFile   jf("app_injected.js");
        if (jf.open(QIODevice::ReadWrite)) {
            jf.resize(0);
            QTextStream(&jf) << mutableData() << Qt::endl;
            jf.close();
        }
#endif

        return EStatusCode::Success;
    }
    bool    isModifyAsync() const override {
        return false;
    }
};

class VendorScript : public WebContent {
public:
    VendorScript(QObject* p = nullptr) : WebContent(p) {}
protected:
    const QString inj_scenario = "browser";
    //const QString inj_scenario = "browser~unlimited";
    //ok values for scenario:  
    // browser, 
    // restricted-drm-ctr-sw, 
    // restricted-drm-ctr-sw-h265,  ! h265 codec
    // browser~unlimited, ! up to 4K
    // chromecast30, ! up to 4K
    // chromecast
    // not ok:
    // restricted-drm-ctr-sw-h265-dovi
    // restricted-drm-ctr-sw-h265-dovi-atmos
    // restricted-drm-ctr-sw-h265-hdr10-atmos~csai
    // browser-h265-hdr10-atmos~ssai
    // chromecast~ssai
    // chromecast-hdr10-atmos
    // chromecast-dovi-atmos
    // return c + l + f + d + u
    Status  modifyContent() override {
        qDebug() << "Interceptor: vendorscript injection...";
#ifdef _DEBUG
        QFile   jf("disney-vendor.js");
        if (jf.open(QIODevice::ReadWrite)) {
            jf.resize(0);
            jf.write(mutableData().toLocal8Bit());
            jf.close();
        }
#endif
        {
            auto size_bkup = mutableData().size();
            QRegularExpression reg("key\\:\"derivePlaybackScenario\"\\,value\\:function\\(\\w+\\)\\{", QRegularExpression::CaseInsensitiveOption);
            auto i = reg.globalMatch(mutableData());
            int pos = -1;
            if (i.hasNext()) {
                auto txt = i.next();
                if (txt.capturedLength() >= 1) {
                    pos = txt.capturedStart();
                    if (pos != -1)
                        mutableData().insert(pos + txt.capturedLength(), "return '" + inj_scenario + "';");
                }
            }
            auto ok = size_bkup != data().size();
            qDebug() << "Injected  vendor_bundle.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << pos;
        }
        {
            // createSdkSession(t).then((function(t) {return (e=t).initialize()
            auto size_bkup = mutableData().size();
            int i = -1;
            QRegularExpression regexp("\\.createSdkSession\\((\\w+)\\)\\.then\\(\\(function\\((\\w+)\\)\\s*\\{", QRegularExpression::PatternOption::CaseInsensitiveOption);
            QRegularExpressionMatch match = regexp.match(mutableData());

            if (match.hasMatch()) {
                auto txt = match.captured(2);
                auto len = match.capturedLength(0);
                i = match.capturedStart(0) + len;
                mutableData().insert(i, "if(" + txt + ".accessTokenProvider.accessToken) console.log('fgapp_access_token: '+" + txt + ".accessTokenProvider.accessToken.token);");
            }
            auto ok = size_bkup != data().size();
            qDebug() << "Injected  vendor_bundle.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << i;
        }
        {   // doesn't help, could be removed:
            // "hidden"==document.visibilityState&&
            //auto size_bkup = mutableData().size();
            //int i = -1;
            //if (i = mutableData().indexOf("\"hidden\"==document.visibilityState&&"); i != -1) {
            //    mutableData().insert(i, "false&&");
            //}
            //auto ok = size_bkup != data().size();
            //qDebug() << "Injected  vendor_bundle.js. Newsize: " << data().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << i;
        }

#ifdef _DEBUG
        QFile   jf_inj("disney-vendor-injected.js");
        if (jf_inj.open(QIODevice::ReadWrite)) {
            jf_inj.resize(0);
            jf_inj.write(mutableData().toLocal8Bit());
            jf_inj.close();
        }
#endif
        return EStatusCode::Success;
    }
    bool    isModifyAsync() const override {
        return false;
    }
};

void DisneyPlsInterceptor::startPage(bool forced_reload_page) {
    Q_UNUSED(forced_reload_page);
    qDebug() << "start page ...";

    invalidate();
    app_script_->reset();
    vendor_script_->reset();
    //scenario_->reset();
    playlist_->reset();
    cdmLicense_->reset();
    cdmLicensePrivate_->reset();
    base_m3u8_url_.clear();
    base_url_.clear();
    access_token_.clear();
    season_url_part_.clear();
    season_ids_.clear();
    season_count_ = 0;
}

void DisneyPlsInterceptor::finishPage(bool ok) {
    Q_UNUSED(ok);
}

void DisneyPlsInterceptor::initialize() {
    qDebug() << "DisneyPlsInterceptor::initialize ...";
    //manifest_ = new WebContent(this);
    //initRequest(manifest_);
    //QObject::connect(manifest_, &WebContent::finished, this, [=]() {
    //    parse_manifest(manifest_->data());
    //    }, Qt::QueuedConnection);

    playlist_ = new WebContent(this);
    initRequest(playlist_);
    QObject::connect(playlist_, &WebContent::finished, this, [=]() {
        qDebug() << "playlist downloaded:" << playlist_->url() << "; access_token_: " << access_token_;
        parsePlaylistDisney(playlist_->data(), playlist_->url());
    }, Qt::QueuedConnection);

    //scenario_ = new WebContent(this);
    //initRequest(scenario_);
    //QObject::connect(scenario_, &WebContent::finished, this, [=]() {
    //    qDebug() << "scenario downloaded."; // << playlist_->url();
    //    parseScenario(scenario_->data());
    //}, Qt::QueuedConnection);

    app_script_ = new AppDisneyScript(this);
    initRequest(app_script_);

    vendor_script_ = new VendorScript(this);
    initRequest(vendor_script_);

    cdmLicense_ = new WebContent(this);
    initRequest(cdmLicense_);

    cdmLicensePrivate_ = new WebContent(this);
    initRequest(cdmLicensePrivate_);
}

Q_INVOKABLE void DisneyPlsInterceptor::interceptAppScript(const QUrl& url) {
    //Q_UNUSED(url);
    app_script_->fetch(url);
    //app_script_->fetch(QUrl("qrc:///scripts/disneyp/app_script_bundle_v3.js"));
    scripts_.addAsync(app_script_);
    scripts_.run(page_);
}

Q_INVOKABLE void DisneyPlsInterceptor::interceptVendorScript(const QUrl& url) {
    vendor_script_->fetch(url);
    scripts_.addAsync(vendor_script_);
    scripts_.run(page_);
}

void DisneyPlsInterceptor::parseDmcVideo(const QJsonObject& data) {
    qDebug() << "parseDmcVideo";
    auto key = data.constBegin().key();

    QJsonObject v0;
    if (key.contains("DmcVideos")) {
        auto json = data.value("DmcVideos").toObject();
        auto videos = json.value("videos").toArray();
        v0 = videos.constBegin()->toObject();
    }
    else if (key.contains("DmcVideo")) {
        auto json = data.value("DmcVideo").toObject();
        v0 = json.value("video").toObject();
    }

    if (!v0.empty()) {
        auto image = v0.value("image").toObject();
        if (!image.isEmpty()) {
            const auto aspect_ratios = { "1.33", "1.78", "0.75", "0.71", "0.67", "1", "0.80", "3.00", "3.32" };

            auto thumbnail = image.value("thumbnail").toObject();
            if (!thumbnail.isEmpty()) {
                for(const auto& aspect_ratio : aspect_ratios) {
                    auto aratio = thumbnail.value(aspect_ratio).toObject();
                    if (!aratio.isEmpty()) {
                        auto program = aratio.value("program").toObject();
                        if (!program.isEmpty()) {
                            auto default_obj = program.value("default").toObject();
                            if (!default_obj.isEmpty()) {
                                video_info_.artwork_url = default_obj["url"].toString() + "/scale?width=400&format=jpeg";
                                break;
                            }
                        }
                    }
                }
            }

            if (video_info_.artwork_url.isEmpty()) {
                auto hero_tile = image.value("hero_tile").toObject();
                if (!hero_tile.isEmpty()) {
                    for (const auto& aspect_ratio : aspect_ratios) {
                        auto aratio = hero_tile.value(aspect_ratio).toObject();
                        if (!aratio.isEmpty()) {
                            auto program = aratio.value("program").toObject();
                            if (!program.isEmpty()) {
                                auto default_obj = program.value("default").toObject();
                                if (!default_obj.isEmpty()) {
                                    video_info_.artwork_url = default_obj["url"].toString() + "/scale?width=400&format=jpeg";
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            if (video_info_.artwork_url.isEmpty()) {
                auto background_up_next = image.value("background_up_next").toObject();
                if (!background_up_next.isEmpty()) {
                    for (const auto& aspect_ratio : aspect_ratios) {
                        auto aratio = background_up_next.value(aspect_ratio).toObject();
                        if (!aratio.isEmpty()) {
                            auto program = aratio.value("program").toObject();
                            if (!program.isEmpty()) {
                                auto default_obj = program.value("default").toObject();
                                if (!default_obj.isEmpty()) {
                                    video_info_.artwork_url = default_obj["url"].toString() + "/scale?width=400&format=jpeg";
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            qDebug() << "artwork_url: " << video_info_.artwork_url;
        }

        const auto& mediaMetadata = v0.value("mediaMetadata").toObject();
        if (!mediaMetadata.isEmpty())
            video_info_.duration = mediaMetadata.value("runtimeMillis").toInt() / 1000;

        const auto& releases = v0.value("releases").toArray();
        if (!releases.isEmpty())
            video_info_.metadata_year = releases[0].toObject().value("releaseYear").toString();

        auto text = v0.value("text").toObject();
        if (!text.isEmpty()) {
            auto title = text.value("title").toObject();
            if (!title.isEmpty()) {
                auto full = title.value("full").toObject();
                if (!full.isEmpty()) {
                    QString video_title;
                    auto program = full.value("program").toObject();
                    if (!program.isEmpty()) {
                        auto default_obj = program.value("default").toObject();
                        if (!default_obj.isEmpty())
                            video_title = default_obj["content"].toString();
                    }

                    QString serial_title;
                    auto series = full.value("series").toObject();
                    if (!series.isEmpty()) {
                        auto default_obj = series.value("default").toObject();
                        if (!default_obj.isEmpty())
                            serial_title = default_obj["content"].toString();
                    }

                    if (!video_title.isEmpty()) {
                        video_info_.metadata_title = normalizeTitle(video_title);
                        if (!serial_title.isEmpty()) {
                            video_info_.metadata_album = normalizeTitle(serial_title);

                            if (!serial_title.isEmpty() &&
                                !video_title.isEmpty())
                                serial_title = serial_title.append(" - ");

                            QString season_number;
                            int season_num = v0.value("seasonSequenceNumber").toInt();
                            if (season_num < 10)
                                season_number = "0" + QString::number(season_num);
                            else
                                season_number = QString::number(season_num);

                            QString episode_number;
                            int episode_num = v0.value("episodeSequenceNumber").toInt();
                            if (episode_num < 10)
                                episode_number = "0" + QString::number(episode_num);
                            else
                                episode_number = QString::number(episode_num);

                            if (season_num > 0
                                && episode_num > 0) {
                                video_info_.metadata_track = QString::number(episode_num) + "/" + QString::number(season_num);
                                video_title = "[S" + season_number + ".E" + episode_number + "] " + serial_title + video_title;
                            }
                        }

                        video_info_.video_title = normalizeTitle(video_title);
                    }
                }

                auto description = text.value("description").toObject();
                if (!description.isEmpty()) {
                    auto full = description.value("full").toObject();
                    if (!full.isEmpty()) {
                        auto program = full.value("program").toObject();
                        if (!program.isEmpty()) {
                            auto default_obj = program.value("default").toObject();
                            if (!default_obj.isEmpty())
                                video_info_.metadata_comment = default_obj["content"].toString();
                        }
                    }
                }
            }
        }

        if (video_info_.video_title.isEmpty()) {
            auto internalTitle = v0.value("internalTitle").toString();
            if (!internalTitle.isEmpty()) {
                video_info_.video_title = normalizeTitle(internalTitle);
                video_info_.metadata_title = video_info_.video_title;
            }
        }

        // get episodes for playlist button
        QString season_id = v0.value("seasonId").toString();
        if (season_id.isEmpty())
            setFlag(kVideoInfoReady_);
    }
}

void DisneyPlsInterceptor::parseSeasons(const QJsonObject& data) {
    qDebug() << "parseSeasons";

    auto bundle = data.value("DmcSeriesBundle").toObject();

    if (!bundle.empty()) {
        auto seasons_data = bundle.value("seasons").toObject();
        if (!seasons_data.isEmpty()) {
            auto seasons = seasons_data.value("seasons").toArray();
            for (const auto& season : seasons) {
                QString seasonId = season.toObject().value("seasonId").toString();

                if (!seasonId.isEmpty()
                    && !season_ids_.contains(seasonId)) {
                    season_ids_.push_back(seasonId);
                    qDebug() << "season_ids_: " << seasonId;
                }
            }

            if (!season_ids_.isEmpty()
                && !season_url_part_.isEmpty()) {

                for (const auto& season_id : season_ids_) {
                    if (!season_id.isEmpty()) {

                        bool season_id_exists = false;
                        for (const auto& it : seasons_data_) {    //get playlists_data from previous item
                            qDebug() << "season_id: " << season_id  << "; it.first: " << it.first;
                            if (season_id == it.first) {
                                season_count_++;

                                season_id_exists = true;
                                
                                qDebug() << "season_id_exists it.first: " << it.first << "; it.second: " << it.second;
                                video_info_.playlist.push_back(it.second);

                                if (season_count_ == season_ids_.size())
                                    setFlag(kVideoInfoReady_);
                                
                                break;
                            }
                        }

                        if (!season_id_exists) {
                            QString season_url = season_url_part_ + season_id + "/pageSize/60/page/1"; //https://disney.content.edge.bamgrid.com/svc/content/DmcEpisodes/version/5.1/region/US/audience/false/maturity/1450/language/en/seasonId/ + id + "/pageSize/60/page/1"; //15,30,60 works
                            qDebug() << "season_url: " << season_url;
                            QMetaObject::invokeMethod(this, "requestSeason", Qt::QueuedConnection, Q_ARG(const QUrl&, season_url), Q_ARG(const QString&, season_id));
                        }
                    }
                }
            }
        }
    }
}

void DisneyPlsInterceptor::setFlag(EInterceptFlags flag) {
    if (!is_tracks_ready_)
        is_tracks_ready_ = checkInterceptFlag(flag, kTracksReady);
    
#ifdef PRIVATE_KEYS_DECRYPTOR
    if (!is_privatekeys_ready_)
        is_privatekeys_ready_ = checkInterceptFlag(flag, kPrivateKeysReady);
#else
    is_privatekeys_ready_ = true;
#endif

    if (!is_video_info_ready_)
        is_video_info_ready_ = checkInterceptFlag(flag, kVideoInfoReady_);
    qDebug() << "Disney " << (is_video_info_ready_? "video_info ready ": " ") << (is_tracks_ready_? " tracks ready": "") << (is_privatekeys_ready_ ? " privatekey ready" : "");
    if (is_tracks_ready_ && is_video_info_ready_ && is_privatekeys_ready_)
        emit metadataIntercepted(this->url());
}

bool DisneyPlsInterceptor::getUrls(const QString& m3u8_url, const QString& url_file_path, QList<QString>& urls, QString& init_data) {
    QString m3u8_data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("path"), url_file_path.toUtf8()));

    bool res = downloadToStringExtended(m3u8_url, m3u8_data, headers);
    qDebug() << "getUrls res: " << res << "; m3u8_url: " << m3u8_url;
    if (res) {
#ifdef _DEBUG
        auto filename = url_file_path.mid(1 + url_file_path.lastIndexOf('/'));
        QFile   file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(m3u8_data.toLocal8Bit());
            file.close();
        }
#endif
        auto base_part = m3u8_url.mid(0, 1 + m3u8_url.lastIndexOf('/'));
        auto data = m3u8_data.split("\n");
        if (!data.isEmpty()) {
            for (QList<QString>::iterator it = data.begin(); it != data.end(); it++) {
                if (!it->startsWith("#")
                    && !it->contains("BUMPER")
                    && !it->contains("DUB_CARD")
                    && !it->isEmpty()
                    && !urls.contains(base_part + *it)) {
                    urls << base_part + *it;
                }
                else if (it->startsWith("#EXT-X-MAP")   // #EXT-X-MAP:URI="8fad6dab-2026-46f6-a7b1-ccf50060ef20/32ce-MAIN/66286a2b-c32e-4d50-b956-04674aaa61ee/map.mp4"
                    && !it->contains("BUMPER")
                    && !it->contains("DUB_CARD")) {
                    auto vars = it->split(QRegularExpression("[:,]"));
                    foreach(const auto &s, vars) {
                        if (s.startsWith("URI")) {
                            auto uri = s.split("=").last().replace("\"", "");
                            urls << base_part + uri;
                        }
                    }
                }
                else if (init_data.isEmpty()
                    && it->startsWith("#EXT-X-KEY:METHOD=SAMPLE-AES-CTR")
                    && it->contains("urn:uuid:edef8ba9-79d6-4ace-a3c8-27dcd51d21ed")) {
                    auto i = it->indexOf("URI=");
                    if (i > 0) {
                        init_data = it->mid(i + 5).remove("\"");
                        init_data = init_data.split(",").last();
                    }
                }
            }
        }
    }

    return res;
}

//void DisneyPlsInterceptor::parseScenario(const QString& data) {
//#ifdef _DEBUG
//    QFile   file("disney-scenario.json");
//    if (file.open(QIODevice::ReadWrite)) {
//        file.resize(0);
//        file.write(data.toUtf8());
//        file.close();
//    }
//#endif
//    const auto& json = QJsonDocument::fromJson(data.toUtf8()).object();
//    const auto& stream = json.value("stream").toObject();
//    const auto& url = stream.value("complete").toString(); // m3u8 url
//    //const auto& qstr = url.toUtf8().toString();
//    base_url_ = url.mid(0, 1 + url.lastIndexOf('/'));
//    qDebug() << "Base path: " << base_url_;
//    QMetaObject::invokeMethod(this, "interceptPlaylist", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
//}

void DisneyPlsInterceptor::parsePlaylistDisney(const QString& data, const QUrl& url) {
#ifdef _DEBUG
    QFile   file("disney-playlist_" + base_url_.split("/").last() + ".m3u8");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(data.toUtf8());
        file.close();
    }
#endif

    auto video_data = data.split("\n");
    if (video_data.isEmpty() || video_data.size() == 1)
        return;

    QString init_data;
    bool is_video_url = false;
    QList<QPair<int, int>> added_video_resolutions;

    //emit loadAudioLanguages();
    //QList<QString> compatible_audio_tags;
    //for (QList<QString>::iterator it = video_data.begin(); it != video_data.end(); it++) {
    //    if (it->startsWith("#EXT-X-MEDIA:TYPE=AUDIO")) {
    //        auto vars = it->split(QRegularExpression("[:,]"));
    //        foreach(const auto & s, vars) {
    //            if (s.startsWith("LANGUAGE")) {
    //                QString tag = s.split("=").last().replace("\"", "").toLower();
    //                if (!tag.isEmpty()) {
    //                    tag = SS2Q(normalizeLangTag(QS2S(tag)));
    //                    foreach(const auto & lang, selected_languages_) {
    //                        QString sel_tag = lang.split("-").first();
    //                        QString cur_tag = tag.split("-").first();
    //                        if (sel_tag == cur_tag
    //                            && !compatible_audio_tags.contains(cur_tag))
    //                            compatible_audio_tags.push_back(cur_tag);
    //                    };
    //                }
    //            }
    //        }
    //    }
    //}

    for (QList<QString>::iterator it = video_data.begin(); it != video_data.end(); it++) {
        if (is_video_url
            && !tracks_info_.video_tracks.isEmpty()
            && tracks_info_.video_tracks.back().urls.isEmpty()) {

            QList<QString> urls;
            QString m3u8_url = base_m3u8_url_ + *it;
            if (getUrls(m3u8_url, *it, urls, init_data)) {
                tracks_info_.pssh = init_data;
                if (!tracks_info_.video_tracks.isEmpty()) {
                    if (!urls.isEmpty()) {
                        tracks_info_.video_tracks.back().urls << urls;
                        normalizeTrack(tracks_info_.video_tracks.back());
                    }
                    else
                        tracks_info_.video_tracks.takeLast();
                }
            }

            is_video_url = false;
        }

        if (it->startsWith("#")) {
            if (it->startsWith("#EXT-X-STREAM-INF")
                || it->startsWith("#EXT-X-I-FRAME-STREAM-INF")) {    // #EXT-X-STREAM-INF:BANDWIDTH=2103197,AVERAGE-BANDWIDTH=1274799,CODECS="avc1.4d401f,mp4a.40.2",RESOLUTION=854x480,FRAME-RATE=23.976,AUDIO="aac-128k",SUBTITLES="sub-main"
                
                bool add_track = true;
                bool is_i_frame_track = false;
                
                VideoTrackInfo v;
                auto vars = it->split(QRegularExpression("[:,]"));
                foreach(const auto & s, vars) {
                    if (s.startsWith("BANDWIDTH")) {
                        v.bitrate = s.split("=").last().toInt();
                    }
                    else if (s.startsWith("RESOLUTION")) { // RESOLUTION=854x480
                        auto res = s.split("=").last();
                        auto reswh = res.split("x");
                        v.width = reswh.first().toInt();
                        v.height = reswh.last().toInt();
                    }
                    else if (s.startsWith("URI")) {
                        QString base = s.split("=").last().replace("\"", "");
                        QString m3u8_url = base_m3u8_url_ + base;
                        QList<QString> urls;
                        if (getUrls(m3u8_url, base, urls, init_data)
                            && !urls.isEmpty()) {
                            v.urls << urls;
                            v.bitrate = v.bitrate * 10;
                            is_i_frame_track = true;
                        }
                        else {
                            add_track = false;
                        }
                    }
                    else if (s.startsWith("FRAME-RATE")) {
                        v.framerate = s.split("=").last().split(".").first().toInt() + 1;
                    }
                }

                QPair<int, int> resolution = qMakePair(v.width, v.height);
                if (!added_video_resolutions.contains(resolution)) {
                    added_video_resolutions.push_back(resolution);

                    v.contains_audio = false;
                    v.is_chunked = true;
                    v.is_encrypted = true;

                    if (it->startsWith("#EXT-X-STREAM-INF"))
                        is_video_url = true;

                    if (add_track) {
                        if (is_i_frame_track) {
                            bool height_exists = false;
                            foreach(const auto & track, tracks_info_.video_tracks) {
                                if (track.height == v.height) {
                                    height_exists = true;
                                    break;
                                }
                            }
                            if (!height_exists) {
                                normalizeVideoTrack(v);
                                tracks_info_.video_tracks.append(v);
                            }
                        }
                        else {
                            normalizeVideoTrack(v);
                            tracks_info_.video_tracks.append(v);
                        }
                    }
                }
            }
            else if (it->startsWith("#EXT-X-MEDIA:TYPE=AUDIO")) {
                //#EXT-X-MEDIA:TYPE=AUDIO,GROUP-ID="eac-3",NAME="Dansk",LANGUAGE="da",AUTOSELECT=YES,CHANNELS="6",URI="r/composite_256k_ec-3_da_PRIMARY_49f5733e-f59d-4749-bd1f-8e18e24e0a7c_98d94034-818d-4ecb-970c-e753d518c440.m3u8"
                AudioTrackInfo a;
                a.is_chunked = true;
                //a.is_encrypted = true;
                auto vars = it->split(QRegularExpression("[:,]"));
                foreach(const auto & s, vars) {
                    //TODO audio for the blind
                    if (s.startsWith("GROUP-ID")) {
                        a.profile = disneyAudioProfile(s.split("=").last().split("-").first().replace("\"", ""));
                    }
                    else if ((s.startsWith("NAME") && s.contains("Audio Description"))
                        || (s.startsWith("CHARACTERISTICS"))) {
                        a.is_assistive = true;
                    }
                    else if (s.startsWith("LANGUAGE")) {
                        QString tag = s.split("=").last().replace("\"", "").toLower();
                        a.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));
                    }
                    else if (s.startsWith("CHANNELS")) {
                        a.channels = s.split("=").last().replace("\"", "").toInt();
                        if (a.profile == AudioProfile::DolbyDigitalPlus
                            && a.channels == 2)
                            a.profile = AudioProfile::AAC;
                    }
                    else if (s.startsWith("URI")) {
                        QString base = s.split("=").last().replace("\"", "");

                        a.bitrate = base.split("composite_").last().split("k_").first().toInt();
                        if (a.bitrate == 0)
                            a.bitrate = base.split("_complete").first().split("/").last().toInt();

                        a.bitrate = a.bitrate * 1000;

                        QList<QString> urls;
                        QString m3u8_url = base_m3u8_url_ + base;
                        if (getUrls(m3u8_url, base, urls, init_data)
                            && !urls.isEmpty())
                            a.urls << urls;
                    }
                }

                if (!a.urls.isEmpty()) {
                    normalizeTrack(a);
                    normalizeAudioTrack(a);
                    tracks_info_.audio_tracks.append(a);
                }
            }
            else if (it->startsWith("#EXT-X-MEDIA:TYPE=SUBTITLES")) {
                //#EXT-X-MEDIA:TYPE=SUBTITLES,GROUP-ID="sub-main",NAME="Dansk",LANGUAGE="da",AUTOSELECT=YES,FORCED=NO,CHARACTERISTICS="public.accessibility.transcribes-spoken-dialog",URI="r/composite_da_NORMAL_c4a40d8a-5203-46d5-8afa-29b6ea7d199b_6d883393-083c-4639-868e-5d70aa1e88e6.m3u8"
                SubtitleTrackInfo sub;
                sub.profile = SubtitleTrackInfo::SubtitleProfile::WEBVTT;
                auto vars = it->split(QRegularExpression("[:,]"));
                foreach(const auto & s, vars) {
                    if (s.startsWith("NAME")) {
                        sub.language = s.split("=").last().replace("\"", "");
                        if (s.contains("[CC]"))
                            sub.is_closed_captions = true;
                    }
                    else if (s.startsWith("LANGUAGE")) {
                        QString tag = s.split("=").last().replace("\"", "").toLower();
                        sub.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));
                    }
                    else if (s.startsWith("FORCED")) {
                        sub.is_additional = (s.split("=").last() == "YES");
                    }
                    else if (s.startsWith("URI")) {
                        QString sub_data;
                        QString sub_url = base_m3u8_url_ + s.split("=").last().replace("\"", "");
                        bool res = downloadToString(sub_url, sub_data);
                        if (res) {
#ifdef _DEBUG
                            QFile   file("subtitle_" + sub.language_tag + ".m3u8");
                            if (file.open(QIODevice::ReadWrite)) {
                                file.resize(0);
                                file.write(sub_data.toUtf8());
                                file.close();
                            }
#endif
                            auto vars = sub_data.split(QRegularExpression("\n"));
                            foreach(const auto & s, vars) {
                                if (!s.isEmpty()
                                    && !s.contains("#")
                                    && !s.contains("BUMPER")
                                    && !s.contains("DUB_CARD"))
                                    sub.urls << sub_url.mid(0, 1 + sub_url.lastIndexOf('/')) + s;
                            }
                            if (sub.urls.size() > 1)
                                sub.is_chunked = true;
                        }
                    }
                }

                if (!sub.urls.isEmpty()) {
                    //normalizeTrack(sub);
                    tracks_info_.subtitle_tracks.append(sub);
                }
            }
        }
    }
#if _DEBUG
    for (const auto& t : tracks_info_.video_tracks)
        qDebug() << " v" << t.bitrate << ", " << t.width << "x" << t.height << ", urls: " << t.urls.size();
#endif

#ifdef PRIVATE_KEYS_DECRYPTOR
    if (!init_data.isEmpty())
        loadPrivateKeys(init_data);
#endif

#ifdef DECRYPTSERVICE
    qDebug() << "disney init_data: " << init_data;
    if (!init_data.isEmpty())
        loadKeys(init_data);
#endif
}


void DisneyPlsInterceptor::loadPrivateKeys(const QString pssh) {
    qDebug() << "load magicKeys ... " << pssh << "; access_token_: " << access_token_;

    if (access_token_.isEmpty())
        return;

    key_client_ = new CdmMagicKeysClient();

    QObject::connect(key_client_, &CdmMagicKeysClient::keysObtained, this, [this, pssh](const QList<QString>& kidKeys) {
        tracks_info_.setPrivateKeys(kidKeys, pssh);
        emit keysObtained(kidKeys, pssh);
        setFlag(kPrivateKeysReady);            
    });

    QObject::connect(key_client_, &CdmMagicKeysClient::error, this, [this](const Status& err) {
        qDebug() << "magicKeyServer error: " << err;
        setFlag(kPrivateKeysReady);
    });

    cdmLicensePrivate_->add_header("authorization", "Bearer " + access_token_);

    key_client_->initCDMSession(pssh, wv_server_url_, cdmLicensePrivate_, video_info_.video_title, 16);
}

void DisneyPlsInterceptor::loadKeys(const QString& init_data) {
#ifdef DECRYPTSERVICE
    qDebug() << "loadKeys init_data: " << init_data << "; access_token_: " << access_token_;

    if (access_token_.isEmpty())
        return;

    auto keys_loader_ = new CdmKeysLoader();
    QObject::connect(keys_loader_, &CdmKeysLoader::keysLoaded, this, &DisneyPlsInterceptor::onKeysLoaded);

    auto init_data_c = QByteArray::fromBase64(init_data.toUtf8());
    std::vector<uint8_t> init_data_b(init_data_c.begin(), init_data_c.end());

    cdmLicense_->add_header("authorization", "Bearer " + access_token_);
    cdmLicense_->add_header("authority", "disney.playback.edge.bamgrid.com");
    cdmLicense_->add_header("Origin", "https://www.disneyplus.com");
    cdmLicense_->add_header("Referer", "https://www.disneyplus.com/");
    
    keys_loader_->initCDMSession(init_data_b, wv_server_url_, cdmLicense_, 0);
#endif
}

void DisneyPlsInterceptor::onKeysLoaded() {
    qDebug() << "onKeysLoaded";
    setFlag(kTracksReady);
}

void DisneyPlsInterceptor::parseEpisodes(const QString& season_id, const QString& jsondata) {
    season_count_++;
#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   file("disney-episodes_count_" + QString::number(season_count_) + "_" + QString::number(ms) + ".json");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(jsondata.toUtf8());
        file.close();
    }
#endif
    QList<QPair<QString, QString>> episodes;
    const auto& config = QJsonDocument::fromJson(jsondata.toUtf8()).object();
    const auto& data = config.value("data").toObject();
    const auto& dmc_episodes = data.value("DmcEpisodes").toObject();
    if (!dmc_episodes.isEmpty()) {
        const auto& videos = dmc_episodes.value("videos").toArray();
        if (!videos.isEmpty()) {
            QString playlist;
            QList<QString> ids;
            QString base = base_url_.mid(0, 1 + base_url_.lastIndexOf("/"));
            QString cur_id = base_url_.split("/").last();
            int season_number = -1;
            for (const auto &video : videos) {
                QString id = video.toObject().value("contentId").toString();

                if (!id.isEmpty()
                    && !ids.contains(id)) {
                    ids << id;
                    QString ep_url = base + id;
                    qDebug() << "parseEpisodes url: " << ep_url;

                    QString ep_duration;
                    auto mediaMetadata = video.toObject().value("mediaMetadata").toObject();
                    if (!mediaMetadata.isEmpty()) {
                        auto runtimeMillis = mediaMetadata.value("runtimeMillis").toInt() / 1000;
                        ep_duration = QDateTime::fromSecsSinceEpoch(runtimeMillis).toUTC().toString(runtimeMillis < 3600 ? "mm:ss" : "hh:mm:ss");
                    }

                    if (season_number < 0)
                        season_number = video.toObject().value("seasonSequenceNumber").toInt();

                    QString ep_title;
                    auto text = video.toObject().value("text").toObject();
                    if (!text.isEmpty()) {
                        auto title = text.value("title").toObject();
                        if (!title.isEmpty()) {
                            auto full = title.value("full").toObject();
                            if (!full.isEmpty()) {
                                auto program = full.value("program").toObject();
                                if (!program.isEmpty()) {
                                    auto default_pr = program.value("default").toObject();
                                    if (!default_pr.isEmpty()) {
                                        auto content = default_pr.value("content").toString();
                                        if (!content.isEmpty())
                                            ep_title = normalizeTitle(content, 40);
                                    }
                                }
                            }
                        }
                    }
                    episodes.push_back(qMakePair(ep_title + " " + ep_duration, ep_url));
                }
            }

            if (!episodes.isEmpty()) {
                video_info_.playlist.push_back(qMakePair(season_number, episodes));
                seasons_data_.push_back(qMakePair(season_id, qMakePair(season_number, episodes)));
            }
        }
    }

    if (season_count_ == season_ids_.size()) {
        if (!video_info_.playlist.isEmpty()) {
            std::sort(video_info_.playlist.begin(), video_info_.playlist.end(), [](const QPair<int, QList<QPair<QString, QString>>>& t1, const QPair<int, QList<QPair<QString, QString>>>& t2)->bool {
                return (t1.first < t2.first);
            });
        }

        setFlag(kVideoInfoReady_);
    }
}