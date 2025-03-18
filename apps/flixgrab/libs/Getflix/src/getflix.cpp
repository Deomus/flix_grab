#include "getflix.h"

#include <assert.h>
//#include <chrono>
//#include <thread>

#include <QScreen>

#ifdef DECRYPTSERVICE
    #include <decrypsis.h>
#endif

#include "web_engine.h"
#include "get_utils.h"
#include "privatekeys_client.h"


#define MAX_LOAD_TRYING                         (3)


Getflix::Getflix(QObject *parent /*= 0*/) 
    : QObject(parent)
    , web_engine_(NULL)
    , web_page_(NULL) {

#ifdef DECRYPTSERVICE
    //Install Shuttle
    PxStatus status = InstallDecrypsis();
    qDebug() << "Install intercept module status: " << status;
#endif
    
    qRegisterMetaType<VideoTrackInfo>();
    qRegisterMetaType<AudioTrackInfo>();
    qRegisterMetaType<TracksInfo>();
    qRegisterMetaType<ImageTrackInfo>();
    qRegisterMetaType<Status>();

    setupWebEngine();
}

Getflix::~Getflix()
{
    qDebug() << "bye getflix...";

#ifdef DECRYPTSERVICE
    PxStatus status = UninstallDecrypsis();
    qDebug() << "Uninstall intercept module status: " << status;
#endif

    if (web_page_ != nullptr) {
        delete web_page_;
        web_page_ = NULL;
    }
    if (profile_) {
        delete profile_;
        profile_ = nullptr;
    }
    if (web_engine_ != nullptr) {
        delete web_engine_;
        web_engine_ = NULL;
    }
}

VideoHandle* Getflix::add(const QString& url) {
    qDebug() << "new url: " << url;
    VideoHandle* new_handle = createVideo(url);

    //VideoHandle* new_handle = new VideoHandle(url, this);

    if (new_handle) {
        //Start analyzing if needed;
        if (video_queue_.isEmpty()
            && video_in_process_ == nullptr)
            QMetaObject::invokeMethod(this, "requestNextVideo", Qt::QueuedConnection);

        video_queue_.enqueue(new_handle);
        /*if (video_in_process_ == nullptr)
        QMetaObject::invokeMethod(this, "requestNextVideo", Qt::QueuedConnection);*/

        emit created(new_handle);
    }
        
    return new_handle;
}

VideoHandle* Getflix::createVideo(const QString& url) {
    QString url_in = url;
    url_in = url_in.simplified();
    url_in.replace(" ", "");

    QUrl media_url = QUrl(url_in);

    if (media_url.host().isEmpty())
        media_url = QUrl(QString("https://%1").arg(url_in));

    if (media_url.scheme() != "https")
        media_url.setScheme("https");
    
    EPortal portal = EPortal::kUnknown;
    bool playlist_url = false;
    recognizeUrl(media_url, portal, playlist_url);
    if (portal != EPortal::kUnknown) {
        if (playlist_url
            && !isActivated()) {
            emit aboutToParsingPlaylist(portal);
        }
        else {
            media_url = correctUrl(portal, media_url);
            return new VideoHandle(portal, media_url, this);
        }
    }
    else {
        status_ = Status(EStatusCode::WrongUrl, url);
        emit error();
    }

    return nullptr;
}

void Getflix::handleTracksInfo(const TracksInfo& info) {
    qDebug() << "Getflix::handleTracksInfo";
    if (video_in_process_) {
        video_in_process_->handleTracksInfo(info);
    }
}

void Getflix::handleVideoInfo(const VideoInfo& info) {
    qDebug() << "Getflix::handleVideoInfo";
    if (video_in_process_) {
        video_in_process_->handleVideoInfo(info);
    }
}

void Getflix::analyze(VideoHandle* handle) {
    qDebug() << "Getflix::analyze";
    Q_ASSERT(video_in_process_ == nullptr);

    qApp->processEvents();

    video_in_process_ = handle;
    //current_url_ = handle->url();
    //QObject::connect(this, &Getflix2::error, handle, &VideoHandle::handleError, Qt::ConnectionType::QueuedConnection);
    status_.clear();
    if (!video_in_process_->isCancelling()) {
        
        if (video_in_process_->portal() != EPortal::kUnknown) {
            //Load Catch handle;
            video_in_process_->handleAnalyzing();

            qDebug() << "Load url: " << video_in_process_->url().toString();
            //std::this_thread::sleep_for(std::chrono::nanoseconds(10000000000));  //10s

            int retry_interval = retryLoadPageTimeout(video_in_process_->portal());
            if (retry_load_interval_ > retry_interval) //proxy case
                retry_interval = retry_load_interval_;

            watchdog_.setInterval(retry_interval);
            watchdog_.start();
            QSize page_size = signInPageSize(video_in_process_->portal());
            if (!page_size.isEmpty()
                && web_engine_)
                web_engine_->resize(page_size);

            //web_engine_->showNormal(); //uncomment to show qtbrowser window

            web_page_->resetKeys();
            web_page_->clearUrl();
            web_page_->initInterceptor(video_in_process_->portal());
            web_page_->setAbortable(handle);
            web_page_->load(video_in_process_->url());
            load_trying_ = 0;
        }
        else {
            newError({ EStatusCode::WrongUrl, video_in_process_->url().toString() });
        }
    }
}

void Getflix::analyzePlaylist(VideoHandle* handle) {
	qDebug() << "Getflix::analyzePlaylist";
	Q_ASSERT(video_in_process_ == nullptr);

	qApp->processEvents();

	video_in_process_ = handle;

	if (handle->isCancelling())
		return;

	handle->handleAnalyzingPlaylist();
	VideoInfo info = handle->videoInfo();
	web_page_->setAbortable(handle);
	web_page_->analyzePlaylist(info);

	video_in_process_ = nullptr;

	qDebug() << "Getflix::analyzePlaylist done with " << info.playlist.count() << " entries";

	if (handle->isCancelling())
		return;

	handle->handleVideoInfo(info);

	requestNextVideo();
}

void Getflix::requestNextVideo() {  
    qDebug() << "Getflix::requestNextVideo";

	web_page_->setAbortable(nullptr);

    if (!video_in_process_) {
        postponed_downloads_.remove(video_in_process_);
        //video_in_process_ = nullptr;
        //current_url_.clear();
        //is_video_processing_ = !video_queue_.isEmpty();
        //if (is_video_processing_) {
        status_.clear();
        if (!video_queue_.isEmpty()) {
            qDebug() << "Getflix::requestNextVideo2";
            //video_in_process_ = video_queue_.dequeue();
            //video_queue_.pop_back();
            QMetaObject::invokeMethod(this, "analyze", Qt::QueuedConnection, Q_ARG(VideoHandle*, video_queue_.dequeue()));
            //analyze(video_queue_.dequeue());
		}
		else if (!playlist_queue_.isEmpty()) {
			QMetaObject::invokeMethod(this, "analyzePlaylist", Qt::QueuedConnection, Q_ARG(VideoHandle*, playlist_queue_.dequeue()));
		}
    }
}

void Getflix::setupWebEngine() {

    watchdog_.setSingleShot(true);

    if (web_page_)
        web_page_->deleteLater();

    profile_ = new QWebEngineProfile("fga");
    profile_->setHttpUserAgent(USER_AGENT);
    //profile_->setHttpCacheType(QWebEngineProfile::HttpCacheType::MemoryHttpCache);
    //profile_->setHttpCacheMaximumSize(100);
    //profile_->clearHttpCache();
    
    QWebEngineProfile::defaultProfile()->setHttpUserAgent(USER_AGENT); //Chrome Generic
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36"); //Chrome 89.0 Win10
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) QtWebEngine/5.12.3 Chrome/69.0.3497.128 Safari/537.36"); Chrome 69 Windows 8
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36"); Chrome 89 Linux
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 11_2_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36"); Chrome 89 Mac
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36 Edg/89.0.774.57"); //Edge 89.0 Win10
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:86.0) Gecko/20100101 Firefox/86.0"); //Firefox 86.0 Win10
    //QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15"); //Safari 14.0 macOS

    if (!QSslSocket::supportsSsl()) // https://slproweb.com/products/Win32OpenSSL.html
        qDebug() << "Error. OpenSSL need to install: " << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    web_page_ = new WebPage(profile_, this);
    
    if (retry_load_interval_ > 0) //proxy case
        web_page_->setRetryLoadPageInterval(retry_load_interval_ / 2);

    //Timeout of parsing;
    QObject::connect(&watchdog_, &QTimer::timeout, [=]() {
        if (video_in_process_) {
            if (++load_trying_ < MAX_LOAD_TRYING) {
                qDebug() << "Watchdog reload...";
                web_page_->clearUrl();
                web_page_->load(video_in_process_->url());
                watchdog_.start();
            }
            else {
                newError({ EStatusCode::ParseTimeout, tr("Video can't be parsed")});
            }
        }
    });
    
    QObject::connect(web_page_, &WebPage::titleInfoCollected, this, [=]() {
        qDebug() << " titleInfoCollected ";
        handleTracksInfo(web_page_->tracksInfo());
        handleVideoInfo(web_page_->videoInfo());
    });

    QObject::connect(web_page_, &WebPage::magicKeysObtained, this, [=]() {
        qDebug() << " magicKeysObtained ";
        //std::this_thread::sleep_for(std::chrono::seconds(10));  //10s
        //handleMagicKeys(web_page_->tracksInfo());
    });

    QObject::connect(web_page_, &WebPage::error, this, &Getflix::newError);

    if (web_engine_)
        web_engine_->deleteLater();

    web_engine_ = new WebEngine();
    web_engine_->setWindowModality(Qt::ApplicationModal);
    //web_engine_->show();
    //HACK:
    web_engine_->setPage(web_page_);

    QObject::connect(web_page_, &WebPage::showWebEngine, this, [=](const ERequestType& type) {
        watchdog_.stop();

        qDebug() << "showWebEngine request type: " << type;

        switch (type) {
            case ERequestType::login:
                qDebug() << "login requested";
                break;
            case ERequestType::profile:
                qDebug() << "profile requested";
                break;
            default:
                qDebug() << "show web window";
                break;
        }

        web_page_->runJavaScript(QString("window.scrollTo(0, 0);"));
        web_engine_->scroll(0, 0);
        web_engine_->setWindowModality(Qt::ApplicationModal);
        web_engine_->activateWindow();
        web_engine_->showNormal();
        if (video_in_process_) {
            if (video_in_process_->portal() == EPortal::kHulu) {
                QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
                    int x = (screenGeometry.width() - web_engine_->width()) / 2;
                    int y = (screenGeometry.height() - web_engine_->height()) / 2;
                    web_engine_->move(x, y);
            }
        }
    });

    QObject::connect(web_page_, &WebPage::hideWebEngine, this, [=](const ERequestType& type) {
        qDebug() << "hideWebEngine request type: " << type;

        switch (type) {
            case ERequestType::login:
                qDebug() << "login completed";
                break;
            case ERequestType::profile:
                qDebug() << "profile completed";
                break;
            case ERequestType::stopshowing:
                qDebug() << "stop showing web";
                web_engine_->close();
                break;
        }

        watchdog_.start();
        //HACK:
        web_engine_->hide();
        show_minimized_ = false;
    });

    QObject::connect(web_engine_, &WebEngine::windowHidden, this, &Getflix::handleWindowHidden);
    QObject::connect(web_page_, &WebPage::iconChanged, web_engine_, &QWebEngineView::setWindowIcon);
    QObject::connect(web_page_, &WebPage::resetWebPage, [this]() {
        qDebug() << "resetWebPage";
        QMetaObject::invokeMethod(this, "stopPage", Qt::QueuedConnection);
        setupWebEngine();
        
        if (video_in_process_) {
            load_trying_ = 0;
            QSize page_size = signInPageSize(video_in_process_->portal());
            if (!page_size.isEmpty()
                && web_engine_)
                web_engine_->resize(page_size);

            qDebug() << "resetWebPage url: " << video_in_process_->url().toString();
            web_page_->initInterceptor(video_in_process_->portal(), true);
            web_page_->forcedReloadPage();
            web_page_->load(video_in_process_->url());
            watchdog_.setInterval(watchdog_.interval() / 2);
            watchdog_.start();
        }
    });
    QObject::connect(web_page_, &WebPage::clickPageButton, this, &Getflix::clickPage);
    QObject::connect(web_page_, &WebPage::saveSettings, this, &Getflix::saveSetting);
    QObject::connect(web_page_, &WebPage::loadSettings, this, &Getflix::loadSetting);
    QObject::connect(web_page_, &WebPage::loadAudioLanguages, this, &Getflix::getAudioLanguages);
    QObject::connect(web_page_, &WebPage::loadPlaylist, this, &Getflix::loadPlaylist);
    QObject::connect(web_page_, &WebPage::loadPlaylistPart, this, &Getflix::loadPlaylistPart);
    QObject::connect(web_page_, &WebPage::showWebEngineMinimized, this, [=](const bool& show_minimized) {
        qDebug() << "showWebEngineMinimized show_minimized_: " << show_minimized_;
        show_minimized_ = show_minimized;
        watchdog_.stop();
        web_engine_->showMinimized();
    });
    
    /* TODO: only for netflix, but not work for now
    RegisterPKInterceptor(private_keys_ = new PrivateKeysClient(this));
    QObject::connect(private_keys_,
        &PrivateKeysClient::onChallenge,
        [](const char* message, uint32_t message_size) {
            OnSessionMessagePK(message, message_size);
    });*/
        
    //QObject::connect(web_page_, &WebPage::loadPlaylistItem, [=](const QString& item) {
    //    emit loadPlaylistItem(item);
    //});

}

void Getflix::handleWindowHidden() {
    if (web_page_->isAuthorizationRequested()
        || web_page_->isProfileRequested()) {
        qDebug() << "show_minimized_: " << show_minimized_;
        if (!show_minimized_) {
            web_page_->clearUrl();

            if (web_page_->isProfileRequested()) {
                web_page_->resetKeys();
                newError({ EStatusCode::CheckProfileFailed, tr("No profile selected") });
            }
            else if (web_page_->isAuthorizationRequested()) {
                web_page_->resetKeys();
                newError({ EStatusCode::RegistrationFailed, tr("Please, provide valid login/password") });
            }


            deleteCookies();
        }
    }
}

void Getflix::clickPage(const QString& pos) {
    // You need to find the first child widget of QWebEngineView. It can accept user input events.
    int left = pos.split(":").first().toInt();
    int top = pos.split(":").last().toInt();
    QWidget* eventsReciverWidget = nullptr;
    foreach(QObject * obj, this->web_engine_->children()) {
        QWidget* wgt = qobject_cast<QWidget*>(obj);
        if (wgt) {
            eventsReciverWidget = wgt;
            break;
        }
    }

    if (eventsReciverWidget == nullptr)
        return;

    qDebug() << "Click on " << left << "x" << top;

    QPoint clickPos(left, top);
    // Click
    // Press
    QMouseEvent* press = new QMouseEvent(QEvent::MouseButtonPress, clickPos, Qt::LeftButton, Qt::MouseButton::NoButton, Qt::NoModifier);
    QCoreApplication::postEvent(eventsReciverWidget, press);
    // Some delay
    QTimer::singleShot(100, [clickPos, eventsReciverWidget]() {
        QMouseEvent* release = new QMouseEvent(QEvent::MouseButtonRelease,
            clickPos,
            Qt::LeftButton,
            Qt::MouseButton::NoButton,
            Qt::NoModifier);
        QCoreApplication::postEvent(eventsReciverWidget, release);
    });
}

void Getflix::saveSetting(const QString& data) {
    emit saveSettings(data);
}

void Getflix::loadSetting(const QString& key) {
    emit loadSettings(key);
}

void Getflix::getAudioLanguages() {
    emit loadAudioLanguages();
}

void Getflix::onLoadPlaylist(const QString& playlist, bool end_of_playlist) {
    qDebug() << "Getflix loadPlaylist: " << playlist;
    watchdog_.stop();

    if (video_in_process_) {
        QList<QString> list = playlist.split("|");
        list.removeAll(QString(""));
        if (!list.isEmpty()) {
            if (end_of_playlist) {
                QString url = list.takeFirst();
                video_in_process_->updateItemUrl(url);
                web_page_->load(url);//load first url from playlist
                watchdog_.start();
            }

            if (!list.isEmpty()) {
                QString newplaylist;
                for (const auto it : list)
                    newplaylist.append(it + "|");
                emit loadPlaylistItem(newplaylist);
            }
        }
    }
}

void Getflix::loadPlaylistPart(const QString& playlist) {
    onLoadPlaylist(playlist);
}

void Getflix::loadPlaylist(const QString& playlist) {
    onLoadPlaylist(playlist, true);
}

void Getflix::newError(const Status& status) {
    qDebug() << "Getflix newError status: " << status_ << " :: " << status.message();

    status_ = status;
    
    //Stop checking state;
    watchdog_.stop();
    
    if (video_in_process_) {
        if (status.code() == EStatusCode::ForcedReloadPage   //reload page for amazon video with downgrade to sd
            || status.code() == EStatusCode::EmptyData) {
            //web_page_->clearUrl();
            if (status.code() == EStatusCode::ForcedReloadPage)
                web_page_->forcedReloadPage();
            web_page_->load(video_in_process_->url());
            watchdog_.start();
            return;
        }
    }

    if (status.code() == EStatusCode::StopPage) {
        QMetaObject::invokeMethod(this, "stopPage", Qt::QueuedConnection);
        return;
    }

    if (web_engine_ != nullptr)
        web_engine_->stop();

    if (video_in_process_) {
        video_in_process_->setError(status_);
        video_in_process_ = nullptr;
        
        /*QMetaObject::invokeMethod(video_in_process_, "handleEngineError", Qt::QueuedConnection
            , Q_ARG(QString, status), Q_ARG(QString, message));*/
    }
    else 
        emit error();
    //video_in_process_ = nullptr;
    
    requestNextVideo();
}

Q_INVOKABLE void Getflix::stopPage() {
    qDebug() << "WebPage stopPage";
    web_page_->triggerAction(QWebEnginePage::WebAction::Stop);
    web_page_->setUrl(QUrl());
    watchdog_.stop();
    //for (const auto& s : netflix_page_->scripts().toList())
    //{
        //qDebug() << "Stop script " << s.name();
        //netflix_page_->scripts().remove(s);
    //}
    //if (web_engine_ != nullptr) {
        //web_engine_->stop();
        //web_engine_->close();
    //}
}

void Getflix::handleVideoReady(VideoHandle* video) {
    qDebug() << "Getflix::handleVideoReady";
    Q_ASSERT(video_in_process_ == nullptr || video == nullptr || video == video_in_process_);
    
    qDebug() << "Video Ready: " << video->url();

    //Stop checking state;
    watchdog_.stop();

    if (video_in_process_ != nullptr) {
        Q_ASSERT(!videos_.contains(video));
        videos_.push_back(video);

        //Download video if it postponed;
        if (postponed_downloads_.contains(video)) {
            this->download(video);
        }

		if (video->videoInfo().playlist_kind == PlaylistKind::kPlaylistPending) {
			playlist_queue_.enqueue(video);
		}

        video_in_process_ = nullptr;
    }
    //else
    //    //Special case: cancel video;
    //    //video->doCancel();
    //    this->cancel(video);
    
    
  /*  QObject::disconnect(web_page_, &WebPage::parsingFinished, video, &VideoHandle::handleTracks);
    QObject::disconnect(parser_, &NetflixParser::parsedVideoInfo, video, &VideoHandle::handleVideo);*/
    //video_in_process_ = nullptr;
    if (web_engine_ != nullptr)
        web_engine_->stop();

    requestNextVideo();
}

void Getflix::handleVideoCancelling(VideoHandle* video) {
    qDebug() << "Getflix::handleVideoCancelling";
    Q_ASSERT(video_in_process_ == nullptr || video == nullptr || video == video_in_process_);

    qDebug() << "Video Cancelling: " << video->url();

    //Stop checking state;
    watchdog_.stop();
    if (web_engine_ != nullptr)
        web_engine_->stop();

    video_in_process_ = nullptr;
    requestNextVideo();
}

void Getflix::removeVideoInProcess(VideoHandle* video) {
    qDebug() << "Getflix::removeVideoInProcess";
    if (video_in_process_ != nullptr
        && video_in_process_ == video) {

        qDebug() << "Video InProcess Cancelling: " << video->url();

        //Stop checking state;
        watchdog_.stop();
        if (web_engine_ != nullptr)
            web_engine_->stop();

        video_in_process_ = nullptr;
        requestNextVideo();
    }
}

QUrl Getflix::correctUrl(const EPortal& portal, const QUrl& url) {
    QUrl new_url = url;

    if (portal == kAmazonMusic) {
        if (new_url.url().contains("?")
            && !new_url.url().contains("trackAsin"))
            new_url = new_url.url().split("?").first();
    }
    else if (portal == kNetflix) {

        if (!new_url.query().isEmpty())
            new_url = new_url.adjusted(QUrl::FormattingOptions(QUrl::RemoveQuery));

        if (new_url.url().contains("title", Qt::CaseInsensitive)) {
            QString last = new_url.url().replace("title", "watch").split("watch/").last();
            QString first = new_url.url().split("netflix.com/").first() + "netflix.com/";
            new_url = QUrl(first + "watch/" + last);
        }
    }
    else if (portal == kYoutube) {
        new_url = new_url.url().replace("#!", "?");

        new_url = new_url.url().replace("youtube.com/embed/", "youtube.com/watch?v=");

        new_url = new_url.url().replace("youtu.be/", "www.youtube.com/watch?v=");

        new_url = new_url.url().replace("youtubekids.com", "youtube.com");

        //youtube music case
        new_url = new_url.url().replace("/music.youtube", "/www.youtube");

        //mobile youtube case
        new_url = new_url.url().replace("/m.youtube", "/www.youtube");

        //short links case
        new_url = new_url.url().replace("/youtube.com", "/www.youtube.com");
    }
    else if (portal == kPornhub) {
        new_url = new_url.url().replace(".net/", ".com/");
        new_url = new_url.url().replace("video/show?", "view_video.php?");
    }
    else if (portal == kTwitch) {
        if (new_url.url().contains("player.twitch")) {
            QString video_id = new_url.url().split("video=").last();
            video_id.replace("v", "");
            new_url = QUrl("https://www.twitch.tv/videos/" + video_id);
        }
    }
    else if (portal == kInstagram) {
        if (!new_url.url().contains("www."))
            new_url = QUrl("https://www." + new_url.url().split("https://").last());
    }
    else if (portal == kDailymotion) {
        new_url = new_url.url().replace("/swf/video/", "/video/");
        new_url = new_url.url().replace("/swf/", "/embed/video/");
        new_url = new_url.url().replace("/video/video/", "/video/");
    }
    else if (portal == kTwitter) {
        new_url = new_url.url().replace("mobile.twitter.com", "twitter.com");
    }

    return new_url;
}

bool Getflix::urlRecognize(const QList<QRegularExpression>& url_regexp_list, const QUrl& url) {
    bool result = false;

    for (int i = 0; i < url_regexp_list.size(); ++i) {
        if (url_regexp_list.at(i).match(url.url()).hasMatch()) {
            result = true;
            break;
        }
    }

    return result;
}

void Getflix::recognizeUrl(const QUrl& url, EPortal& portal, bool& playlist_url) {
    QList<QRegularExpression> url_regexp;

    if (NETFLIXPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*netflix.*/watch/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*netflix.*/title/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kNetflix;
            return;
        }
    }

    if (AMAZONPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*primevideo.*/detail/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*amazon.*/video/detail/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*amazon.*/product/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*amazon.*/dp/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*amazon.*/gp/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*amazon.*/Amazon-Video/.*", QRegularExpression::PatternOption::CaseInsensitiveOption); //free with ads

        if (urlRecognize(url_regexp, url)
            && !url.url().contains("signin")) {
            portal = EPortal::kAmazon;
            return;
        }
    }

    if (AMAZONMUSICPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*music.amazon.com.*marketplaceId=.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*music.amazon.com.*/playlists/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*music.amazon.com.*/albums/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*music.amazon.com.*/artists/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*music.amazon.com.*/artist/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        //url_regexp << QRegularExpression("http[s]?://.*music.amazon.com.*/podcasts/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kAmazonMusic;
            if (!url.toEncoded().contains("trackAsin"))
                playlist_url = true;
            return;
        }
    }

    if (DISNEYPLUSPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*disneyplus.*/video/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*disneyplus.*/movies/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kDisney;
            return;
        }
    }

    if (HBOPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*hbomax.*/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*play.max.com.*/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kHBO;
            return;
        }
    }

    if (HULUPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*hulu.*/watch/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://auth.hulu.*/web/login.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kHulu;
            return;
        }
    }

    if (YOUTUBEPORTALENABLED) {
        url_regexp.clear();
        //playlist item
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/playlist\\?list\\=.*", QRegularExpression::PatternOption::CaseInsensitiveOption); //playlist https://www.youtube.com/playlist?list=PLbaNZQ0FlptfNsDh_4fY_Z5cnT6txSJ4V
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/featured", QRegularExpression::PatternOption::CaseInsensitiveOption); //featured https://www.youtube.com/@explosiveexk7238/featured
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/channel/.*", QRegularExpression::PatternOption::CaseInsensitiveOption); //featured https://www.youtube.com/channel/UCpza-HUQDKzOtaODou_yBgA
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/user/.*", QRegularExpression::PatternOption::CaseInsensitiveOption); //featured https://www.youtube.com/user/allonwolf
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/videos", QRegularExpression::PatternOption::CaseInsensitiveOption); //videos https://www.youtube.com/@explosiveexk7238/videos
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/shorts", QRegularExpression::PatternOption::CaseInsensitiveOption); //shorts https://www.youtube.com/@fusion_technologies/shorts
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/streams", QRegularExpression::PatternOption::CaseInsensitiveOption); //streams https://www.youtube.com/@iamlpofficial/streams
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/playlists", QRegularExpression::PatternOption::CaseInsensitiveOption); //playlists https://www.youtube.com/@fusion_technologies/playlists
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/channels", QRegularExpression::PatternOption::CaseInsensitiveOption); //channels https://www.youtube.com/@YouTube/channels?view=49&shelf_id=6
        //url_regexp << QRegularExpression("http[s]?://.*youtube.*/@.*/community", QRegularExpression::PatternOption::CaseInsensitiveOption); //community https://www.youtube.com/@Wylsacom/community
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/channel/.*/playlists", QRegularExpression::PatternOption::CaseInsensitiveOption); //playlists https://www.youtube.com/channel/UCpza-HUQDKzOtaODou_yBgA/playlists
        //url_regexp << QRegularExpression("http[s]?://.*youtube.*search_query=.*", QRegularExpression::PatternOption::CaseInsensitiveOption);  //search https://www.youtube.com/results?search_query=ghmgfhmdghmsfhmn

        if (urlRecognize(url_regexp, url)
            && !url.toEncoded().endsWith("/about")) {
            portal = EPortal::kYoutube;
            playlist_url = true;
            return;
        }

        url_regexp.clear();
        //single item
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/watch.*v\\=.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/embed/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*youtu.be/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*youtube.*/shorts/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kYoutube;
            return;
        }
    }

    if (SPOTIFYPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*open.spotify.com/track/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*open.spotify.com/playlist/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*open.spotify.com/album/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*open.spotify.com/artist/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        for (int i = 0; i < url_regexp.size(); ++i) {
			if (url_regexp.at(i).match(url.url()).hasMatch()) {
				portal = EPortal::kSpotify;
				return;
			}
        }
    }

    if (PORNHUBPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium\\.com/view_video.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium\\.net/view_video.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium\\.com/video/show.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium\\.net/video/show.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium\\.com/gif/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium\\.net/gif/show.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        url_regexp << QRegularExpression("http[s]?://.*pornhubpremium.*/embed.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*pornhub.*/embed.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        for (int i = 0; i < url_regexp.size(); ++i) {
			if (url_regexp.at(i).match(url.url()).hasMatch()) {
				portal = EPortal::kPornhub;
				return;
			}
        }
    }

    if (XVIDEOSPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://\\w*\\.xvideos\\.com.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kXvideos;
            return;
        }
    }

    if (TWITCHPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*twitch\\.tv.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kTwitch;
            return;
        }
    }

    if (INSTAGRAMPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*instagram.*/p/.*", QRegularExpression::PatternOption::CaseInsensitiveOption); // todo playlist support
        url_regexp << QRegularExpression("http[s]?://.*instagram.*/reels/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*instagram.*/reel/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)
            && !url.url().contains("/locations/")) {
            portal = EPortal::kInstagram;

            if (!url.toEncoded().contains("/p/")
                && !url.toEncoded().contains("/tv/"))
                playlist_url = true;

            return;
        }
    }

    if (FACEBOOKPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*facebook.*/reel/.*(\\d+)", QRegularExpression::PatternOption::CaseInsensitiveOption); //https://www.facebook.com/reel/2874087906075839
        url_regexp << QRegularExpression("http[s]?://.*facebook.*/video.*(\\d+)", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*facebook.*/videos.*(\\d+)", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*facebook.*/(?:pg/)?.*/videos/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*facebook.*/watch/.*(\\d+)", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*facebook.*/groups/.*(\\d+)", QRegularExpression::PatternOption::CaseInsensitiveOption);

        url_regexp << QRegularExpression("http[s]?://.*fb.watch/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kFacebook;
            return;
        }
    }

    if (DAILYMOTIONPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*dailymotion.*/video/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://.*dailymotion.*/swf/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        url_regexp << QRegularExpression("http[s]?://dai\\.ly/([^?/]+)", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kDailymotion;
            return;
        }
    }

    if (VIMEOPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*vimeo.*", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kVimeo;
            return;
        }
    }

    if (TWITTERPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*twitter.*/.*$", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kTwitter;
            return;
        }
    }

    if (SOUNDCLOUDPORTALENABLED) {
        url_regexp.clear();
        url_regexp << QRegularExpression("http[s]?://.*soundcloud.*/.*$", QRegularExpression::PatternOption::CaseInsensitiveOption);

        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kSoundcloud;
            return;
        }
    }

    if (APPLEMUSICPORTALENABLED) {
        url_regexp.clear();

        //single item
        url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/album/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
        if (urlRecognize(url_regexp, url)) {
            portal = EPortal::kAppleMusic;
            if (!url.toEncoded().contains("?i="))
                playlist_url = true;
            return;
        }
        url_regexp.clear();

        //playlist item
        url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/playlist/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);        //playlist
        url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/artist/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);          //artist
        //url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/station/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);       //live radio
        //url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/curator/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);       //live curator
        //url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/post/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);          //video interview
        //url_regexp << QRegularExpression("http[s]?://music.apple.com/.*/music-video/.*", QRegularExpression::PatternOption::CaseInsensitiveOption);
    }
}

void Getflix::signIn() {
    //If queue not empty, getflix is show sign in page automatically;
    if (video_in_process_ == nullptr
        && video_queue_.isEmpty()) {
        web_page_->loadSignInPage();
    }
}

void Getflix::deleteCookies() {
    qDebug() << "Getflix: deleteCookies and clear url_";
    web_page_->clearUrl();
    web_page_->profile()->cookieStore()->deleteAllCookies();
}

void Getflix::handleIconChanged(const QIcon& icon) {
    if (web_engine_ != nullptr)
        web_engine_->setWindowIcon(icon);
}
