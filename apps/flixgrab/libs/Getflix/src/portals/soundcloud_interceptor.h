#pragma once

#include "web_portal.h"


class SoundcloudInterceptor;

class SoundcloudInterceptor : public WebPortal {
    Q_OBJECT

public:
    SoundcloudInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~SoundcloudInterceptor();

private:
    void startPage(bool forced_reload_page) override;
    void initialize() override;
    Q_INVOKABLE void interceptRequest(QWebEngineUrlRequestInfo& info) override;    //Called from IO thread of browser;

private:
    Q_INVOKABLE void interceptHTML(const QUrl& url) {
        html_->fetch(url);
    }

    Q_INVOKABLE void interceptM3U8(const QUrl& url) {
        m3u8_data_->fetch(url);
    }

private:
    enum EInterceptFlags {
        kTracksReady = 0,
        kVideoInfoReady = 0x1,
    };

    bool checkInterceptFlag(int value, int flag) {
        return (value == flag);
    }

    void setFlag(EInterceptFlags flag);

    bool is_tracks_ready_ = false;
    bool is_video_info_ready_ = false;

private:
    void deleteCatalogCache();
    void parseM3U8(const QString& m3u8_data);
    void parseHTML(const QString& html);

private:
    QNetworkAccessManager*      nam_ = nullptr;
    QPointer<WebContent>        html_;
    QPointer<WebContent>        m3u8_data_;
};
