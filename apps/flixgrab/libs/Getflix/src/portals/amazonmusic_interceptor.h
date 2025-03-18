#pragma once

#include "web_portal.h"
#ifdef DECRYPTSERVICE
    #include "amazon_music_keys_loader.h"
#endif

class VendorWebPlayerScript;
class AmazonMusicKeysLoader;

class AmazonMusicInterceptor : public WebPortal {
    Q_OBJECT

public:
    AmazonMusicInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~AmazonMusicInterceptor();

private:
    void                            initialize() override;
    void                            startPage(bool forced_reload_page) override;
    void                            finishPage(bool ok) override;
    void                            interceptRequest(QWebEngineUrlRequestInfo& info) override;    //Called from IO thread of browser;
    bool                            parserExists() override { return true; }

private:
    void                            initRequest(WebContent* request);

    void                            loadKeysWithPssh(const std::vector<uint8_t> init_data);
    void                            loadKeys(const QString url);
    void                            onKeysLoaded();
    QString                         postChallenge(QByteArray& challenge);
    QByteArray                      extractLicense(const QByteArray& response);
    void                            setTrackInfo(const TracksInfo& t);

    Q_INVOKABLE void                dumpOrigScript(const QUrl& url);
    Q_INVOKABLE void                requestManifest(const QUrl& url);
    Q_INVOKABLE void                requestMetadata(const QUrl& url);
    Q_INVOKABLE void                requestShowHome(const QUrl& url);
    Q_INVOKABLE void                interceptConfig(const QString& jsonStr, const QString& confName) override;

private:
    QWebEnginePage*                 page_ = nullptr;
    QNetworkAccessManager*          nam_ = nullptr;

    QPointer<WebContent>            metadata_;
    QPointer<WebContent>            manifest_;
    QPointer<WebContent>            showhome_;
#ifdef _DEBUG
    QList<WebContent*>              all_scripts_;
#endif
    QString                         base_url_;
    QString                         accessToken_;
    QString                         token_;
    QString                         timestamp_;
    QString                         rndNonce_;
    QString                         sessionId_;
    QString                         deviceId_;
    QString                         customerId_;
    QString                         marketplaceId_;
    QString                         musicTerritory_;
    QString                         trackUrl_; // https://music.amazon.com/albums/B091V7R59M?marketplaceId=ATVPDKIKX0DER&musicTerritory=US&trackAsin=B091V7JHXT
    QString                         trackAsin_; // B091V7JHXT
    QString                         albumAsin_; // B091V7R59M
    QString                         mediaId_;   // B091V6WKKX

    int                             tracks_count_ = 0;
    int                             host_primise_id_ = -1;

    //QList<WebContent*>              cdm_license_requests_;
//#ifdef DECRYPTSERVICE
//    QList<AmazonMusicKeysLoader*>   keys_loaders_;
//#endif
};
