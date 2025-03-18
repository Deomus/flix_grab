#include "hbo_interceptor.h"

#include <functional>

#include <decrypsis.h>
#include <recode_op.h>

#include "get_utils.h"
#include "hbo_js_scripts.h"
#include "cdm_magickeys_client.h"


HBOInterceptor::HBOInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p) : WebPortal(page, nam, p)
    , page_(page)
    , nam_(nam) {

    Q_INIT_RESOURCE(scripts);
}

HBOInterceptor::~HBOInterceptor() {
    qDebug() << "Interceptor: ~HBOInterceptor";
}

void HBOInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info) {
//    if (!wv_server_.isEmpty())
//        info.block(true);
    const QUrl url = info.requestUrl();
    const QString qstr = url.toEncoded();
    const auto m = QString::fromLatin1(info.requestMethod());
    info.setHttpHeader("User-Agent", USER_AGENT);

    qDebug() << "####### Request url: " << url.fileName() << "   " << m << "   " << qstr << "signed_in_: " << signed_in_;
    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame)
        base_url_ = qstr;

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript && !url.isLocalFile()) {
#ifdef _DEBUG 
        //QMetaObject::invokeMethod(this, "interceptOrigScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
#endif
        if (qstr.contains("/app.js", Qt::CaseInsensitive) && !url.isLocalFile()) {
                qDebug() << "Interceptor APP Script: " << qstr; // https://player.hulu.com/site/dash/305715-site-curiosity/js/app.js
                QMetaObject::invokeMethod(this, "interceptAppScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
                if (auth_state_.isAuthorizated())
                    info.block(true);

                return;
        }
        //else if (qstr.contains("config.json", Qt::CaseInsensitive)) {
        //    qDebug() << "Interceptor: Script blocked: " << qstr;
        //    info.block(true);

        //    // don't block;
        //}
        //else {
        //    qDebug() << "Interceptor: Script blocked: " << qstr;
        //    info.block(true);
        //}
    }

    //if (qstr.contains("auth/tokens", Qt::CaseInsensitive)) {
    //    if (!access_token_.isEmpty()) {
    //        qDebug() << "auth/tokens: " << url.toString();

    //        QString post_data = "{\"grant_type\":\"user_name_password\", \"scope\" : \"browse video_playback device elevated_account_management\", \"username\" : \"freegrabapp@gmail.com\", \"password\" : \"P@ssw0rd\"}";

    //        auth_->post(url, post_data.toLocal8Bit());
    //        info.block(true);
    //    }
    //}

    signed_in_ = auth_state_.isAuthorizated();

    if (signed_in_
        && qstr.contains("profile-locked.png", Qt::CaseInsensitive)) {
        QMetaObject::invokeMethod(this, "autoSelectProfile", Qt::QueuedConnection);
    }
    else if (signed_in_ && qstr.contains("images/buttons/", Qt::CaseInsensitive)) { //btn_play_filled_univ_x_3.png
        QMetaObject::invokeMethod(this, "autoPlay", Qt::QueuedConnection);
    }
    else if (!signed_in_ && qstr.contains("Protection-Image-DT.png", Qt::CaseInsensitive)) {
        QMetaObject::invokeMethod(this, "autoLogin", Qt::QueuedConnection);
    }
    //else {
    //    if (qstr.contains("btn_close_default.png", Qt::CaseInsensitive))
    //        QMetaObject::invokeMethod(this, "interceptLoginClicked", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    //}
    //if (qstr.contains("express-content/urn:hbo:feature", Qt::CaseInsensitive))
    //{
    //    qDebug() << "Interceptor Manifest: " << qstr; // https://comet.api.hbo.com/express-content/urn:hbo:feature:GX_Z4kAgTsrqkwgEAAAAC?
    //    QMetaObject::invokeMethod(this, "interceptManifest", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
    //}

    /*if (qstr.contains(".mpd", Qt::CaseInsensitive) && tracks_info_.video_tracks.length() == 0)
    {
        parseMPD(url);
    }*/
}

void HBOInterceptor::autoSelectProfile() {
    qDebug() << "autoSelectProfile";

    page_->toHtml([this](const QString& html) {
        //this->runJavaScript(QString("var d=document.getElementById('onetrust-accept-btn-handler');if(d){d.click();console.log('cookies accepted!');}"));

#ifdef _DEBUG
        QFile   html_file(page_->url().host() + "-profile.html");
        if (html_file.open(QIODevice::ReadWrite)) {
            html_file.resize(0);
            QTextStream(&html_file) << html << endl;
            html_file.close();
        }
#endif
        });

    if (!profile_.isEmpty()) {
        page_->runJavaScript(QString(JS_SCRIPT_AUTO_SELECT_PROFILE).arg(profile_),
            [this](const QVariant& v) {
                clickByJSONCoords(v, true);
            }
        );
    }

    //UserAutomation::autoSelectProfile(page_, [this] (int left, int top) {
    //    emit clickPageButton(left, top);
    //});
}

void HBOInterceptor::autoPlay() {
    qDebug() << "autoPlay";

#ifdef _DEBUG
    page_->toHtml([this](const QString& html) {
        //this->runJavaScript(QString("var d=document.getElementById('onetrust-accept-btn-handler');if(d){d.click();console.log('cookies accepted!');}"));

        QFile   html_file(page_->url().host() + "-info.html");
        if (html_file.open(QIODevice::ReadWrite)) {
            html_file.resize(0);
            QTextStream(&html_file) << html << endl;
            html_file.close();
        }
    });
#endif

    constexpr const auto profileXPath = "/html/body/div[1]/div[2]/div/div[5]/div/div[2]/span/span";

    static bool isProfileFound = false;

    if (!isProfileFound) {
        page_->runJavaScript(QString(JS_SCRIPT_FIND_PROFILE).arg(profileXPath),
            [this](const QVariant& v) {
                const auto profile = v.toString();
                if (!profile.isEmpty() 
                    && profile != profile_) {
                    profile_ = profile;
                    qDebug() << "profile_autoPlay: " << profile_;
                    isProfileFound = true;
                    emit saveSettings(profile_key_name_ + ":" + profile_);
                }
            }
        );
    }

    page_->runJavaScript(QString(JS_SCRIPT_AUTO_PLAY),
        [this](const QVariant& v) {
            clickByJSONCoords(v);
        }
    );

    //UserAutomation::autoPlay(page_, [this] (int left, int top) {
    //    emit clickPageButton(left, top);
    //});
}

void HBOInterceptor::autoLogin() {
    if (auth_state_.isFormShowed())
        return;
    qDebug() << "autoLogin";

#ifdef _DEBUG
    page_->toHtml([this](const QString& html) {
        //this->runJavaScript(QString("var d=document.getElementById('onetrust-accept-btn-handler');if(d){d.click();console.log('cookies accepted!');}"));

        QFile html_file(page_->url().host() + "-login.html");
        if (html_file.open(QIODevice::ReadWrite)) {
            html_file.resize(0);
            QTextStream(&html_file) << html << endl;
            html_file.close();
        }
        });
#endif

    const auto clickByXPath = [this](const QString& elementXPath, std::function<void(const bool)> callback) {
        page_->runJavaScript(QString(JS_SCRIPT_FIND_BY_XPATH).arg(elementXPath),
            [this, callback](const QVariant& v) {
                callback(clickByJSONCoords(v));
            }
        );
    };

    const auto findAndFill = [this](const QString& inputPlaceholder, const QString& elementValue, std::function<void(const bool)> callback) {
        page_->runJavaScript(QString(JS_SCRIPT_FIND_INPUT_BY_PLACEHOLDER).arg(inputPlaceholder, elementValue),
            [this, callback](const QVariant& v) {
                callback(v.toBool());
            }
        );
    };

    constexpr const auto xPathOpenSignInFormButton = "/html/body/div[2]/div[2]/div/div[5]/div[2]";
    constexpr const auto xPathSignInSubmitButton = "/html/body/div[2]/div[9]/div[1]/div[2]/div/div[6]/div[3]";

    const auto showSignInForm = [this, clickByXPath, xPathOpenSignInFormButton]() {
        clickByXPath(xPathOpenSignInFormButton, [this](const bool ok) {    // Click on SignIn
            if (ok)
                auth_state_.setFormShowed();
        });
    };

    const auto fiilAndSubmitSignInForm = [this, findAndFill, clickByXPath, xPathSignInSubmitButton]() {
        findAndFill("Email", login_, [this, findAndFill, clickByXPath, xPathSignInSubmitButton](const bool ok) {
            if (ok) findAndFill("Password", password_, [this, findAndFill, clickByXPath, xPathSignInSubmitButton](const bool ok) {
                if (ok) clickByXPath(xPathSignInSubmitButton, [this](const bool ok) {
                    auth_state_.setAuthorizated();
                    // emit loginCompleted();
                });
            });
        });
    };

    if (auth_state_.isRequired()) {
        showSignInForm();
    }
    else if (auth_state_.isFormShowed()) {
        fiilAndSubmitSignInForm();
    }
    else if (auth_state_.isFormSubmited()) {

    }
    else if (auth_state_.isAuthorizated()) {
        showSignInForm();           //auto signIn
        fiilAndSubmitSignInForm();
    }

    //UserAutomation::autoLogin(
    //    page_, &auth_state_, login, password, 
    //    [this] (int left, int top) {
    //        emit clickPageButton(left, top);
    //});
}

bool HBOInterceptor::clickByJSONCoords(const QVariant& v, bool autoprofile) {
    const auto json = v.toString();
    qDebug() << json;
    if (json.isEmpty()) {
        qDebug() << "error: empty return from interesting sctipt.";
        return false;
    }

    const auto root = QJsonDocument::fromJson(json.toUtf8()).object();
    if (!root.isEmpty()) {
        const auto left = root.value("left").toInt();
        const auto top = root.value("top").toInt();
        if (left != -1 && top != -1) {
            const auto hPadding = root.contains("width") ? root.value("width").toInt() / 2 : 10;
            const auto vPadding = root.contains("height") ? root.value("height").toInt() / 2 : 20;

            qDebug() << "clickByJSONCoords: " << " left: " << left << " hPadding: " << hPadding << " top: " << top << " vPadding: " << vPadding;
            if (autoprofile)
                emit clickPageButton(QString::number(left + hPadding) + ":" + QString::number(top - vPadding));
            else 
                emit clickPageButton(QString::number(left + hPadding) + ":" + QString::number(top + vPadding));
#ifdef _DEBUG   
            qDebug() << "    >>>>   CLICK    <<<<   => " << left + hPadding << "   " << top + vPadding << endl;

            page_->toHtml([this](const QString& html) {
                QFile html_file(page_->url().host() + "-click.html");
                if (html_file.open(QIODevice::ReadWrite)) {
                    html_file.resize(0);
                    QTextStream(&html_file) << html << endl;
                    html_file.close();
                }
                });
#endif
            return true;
        }
    }

    return false;
}

void HBOInterceptor::initialize() {
    qDebug() << "HBOInterceptor::initialize ...";
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(nam_);
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };

    app_script_ = new WebContent(this);
    initContent(app_script_);
#ifdef _DEBUG
    QObject::connect(app_script_, &WebContent::finished, this, [=]() {
        QFile   file("hbo_app_new.js");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(app_script_->data().toLocal8Bit());
        }
    }, Qt::QueuedConnection);
#endif

    cdmLicense_ = new WebContent(this);
    initContent(cdmLicense_);
    cdmLicense2_ = new WebContent(this);
    initContent(cdmLicense2_);

    emit loadSettings(profile_key_name_);
    if (settings_.contains(profile_key_name_)) {
        profile_ = settings_.split(":").last();
        qDebug() << "profile_: " << profile_;
    }
}

void HBOInterceptor::startPage(bool forced_reload_page) {
    qDebug() << "start page ...";
    if (!forced_reload_page)
        invalidate();

    app_script_->reset();
    cdmLicense_->reset();
    cdmLicense2_->reset();
    episodes_ = nullptr;
    access_token_.clear();
    base_url_.clear();
}

void HBOInterceptor::setSignedIn(bool signed_in) { 
    signed_in_ = signed_in;
    if (signed_in_)
        auth_state_.setAuthorizated();
}

void HBOInterceptor::fetchApp(const EFetchMode mode) {
    if (mode == EFetchMode::Self) {
        qDebug() << "fetch self app.js";
        app_script_->fetch(QUrl("qrc:///scripts/hbo/hbo_app.js"));        // Self app
        emit showWebEngine(ERequestType::profile);
    }
    else {
        qDebug() << "fetch original app.js";
        assert(!app_js_url_.isEmpty());
        app_script_->fetch(app_js_url_);                                  // Original app
        emit showWebEngine(ERequestType::unknown);
    }

    scripts_.addAsync(app_script_);
    scripts_.run(page_);
}

Q_INVOKABLE void HBOInterceptor::interceptAppScript(const QUrl& url) {
    if (!url.isEmpty())
        app_js_url_ = url;

    fetchApp(
        auth_state_.isAuthorizated()
        ? EFetchMode::Self
        : EFetchMode::Original
    );
}

//Q_INVOKABLE void HBOInterceptor::interceptLoginClicked(const QUrl& url) {
//    Q_UNUSED(url);
//    emit showWebEngine(ERequestType::login);
//}

//Q_INVOKABLE void HBOInterceptor::interceptManifest(const QUrl& url) {
//    manifest_->fetch(QUrl(url));
//    // see parse_manifest
//}

Q_INVOKABLE void HBOInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    qDebug() << "interceptConfig: " << confName;

    if (jsonStr.isEmpty())
        return;

#ifdef _DEBUG
    QFile   file(confName + QString::number(k) + ".json");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(jsonStr.toUtf8());
        file.close();
    }
    k++;
#endif

    const auto root = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (root.isEmpty())
        return;

    if (root.isArray()) {
        const auto items = root.array();
        //const auto items = root.isArray().object().constBegin();
        if (root.isArray()
            && !items.isEmpty()) {

            for (const auto& item : items) {
                parse_response(item);
            }
        }
    }
    else if (confName == "AUTHORIZATION:") {
        parse_authorization(root.object());
    }
    //else if (confName == "MpdNodes:") {
    //    const auto rootObj = root.object();
    //    const auto& mpd_url = rootObj.value("url").toString();
    //    const auto& mpd = rootObj.value("mpd").toObject();
    //    if (ParseMPD(mpd_url)) {
    //        std::vector<uint8_t> init_data = getInitData(tracks_info_.video_tracks[0].urls[0]);
    //        loadKeys(/*wv_server_url*/QString(), init_data);
    //        emit metadataIntercepted(QUrl());
    //        //return true;
    //    }
    //}
    else {
        parse_access_token(root.object());
    }
}

bool HBOInterceptor::parse_response(const QJsonValue& item) {
    const auto obj = item.toObject();
    if (obj.isEmpty())
        return false;

    const auto& body = obj.value("body").toObject();
    if (body.isEmpty())
        return false;
    const auto& id = obj.value("id").toString(); 
    
    if (id.contains("urn:hbo:content-details:"))
        return parse_metadata(id, body); // id="urn:hbo:content-details:GX_Z4kAgTsrqkwgEAAAAC:type:feature"
    
    if (auth_state_.isAuthorizated()
        && id.contains("urn:hbo:video:"))
        return parse_manifest(id, body); // id="urn:hbo:video:GX_Z4kAl7i5JkiQEAAAAC:feature:GX_Z4kAgTsrqkwgEAAAAC"
    
    if (auth_state_.isRequired()
        && id.contains("urn:hbo:profiles:mine"))
        return parse_profiles(id, body);

    return false;
}

bool HBOInterceptor::parse_profiles(const QString& id, const QJsonObject& body) {
    if (auth_state_.isRequired()) {
        const auto profiles = body.value("profiles").toArray();
        if (!profiles.isEmpty()) {
            auth_state_.setAuthorizated();
            emit hideWebEngine(ERequestType::profile); //emit resetWebPage(QUrl());
        }
    }

    return true;
}

bool HBOInterceptor::parse_authorization(const QJsonObject& root) {
    const auto& login_val = root.value("email");
    const auto& password_val = root.value("password");

    if (!login_val.isString() ||
        !password_val.isString()) {
        return false;
    }

    login_ = login_val.toString();
    password_ = password_val.toString();
    auth_state_.setAuthorizated();

    //emit hideWebEngine(ERequestType::profile); //emit resetWebPage(QString());

    qDebug() << QString(R"(AUTHORIZATION saved : { "email": "%1", "password": "%2" })").arg(login_).arg(password_);

    return true;
}

bool HBOInterceptor::parse_metadata(const QString& id, const QJsonObject& body) {
#ifdef _DEBUG
    QFile   jf("hbo-metadata id.json");
    if (jf.open(QIODevice::ReadWrite)) {
        jf.resize(0);
        QTextStream(&jf) << QJsonDocument(body).toJson(QJsonDocument::Indented) << endl;
        jf.close();
    }
#endif

    const auto& details = body.value("details").toObject();
    if (!details.isEmpty()) {
        // id="urn:hbo:content-details:GX_Z4kAgTsrqkwgEAAAAC:type:feature"
        auto p2 = id.indexOf(":type:series");
        if (p2 != -1)
            return false; // tv series

        p2 = id.indexOf(":type:feature");
        if (p2 == -1)
            p2 = id.indexOf(":type:episode");
        if (p2 == -1)
            p2 = id.indexOf(":type:extra");
        if (p2 == -1)
            return false;

        video_info_.video_title = normalizeTitle(details.value("title").toString());

        QString album_name = details.value("umbrella").toString();
        if (!album_name.isEmpty()) {
            QString movie_name = video_info_.video_title;
            QStringList data = movie_name.split(" ");
            if (data.size() > 2) {
                QString season_number = data[0];
                QString episode_number = data[1];
                movie_name = movie_name.split(episode_number + " ").last();
                video_info_.video_title = "[" + season_number + "." + episode_number + "] " + album_name + " - " + movie_name;
            }
        }


        video_info_.metadata_comment = details.value("description").toString();

        QString data = details.value("metadata").toString();
        QList<QString> list = data.split(" ");
        list.removeAll("");
        if (list.size() > 3) {
            QString data = list[2];
            if (!data.isEmpty()) {
                data = data.simplified();
                video_info_.metadata_year = data.replace(" ", "");
            }
        }

        const auto& img = details.value("image").toObject();
        video_info_.artwork_url = getImage(img.value("uri").toString());

        QString parent_url_part = details.value("parentTarget").toString(); //urn:hbo:page:GVU2cggagzYNJjhsJATwo:type:series
        if (!parent_url_part.isEmpty()) {
            QUrl episodes_url = "https://comet.api.hbo.com/express-content/" + parent_url_part + "?device-code=desktop&product-code=hboMax&api-version=v9.0&country-code=US&signed-in=true&profile-type=adult&content-space=hboMaxExperience&brand=HBO%20MAX&territory=HBO%20MAX%20DOMESTIC&editorial-country=US&navigation-channels=HBO%20MAX%20SUBSCRIPTION%7CHBO%20MAX%20FREE&upsell-channels=HBO%20MAX%20SUBSCRIPTION%7CHBO%20MAX%20FREE&playback-channels=HBO%20MAX%20SUBSCRIPTION%7CHBO%20MAX%20FREE&client-version=hadron_50.55&language=en-us";
            QMetaObject::invokeMethod(this, "requestEpisodes", Qt::QueuedConnection, Q_ARG(const QUrl&, episodes_url));
            qDebug() << "episodes_url: " << episodes_url;
        }
    }
}

void HBOInterceptor::requestEpisodes(const QUrl& url) {
    qDebug() << "episodes: " << url << ", accessToken: " << access_token_;

    if (access_token_.isEmpty()
        || episodes_ != nullptr)
        return;

    episodes_ = new WebContent(this);
    initRequest(episodes_);

    QObject::connect(episodes_, &WebContent::finished, this, [this]() {
        const auto& json_str = episodes_->data().toUtf8();
#ifdef _DEBUG
        QFile   file("episodes_" + QString::number(m) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(json_str);
            file.close();
        }
        m++;
#endif
        const auto root = QJsonDocument::fromJson(json_str);
        if (root.isEmpty())
            return;

        if (root.isArray()) {
            const auto items = root.array();
            if (root.isArray()
                && !items.isEmpty()) {

                QString playlist;
                QString base_id = base_url_.split(":").last();
                QString base_url_part = base_url_.split(base_id).first();

                if (!base_id.isEmpty()) {

                    for (const auto& item : items) {
                        const auto obj = item.toObject();
                        if (!obj.isEmpty()) {
                            const auto& id = obj.value("id").toString();
                            if (id.contains("urn:hbo:grid:")) {
                                const auto& body = obj.value("body").toObject();
                                if (!body.isEmpty()) {

                                    bool rigth_season = false;

                                    const auto references = body["references"].toObject();
                                    if (!references.isEmpty()) {
                                        const auto items = references["items"].toArray();
                                        if (!items.isEmpty()) {
                                            QList<QString> ids;
                                            playlist.clear();
                                            for (const auto i : items) {
                                                QString url_part = i.toString();
                                                QString id = url_part.split("tile:").last().split(":type").first();

                                                if (id == base_id)
                                                    rigth_season = true;
                                                else {
                                                    if (!ids.contains(id)) {
                                                        ids << id;

                                                        QString url = base_url_part + id;
                                                        qDebug() << "playlist item url: " << url;
                                                        playlist.append(url + "|");
                                                    }
                                                }
                                            }

                                            if (rigth_season)
                                                break;
                                        }

                                        if (rigth_season)
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
                //if (!playlist.isEmpty())
                //    video_info_.playlist = playlist;
            }
        }
    }, Qt::QueuedConnection);

    episodes_->reset();
    episodes_->add_header("authorization", access_token_);
    episodes_->fetch(url);
}

void HBOInterceptor::initRequest(WebContent* request) {
    request->setNetworkAccess(nam_);
    QObject::connect(request, &WebContent::finished, this, [=]() {
        if (!request->status())
            emit error(request->status());
        });
}

bool HBOInterceptor::parse_access_token(const QJsonObject& root) {
    const auto& access_token_val = root.value("access_token");
    if (!access_token_val.isString())
        return false;
    access_token_ = "Bearer " + access_token_val.toString();
    cdmLicense_->add_header("authorization", access_token_);
    cdmLicense2_->add_header("authorization", access_token_);
    return true;
}

//static
QString HBOInterceptor::getImage(const QString& uri_tmpl) {
    // "uri":"https://artist.api.cdn.hbo.com/images/GX_Z4kAgTsrqkwgEAAAAC/tile?v=b0b1321fcc15952ff728f705e6fa88a0&size={{size}}&compression={{compression}}&protection={{protection}}"
    QString uri = uri_tmpl;
    uri = uri.mid(0, uri.indexOf("&size"));
    uri += "&size=304x171&compression=low&protection=false&scaleDownToFit=false&productCode=hboMax";
    return uri;
}

bool HBOInterceptor::parse_manifest(const QString& id, const QJsonObject& body) {
    //if (id.contains(":extra:"))
    //    return false;

    emit hideWebEngine(ERequestType::unknown);

#ifdef _DEBUG
    QFile   jf("hbo-manifests id: " + id + "_" + QString::number(i) + + ".json");
    if (jf.open(QIODevice::ReadWrite)) {
        jf.resize(0);
        QTextStream(&jf) << QJsonDocument(body).toJson(QJsonDocument::Indented) << endl;
        jf.close();
    }
    i++;
#endif

    auto videos = body.value("videos").toArray();
    if (!videos.isEmpty()) {
        for (const auto& mObj : videos) {
            const auto m = mObj.toObject();
            const auto type = m["type"].toString();
            if (type != "urn:video:main")
                continue;

            QString duration = m["duration"].toVariant().toString().split(".").first();
            video_info_.duration = duration.toInt();

            const auto subs = m["textTracks"].toArray();
            if (!subs.isEmpty()) {
                for (const auto& sObj : subs) {
                    const auto s = sObj.toObject();
                    SubtitleTrackInfo track;
                    track.profile = SubtitleTrackInfo::SubtitleProfile::XML;
                    track.language_tag = SS2Q(normalizeLangTag(QS2S(s["language"].toString()))); // "en-US"
                    track.language = s["displayName"].toString(); // "English CC"
                    QString type = s["type"].toString();
                    if (type == "ClosedCaptions")
                        track.is_closed_captions = true;
                    else if (type == "Forced")
                        track.is_additional = true;

                    QString url = s["url"].toString();
                    if (!url.isEmpty()) {
                        track.urls.push_back(url);
                        //normalizeTrack(track);
                        tracks_info_.subtitle_tracks.push_back(track);
                    }
                }
            }
        }
    }

    bool res = false;
    auto mpd_url = body["manifest"].toString();
    if (mpd_url.contains("dash.mpd"))
        mpd_url = body["fallbackManifest"].toString();
    
    if (!mpd_url.isEmpty()) {
        if (ParseMPD(mpd_url)) {
            if (!tracks_info_.video_tracks.isEmpty()) {
                const auto drm = body["drm"].toObject();
                if (!drm.isEmpty()) {
                    QString wv_server_url = drm["licenseUrl"].toString();  // "https://comet.api.hbo.com/drm/license/widevine?keygen=playready&drmKeyVersion=2"
                    if (!wv_server_url.isEmpty()) {
#ifdef DECRYPTSERVICE
                        bool cdmRequestSent = false;
                        QString pssh_sent;
                        for (const auto& track : tracks_info_.video_tracks) {
                            if (track.is_encrypted
                                && !track.urls.isEmpty()) {

#ifdef PRIVATE_KEYS_DECRYPTOR
                                if (track.cench_pssh != pssh_sent)
                                    loadKeys(wv_server_url, track.cench_pssh);
#endif

                                if (!cdmRequestSent && track.profile == VideoProfile::AVC) {
                                    std::vector<uint8_t> init_data = getInitData(track.urls[0]);
                                    loadKeys(wv_server_url, init_data);
                                    cdmRequestSent = true;
                                }
                                
                                break; // todo: multiple pssh...
                            }
                        }
#endif
                    }
                }
#ifndef PRIVATE_KEYS_DECRYPTOR
                // emit after MagicKeys Server response if defined
                emit metadataIntercepted(QUrl());
#endif
                res = true;
            }
        }
    }

    return res;
}

bool HBOInterceptor::ParseMPD(const QString& url) {
    if (url.isEmpty())
        return false;

    QFileInfo   mpd_info = url;
    qDebug() << "Parsing MPD " << mpd_info.fileName() << " ...";
    QString                     manifest;
    MpdParserHBO                mpd_;
    if (downloadToString(url, manifest)) {
#ifdef _DEBUG
        QFile   file("hbo.mpd.xml");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(manifest.toStdString().c_str());
            file.close();
        }
#endif
        if (mpd_.read(manifest)) {
            qDebug() << "Parsed " << mpd_info.fileName();
            
            auto tracks = mpd_.get_tracks();
            
            if (video_info_.duration == 0
                && tracks.duration > 0)
                video_info_.duration = tracks.duration;

            if (tracks_info_.key_id.length() == 0
                && !tracks.key_id.isEmpty())
                tracks_info_.key_id = tracks.key_id;

            auto base_url = url.mid(0, url.lastIndexOf("/"));
            
            for each (auto &v in tracks.video_tracks) {
                if (video_info_.video_title.isEmpty())
                    break;

                auto ti = v;

                ti.contains_audio = false; // todo

                for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o) {
                    *o = base_url + "/" + *o;
                }

                normalizeTrack(ti);
                normalizeVideoTrack(ti);
                tracks_info_.video_tracks.push_back(ti);
#if _DEBUG
                for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                    qDebug() << "video (" << ti.size / 1024 / 1024 << " MB); codec:"<< ti.profile << ", size " << ti.height << " x " << ti.width << "; bitrate " << ti.bitrate << "; height " << ti.height << "; is_encrypted " << ti.is_encrypted << "; key_id " << tracks_info_.key_id << " : " << *o;
#endif
            }

            for each (auto &a in tracks.audio_tracks) {
                if (video_info_.video_title.isEmpty())
                    break;

                auto ti = a;

                for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                    *o = base_url + "/" + *o;

                normalizeTrack(ti);
                normalizeAudioTrack(ti);
                tracks_info_.audio_tracks.push_back(ti);
#if _DEBUG
                for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                    qDebug() << "audio " << ti.language_tag << /*" codec:" << ti.codec <<*/ " bitrate:" << ti.bitrate << " is_encrypted " << ti.is_encrypted << " size(" << ti.size / 1024 / 1024 << " MB): " << *o;
#endif
            }

            for each (auto &s in tracks.subtitle_tracks) {
                if (video_info_.video_title.isEmpty())
                    break;

                auto ti = s;

                for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                    *o = base_url + "/" + *o;

                //normalizeTrack(ti);
                tracks_info_.subtitle_tracks.push_back(ti);
#if _DEBUG
                for (QList<QString>::iterator o = ti.urls.begin(); o != ti.urls.end(); ++o)
                    qDebug() << "subtitle " << ti.language_tag << " ; " << ti.is_additional << " ; " << ti.is_closed_captions << " : " << *o;
#endif
            }
        }

        return true;
    }
    else {
        return false;
    }
}

#ifdef DECRYPTSERVICE
std::vector<uint8_t>  HBOInterceptor::getInitData(const QString& url) {
    auto recode_op = std::make_shared<recode::RecodeOp>(url.toUtf8().constData(), "", nullptr);
    auto init_data = recode_op->GetInitData();
    return init_data;
}

void HBOInterceptor::loadKeys(const QString& wv_server_url, std::vector<uint8_t> init_data) {
    qDebug() << "loadKeys ...";
    //auto recode_op = std::make_shared<recode::RecodeOp>(v.urls[urlscnt - 1].toUtf8().constData(), "", nullptr);
    //auto init_data = recode_op->GetInitData();
    //auto str = hexStr(init_data.data(), init_data.size());
    //qDebug() << "init_data extracted " << "(" << v.width << "x" << v.height << ") from: " << v.urls[0].toUtf8() << " | " << str.c_str();
    //cdm_initializing_ = true;
    
    //lic_url.setQuery(widevine2LicenseQuery);
    
    if (keys_loader_)
        delete keys_loader_;

    keys_loader_ = new CdmKeysLoader();
    keys_loader_->initCDMSession(init_data, wv_server_url, cdmLicense_);

}

void HBOInterceptor::loadKeys(const QString& wv_server_url, const QString pssh) 
{
    qDebug() << "load magicKeys ...";
    key_client_ = new CdmMagicKeysClient();
    QObject::connect(key_client_, &CdmMagicKeysClient::keysObtained, this, [this, pssh](const QList<QString>& kidKeys) {
        tracks_info_.setPrivateKeys(kidKeys, pssh);

        emit keysObtained(kidKeys, pssh);
        emit metadataIntercepted(QUrl());
    });
    
    QObject::connect(key_client_, &CdmMagicKeysClient::error, this, [this](const Status& err) {
        qDebug() << "magicKeyServer error: " << err;
        emit metadataIntercepted(QUrl());
    });

    key_client_->initCDMSession(pssh, wv_server_url, cdmLicense2_, video_info_.video_title, 8);
}

#endif
