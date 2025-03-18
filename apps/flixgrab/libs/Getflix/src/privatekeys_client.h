#pragma once
#include <QtCore>

#include <web_content.h>
#include <string>
#include <functional>

#include <service_callback.h>

class PrivateKeysClient: 
    public QObject,
    public PrivateKeyInterceptorCallback
{
    Q_OBJECT
public:
    PrivateKeysClient(QObject* p = nullptr) : QObject(p) {};
    ~PrivateKeysClient();
//public:
    //void initCDMSession(QString  pssh, QString  trackId, int product);

public:
    virtual void OnSessionCreated(const uint8_t* serv_cert, int serv_cert_size, const uint8_t* init_data, int init_data_size) override;
    virtual void UpdateSession(const char* license, int license_size) override;

signals:
    void                onChallenge(const char* message, uint32_t message_size);
    void                keysObtained(const QList<QString>& kidKeys);
    void                error(const Status&);

private:
    int                         requestCnt_ = 0; // first request for cert, second for license.
    QString                     cdm_session_;
    
    //QString                     pssh_;
    QString                     trackId_;
    QString                     product_; // 1 = Amazon, 2 = Hulu, 3 = Netflix, 4 = HBOMax, 5 = DisneyPlus
    QPointer<WebContent>        keyServer_;
    

    QString keyMagicBoxSrv_ = "http://185.105.118.229:5050";
    //QString keyMagicBoxSrv_ = "http://localhost:19892";
    QUrl keyMagicBoxInit_ = keyMagicBoxSrv_ + "/api/cdm/CdmInit";
    QUrl keyMagicBoxUpdate_ = keyMagicBoxSrv_ + "/api/cdm/CdmUpdate";

private:
    Q_INVOKABLE void KeyServerInitSession(const QString& customerMail, const QString& cert64, const QString& pssh);
    void KeyServerUpdateSession(QString responce);
    bool GetKeysFromJson(const QJsonValue& jKeys);
};
