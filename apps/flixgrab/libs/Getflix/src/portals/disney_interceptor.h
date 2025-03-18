#pragma once

#include <service_callback.h>

#include "web_portal.h"


class AppDisneyScript;
class VendorScript;
class CdmKeysLoader;
class CdmMagicKeysClient;

class DisneyPlsInterceptor : public WebPortal {
    Q_OBJECT

public:
    DisneyPlsInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~DisneyPlsInterceptor();

private:
    void                        initialize() override;
    void                        startPage(bool forced_reload_page) override;
    void                        finishPage(bool ok) override;
    void                        interceptRequest(QWebEngineUrlRequestInfo& info) override;
    bool                        parserExists() override { return true; }

private:
    enum EInterceptFlags {
        kTracksReady = 0,
        kVideoInfoReady_ = 0x1,
        kPrivateKeysReady = 0x2,
    };

    bool checkInterceptFlag(int value, int flag) {
        return (value == flag);
    }

    void setFlag(EInterceptFlags flag);
    bool is_tracks_ready_ = false;
    bool is_privatekeys_ready_ = false;
    bool is_video_info_ready_ = false;

private:
    void                        initRequest(WebContent* request);

    bool                        getUrls(const QString& m3u8_url, const QString& url_file_path, QList<QString>& urls, QString& init_data);
    
    void                        parsePlaylistDisney(const QString& data, const QUrl& url);
    void                        parseEpisodes(const QString& season_id, const QString& jsondata);
    //void                        parseScenario(const QString& data);
    void                        parseDmcVideo(const QJsonObject& data);
    void                        parseSeasons(const QJsonObject& data);
    
    void                        loadPrivateKeys(const QString pssh);
    void                        loadKeys(const QString& init_data);
    void                        onKeysLoaded();

    //void                        loadChunckUrls(const QString& url, int track_id);

    Q_INVOKABLE void            interceptConfig(const QString& jsonStr, const QString& confName) override;
    Q_INVOKABLE void            interceptAppScript(const QUrl& url);
    Q_INVOKABLE void            interceptVendorScript(const QUrl& url);
    //Q_INVOKABLE void            interceptScenario(const QUrl& url);
    Q_INVOKABLE void            interceptPlaylist(const QUrl& url);
    Q_INVOKABLE void            requestSeason(const QUrl& url, const QString& season_id);
    Q_INVOKABLE void            dumpOrigScript(const QUrl& url);
    //Q_INVOKABLE void            interceptManifest(const QUrl& url);
    //Q_INVOKABLE void            interceptDmcVideos(const QUrl& url);

private:
    QWebEnginePage*             page_ = nullptr;
    QPointer<AppDisneyScript>   app_script_;
    QPointer<VendorScript>      vendor_script_;
    //QPointer<WebContent>        scenario_;
    QPointer<WebContent>        playlist_;

    QString                     base_url_; // https://www.disneyplus.com/video/
    QString                     base_m3u8_url_; // https://vod-l3c-na-east-1.media.dssott.com/ps01/disney/6fd8339f-f20c-4185-867a-ef92c622e06a

    QPointer<WebContent>        cdmLicense_;//"https://global.edge.bamgrid.com/widevine/v1/obtain-license"
    QPointer<WebContent>        cdmLicensePrivate_; // request to license server for private keys
    QPointer<CdmMagicKeysClient>     key_client_;
    const QUrl                  wv_server_url_ = QUrl("https://disney.playback.edge.bamgrid.com/widevine/v1/obtain-license");
    QString                     access_token_;

    QString                     season_url_part_;
    QList<QString>              season_ids_;
    int                         season_count_;

#ifdef _DEBUG
    int                         i = 0;
    int                         j = 0;
    int                         k = 0;
    QList<WebContent*>          all_scripts_;
#endif

private:
    QList<QPair<QString, QPair<int, QList<QPair<QString, QString>>>>> seasons_data_; //QPair<season_id, QList<QPair<season_number, QList<QPair<video_title + video_duration, video_url>>>>>
};
