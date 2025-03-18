#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include <portals.h>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"

#include "web_portal.h"


class WebInterceptor : public QWebEngineUrlRequestInterceptor {
    Q_OBJECT
public:
    WebInterceptor(QWebEnginePage *p = nullptr);
    ~WebInterceptor();

public:
    void                        initialize(const EPortal& portal, bool is_forced = false);

public:
    void                        startPage(bool forced_reload_page = false);
    void                        finishPage(bool ok);    
    void                        setSignedIn(bool signed_in = true);
    bool                        isSignedIn();
    void                        setSetting(const QString& data);
    void                        setAudioLanguages(const QList<QString>& selected_languages);
    void                        setAbortable(Abortable* abortable) { if (portal_interceptor_) portal_interceptor_->setAbortable(abortable); }
    bool                        cookiesAccepted();
	void						analyzePlaylist(VideoInfo& info) { portal_interceptor_->analyzePlaylist(info); }

    const VideoInfo&            videoInfo() const;
    const TracksInfo&           tracksInfo() const;

    QNetworkAccessManager*      networkAccess() { return &nam_; }

signals:
    void                        showWebEngine(const ERequestType&);
    void                        hideWebEngine(const ERequestType&);
    void                        metadataIntercepted(const QUrl&);
    void                        error(const Status&);
    void                        loadPlaylist(const QString&);
    void                        loadPlaylistPart(const QString&);
    void                        clickPageButton(const QString&);
    void                        saveSettings(const QString&);
    void                        loadSettings(const QString&);
    void                        loadAudioLanguages();
    void                        loginRequested(const QString&);
    void                        resetWebPage(const QString&);
    void                        keysObtained(const QList<QString>& kidKeys, const QString& pssh);

    //void                        loadPlaylistItem(const QString&);

private:
    Q_INVOKABLE void            interceptRequest(QWebEngineUrlRequestInfo &info) override;  //Called from IO thread of browser;
    Q_INVOKABLE void            interceptConfig(const QString& jsonStr, const QString& confName);
    Q_INVOKABLE void            interceptAppScript(const QUrl& url);
    Q_INVOKABLE void            autoLogin();

private:
    QWebEnginePage*             page_ = nullptr;
    WebPortal*                  portal_interceptor_ = nullptr;
    QNetworkAccessManager       nam_;
    QMap<EPortal, WebPortal*>   interseptors_;
};
