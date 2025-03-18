#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "web_scripts.h"
#include "web_content.h"
#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


struct VideoInfo;
struct TracksInfo;

enum ERequestType {
    unknown = 0,
    login = 1,
    profile = 2,
    showminimized = 3,
    stopshowing = 4
};
Q_DECLARE_METATYPE(ERequestType)


class WebPortal : public WebContent {
    Q_OBJECT
public:
    WebPortal(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr);
    virtual ~WebPortal();

public:
    virtual void                startPage(bool forced_reload_page);
    virtual void                finishPage(bool ok);
    virtual void                initialize();
    virtual void                interceptRequest(QWebEngineUrlRequestInfo &info);
    virtual void                interceptConfig(const QString& jsonStr, const QString& confName);
    virtual void                interceptAppScript(const QUrl& url);
    virtual void                autoLogin();
    virtual void                invalidate();
    virtual void                setSignedIn(bool signed_in = true) { signed_in_ = signed_in; }
    virtual void                analyzePlaylist(VideoInfo& info) { }    

public:
    const VideoInfo&            video_info() const { return video_info_; }
    const TracksInfo&           tracks_info() const { return tracks_info_; }
    QNetworkAccessManager*      networkAccess() { return nam_; }

    bool                        cookiesAccepted() { return cookies_accepted_; }
    bool                        isSignedIn() { return signed_in_; }
    void                        setSetting(const QString& data) { settings_ = data; }
    void                        setAudioLanguages(const QList<QString>& selected_languages) { selected_languages_ = selected_languages; }
	void						setAbortable(Abortable* abortable) { abortable_ = abortable; }

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

protected:
    Q_INVOKABLE void            interceptDeferScript(const QUrl& url);
    Q_INVOKABLE void            interceptMetadata(const QUrl& url);

protected:
    QWebEnginePage*             page_ = nullptr;
    QNetworkAccessManager*      nam_ = nullptr;
    WebScripts                  scripts_;
    VideoInfo                   video_info_;
    TracksInfo                  tracks_info_;
    bool                        signed_in_ = true;
    QString                     settings_;
    QList<QString>              selected_languages_;
    bool                        cookies_accepted_ = false;
	Abortable*					abortable_ = nullptr;

private:
    bool                        isModifyAsync() const override { return true; }
    Status                      modifyContent() override;

private:
    virtual void                createParser(QWebEnginePage* page, QNetworkAccessManager* nam);
    virtual bool                parserExists();
    virtual Status              parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url);
    virtual QString             signinURL();

private:
    QList<WebContent*>          defer_scripts_;	
//signals:
//    void                        loadPlaylistItem(const QString&);
};
