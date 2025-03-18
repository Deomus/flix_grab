#pragma once

#include "web_portal.h"
#include "amazon_parser.h"
#ifdef DECRYPTSERVICE
    #include "amazon_keys_loader.h"
#endif


class AmazonInterceptor : public WebPortal {
    Q_OBJECT

public:
    AmazonInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~AmazonInterceptor();
#ifdef DECRYPTSERVICE
    void setKeysLoader(QPointer<AmazonKeysLoader> keys_loader) { keys_loader_ = keys_loader; };
#endif

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
    void initialize() override;
    void startPage(bool forced_reload_page) override;
    void invalidate() override;
    void interceptRequest(QWebEngineUrlRequestInfo &info) override;    //Called from IO thread of browser;
    bool parserExists() override { return true; }

    Status parse(const QString& metadata, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) override {
        Q_UNUSED(url);

        mpd_url_.clear();
        Status status = parser::parseMetadata(metadata, video_info, tracks_info, audioLangAvail_, mpd_url_);
        qDebug() << "video_info.url_type: " << video_info.url_type << " video_info_.video_title: " << video_info_.video_title << " base_url_: " << base_url_ << " season_urls_: " << season_urls_;

        if (video_info.url_type.contains("Show", Qt::CaseInsensitive)
            && !base_url_.isEmpty()
            && season_urls_.isEmpty()) {

            QMetaObject::invokeMethod(this, "interceptHTML", Qt::QueuedConnection, Q_ARG(const QUrl&, base_url_));
        }
        else {
            if (!video_info_.video_title.isEmpty())
                setFlag(kVideoInfoReady_);
        }

        qApp->processEvents();

        return status;
    }

private:
    Q_INVOKABLE void        interceptConfig(const QString& jsonStr, const QString& confName) override;
    Q_INVOKABLE void        interceptHTML(const QUrl& url);
    Q_INVOKABLE void        interceptSeason(const QUrl& url);
    //Q_INVOKABLE void        loadWidevine2License(const QUrl& url, const QString& post_data);
    //Q_INVOKABLE void        interceptManifest(const QString& url);

    //Q_INVOKABLE void        interceptDeferScript(const QUrl& url);

    //Q_INVOKABLE void      interceptPlayerScript(const QUrl& url);
    //Q_INVOKABLE void      interceptWebLoaderScript(const QUrl& url);
#ifdef _DUMP_ORIGINAL
    Q_INVOKABLE void        interceptOrigScript(const QUrl& url);
#endif
private:
    //void                    initCDMSession();
    //void                    onLoadResponseFromPRS(QPointer<WebContent>& content);
    void                    interceptMetadata();
    const QString           getAsin(const QString& url);
    void                    setOtherQueryParams();
    bool                    parReady(const QString& name, const QString& value);
    bool                    isClassic(const QString& marketplaceId);

    void                    parseMPD(/*const QString& url*/);
    void                    setTrackInfo(const TracksInfo& t, const QString& track_base_url);
    const QString           getNextAvailAudio();
    bool                    isTrackReady();
    
    const void              parseHTML(const QString& data);
    void                    parseSeason(const QString& data);

private:
    WebScripts                      scripts_;
#ifdef _DEBUG
    QList<WebContent*>              all_scripts_;
#endif

    QWebEnginePage*                 page_ = nullptr;
    QPointer<WebContent>            html_;
    QPointer<WebContent>            cdmLicense_;
    //QPointer<WebContent>            cdmLicense2_;
    QPointer<WebContent>            webloader_script_;
    //QByteArray                      cdm_session_;
#ifdef DECRYPTSERVICE
    QPointer<AmazonKeysLoader>      keys_loader_;
#endif
    QUrl                            res_req_url_ = "https://atv-ps.amazon.com/cdp/catalog/GetPlaybackResources";
    QUrlQuery                       widevine_2_license_query_;
    QUrlQuery                       query_;
    bool                            ready_for_metadata_ = false;
    QString                         mpd_url_;
    QList<QString>                  season_urls_;
    QList<QString>                  audioLangAvail_; // list audioTrackIds
    QList<QString>                  audioLangLoaded_;

    QString                         marketplaceId_;
    QString                         asin_;
    QString                         clientId_;
    const QString                   deviceTypeID_ = "AOAGZA014O5RE"; // t.SILVERLIGHT_PC = "A63V4FRV3YUP9",t.SILVERLIGHT_MAC = "A2G17C9GWLWFKO",t.HTML5 = "AOAGZA014O5RE"
    bool                            forced_reload_page_ = false;
    bool                            cdm_initializing_ = false;
    bool                            cdm_no_error_ = false;
    QString                         base_url_;
    int                             seasons_count_ = 0;

    std::chrono::steady_clock::time_point   begin_metadata_;
    int i = 0;

private:
    static inline std::string hexStr(const unsigned char* data, int len)
    {
        static const char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
        std::string s(len * 2, ' ');
        for (int i = 0; i < len; ++i) {
            s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
            s[2 * i + 1] = hexmap[data[i] & 0x0F];
        }
        return s;
    }
};

