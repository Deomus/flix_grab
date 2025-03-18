#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>
#include <QtGui>

#include <product_config.h>

#include "web_interceptor.h"


class WebPage : public QWebEnginePage {
    Q_OBJECT

public:
    explicit WebPage(QObject *parent = Q_NULLPTR);
    WebPage(QWebEngineProfile *profile, QObject *parent = Q_NULLPTR);

    ~WebPage();

public:
    void                    initInterceptor(const EPortal& portal, bool is_forced = false);

    const TracksInfo&       tracksInfo() const { return tracks_info_; }

    const VideoInfo&        videoInfo() const { return interceptor_->videoInfo(); }

    const Status&           status() const {return status_;}

    void                    clearUrl() { 
        url_.clear();
        forced_reload_page_ = false;
    }

    bool                    isAuthorizationRequested() const { return authorization_requested_; }
    bool                    isProfileRequested() const { return profile_requested_; }
    void                    resetKeys();

public:
    void                    forcedReloadPage() { forced_reload_page_ = true; };
    void                    loadSignInPage(const QString& sign_url = "");
    void                    setRetryLoadPageInterval(int retry_load_page_interval) { retry_load_page_interval_ = retry_load_page_interval; }
    void                    setSetting(const QString& data) { interceptor_->setSetting(data); }
    void                    setAudioLanguages(const QList<QString>& selected_languages) { interceptor_->setAudioLanguages(selected_languages); }
	void					setAbortable(Abortable* abortable) { interceptor_->setAbortable(abortable); }
	void					analyzePlaylist(VideoInfo& info) { interceptor_->analyzePlaylist(info); }
    //void                  parsePlaylist(const EPortal& portal, const QUrl& url);
    //void                  loadSignOutPage();

protected:
    //Here is json message of hooked object;
    bool                    javaScriptConfirm(const QUrl &securityOrigin, const QString &msg) override;
    void                    javaScriptConsoleMessage(JavaScriptConsoleMessageLevel , const QString &message, int , const QString &) override;

private:
    void                    initialize();
    void                    initUrlChecker();
    void                    createInterceptor();
    void                    clearTracksAndStatus();
    void                    initCookies();
    void                    initPageWatcher();
    void                    webEngineShow(const ERequestType& type);
    void                    webEngineHide(const ERequestType& type);
    void                    reloadVideoUrl();

signals:
    //Title Loaded all required data;
    void                    clickPageButton(const QString&);
    void                    saveSettings(const QString&);
    void                    loadSettings(const QString&);
    void                    loadAudioLanguages();
    void                    loadPlaylist(const QString&);
    void                    loadPlaylistPart(const QString&);
    void                    showWebEngineMinimized(const bool&);
    void                    titleInfoCollected();
    void                    magicKeysObtained();
    void                    error(const Status&);
    void                    resetWebPage();

    //Started Logging in process;
    void                    showWebEngine(const ERequestType&);
    void                    hideWebEngine(const ERequestType&);

private slots:
    Q_INVOKABLE void        handleError(const Status&);
    void                    newError(EStatusCode, const QString& message);
private:
    Status                                  status_;
    QString                                 url_;
    TracksInfo                              tracks_info_;
    //VideoInfo                               video_info_;
    int                                     retry_count_ = 0;
    bool                                    authorization_requested_ = false;
    bool                                    profile_requested_ = false;

    WebInterceptor*                         interceptor_ = nullptr;
    EPortal                                 portal_ = EPortal::kUnknown;
    bool                                    forced_reload_page_ = false;
    int                                     retry_load_page_interval_ = 0;
    bool                                    is_signed_in_ = false;
    QString                                 redirect_url_;

private:
    Q_DISABLE_COPY(WebPage);
};
