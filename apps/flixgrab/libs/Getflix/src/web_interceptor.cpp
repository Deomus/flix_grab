#include "web_interceptor.h"

#include "amazon_interceptor.h"
#include "amazonmusic_interceptor.h"
#include "hulu_interceptor.h"
#include "netflix_interceptor.h"
#include "hbo_interceptor.h"
#include "disney_interceptor.h"
#include "youtube_interceptor.h"
#include "spotify_interceptor.h"
#include "pornhub_interceptor.h"
#include "xvideos_interceptor.h"
#include "twitch_interceptor.h"
#include "instagram_interceptor.h"
#include "facebook_interceptor.h"
#include "dailymotion_interceptor.h"
#include "vimeo_interceptor.h"
#include "twitter_interceptor.h"
#include "soundcloud_interceptor.h"
#include "applemusic_interceptor.h"


WebInterceptor::WebInterceptor(QWebEnginePage *p)
    : QWebEngineUrlRequestInterceptor(p)
    , page_(p) {
}

WebInterceptor::~WebInterceptor() {
    qDebug() << "~WebInterceptor";
}

void WebInterceptor::startPage(bool forced_reload_page) {
    portal_interceptor_->startPage(forced_reload_page);
}

void WebInterceptor::finishPage(bool ok) {
    portal_interceptor_->finishPage(ok);
}

const VideoInfo& WebInterceptor::videoInfo() const {
    return portal_interceptor_->video_info();
}

const TracksInfo& WebInterceptor::tracksInfo() const {
    return portal_interceptor_->tracks_info();
}

void WebInterceptor::setSetting(const QString& data) {
    portal_interceptor_->setSetting(data);
}

void WebInterceptor::setAudioLanguages(const QList<QString>& selected_languages) {
    portal_interceptor_->setAudioLanguages(selected_languages);
}

void WebInterceptor::setSignedIn(bool signed_in) {
    portal_interceptor_->setSignedIn(signed_in);
}

bool WebInterceptor::isSignedIn() {
    return portal_interceptor_->isSignedIn();
}

bool WebInterceptor::cookiesAccepted() {
    return portal_interceptor_->cookiesAccepted();
}

Q_INVOKABLE void WebInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
	// Workaround for previous URL being intercepted instead of the current one.
	qDebug() << "Page URL: " << page_->url() << "; request URL: " << info.requestUrl();
	if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame && info.requestUrl() == page_->url())
		return;

    portal_interceptor_->interceptRequest(info);
}

Q_INVOKABLE void WebInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    portal_interceptor_->interceptConfig(jsonStr, confName);
}

Q_INVOKABLE void WebInterceptor::interceptAppScript(const QUrl& url) {
    portal_interceptor_->interceptAppScript(url);
}

Q_INVOKABLE void WebInterceptor::autoLogin() {
    portal_interceptor_->autoLogin();
}

void WebInterceptor::initialize(const EPortal& portal, bool is_forced) {
    if (is_forced) {
        if (portal_interceptor_ != nullptr)
            delete portal_interceptor_;
    }

    for (auto it : interseptors_.keys()) {
        if (it == portal) {
            if (is_forced) {
                interseptors_.remove(it);
            }
            else {
                portal_interceptor_ = interseptors_.value(it);
                qDebug() << "Interceptor initialize existing portal: " << portal;
                return;
            }
        }
    }

    qDebug() << "Interceptor initialize new portal: " << portal;
    switch (portal) {
        case kAmazon:
            portal_interceptor_ = new AmazonInterceptor(page_, &nam_, this);
            break;
        case kAppleMusic:
            portal_interceptor_ = new AppleMusicInterceptor(page_, &nam_, this);
            break;
        case kAmazonMusic:
            portal_interceptor_ = new AmazonMusicInterceptor(page_, &nam_, this);
            break;
        case kHulu:
            portal_interceptor_ = new HuluInterceptor(page_, &nam_, this);
            break;
        case kNetflix:
            portal_interceptor_ = new NetflixInterceptor(page_, &nam_, this);
            break;
        case kHBO:
            portal_interceptor_ = new HBOInterceptor(page_, &nam_, this);
            break;
        case kDisney:
            portal_interceptor_ = new DisneyPlsInterceptor(page_, &nam_, this);
            break;
        case kYoutube:
            portal_interceptor_ = new YoutubeInterceptor(page_, &nam_, this);
            break;
        case kSpotify:
            portal_interceptor_ = new SpotifyInterceptor(page_, &nam_, this);
            break;
        case kPornhub:
            portal_interceptor_ = new PornhubInterceptor(page_, &nam_, this);
            break;
        case kXvideos:
            portal_interceptor_ = new XVideosInterceptor(page_, &nam_, this);
            break;
        case kTwitch:
            portal_interceptor_ = new TwitchInterceptor(page_, &nam_, this);
            break;
        case kInstagram:
            portal_interceptor_ = new InstagramInterceptor(page_, &nam_, this);
            break;
        case kFacebook:
            portal_interceptor_ = new FacebookInterceptor(page_, &nam_, this);
            break;
        case kDailymotion:
            portal_interceptor_ = new DailymotionInterceptor(page_, &nam_, this);
            break;
        case kVimeo:
            portal_interceptor_ = new VimeoInterceptor(page_, &nam_, this);
            break;
        case kTwitter:
            portal_interceptor_ = new TwitterInterceptor(page_, &nam_, this);
            break;
        case kSoundcloud:
            portal_interceptor_ = new SoundcloudInterceptor(page_, &nam_, this);
            break;
    }

    QObject::connect(portal_interceptor_, SIGNAL(showWebEngine(const ERequestType&)), this, SIGNAL(showWebEngine(const ERequestType&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(hideWebEngine(const ERequestType&)), this, SIGNAL(hideWebEngine(const ERequestType&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(metadataIntercepted(const QUrl&)), this, SIGNAL(metadataIntercepted(const QUrl&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(keysObtained(const QList<QString>&, const QString&)), this, SIGNAL(keysObtained(const QList<QString>&, const QString&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(error(const Status&)), this, SIGNAL(error(const Status&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(loadPlaylist(const QString&)), this, SIGNAL(loadPlaylist(const QString&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(loadPlaylistPart(const QString&)), this, SIGNAL(loadPlaylistPart(const QString&)), Qt::UniqueConnection);

    QObject::connect(portal_interceptor_, SIGNAL(clickPageButton(const QString&)), this, SIGNAL(clickPageButton(const QString&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(saveSettings(const QString&)), this, SIGNAL(saveSettings(const QString&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(loadSettings(const QString&)), this, SIGNAL(loadSettings(const QString&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(loadAudioLanguages()), this, SIGNAL(loadAudioLanguages()), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(loginRequested(const QString&)), this, SIGNAL(loginRequested(const QString&)), Qt::UniqueConnection);
    QObject::connect(portal_interceptor_, SIGNAL(resetWebPage(const QString&)), this, SIGNAL(resetWebPage(const QString&)), Qt::UniqueConnection);
    

    //QObject::connect(portal_interceptor_, SIGNAL(loadPlaylistItem(const QString&)), this, SIGNAL(loadPlaylistItem(const QString&)), Qt::UniqueConnection);

    portal_interceptor_->initialize();
    if (portal == kAppleMusic || portal == kAmazon || portal == kAmazonMusic || portal == kHulu || portal == kNetflix || portal == kHBO
        || portal == kDisney || portal == kSpotify || portal == kPornhub || portal == kInstagram || portal == kFacebook)
        portal_interceptor_->setSignedIn(false);

    interseptors_.insert(portal, portal_interceptor_);
}