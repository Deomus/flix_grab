#pragma once

#include <service_callback.h>

#include "web_portal.h"
#include "mpd_parser_hulu.h"

class AppHuluScript;
class CdmMagicKeysClient;

class HuluInterceptor : public WebPortal, public SessionMessageCallback {
    Q_OBJECT

public:
    HuluInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~HuluInterceptor();

signals:
#ifdef LOGIN_HULU_ALTERNATIVE
    void                        authenticated(const QUrl&);
#endif

    // SessionMessageCallback
public:
    virtual void OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size) override;
    virtual void OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size) override;
    virtual void OnSessionKeysChange() override;
    virtual void OnRejectPromise() override;

private:
    enum EInterceptFlags {
        kTracksReady = 0,
        kVideoInfoReady_ = 0x1,
        kCDMLicenseReady_ = 0x2,
        kPrivateKeysReady_ = 0x3,
    };

    bool checkInterceptFlag(int value, int flag) {
        return (value == flag);
    }

    void setFlag(EInterceptFlags flag);

    bool is_tracks_ready_ = false;
    bool is_video_info_ready_ = false;
    bool is_cdm_license_ready_ = false;
    bool is_private_keys_ready_ = false;

private:
    void                        initialize() override;
    void                        interceptRequest(QWebEngineUrlRequestInfo& info) override;
    void                        startPage(bool forced_reload_page) override;

private:
    void                        loadPrivateKeys(const QString pssh);
    void                        initCDMSession();
    void                        onLoadResponseFromPRS(QPointer<WebContent>& content);
    void                        parseMPD(const QUrl&);
    void                        parseSubtitles(const QJsonObject&);
    void                        parse_manifest(const QString& data);
    void                        parse_playlist(const QString& data);
    void                        parse_collection(const QString& data);
    void                        parse_deeplink(const QString& data);
    void                        eab_extract();

    static inline std::string hexStr(const unsigned char* data, int len);

    Q_INVOKABLE void        interceptConfig(const QString& jsonStr, const QString& confName) override;
    Q_INVOKABLE void        interceptMainScript(const QUrl& url);
    Q_INVOKABLE void        interceptAppScript(const QUrl& url);
    Q_INVOKABLE void        interceptLoaderScript(const QUrl& url);
    Q_INVOKABLE void        interceptIndexScript(const QUrl& url);
    Q_INVOKABLE void        interceptPlaylist(const QUrl& url);
    Q_INVOKABLE void        interceptManifest(const QUrl& url);
    Q_INVOKABLE void        interceptCollection(const QUrl& url);
    Q_INVOKABLE void        interceptDeeplink(const QString& eabId);
#ifdef _DEBUG
    Q_INVOKABLE void        interceptOrigScript(const QUrl& url);
#endif

private:
    QNetworkAccessManager* nam_ = nullptr;
    QWebEnginePage* page_ = nullptr;

    QPointer<WebContent>        index_script_;
    QPointer<AppHuluScript>     app_script_;
    QPointer<WebContent>        loader_script_;
    //QPointer<WebContent>        main_script_;
#ifdef _DEBUG
    QList<WebContent*>          all_scripts_;
#endif
    QPointer<CdmMagicKeysClient> key_client_;

private:
    bool                        registration_requested_;
    MpdParserHulu               mpd_;
    QString                     base_url_;
    QUrlQuery                   query_;
    QString                     subscription_json_; // = "\"subscription\":{\"id\":\"343416593\",\"features\":{\"LIVE_STREAM\":{\"enabled\":true}},\"policy_id\":\"1000\",\"status\":\"1\",\"subscriber_id\":\"143212745\",\"feature_ids\":[\"6\"],\"product_id\":\"786448\",\"plan_id\":\"1049576\",\"pgid\":\"50331651\",\"package_ids\":[\"1\",\"2\",\"25\",\"26\"],\"customer_type\":\"100\",\"acting_product_id\":\"786448\",\"is_ppp\":true,\"customer_type_name\":\"hulu\"}";
    QString                     content_config_; // = "{\"computerGuid\":\"1D8F5CABE269B7CC974A45EA0224F780\",\"latitude\":-1,\"longitude\":-1,\"subscription\":{\"id\":\"343416593\",\"features\":{\"LIVE_STREAM\":{\"enabled\":true}},\"policy_id\":\"1000\",\"status\":\"1\",\"subscriber_id\":\"143212745\",\"feature_ids\":[\"6\"],\"product_id\":\"786448\",\"plan_id\":\"1049576\",\"pgid\":\"50331651\",\"package_ids\":[\"1\",\"2\",\"25\",\"26\"],\"customer_type\":\"100\",\"acting_product_id\":\"786448\",\"is_ppp\":true,\"customer_type_name\":\"hulu\"},\"isNonSub\":false,\"liveGuideEnabled\":true,\"isForcedNonSub\":false,\"siteAPI\":{}}";
    QString                     wv_server_;
    QString                     eab_; //EAB::<eabId>::<entityId>::<assetId> // EAB::23d15606-6210-4519-b2e8-c2fcbe22f2a8::61430820::70924722
    QString                     eabId_; // 23d15606-6210-4519-b2e8-c2fcbe22f2a8
    const QList<QString>        eab_source_ = { "hulu.com/watch/", "/hubs/series/","/hubs/movie/" };

    bool                        cdm_initializing_ = false;
    QByteArray                  cdm_session_;
    QPointer<WebContent>        cdmLicense_;
    QPointer<WebContent>        cdmLicense2_;
    QPointer<WebContent>        cdmLicensePrivate_; // request to license server for private keys

    //QPointer<WebContent>        load_playlist_;
    QPointer<WebContent>        manifest_;
    QPointer<WebContent>        playlist_;
    QPointer<WebContent>        collection_;
    QPointer<WebContent>        deeplink_;
    QString                     url_deeplink_;
    QPointer<WebContent>        artwork_cache_;
#ifdef LOGIN_HULU_ALTERNATIVE
    QPointer<WebContent>        generate_csrf_;
    QPointer<WebContent>        auth_;
#endif
    std::chrono::system_clock::time_point   start_time_; // performance profiling
};