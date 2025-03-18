#pragma once

#include "web_portal.h"


class PornhubInterceptor : public WebPortal {
    Q_OBJECT

public:
    PornhubInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~PornhubInterceptor();
 
private:
    void                        initialize() override;
    void                        startPage(bool forced_reload_page) override;
    void                        interceptRequest(QWebEngineUrlRequestInfo &info) override;

private:
    enum EInterceptFlags {
        kTracksReady = 0,
        kVideoInfoReady_ = 0x1,
    };

    bool checkInterceptFlag(int value, int flag) {
        return (value == flag);
    }

    void setFlag(EInterceptFlags flag);
    bool is_tracks_ready_ = false;
    bool is_video_info_ready_ = false;

private:
    Q_INVOKABLE void            interceptLoaderScript(const QUrl& url);
    Q_INVOKABLE void            requestMetadata(const QUrl& url);
    Q_INVOKABLE void            requestHTML(const QUrl& url);
    Q_INVOKABLE void            requestM3U8(const QUrl& url);

private:
    void                        initRequest(WebContent* request);
    void                        parseHTML(const QString& content);
    Status                      parseRedirectedHTML(const QString& html, VideoInfo& video_info);
    void                        parseMetadata(const QString& content);

private:
    QPointer<WebContent>        webloader_script_;
    QPointer<WebContent>        metadata_;
    QPointer<WebContent>        html_;
    QPointer<WebContent>        m3u8_data_;


private:
    QWebEnginePage*             page_ = nullptr;
    QNetworkAccessManager*      nam_ = nullptr;
    int                         tracks_count_ = 0;
    int                         i = 0;
};
