#include "web_page.h"

#include <QtConcurrent>
#include <QFutureWatcher>

#include <product_config.h>

#include "web_interceptor.h"
#include "netflix_interceptor.h"

#define MAX_RETRY                       3


WebPage::WebPage(QObject *parent /*= Q_NULLPTR*/) : QWebEnginePage(parent) {
    setAudioMuted(true);
    initialize();
}

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent /*= Q_NULLPTR*/) : QWebEnginePage(profile, parent) {
    setAudioMuted(true);
    initialize();
}

WebPage::~WebPage() {
    qDebug() << "~WebPage";
}

void WebPage::initInterceptor(const EPortal& portal, bool is_forced) {
    portal_ = portal;
    interceptor_->initialize(portal, is_forced);
    if (portal_!= kPornhub)
        is_signed_in_ = interceptor_->isSignedIn();

    if (portal_ == kFacebook) {
        is_signed_in_ = false;
        interceptor_->setSignedIn(is_signed_in_);
    }

    if (is_forced) { //hbo case
        is_signed_in_ = true;
        interceptor_->setSignedIn(is_signed_in_);
    }
}

void WebPage::initialize() {
    createInterceptor();
    initUrlChecker();
    this->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    this->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    this->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    this->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    this->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    this->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    this->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    //this->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    //this->settings->setAttribute(QWebEngineSettings::AutoLoadImages, false);

    initCookies();
    initPageWatcher();

    //profile()->clearHttpCache();
    //profile()->setHttpCacheType(QWebEngineProfile::HttpCacheType::NoCache);

    QObject::connect(this, &QWebEnginePage::proxyAuthenticationRequired, [](const QUrl&, QAuthenticator *authenticator, const QString&) {
        qDebug() << "Request proxy authentication.";
        QNetworkProxy proxy{ QNetworkProxy::applicationProxy() };
        authenticator->setUser(proxy.user());
        authenticator->setPassword(proxy.password());
    });
}

void WebPage::initUrlChecker() {
    QObject::connect(this, &QWebEnginePage::loadStarted, [=]() {
        clearTracksAndStatus();
        
        const QString url = requestedUrl().toEncoded();
        qDebug() << "Page: urlchecker start:"
            << " url: " << url
            << " url_: " << url_
            << " authorization_requested_: " << authorization_requested_ 
            << " profile_requested_: " << profile_requested_
            << " forced_reload_page: " << forced_reload_page_
            << " is_signed_in_: " << is_signed_in_;

        if (!url.isEmpty()
            && url != url_
            && !authorization_requested_
            && !profile_requested_) {

            if (!loginId(portal_).isEmpty()) {
                if (url.contains(loginId(portal_))) {
                    webEngineShow(ERequestType::login);
                }
                else if (!profileId(portal_).isEmpty()
                    && (url.contains(profileId(portal_))
                        || (portal_ == kDisney && (url.contains("password") || url.contains("enter-pin"))))) {
                    webEngineShow(ERequestType::profile);
                }
                else if (!(portal_ == kDisney && (url.contains("identity/loading") || url.contains("sign-up")))) {

                    url_ = url;

                    interceptor_->setSignedIn(is_signed_in_);
                            if (portal_ == kPornhub
                                && !url.contains("pornhubpremium"))
                                interceptor_->setSignedIn(true);
                    
                    interceptor_->startPage(forced_reload_page_);

                    qDebug() << "Page: urlchecker finish url_: " << url_;
                }
            }
            else if (portal_ == kYoutube) { //youtube  
                if (!url.contains(cookieAcceptedId(portal_))
                    || (url.contains(cookieAcceptedId(portal_))
                        && url.contains("continue="))) {

                    url_ = url;

                    webEngineHide(ERequestType::unknown);

                    interceptor_->startPage(forced_reload_page_);

                    interceptor_->setSignedIn(is_signed_in_);
                    qDebug() << "Page: urlchecker finish url_: " << url_;
                }

            }
            else {
                url_ = url;

                interceptor_->startPage(forced_reload_page_);

                interceptor_->setSignedIn(true);
                qDebug() << "Page: urlchecker finish url_: " << url_;
            }
        }
    });
}

void WebPage::createInterceptor() {
    interceptor_ = new WebInterceptor(this);
    qDebug() << "WebPage::createInterceptor ... ";
    profile()->setHttpUserAgent(USER_AGENT);

    //Add interceptor to page;
    profile()->setUrlRequestInterceptor(interceptor_); //profile()->setRequestInterceptor (interceptor_) qt-5.12.3;

    //Error handling;
    QObject::connect(interceptor_, &WebInterceptor::error, this, &WebPage::handleError);

    //Metadata;
    QObject::connect(interceptor_, &WebInterceptor::metadataIntercepted, this, [this](const QUrl&) {
        tracks_info_ = interceptor_->tracksInfo();

        qDebug() << "Page: metadata intercepted";
        if (!tracks_info_.video_tracks.isEmpty()
            || !tracks_info_.image_tracks.isEmpty()
            || (!tracks_info_.audio_tracks.isEmpty() && DEFAULTPRESET == kPreset_MP3Original)) {
            retry_count_ = 0;
            emit titleInfoCollected();
        }
    });

    QObject::connect(interceptor_, &WebInterceptor::keysObtained, this, [this](const QList<QString>& kidKeys, const QString& pssh) {
        qDebug() << "Page: "<< kidKeys.size() << " keys Obtained for " << pssh;
        //emit magicKeysObtained();
    });

    QObject::connect(interceptor_, &WebInterceptor::loginRequested, this, [=](const QString& url) {
        qDebug() << "Page: loginRequested url: " << url;
        is_signed_in_ = false;
        loadSignInPage(url);
    });

    QObject::connect(interceptor_, &WebInterceptor::showWebEngine, [=](const ERequestType& type) {
        qDebug() << "Page: showWebEngine type: " << type;
        webEngineShow(type);
    });
    
    QObject::connect(interceptor_, &WebInterceptor::hideWebEngine, [=](const ERequestType& type) {
        qDebug() << "Page: hideWebEngine type: " << type;
        webEngineHide(type);
    });

    QObject::connect(interceptor_, &WebInterceptor::loadPlaylist, this, [=](const QString& playlist) {
        emit loadPlaylist(playlist);
    });

    QObject::connect(interceptor_, &WebInterceptor::loadPlaylistPart, this, [=](const QString& playlist) {
        emit loadPlaylistPart(playlist);
    });

    //hbo cases
    QObject::connect(interceptor_, &WebInterceptor::clickPageButton, this, [=](const QString& pos) {
        emit clickPageButton(pos);
    });
    QObject::connect(interceptor_, &WebInterceptor::saveSettings, this, [=](const QString& data) {
        emit saveSettings(data);
    });
    QObject::connect(interceptor_, &WebInterceptor::loadSettings, this, [=](const QString& data) {
        emit loadSettings(data);
    });
    QObject::connect(interceptor_, &WebInterceptor::loadAudioLanguages, this, [=]() {
        emit loadAudioLanguages();
    });

    QObject::connect(interceptor_, &WebInterceptor::resetWebPage, this, [=](const QString& url) {
        emit resetWebPage();
    });
}

void WebPage::webEngineShow(const ERequestType& type) {
    bool is_show = false;
    bool is_show_minimized = false;

    switch (type) {
        case ERequestType::login:
            is_show = (!is_signed_in_ && !authorization_requested_ && !profile_requested_);
            authorization_requested_ = true;
            break;
        case ERequestType::profile:
            is_show = (!is_signed_in_ && !profile_requested_);
            is_signed_in_ = false;
            authorization_requested_ = false;
            profile_requested_ = true;
            break;
        case ERequestType::unknown:
            is_show = true;
            break;
        case ERequestType::showminimized:
            is_show_minimized = true;
            break;
    }

    if (is_show) {
        qDebug() << "webEngineShow type: " << type;
        emit showWebEngine(type);
    }
    else if (is_show_minimized) {
        qDebug() << "webEngineShowMinimized";
        emit showWebEngineMinimized(is_show_minimized);
    }
}

void WebPage::webEngineHide(const ERequestType& type) {
    bool is_hide = false;

    switch (type) {
        case ERequestType::login:
            is_hide = (authorization_requested_ && !profile_requested_);
            authorization_requested_ = false;
            break;
        case ERequestType::profile:
            is_hide = profile_requested_;
            is_signed_in_ = true;
            authorization_requested_ = false;
            profile_requested_ = false;
            break;
        case ERequestType::unknown:
            is_hide = true;
            resetKeys();
            break;
    }

    if (is_hide) {
        qDebug() << "webEngineHide type: " << type;

        if (type == ERequestType::profile)
            interceptor_->setSignedIn(is_signed_in_);

        emit hideWebEngine(type);
    }
}

void WebPage::clearTracksAndStatus() {
    tracks_info_.audio_tracks.clear();
    tracks_info_.all_audio_tracks.clear();
    tracks_info_.audio_languages.clear();
    tracks_info_.all_audio_languages.clear();
    tracks_info_.video_tracks.clear();
    tracks_info_.image_tracks.clear();
    tracks_info_.all_image_tracks.clear();
    tracks_info_.subtitle_tracks.clear();

    status_.clear();
}

void WebPage::newError(EStatusCode code, const QString& message) {
    handleError({code, message});
}

void WebPage::handleError(const Status& status) {
    qDebug() << " WebPage::handleError status_: " << status_ << "; status: " << status;
    authorization_requested_ = false;
    profile_requested_ = false;

    emit hideWebEngine(ERequestType::stopshowing);
    
    if (status_ != status) {
        if (status.code() == EStatusCode::StopPage) { //StopPage status.code() = 26
            is_signed_in_ = true;
            interceptor_->setSignedIn(is_signed_in_);
        }

        status_ = status;
        emit error(status_);

        //stop retry download
        retry_count_ = 0;
        url_.clear();
    }
}

void WebPage::reloadVideoUrl() {
    qDebug() << "reload_video_url: " << url_;
    if (!url_.isEmpty()) {

        const QString url = url_;
        url_.clear();

        if (portal_ == EPortal::kHBO) {
            QMetaObject::invokeMethod(interceptor_, "interceptAppScript", Qt::QueuedConnection, Q_ARG(QUrl, QUrl()));
            qDebug() << "reload hbo app.js";
            this->setUrl(url);
        }
        else {
            this->load(url);    //reload the same page
        }
    }
}

void WebPage::initCookies() {
#if _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    static QFile cookieLog("cookie_" + QString::number(ms) + ".log");
    cookieLog.open(QFile::WriteOnly);
    static QTextStream sCookieLog(&cookieLog);
#endif

    QWebEngineCookieStore* store = this->profile()->cookieStore();

    QObject::connect(store, &QWebEngineCookieStore::cookieAdded, this, [=](const QNetworkCookie& cookie) {
        QString requested_url = requestedUrl().toEncoded();
        QString cookie_name = cookie.name();
        QString cookie_value = cookie.value();
        qDebug() << "cookie+: " << cookie.name() << " cookie.value: " << cookie.value() << " cookie.domain: " << cookie.domain() << "; requested_url: " << requested_url <<  "; authorization_requested_: " << authorization_requested_ << "; profile_requested_: " << profile_requested_ << "; is_signed_in_: " << is_signed_in_ << "; url_: " << url_ << Qt::endl << Qt::endl;
#if _DEBUG
        sCookieLog << "cookie+: " << cookie.name() << " cookie.value: " << cookie.value() << " cookie.domain: " << cookie.domain() << "; requested_url: " << requested_url << "; authorization_requested_: " << authorization_requested_ << "; profile_requested_: " << profile_requested_ << "; is_signed_in_: " << is_signed_in_ << "; url_: " << url_ << Qt::endl << Qt::endl;
        sCookieLog.flush();
#endif
        bool reload_video_url = false;
        
        if (!cookieLoginPassedId(portal_).isEmpty()) {

            if (!is_signed_in_
                && !authorization_requested_
                && !profile_requested_
                && !cookieLoginRequestedId(portal_).isEmpty()
                && cookie_name == cookieLoginRequestedId(portal_)) {
                qDebug() << "cookieLoginRequestedId cookie_name: " << cookie_name;

                if (portal_ == EPortal::kSpotify) {
                    loadSignInPage(defaultLoginUrl(portal_) + "?continue=" + QString(requestedUrl().toEncoded()).toHtmlEscaped());
                }
                else if (portal_ == EPortal::kDisney) {
                    if (!loginId(portal_).isEmpty()
                        && requested_url.contains(loginId(portal_)))
                    loadSignInPage();
                }
                else {
                    loadSignInPage();
                }
            }

            if (requested_url.contains(profileId(portal_))) {
                is_signed_in_ = false;
                interceptor_->setSignedIn(is_signed_in_);

                qDebug() << "requested_url contains profileId" << profileId(portal_);
                webEngineShow(ERequestType::profile);
            }

            if ((portal_ != EPortal::kDisney && cookie_name.contains(cookieLoginPassedId(portal_)))
                || (portal_ == EPortal::kDisney && cookie_name == cookieLoginPassedId(portal_))
                || (portal_ == EPortal::kAmazon && cookie_name.contains("sess-at-acb"))) {
                qDebug() << "cookieLoginPassedId cookie_name: " << cookie_name;

                if (portal_ != EPortal::kNetflix) {
                    is_signed_in_ = true;
                    interceptor_->setSignedIn(is_signed_in_);
            }

                if (portal_ == EPortal::kHBO
                    && authorization_requested_) {
                    emit resetWebPage();
                }

                if (authorization_requested_
                    && !profile_requested_
                    && (requested_url.contains(loginId(portal_))
                    || (!oneTimePasswordId(portal_).isEmpty() && requested_url.contains(oneTimePasswordId(portal_)))  //amazon case
                    || (requestedUrl().toEncoded().contains("forceSignIn") && portal_ == EPortal::kAmazonMusic)
                    || (portal_ == EPortal::kAmazon)
                    || (portal_ == EPortal::kAppleMusic)
                    || (portal_ == EPortal::kHulu)
                    || (portal_ == EPortal::kHBO)
                    || (portal_ == EPortal::kPornhub)
                    || (portal_ == EPortal::kInstagram))) {
                    qDebug() << "requested_url contains loginId" << loginId(portal_);

                    if (portal_ != EPortal::kAmazon)
                        reload_video_url = true;

                    if (portal_ == EPortal::kHBO
                        || portal_ == EPortal::kAmazon)
                        webEngineShow(ERequestType::showminimized);
                else
                    webEngineHide(ERequestType::login);
           }
        }
        else if (portal_ == EPortal::kSpotify) {
            if (!is_signed_in_
                && !authorization_requested_
                && cookie_name == "sss")
                loadSignInPage(defaultLoginUrl(portal_) + "?continue=" + QString(requestedUrl().toEncoded()).toHtmlEscaped());
        }

        if (!cookieProfilePassedId(portal_).isEmpty()
            && cookie_name.contains(cookieProfilePassedId(portal_))
            && profile_requested_) {
            qDebug() << "cookieProfilePassedId cookie_name: " << cookie_name;

            is_signed_in_ = true;
            interceptor_->setSignedIn(is_signed_in_);

            webEngineHide(ERequestType::profile);
            reload_video_url = true;
        }

        if (portal_ == EPortal::kYoutube) {
            if (!is_signed_in_
                && cookie_name == "GPS") {
                is_signed_in_ = true;
                interceptor_->setSignedIn(is_signed_in_);
                reload_video_url = true;
            }
        }
        }

        bool blocked_cookie = false;
        for (int i = 0; i < blockedCookieIds(portal_).size(); ++i) {
            if (cookie.name().contains(blockedCookieIds(portal_).at(i).toUtf8())) {
                blocked_cookie = true;
                break;   
            }
        }
        if (!blocked_cookie)
            interceptor_->networkAccess()->cookieJar()->insertCookie(cookie);

        if (reload_video_url)
            reloadVideoUrl();
    });

    QObject::connect(store, &QWebEngineCookieStore::cookieRemoved, this, [=](const QNetworkCookie& cookie) {
        qDebug() << "cookie-: " << cookie.name() << "=" << cookie.value() << ";" << cookie.domain();
#if _DEBUG
        sCookieLog << "cookie-: " << cookie.name() << "=" << cookie.value() << ";" << cookie.domain();
        sCookieLog.flush();
#endif
        interceptor_->networkAccess()->cookieJar()->deleteCookie(cookie);
    });

    store->loadAllCookies();
}

void WebPage::initPageWatcher() {
    QObject::connect(this, &QWebEnginePage::loadProgress, [=](int progress) {
        qDebug() << "Getflix::loadProgress " << progress;
        if (portal_ == EPortal::kAmazon) {
            if (!interceptor_->cookiesAccepted())
                this->runJavaScript(QString("var d=document.getElementById('sp-cc');if(d){d.submit();console.log('cookies accepted!');}"));
            if (!interceptor_->cookiesAccepted()) //amazon prime case
                this->runJavaScript(QString("var d=document.getElementById('cookie-accept-all');if(d){d.submit();console.log('cookies accepted!');}"));
        }
        else if (portal_ == EPortal::kSpotify) {
            if (!interceptor_->cookiesAccepted())
                this->runJavaScript(QString("var d=document.getElementById('sp-cc');if(d){d.submit();console.log('cookies accepted!');}"));
        }
        else if (portal_ == EPortal::kDisney) {
            this->toHtml([this, progress](const QString& html) {
                this->runJavaScript(QString("var s=document.getElementById('onetrust-accept-btn-handler');if(s){s.click();console.log('cookies accepted!');}"));
                //this->runJavaScript(QString("var s=document.getElementById('Play');if(s)s.click();"));
            });
        }
        else if (portal_ == EPortal::kHulu) {
            this->runJavaScript(QString("var f=document.getElementsByTagName('footer'); if(f&&f.length>0){var next=f[0]; do{next=next.nextSibling; if(next&&next.nodeType==1){var d=next.getElementsByTagName('button');if(d&&d.length>0){var st=window.getComputedStyle(d[0]);if(!((st.display==='none')||(st.visibility==='hidden'))){ console.log('tag: ['+next.className+']. found button: ['+d[0].className+']');d[0].click();}}} }while(next);}"));
            //this->runJavaScript(QString("var f=document.getElementsByTagName('PauseAd PauseAd--expanded'); if(f&&f.length>0){var next=f[0]; do{next=next.nextSibling; if(next&&next.nodeType==1){var d=next.getElementsByTagName('button');if(d&&d.length>0){var st=window.getComputedStyle(d[0]);if(!((st.display==='none')||(st.visibility==='hidden'))){ console.log('ads enable: ['+next.className+']. found button: ['+d[0].className+']');d[0].click();}}} }while(next);}"));
            //this->runJavaScript(QString("var d=document.getElementById('a.ad-blocker-button');if(d){d.click();console.log('hulu ads accepted!');}"));
            this->toHtml([this, progress](const QString& html) {
#ifdef _DEBUG
                if (!requestedUrl().host().isEmpty()) {
                    QFile   html_file(requestedUrl().host() + "_progress" + QString::number(progress) + ".html");
                    if (html_file.open(QIODevice::ReadWrite)) {
                        html_file.resize(0);
                        QTextStream(&html_file) << html << Qt::endl;
                        html_file.close();
                    }
                }
#endif
                // <script id="__NEXT_DATA__" type="application/json">
                auto i = html.indexOf("type=\"application/json\"");
                if (i > 0) {
                    auto j = html.indexOf("{", i);
                    auto k = html.indexOf("</", i); // </script>
                    if (0 < j && j < k) {
                        QString jsonStr = html.mid(j, k - j); // "{..}"
                        QMetaObject::invokeMethod(interceptor_, "interceptConfig", Qt::QueuedConnection, Q_ARG(const QString&, jsonStr), Q_ARG(const QString&, "props:"));
                    }
                }
            });
        }
    });

    QObject::connect(this, &QWebEnginePage::loadFinished, [=](const bool ok) {
        qApp->processEvents();

        qDebug() << "Page: page load finished - requestedUrl: " << requestedUrl()
            << " url: " << url()
            << " result: " << ok
            << " authorization_requested_: " << authorization_requested_
            << " profile_requested_: " << profile_requested_
            << " is_signed_in_: " << is_signed_in_;

        const QString page_url = url().toEncoded();
        if ((page_url.isEmpty()
            || loginId(portal_).isEmpty())
            && portal_ != EPortal::kYoutube)
            return;

        if (ok) {
            if (page_url.contains(loginId(portal_))) {
                webEngineShow(ERequestType::login);

                //accepting cookies and other
                if (portal_ == EPortal::kNetflix)
                    this->runJavaScript("var e=document.querySelector('#cookie-disclosure .close-button'); if(e&&'function'==typeof e.click){e.click()}");
    #ifdef _DEBUG
                //if (portal_ == EPortal::kNetflix) {
                //    this->runJavaScript("var s=document.getElementById('id_userLoginId');if(s) s.value='freegrabapp@gmail.com'; s=document.getElementById('id_password');if(s) s.value='P#@ssw0rd121';");
                    //else if (portal_ == EPortal::kAmazon)
                    //    this->runJavaScript("var s=document.getElementById('ap_email');if(s) s.value='freegrabapp@gmail.com'; s=document.getElementById('ap_password');if(s) s.value='P2@ssw1rd'; s=document.getElementsByName('rememberMe');if(s.length>0)s[0].checked=true;");
                    //else if (portal_ == EPortal::kHBO) {
                    //    this->runJavaScript("var s=document.getElementById('textInput1207');if(s) s.value='freegrabapp@gmail.com'; s=document.getElementById('textInput1222');if(s) s.value='P@ssw0rd';");
                //}
    #endif
            }
            else if (page_url.contains(profileId(portal_))) {
                webEngineShow(ERequestType::profile);
            }
            else {
                this->toHtml([this](const QString& html) {
#ifdef _DEBUG
                    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
                    QFile   html_file(requestedUrl().host() + "_" + QString::number(ms) + ".html");
                    if (!requestedUrl().host().isEmpty()
                        && html_file.open(QIODevice::ReadWrite)) {
                        html_file.resize(0);
                        QTextStream(&html_file) << html << Qt::endl;
                        html_file.close();
                    }
#endif
                    if (portal_ == EPortal::kAmazon) {
                        QString sign_url;
                        auto match = QRegularExpression("\"asin\":\"([^\"]+)[\"]").match(html);     //amazon.com case
                        auto match_prime_block = QRegularExpression("\"atf\":([^&]+)[&]").match(html); //primevideo.com case
                        if (match.hasMatch() || match_prime_block.hasMatch()) {
                            QString asin;
                            if (match.hasMatch()) {
                                asin = match.captured(1);
                            }
                            else if (match_prime_block.hasMatch()) {
                                QString prime_block = match_prime_block.captured(1) + "\"";
                                auto match = QRegularExpression("gti=([^\"]+)[\"]").match(prime_block);
                                if (match.hasMatch())
                                    asin = match.captured(1);
                            }
                                
                            auto match = QRegularExpression("\"marketplaceID\":\"([^\"]+)[\"]").match(html);
                            QString marketplaceID;
                            if (match.hasMatch())
                                marketplaceID = match.captured(1);

                            QMetaObject::invokeMethod(interceptor_, "interceptConfig", Qt::QueuedConnection, Q_ARG(const QString&, asin), Q_ARG(const QString&, marketplaceID));
                        }
                    }
                    
                    if (is_signed_in_) {
                        if (portal_ == EPortal::kAmazonMusic) {
                            auto pos = html.indexOf("window.amznMusic =");
                            if (pos > 0 && (pos = html.indexOf("{", pos)) > 0) {
                                auto pos2 = html.indexOf("</script>", pos);
                                pos2 = html.lastIndexOf("}", pos2);
                                auto jsonStr = html.mid(pos, pos2 - pos + 1);
                                qDebug() << "config: " << jsonStr;
                                auto configName = "config";
                                QMetaObject::invokeMethod(interceptor_, "interceptConfig", Qt::QueuedConnection, Q_ARG(const QString&, jsonStr), Q_ARG(const QString&, configName));
                            }
                        }
                        else if (portal_ == EPortal::kAppleMusic) {
                            auto pos = html.indexOf("token%22%3A%22");
                            if (pos > 0) {
                                pos += 14;
                                auto pos2 = html.indexOf("%22", pos);
                                auto token = html.mid(pos, pos2 - pos);
                                qDebug() << "token: " << token;
                                auto configName = "token";
                                QMetaObject::invokeMethod(interceptor_, "interceptConfig", Qt::QueuedConnection, Q_ARG(const QString&, token), Q_ARG(const QString&, configName));
                            }
                        }
                    }
                    else {
                        qDebug() << "Page: page load finished toHtml";
                        if (portal_ == EPortal::kDisney) {
                            QTimer::singleShot(30000, [this]() {    //if still no secure cookie, then redirect to login page
                                qDebug() << "loadSignInPage Disney singleShot";
                                loadSignInPage();
                            });
                        }
                        else if (portal_ == EPortal::kHBO) {
                            QTimer::singleShot(20000, [this]() {     //if still no secure cookie, then redirect to login page
                                qDebug() << "loadSignInPage HBO singleShot";
                                loadSignInPage();
                            });
                        }
                        else if (portal_ == EPortal::kAmazon) {
                            QString sign_url;
                            auto match = QRegularExpression("\"feedbackSignInUrl\":\"([^\"]+)[\"]").match(html); //amazon prime case
                            if (match.hasMatch()) {
                                sign_url = match.captured(1).replace("&#38;","&");
                                qDebug() << "kAmazon sign url: " << sign_url;
                                if (!sign_url.startsWith("http")
                                    && !sign_url.startsWith("www"))
                                    sign_url = url().scheme() + "://" + url().host() + sign_url; // "https://www.primevideo.com/region/na/auth-redirect/ref=atv_nb_sign_in?signin=1&amp;returnUrl=...";
                            }

                            if (!sign_url.isEmpty()) {
                                qDebug() << "Sign url: " << sign_url;
                                loadSignInPage(sign_url);
                            }
                            else {//
                                auto match = QRegularExpression("<a\\s*href='([^()\"' ]+)'.*data-nav-role='signin'").match(html); //amazon case
                                if (match.hasMatch()) {
                                    sign_url = match.captured(1);
                                    if (!sign_url.startsWith("http") && !sign_url.startsWith("www"))
                                        sign_url = url().scheme() + "://" + url().host() + sign_url; // "https://www.amazon.com/ap/signin?openid.pape.max_auth_age=0&openid.return_to=https%3A%2F%2Fwww.amazon.com%2Fgp%2Fproduct%2FB016J3XU9Y%2F%3Fie%3DUTF8%26pf_rd_p%3D2d1ab404-3b11-4c97-b3db-48081e145e35%26pf_rd_r%3DA6DJP1PWZEP0N9CFMXN6%26redirect%3Dtrue%26returnFromLogin%3D1%26ref_%3Dnav_custrec_signin&openid.identity=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.assoc_handle=usflex&openid.mode=checkid_setup&openid.claimed_id=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.ns=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0&";
                                }

                                if (!sign_url.isEmpty()) {
                                    qDebug() << "Sign url: " << sign_url;
                                    loadSignInPage(sign_url);
                                }
                            }
                            //QString sign_url_direct = url().toEncoded();
                            //if (sign_url_direct == "about:blank")
                            //    return;

                            ////primvideo signedin: "customerID":"A2GB4MZ7IAX0TJ"
                            ////not signedin: "customerId":0
                            //QRegExp non_registered_rx_amazon("\"?customerId\"?\\s*:\\s*'?\"?([^'?\"?]*)['?\"?]", Qt::CaseInsensitive);
                            //if (non_registered_rx_amazon.indexIn(html) >= 0
                            //    && (non_registered_rx_amazon.cap(1).isEmpty() || non_registered_rx_amazon.cap(1) == "0")) {
                            //    qDebug() << "Page: registration request!";
                            //    QString sign_url;

                            //    QRegExp is_amazon_url("http[s]?://.*amazon.*", Qt::CaseInsensitive);
                            //    if (is_amazon_url.indexIn(requestedUrl().toEncoded()) >= 0) {
                            //        QRegExp sign_url_ex("\"signinContent\":([^}]+)[}]", Qt::CaseInsensitive);
                            //        if (sign_url_ex.indexIn(html) >= 0)
                            //            sign_url = sign_url_ex.cap(1).split("</a>").first().split("href='").last().split("'").first().replace("&amp;", "&");
                            //    }
                            //    //prime video url case      "signInLink":"https://www.primevideo.com:443/gp/redirect.html?_encoding=.....
                            //    if (sign_url.isEmpty()) {
                            //        QRegExp sign_url_ex("\"(signInLink|signInURL)\":\"([^()\"' ]*)\"", Qt::CaseInsensitive);
                            //        if (sign_url_ex.indexIn(html) >= 0) {
                            //            QString a = sign_url_ex.cap(1);
                            //            auto txt = sign_url_ex.capturedTexts();
                            //            if (txt.length() >= 3)
                            //                sign_url = txt[2];
                            //            sign_url = sign_url.replace("&#38;", "&");
                            //        }
                            //    }

                            //    if (sign_url.isEmpty())
                            //        sign_url = defaultSignInUrl(portal_);

                            //    QUrl qurl(url());
                            //    if (!sign_url.startsWith("http") && !sign_url.startsWith("www"))
                            //        sign_url = url().scheme() + "://" + url().host() + sign_url; // "https://www.amazon.com/gp/video/auth-redirect/ref=atv_dp_wn_sign_in?signin=1";
                            //    qDebug() << "Sign url: " << sign_url;

                            //    loadSignInPage(sign_url);
                            //}
                            //else if (sign_url_direct.contains(signInKeyword(portal_))) {
                            //    qDebug() << "Page: registration request:" << sign_url_direct;
                            //    loadSignInPage(sign_url_direct);
                            //}
                        }
                        else if (portal_ == EPortal::kAmazonMusic) {
                            auto match = QRegularExpression("href=\"(/forceSignIn\?[^()\"' ]+)\"").match(html);
                            if (match.hasMatch()) {
                                QString sign_url = match.captured(1);
                                sign_url = sign_url.replace("&#38;", "&").replace("&amp;", "&");
                                if (!sign_url.startsWith("http")
                                    && !sign_url.startsWith("www"))
                                    sign_url = url().scheme() + "://" + url().host() + sign_url;

                                if (!sign_url.isEmpty())
                                    loadSignInPage(sign_url);
                            }
                            else {
                                auto match = QRegularExpression("\"?customerId\"?\\s*:\\s*'?\"?([^'?\"?]*)['?\"?]", QRegularExpression::CaseInsensitiveOption).match(html);
                                if (match.hasMatch()) {
                                    QString cap0 = match.captured(0);
                                    QString cap = match.captured(1);
                                    if (cap.isEmpty()
                                        || cap == "0") {
                                        qDebug() << "Page: registration request!";
                                        loadSignInPage();
                                    }
                                }
                            }
                        }
                        else if (portal_ == EPortal::kHulu) {
                            QString sign_url;
                            QRegularExpressionMatch match = QRegularExpression("<button\\s+(?:[^>]*?\\s+)?href=([\"'])([^\"^'].*?)\\1.*?>Log In").match(html);
                            if (match.hasMatch())
                                sign_url = match.captured(2); // https://auth.hulu.com/web/login?next=...
                            else if (html.contains("href=\"/login\""))
                                sign_url = "https://id.hulu.jp/account/login?service=hulu&url=" + url().toEncoded(); // https%3A%2F%2Fwww.hulu.jp%2Fwatch%2F60844984
                            else if ((match = QRegularExpression("<a class=\"navigation__login-button\\s+(?:[^>]*?\\s+)?href=([\"'])([^\"^'].*?)\\1.*?>Log In").match(html))
                                .hasMatch())
                                sign_url = match.captured(2); // https://auth.hulu.com/web/login?next=...

                            if (!sign_url.isEmpty()) {
                                qDebug() << "Page: registration request!: " + sign_url;
                                redirect_url_ = requestedUrl().toString(); // backup originally requested url
                                loadSignInPage(sign_url);
                            }
                        }
                        else if (portal_ == EPortal::kNetflix) {
                            if (url().toEncoded().contains("watch/0?origId="))
                                newError(EStatusCode::VideoBlockedInLocation, "");
                            else {
                                QRegularExpression regexp("\"CURRENT_MEMBER\"\\s*:\\s*false", QRegularExpression::PatternOption::CaseInsensitiveOption);
                                QRegularExpressionMatch match = regexp.match(html);
                                if (match.hasMatch()) {
                                    qDebug() << "Page: registration request!";
                                    loadSignInPage();
                                }
                            }
                        }
                        else if (portal_ == EPortal::kYoutube) {
                            QString page_url = this->url().toEncoded();
                            if (!cookieAcceptedId(portal_).isEmpty()
                                && page_url.contains(cookieAcceptedId(portal_))) {

                                webEngineShow(ERequestType::showminimized);
                                this->runJavaScript(QString("var e = document.querySelector('form[method=\"POST\"]');if(e && e.length>0 && 'function' == typeof e[0].click){ e[0].click();console.log('I Agree click!')} else console.log('no found Agree btn.')"));
                            }

                            is_signed_in_ = true;
                            interceptor_->setSignedIn(is_signed_in_);
                            //QRegExp expression = QRegExp("\"SIGNIN_URL\":\"([^\"]+)[\"]", Qt::CaseInsensitive);
                            //if (expression.indexIn(html) != -1) {
                            //    QString sign_url = expression.cap(1).replace("\\/", "/").replace("\\u0026", "&");
                            //    if (!sign_url.isEmpty()) {
                            //        qDebug() << "Page: registration request!: " + sign_url;
                            //        loadSignInPage(sign_url);
                            //    }
                            //}
                            //else {
                            //    loadSignInPage(defaultSignInUrl(portal_) + url().toEncoded());
                            //}
                        }
                        else if (portal_ == EPortal::kSpotify) {
                            auto pos = html.indexOf("id=\"config\"");
                            if (pos > 0
                                && (pos = html.indexOf(">", pos)) > 0) {

                                auto pos2 = html.indexOf("</script>", pos);
                                auto jsonStr = html.mid(pos + 1, pos2 - pos - 1);
                                qDebug() << "config: " << jsonStr;
                                auto configName = "config";
                                QMetaObject::invokeMethod(interceptor_, "interceptConfig", Qt::QueuedConnection, Q_ARG(const QString&, jsonStr), Q_ARG(const QString&, configName));
                            }

                            if (html.indexOf("\"isAnonymous\":true") > 0
                                || html.indexOf(defaultLoginUrl(portal_)) > 0)
                                loadSignInPage(defaultLoginUrl(portal_) + "?continue=" + QString(requestedUrl().toEncoded()).toHtmlEscaped());
                        }
                        else if (portal_ == EPortal::kAppleMusic) {
                            auto pos = html.indexOf("href=\"/login\"");
                            if (pos > 0) { 
                                loadSignInPage(defaultLoginUrl(portal_) + "?continue=" + QString(requestedUrl().toEncoded()).toHtmlEscaped());
                            }
                        }
                        else if (portal_ == EPortal::kInstagram) {
                            QRegularExpression regexp("class=\"ENC4C\"\\s*href=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
                            QRegularExpressionMatch match = regexp.match(html);
                            if (match.hasMatch()) {
                                QString sign_url = match.captured(1).replace("&amp;", "&");
                                if (!sign_url.isEmpty()) {

                                    sign_url = requestedUrl().scheme() + "://" + requestedUrl().host() + sign_url;
                                    qDebug() << "Page: registration request!: " + sign_url;

                                    if (sign_url.contains(loginId(portal_)))
                                        loadSignInPage(sign_url);
                                    else
                                        loadSignInPage();
                                }
                            }
                            else {
                                loadSignInPage(defaultLoginUrl(portal_));
                            }
                        }
                        else if (portal_ == EPortal::kPornhub) {
                            QRegularExpression login_rx("canonical\" href=\"([^\">]+)[\">]", QRegularExpression::PatternOption::CaseInsensitiveOption);
                            QRegularExpressionMatch match_login_rx = login_rx.match(html);

                            QRegularExpression premium_rx("premiumUrl\":\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
                            QRegularExpressionMatch match_premium_rx = premium_rx.match(html);

                            //load login url
                            if (match_login_rx.hasMatch()) {
                                qDebug() << "Page: registration request!";
                                QString sign_url = match_login_rx.captured(1);
                                if (sign_url.contains(loginId(portal_)))
                                    loadSignInPage(sign_url);
                            }
                            //load premium redirect
                            else if (match_premium_rx.hasMatch()) {
                                QString premium_url = match_premium_rx.captured(1);
                                premium_url.replace("\\/", "/");
                                if (!premium_url.isEmpty()) {
                                    url_.clear();
                                    load(premium_url);
                                }
                            }
                        }
                    }
                });
            }
        }
        else {
            if ((!is_signed_in_
                || (is_signed_in_
                    && portal_ == EPortal::kAmazonMusic))
                && !url_.isEmpty()) {
                if (retry_count_ < MAX_RETRY) {
                    qDebug() << "reload page if error";
                    int retry_interval = retryLoadPageOnErrorTimeout(portal_);
                    if (retry_load_page_interval_ > retry_interval) //proxy case
                        retry_interval = retry_load_page_interval_;

                    QTimer::singleShot(retry_interval, [this]() {
                        if (!authorization_requested_
                            && !profile_requested_) {

                            if (retry_count_++ < MAX_RETRY)
                                this->load(url_);
                        }
                    });
                }
                else {
                    newError(EStatusCode::UnknownError, QString("Video '%1' can't load page").arg(requestedUrl().toString()));
                }
            }
        }
        //finish page by running defered scripts;
        interceptor_->finishPage(ok);
    });
}

void WebPage::loadSignInPage(const QString& sign_url) {
    qDebug() << "loadSignInPage portal_: " << portal_
        << " is_signed_in_: " << is_signed_in_
        << " authorization_requested_: " << authorization_requested_
        << " profile_requested_: " << profile_requested_
        << "loadSignInPage sign_url: " << sign_url;

    if (!is_signed_in_
        && !authorization_requested_
        && !profile_requested_) {

        webEngineShow(ERequestType::login);

        interceptor_->setSignedIn(false);

        QString url = sign_url;
        if (url.isEmpty())
            url = defaultLoginUrl(portal_);

        if (portal_ == EPortal::kHBO)
            QMetaObject::invokeMethod(interceptor_, "autoLogin", Qt::QueuedConnection);
        else
            this->load(url);
    }
}

void WebPage::resetKeys() {
    qDebug() << "resetKeys";
    authorization_requested_ = false;
    profile_requested_ = false;
}

bool WebPage::javaScriptConfirm(const QUrl& securityOrigin, const QString& msg) {
    Q_UNUSED(securityOrigin)
    Q_UNUSED(msg)

    qDebug() << "javaScriptConfirm: msg.length()=" << msg.length();

    return true;
}

void WebPage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int line, const QString& source) {
    if (message.contains("Active resource loading counts")) // no spam: Active resource loading counts reached to a per-frame limit while the tab is in background.
        return;
    // copy message to filelog:
    qDebug() << (level == JavaScriptConsoleMessageLevel::ErrorMessageLevel ? "JSERROR: " : level == JavaScriptConsoleMessageLevel::WarningMessageLevel ? "JSWARN: " : "JSINFO: ") << message << "; " << source << "(line:" << line << ")";

    const QList<QString> configs = { "Player config parameter:", "Final player config:", "Content config parameter:", "newContentConfig:", "fgapp_access_token:", "AUTHORIZATION:" };
    for (const auto& configName : configs) {
        const auto iConf = message.indexOf(configName, 0, Qt::CaseInsensitive);
        if (0 <= iConf) {
            const auto jsonStr = message.mid(iConf + configName.length() + 1);
            QMetaObject::invokeMethod(interceptor_, "interceptConfig", Qt::QueuedConnection, Q_ARG(const QString&, jsonStr), Q_ARG(const QString&, configName));
        }
    }
}
