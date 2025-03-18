#pragma once

#include "web_portal.h"
#ifdef DECRYPTSERVICE
    #include "cdm_keys_loader.h"
#endif

class VendorWebPlayerScript;
class CdmKeysLoader;

class AppleMusicInterceptor : public WebPortal {
    Q_OBJECT

public:
    AppleMusicInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~AppleMusicInterceptor();

private:
    void                            initialize() override;
    void                            startPage(bool forced_reload_page) override;
    void                            finishPage(bool ok) override;
    void                            interceptRequest(QWebEngineUrlRequestInfo& info) override;    //Called from IO thread of browser;
    bool                            parserExists() override { return true; }

private:
    void                            initRequest(WebContent* request);

    //void                            loadKeysWithPssh(const std::vector<uint8_t> init_data);
    void                            loadKeys(const QString url);
    void                            onKeysLoaded();
    QString                         postChallenge(QByteArray& challenge);
    QByteArray                      extractLicense(const QByteArray& response);
    void                            parseMediaId(const QUrl& url);

    Q_INVOKABLE void                dumpOrigScript(const QUrl& url);
    //Q_INVOKABLE void                requestManifest(const QUrl& url);
    //Q_INVOKABLE void                requestMetadata(const QUrl& url);

    Q_INVOKABLE void                interceptConfig(const QString& jsonStr, const QString& confName);
    Q_INVOKABLE void                requestMetadata(const QUrl& url);

private:
    QWebEnginePage*                 page_ = nullptr;
    QNetworkAccessManager*          nam_ = nullptr;

    QPointer<WebContent>            metadata_;
    QPointer<WebContent>            manifest_;

#ifdef _DEBUG
    QList<WebContent*>              all_scripts_;
#endif

    QString                         media_user_token_;
    QString                         token_;
    QString                         trackUrl_; // https://music.apple.com/ru/album/close-to-you/1552018965?i=1552019511
    QString                         mediaId_;   // 1552019511
    //int                             i, j, k = 0;

    //int                             tracks_count_ = 0;
    int                             host_primise_id_ = -1;

    //QList<WebContent*>              cdm_license_requests_;
//#ifdef DECRYPTSERVICE
//    QList<AppleMusicKeysLoader*>   keys_loaders_;
//#endif
};
