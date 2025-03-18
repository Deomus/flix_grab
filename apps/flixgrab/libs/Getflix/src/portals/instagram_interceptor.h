#pragma once

#include "web_portal.h"


class InstagramInterceptor : public WebPortal
{
    Q_OBJECT
public:
    InstagramInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~InstagramInterceptor();

private:
    void                        initialize() override;
    void                        startPage(bool downgrade_to_sd = false) override;
    void                        finishPage(bool ok) override;
    void                        interceptRequest(QWebEngineUrlRequestInfo& info) override;

private:
    void                        initRequest(WebContent* request);

    Q_INVOKABLE void            interceptHTML(const QUrl& url);
    Q_INVOKABLE void            requestMedia(const QUrl& url);
    void                        getToken();
    void                        getImageTracks(const QJsonArray& candidates, int& images_count, QList<QString>& image_urls);
    void                        getVideoTracks(const QJsonArray& video_versions, int& video_count, const bool has_audio, QList<QString>& video_urls);

    void                        formatText(QString& text);
    
private:
    QWebEnginePage*                     page_ = nullptr;
    QNetworkAccessManager*              nam_ = nullptr;

    QPointer<WebContent>                html_;
    QPointer<WebContent>                media_;

private:
    QString                             base_url_;
    QString                             app_id_;
    QString                             claim_;
    QString                             media_url_;
    QString                             csrf_token_;
    std::chrono::system_clock::time_point start_time_; // performance profiling
};
