#pragma once

#include <QtCore>

#include <service_callback.h>
#include <web_content.h>


class CdmKeysLoader : public QObject, public SessionMessageCallback {
    Q_OBJECT
public:
    CdmKeysLoader(QObject* p = nullptr): QObject(p) {};
    ~CdmKeysLoader();
public:
    void initCDMSession(std::vector<uint8_t> init_data, QUrl widevine2LicenseQuery, QPointer<WebContent> sender, int* host_primise_id = nullptr);
    //void initCDMSession(AbstractDecryptService* );

signals:
    void                keysLoaded();
//  void                error(const Status&);

private:
    WebContent*                     sender_ = nullptr;

    QByteArray                      cdm_session_;
    QUrl                            widevine2LicenseQuery_;
    int                             host_primise_id_; // to identify the service

private:
    void onLoadResponseFromPRS(const WebContent* content);


    // SessionMessageCallback
public:
    virtual void OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size) override;

    virtual void OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size) override;
    virtual void OnSessionKeysChange() override;
    virtual void OnRejectPromise() override;
};
