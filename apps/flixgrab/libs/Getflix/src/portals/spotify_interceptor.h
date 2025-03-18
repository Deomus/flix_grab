#pragma once

#include "web_portal.h"

class VendorWebPlayerScript;
class CdmKeysLoader;
class CdmMagicKeysClient;

class SpotifyInterceptor : public WebPortal
{
    Q_OBJECT
public:
    SpotifyInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~SpotifyInterceptor();

private:
    void                        initialize() override;
    void                        startPage(bool forced_reload_page = false) override;
    void                        interceptRequest(QWebEngineUrlRequestInfo& info) override;

private:
    void                        initRequest(WebContent* request);
    //QString                     parseState(const QString& data, const QUrl& url);
    void                        loadTrackUrls(const QString& file_id, int bitrate);
    void                        LoadKeysForFileId(const QString& fileId, const QString& track_url);
    void                        LoadKeysWithPssh(const std::vector<uint8_t> init_data);
    void                        loadPrivateKeys(const QString pssh);

    void                        LoadKeys(const QString& url);
    void                        PlayButtonClick();

    void                        onKeysLoaded();

    Q_INVOKABLE void            LoadLicenseUrl(const QUrl& wvGetUrl);
    Q_INVOKABLE void            interceptConfig(const QString& jsonStr, const QString& confName) override;
    Q_INVOKABLE void            requestPlaylist(const QUrl& url);
    Q_INVOKABLE void            interceptVendorScript(const QUrl& url);
    Q_INVOKABLE void            dumpOrigScript(const QUrl& url);

private:
    QWebEnginePage*                     page_ = nullptr;
    QNetworkAccessManager*              nam_ = nullptr;

    QPointer<VendorWebPlayerScript>     vendor_script_;
    QPointer<WebContent>                playlist_;
    QPointer<WebContent>                cdm_url_;

    QList<WebContent*>                  cdm_license_requests_;
#ifdef _DEBUG
    QList<WebContent*>                  all_scripts_;
#endif

    QList<WebContent*>                  cdmLicensePrivate_;// request to license server for private keys
    QList<CdmMagicKeysClient*>          key_client_;       // private keys loader

    //QPointer<WebContent>                track_;
    //QPointer<WebContent>                state_;
    //QString                             baseUrl_; // https://vod-l3c-na-east-1.media.dssott.com/ps01/disney/6fd8339f-f20c-4185-867a-ef92c622e06a

    QString                             base_url_;
    QString                             wv_server_url_; // = "https://gew-spclient.spotify.com/melody/v1/license_url?keysystem=com.widevine.alpha&mediatype=audio&sdk_name=harmony&sdk_version=4.15.0";
    //QString                             wv_server_url_;//https://gew-spclient.spotify.com/widevine-license/v1/audio/license?exp=1621365370&cp=&tok=A1122CF15DB1C555961BE4D5C0CFF9513CBCFACA4034E1DAF027584864180A89
    QString                             access_token_;

    volatile long                       tracks_count_ = 0;
    volatile long                       private_keys_count_ = 0;

#ifdef DECRYPTSERVICE
    int                                 host_primise_id_ = -1;
    QList<CdmKeysLoader*>               keys_loaders_;
#endif

private:
    std::chrono::system_clock::time_point start_time_; // performance profiling
};
