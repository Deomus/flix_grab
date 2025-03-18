#pragma once
#include <service_callback.h>
#include <web_content.h>

#include <QtCore>

class AmazonKeysLoader : public QObject, public SessionMessageCallback
{
    Q_OBJECT
public:
    AmazonKeysLoader(QObject* p = nullptr): QObject(p) {};
    ~AmazonKeysLoader();
public:
    void initCDMSession(std::vector<uint8_t> init_data, QUrl widevine2LicenseQuery, QPointer<WebContent> sender);
    void initCDMSession(AbstractDecryptService* );

signals:
    void                error(const Status&);
    void                keysLoaded();

private:
    WebContent*         sender_ = NULL;

    QByteArray                      cdm_session_;
    QUrl                            widevine2LicenseQuery_;

private:
    void onLoadResponseFromPRS(const WebContent* content);


    // SessionMessageCallback
public:
    virtual void OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size) override;

    virtual void OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size) override;
    virtual void OnSessionKeysChange() override;
    virtual void OnRejectPromise() override;

    //virtual void OnSessionKeysChange(const char* session_id, uint32_t session_id_size) override;
    //virtual void OnReject(..);

};
