#pragma once

#include <QtCore>

#include <service_callback.h>
#include <web_content.h>


class CdmMagicKeysClient : public QObject {
    Q_OBJECT
public:
    CdmMagicKeysClient(QObject* p = nullptr): QObject(p) {};
    ~CdmMagicKeysClient();
public:
    void initCDMSession(QString pssh, QUrl widevine2LicenseQuery, QPointer<WebContent> sender, QString trackId, int product);

signals:
    void                keysObtained(const QList<QString>& kidKeys);
    void                error(const Status&);

private:
    WebContent*                     cdmSender_ = nullptr;
    
    int                             requestCnt_ = 0; // first request for cert, second for license.
    QString                         cdm_session_;
    QUrl                            widevine2LicenseQuery_;
    QString                         pssh_;
    QString                         trackId_;
    QString                         product_; // 1 = Amazon, 2 = Hulu, 3 = Netflix, 4 = HBOMax, 5 = DisneyPlus
    QPointer<WebContent>            keyServer_;
    

    QString keyMagicBoxSrv_ = "http://185.105.118.229:5050";
    QUrl keyMagicBoxInit_ = keyMagicBoxSrv_ + "/api/cdm/CdmInit";
    QUrl keyMagicBoxUpdate_ = keyMagicBoxSrv_ + "/api/cdm/CdmUpdate";

private:
    void onLoadResponseFromPRS(const WebContent* content);
    void KeyServerInitSession(QString customerMail, QByteArray& cert64, QString pssh);
    void KeyServerUpdateSession(QString responce);
    bool GetKeysFromJson(const QJsonValue& jKeys);

};
