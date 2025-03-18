#include "privatekeys_client.h"

#include <decrypsis.h>
#include <ctime>
#include <iostream>


PrivateKeysClient::~PrivateKeysClient() {
    qDebug() << "bye PrivateKeysClient " << trackId_ ;
}

void PrivateKeysClient::OnSessionCreated(const uint8_t* serv_cert, int serv_cert_size, const uint8_t* init_data, int init_data_size) {
    qDebug() << "initCDMSession ...";
    QString customerMail = "freegrabapp@gmail.com";

    QByteArray cert ((const char*)serv_cert, serv_cert_size);
    QString cert64 = "CAUSwwUKvQIIAxIQ5US6QAvBDzfTtjb4tU/7QxiH8c+TBSKOAjCCAQoCggEBAObzvlu2hZRsapAPx4Aa4GUZj4/GjxgXUtBH4THSkM40x63wQeyVxlEEo1D/T1FkVM/S+tiKbJiIGaT0Yb5LTAHcJEhODB40TXlwPfcxBjJLfOkF3jP6wIlqbb6OPVkDi6KMTZ3EYL6BEFGfD1ag/LDsPxG6EZIn3k4S3ODcej6YSzG4TnGD0szj5m6uj/2azPZsWAlSNBRUejmP6Tiota7g5u6AWZz0MsgCiEvnxRHmTRee+LO6U4dswzF3Odr2XBPD/hIAtp0RX8JlcGazBS0GABMMo2qNfCiSiGdyl2xZJq4fq99LoVfCLNChkn1N2NIYLrStQHa35pgObvhwi7ECAwEAAToQdGVzdC5uZXRmbGl4LmNvbRKAA4TTLzJbDZaKfozb9vDv5qpW5A/DNL9gbnJJi/AIZB3QOW2veGmKT3xaKNQ4NSvo/EyfVlhc4ujd4QPrFgYztGLNrxeyRF0J8XzGOPsvv9Mc9uLHKfiZQuy21KZYWF7HNedJ4qpAe6gqZ6uq7Se7f2JbelzENX8rsTpppKvkgPRIKLspFwv0EJQLPWD1zjew2PjoGEwJYlKbSbHVcUNygplaGmPkUCBThDh7p/5Lx5ff2d/oPpIlFvhqntmfOfumt4i+ZL3fFaObvkjpQFVAajqmfipY0KAtiUYYJAJSbm2DnrqP7+DmO9hmRMm9uJkXC2MxbmeNtJHAHdbgKsqjLHDiqwk1JplFMoC9KNMp2pUNdX9TkcrtJoEDqIn3zX9p+itdt3a9mVFc7/ZL4xpraYdQvOwP5LmXj9galK3s+eQJ7bkX6cCi+2X+iBmCMx4R0XJ3/1gxiM5LiStibCnfInub1nNgJDojxFA3jH/IuUcblEf/5Y0s1SzokBnR8V0KbA==";
    // cert.toBase64();

    QByteArray pssh_arr((const char*)init_data, init_data_size);
    QString pssh64 = pssh_arr.toBase64(); // QString::fromLocal8Bit((char*)init_data, init_data_size);

    product_ = QString::number(4); //todo: netflix
    QMetaObject::invokeMethod(this, "KeyServerInitSession", Qt::QueuedConnection, Q_ARG(const QString&, customerMail), Q_ARG(const QString&, cert64), Q_ARG(const QString&, pssh64));
}

void PrivateKeysClient::UpdateSession(const char* license, int license_size) {
    KeyServerUpdateSession(QString::fromLatin1(license, license_size));
}

Q_INVOKABLE void PrivateKeysClient::KeyServerInitSession(const QString& customerMail, const QString& cert64, const QString& pssh64) {
    keyServer_ = new WebContent();

    QObject::connect(keyServer_, &WebContent::finished, this, [this]() {
        QObject::disconnect(keyServer_, &WebContent::finished, this, 0);

        const auto& jsonStr = keyServer_->raw_data();
#ifdef _DEBUG
        QFile   file("CdmKeysClient_"+ cdm_session_ +".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(jsonStr);
            file.close();
        }
#endif
        qDebug() << cdm_session_ << " from keyMagicBox: " << QString(jsonStr);
        const auto root = QJsonDocument::fromJson(jsonStr);
        if (root.isEmpty()) {
            emit error(Status(EStatusCode::CdmError, "error getting keys from keyserver."));
            return;
        }

        const auto challengeObj = root.object(); // .value("challenge").toObject();
        if (!challengeObj.isEmpty()) {
            if (GetKeysFromJson(challengeObj.value("keys"))) {
                if (cdm_session_.isEmpty())
                    qDebug() << "magicKeys from cache";
                
                return; // success !
            }

            if (!cdm_session_.isEmpty()) { // after request license
                emit error(Status(EStatusCode::CdmError, "error getting keys from keyserver."));
                return;
            }
            // else send request license...
        }
        
        cdm_session_ = challengeObj.value("cdmsess").toString();
        auto challenge64 = challengeObj.value("cdmchall").toString();
        //challenge64 = challenge64.replace();
        auto qbChallenge = QByteArray::fromBase64(challenge64.toLocal8Bit());
        qDebug() << " to licenseServer msg size: " << qbChallenge.size(); // << ", base64: " << challenge64;
        onChallenge(qbChallenge.constData(), qbChallenge.size());

    }, Qt::QueuedConnection);

    QString post_data = "{\"product\":" + product_ + ",\"track\":\""+ trackId_ +"\",\"customer\":\"" + customerMail + "\",\"cert\":\"" + cert64 + "\",\"pssh\":\"" + pssh64 + "\"}";
    
    qDebug() << "to keyMagicBox Init: " << post_data;
    
    keyServer_->post(keyMagicBoxInit_, post_data.toUtf8(), "application/json");
}

bool PrivateKeysClient::GetKeysFromJson(const QJsonValue& jKeys) {
    if (jKeys.isArray()) {
        QList<QString> kidKeys;
        for (const auto k : jKeys.toArray()) {
            const auto& keypair = k.toString();
            qDebug() << "key: " << keypair;
            kidKeys.append(keypair);
        }

        emit keysObtained(kidKeys);
        return kidKeys.size() > 0;
    }
    return false;
}

void PrivateKeysClient::KeyServerUpdateSession(QString base64license) {
    QObject::connect(keyServer_, &WebContent::finished, this, [this]() {
        QObject::disconnect(keyServer_, &WebContent::finished, this, 0);
        
        const auto& jsonStr = keyServer_->raw_data();
        qDebug() << "from keyMagicBox license: " << QString(jsonStr);

        const auto root = QJsonDocument::fromJson(jsonStr);
        if (root.isEmpty())
            return;
        
        if (!GetKeysFromJson(root.object().value("keys")))
            qDebug() << "error getting keys from keyserver.";
        }
    , Qt::QueuedConnection);

    QString post_data = "{\"cdmsess\":\"" + cdm_session_ + "\""
        + ",\"license\":\"" + base64license + "\""
        + ",\"file\":\"" + trackId_ + "\""
        + "}";
    qDebug() << "to keyMagicBoxUpdate: " << post_data;
    keyServer_->post(keyMagicBoxUpdate_, post_data.toLocal8Bit(), "application/json");
}
