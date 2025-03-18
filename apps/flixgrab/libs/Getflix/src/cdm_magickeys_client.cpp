#include "cdm_magickeys_client.h"

#include <decrypsis.h>
#include <ctime>


CdmMagicKeysClient::~CdmMagicKeysClient() {
    qDebug() << "bye CdmMagicKeysClient " << trackId_;
}

void CdmMagicKeysClient::initCDMSession(QString pssh, QUrl widevine2LicenseQuery, QPointer<WebContent> sender, QString trackId, int product) {

    cdmSender_ = sender;
    
    requestCnt_ = 0;
    trackId_ = trackId;
    product_ = std::to_string(product).c_str();
    widevine2LicenseQuery_ = widevine2LicenseQuery;
    pssh_ = pssh;
    //send pssh + cert    
    
    QObject::connect(cdmSender_, &WebContent::finished, this, [=]() {
        QObject::disconnect(cdmSender_, &WebContent::finished, this, 0);
        CdmMagicKeysClient::onLoadResponseFromPRS(cdmSender_);
    }, Qt::QueuedConnection);

    //QString certRequest = "CAQ=";
    QByteArray qbCertRequest(2, 0);
    qbCertRequest[0] = 8;
    qbCertRequest[1] = 4;
    cdmSender_->post(widevine2LicenseQuery_, qbCertRequest, "application/octet-stream");
}

void CdmMagicKeysClient::onLoadResponseFromPRS(const WebContent* content) {
    try {
        qDebug() << "got response from PRS: (" << content->raw_data().size() << "): "; // << license.toUtf8();
        const auto& r = content->raw_data();
        //qDebug() << r.size() << "): " << r.constData();
        //DecryptUpdateSession(cdm_session_.constData(), cdm_session_.size(), r.constData(), r.size(), host_primise_id_);
        
        requestCnt_++;
        qDebug() << "requestCnt: " << requestCnt_;
        if (requestCnt_ == 1) { //cdm_session_.isEmpty())
            auto cert64 = r.toBase64();
            QString customerMail = "freegrabapp@gmail.com";
            KeyServerInitSession(customerMail, cert64, pssh_);
        }
        else if (requestCnt_ == 2 && !cdm_session_.isEmpty()) {
            auto base64license = r.toBase64();
            KeyServerUpdateSession(base64license);
        }
        else {
            emit error(Status(EStatusCode::CdmError, "error loading From PRS."));
        }
    }
    catch (...) {
        qDebug() << "error loading From PRS.";
    }
}

void CdmMagicKeysClient::KeyServerInitSession(QString customerMail, QByteArray& cert64, QString pssh) {
    keyServer_ = new WebContent(this);

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

            if (!cdm_session_.isEmpty()) {  // after request license     
                emit error(Status(EStatusCode::CdmError, "error getting keys from keyserver."));
                return;
            }
            // else send request license...
        }
        
        cdm_session_ = challengeObj.value("cdmsess").toString();
        const auto challenge64 = challengeObj.value("cdmchall").toString();
        auto qbChallenge = QByteArray::fromBase64(challenge64.toLocal8Bit());
        qDebug() << " to " << widevine2LicenseQuery_ << " << " << challenge64;
        cdmSender_->reset();
        QObject::connect(cdmSender_, &WebContent::finished, this, [=]() {
            QObject::disconnect(cdmSender_, &WebContent::finished, this, 0);
            CdmMagicKeysClient::onLoadResponseFromPRS(cdmSender_);
        }, Qt::QueuedConnection);

        cdmSender_->post(widevine2LicenseQuery_, qbChallenge, "application/octet-stream");

    }, Qt::QueuedConnection);

    QString post_data = "{\"product\":" + product_ + ",\"track\":\""+ trackId_ +"\",\"customer\":\"" + customerMail + "\",\"cert\":\"" + cert64 + "\",\"pssh\":\"" + pssh_ + "\"}";
    
    qDebug() << "to keyMagicBox Init: " << post_data;
    
    keyServer_->post(keyMagicBoxInit_, post_data.toUtf8(), "application/json");
}

bool CdmMagicKeysClient::GetKeysFromJson(const QJsonValue& jKeys) {
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

void CdmMagicKeysClient::KeyServerUpdateSession(QString base64license) {
    QObject::connect(keyServer_, &WebContent::finished, this, [this]() {
        QObject::disconnect(keyServer_, &WebContent::finished, this, 0);
        
        const auto& jsonStr = keyServer_->raw_data();
        qDebug() << "from keyMagicBox license: " << QString(jsonStr);

        const auto root = QJsonDocument::fromJson(jsonStr);
        if (root.isEmpty())
            return;
        
        if (!GetKeysFromJson(root.object().value("keys")))
            qDebug() << "error getting keys from keyserver.";

    }, Qt::QueuedConnection);

    QString post_data = "{\"cdmsess\":\"" + cdm_session_ + "\""
        + ",\"license\":\"" + base64license + "\""
        + ",\"file\":\"" + trackId_ + "\""
        + "}";
    qDebug() << "to keyMagicBoxUpdate: " << post_data;
    keyServer_->post(keyMagicBoxUpdate_, post_data.toLocal8Bit(), "application/json");
}
