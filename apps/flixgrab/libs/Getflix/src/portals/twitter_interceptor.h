#pragma once

#include "web_portal.h"


class TwitterInterceptor : public WebPortal {
    Q_OBJECT

public:
    TwitterInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~TwitterInterceptor();

private:
    void                            initialize() override;
    void                            startPage(bool forced_reload_page) override;
    void                            finishPage(bool ok) override;
    void                            interceptRequest(QWebEngineUrlRequestInfo& info) override;    //Called from IO thread of browser;
    bool                            parserExists() override { return true; }

private:
    void                            initRequest(WebContent* request);
    Q_INVOKABLE void                requestGuestToken(const QUrl& url);
    Q_INVOKABLE void                requestMetadata(const QUrl& url);
    Q_INVOKABLE void                requestMain(const QUrl& url);
    //Q_INVOKABLE void                interceptConfig(const QString& jsonStr, const QString& confName);

private:
    void                            loadVmap(const QString& url, VideoInfo& video_info, TracksInfo& tracks_info);
    void                            parseM3U8(const QString& data, const QString& url, TracksInfo& tracks_info);

private:
    QWebEnginePage*                 page_ = nullptr;
    QNetworkAccessManager*          nam_ = nullptr;

    QPointer<WebContent>            guestTokendata_;
    QPointer<WebContent>            metadata_;
    QPointer<WebContent>            main_script_;

private:
    const QString                   token_url_ = "https://api.twitter.com/1.1/guest/activate.json";
    const QString                   bearer_ = "Bearer AAAAAAAAAAAAAAAAAAAAAPYXBAAAAAAACLXUNDekMxqa8h%2F40K4moUkGsoc%3DTYfbDKbT3jJPCEVnMYqilB28NHfOPqkca3qaAxGfsyKCs0wRbw";
    QString                         guest_token_;
    QString                         base_url_;
};
