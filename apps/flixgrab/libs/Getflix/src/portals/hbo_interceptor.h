#pragma once

#include "web_portal.h"
#include "mpd_parser_hbo.h"
#ifdef DECRYPTSERVICE
#include "cdm_keys_loader.h"
class CdmMagicKeysClient;
#endif

enum class EFetchMode {
    Original,
    Self
};

class AuthorizationState {
public:
    inline bool isRequired() const {
        return state != States::Authorizated;
    }

    inline bool isFormShowed() const {
        return state == States::FormShowed;
    }

    inline bool isFormSubmited() const {
        return state == States::FormSubmited;
    }

    inline bool isAuthorizated() const {
        return state == States::Authorizated;
    }


    inline void setRequired() {
        state = States::Required;
    }

    inline void setFormShowed() {
        state = States::FormShowed;
    }

    inline void setFormSubmited() {
        state = States::FormSubmited;
    }

    inline void setAuthorizated() {
        state = States::Authorizated;
    }

private:
    enum class States {
        Required,
        FormShowed,
        FormSubmited,
        Authorizated
    }                       state = States::Required;
};

class HBOInterceptor : public WebPortal {
    Q_OBJECT
public:
    HBOInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p);
    ~HBOInterceptor();

private:
    void                        initialize() override;
    void                        interceptRequest(QWebEngineUrlRequestInfo& info) override;
    void                        startPage(bool forced_reload_page = false) override;
    void                        setSignedIn(bool signed_in) override;

private:
    bool                        parse_response(const QJsonValue& item);
    bool                        parse_manifest(const QString& id, const QJsonObject& body);
    bool                        parse_metadata(const QString& id, const QJsonObject& body);
    bool                        parse_profiles(const QString& id, const QJsonObject& body);
    bool                        parse_authorization(const QJsonObject& root);
    bool                        parse_access_token(const QJsonObject& root);
    bool                        ParseMPD(const QString& url);
#ifdef DECRYPTSERVICE
    void                        loadKeys(const QString& wv_server_url, const QString pssh); // magicKeys loader
    void                        loadKeys(const QString& wv_server_url, std::vector<uint8_t> init_data);
    std::vector<uint8_t>        getInitData(const QString& url);
#endif
    void                        fetchApp(const EFetchMode mode);
    static QString              getImage(const QString& uri_tmpl);

    void                        initRequest(WebContent* request);

    Q_INVOKABLE void            interceptConfig(const QString& jsonStr, const QString& confName) override;
    Q_INVOKABLE void            autoLogin() override;
    Q_INVOKABLE void            autoSelectProfile();
    Q_INVOKABLE void            autoPlay();
    Q_INVOKABLE void            interceptAppScript(const QUrl& url);
    Q_INVOKABLE void            requestEpisodes(const QUrl& url);

    //Q_INVOKABLE void          requestMpd(const QString& url);
    //Q_INVOKABLE void          interceptLoginClicked(const QUrl& url);
    //Q_INVOKABLE void          interceptManifest(const QUrl& url);

private:
    bool                        clickByJSONCoords(const QVariant& v, bool autoprofile = false);

private:
    QNetworkAccessManager*      nam_ = nullptr;
    QWebEnginePage*             page_ = nullptr;
    QPointer<WebContent>        app_script_;
    QPointer<WebContent>        cdmLicense_;
    QPointer<WebContent>        cdmLicense2_;   // license request for magicKeys loader
    QPointer<WebContent>        episodes_;
    QString                     base_url_;
    QString                     access_token_;
    QString                     profile_key_name_ = "HBOMAXUserProfile";
    //QString                     wv_server_url_; // "https://comet.api.hbo.com/drm/license/widevine?keygen=playready&drmKeyVersion=2"
#ifdef DECRYPTSERVICE
    QPointer<CdmKeysLoader>     keys_loader_;
    QPointer<CdmMagicKeysClient> key_client_;    // magicKeys loader
#endif
    int                         i, j, k, m = 0;
    QUrl                        app_js_url_;
    QString                     login_;
    QString                     password_;
    QString                     profile_;
    AuthorizationState          auth_state_;
};
