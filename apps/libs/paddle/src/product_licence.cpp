#include "product_licence.h"
#include "paddle_api.h"
#include "paddle_credentials.h"
#include "utils.h"
#include "paddle_keys.h"

#include "simplecrypt.h"

#define CRYPTO_KEY(a,b)  (quint64)(a*b*0x4fe0f)

//#define VERIFICATION_PERIOD         (3) //Days;
//#define EXPIRATION_PERIOD           (365) //Days;

#define VERIFICATION_PERIOD         (3*24*60*60) //Secs;
#define EXPIRATION_PERIOD           (365*24*60*60) //Secs; 

//#define VERIFICATION_PERIOD         (1*24*60*60) //Secs;
//#define EXPIRATION_PERIOD           (14*24*60*60) //Secs;

//#define VERIFICATION_PERIOD         (1*60) //Secs;
//#define EXPIRATION_PERIOD           (1*60) //Secs;


using namespace paddle;
using namespace utils;


ProductLicence::ProductLicence(Credentials* credentials ) 
    : QObject(credentials)
    , credentials_(credentials) {
       
    api_ = new PaddleApi(this);
  
    QObject::connect(&timer_, &QTimer::timeout, this, &ProductLicence::verify);
    timer_.setSingleShot(true);
    load();
    restartIntegrity();
}

ProductLicence::~ProductLicence() {
    //save();
}

bool ProductLicence::isActivated() const {
    return licence_.contains(ActivationId);
}


void ProductLicence::activate(QString email, QString license_key)
{
    QString uuid = getMachineUUID();
    QVariantMap api_params = {  { LicenseEmail, email },
                                { LicenseKey, license_key },
                                { MachineID, uuid } };
    api_params.insert(credentials_->licenseCredentials());
    
    if (licence_.contains(ActivationId) 
        && licence_[LicenseKey] != license_key )
        deactivate();

    PaddleReply* reply = api_->post(ActivateRequest, api_params);
   // QObject::connect(reply, &PaddleApi::error, this, &PaddleProduct::error);
    QObject::connect(reply, &PaddleReply::response, [=](const QVariantMap& result) {
        status_ = reply->status();
        bool is_activated = result.contains(ActivationId);
        qDebug() << "License Activated: " << is_activated;
        if (is_activated 
            && licence_[ActivationId] != result[ActivationId]) {
            licence_ = { { ProductId, credentials_->productId() },
            { LicenseEmail, email },
            { LicenseKey, license_key },
            { ExpireDate, QDateTime::currentDateTimeUtc().addSecs(EXPIRATION_PERIOD) },
            { VerifyDate, QDateTime::currentDateTimeUtc().addSecs(VERIFICATION_PERIOD)}  };
            licence_.insert(result);
            save();
            restartIntegrity();
        }

        //Block other activations;
        if (is_activated)
            blockActivation();

        emit activated(is_activated);

        if (!status_.isOk())
            emit error(&status_);

        reply->deleteLater();
    });
}

void ProductLicence::deactivate() {
    QVariantMap api_params = { { ActivationId, licence_[ActivationId] } };
    api_params.insert(credentials_->licenseCredentials());

    PaddleReply* reply = api_->post(DeactivateRequest, api_params);
        
    QObject::connect(reply, &PaddleReply::response, [=](const QVariantMap& result) {
        status_ = reply->status();
        if (status_.isOk()) {
            qDebug() << "License Deactivated";
        }
        else {
            qDebug() << "Deactivation error: " << status_.message();
            emit error(&status_);
        }

        licence_.remove(ActivationId);
        save();
        restartIntegrity();
        
        emit activated(false);
        
        reply->deleteLater();
    });
}

void ProductLicence::verify() { 
    if (licence_.contains(ActivationId)) {
        QString uuid = getMachineUUID();
        QVariantMap api_params = {  { LicenseEmail, licence_[LicenseEmail] },
                                    { LicenseKey, licence_[LicenseKey] },
                                    //{ MachineID, uuid },
                                    { ActivationId, licence_[ActivationId] } };

        api_params.insert(credentials_->licenseCredentials());

        PaddleReply* reply = api_->post(VerifyLicenceRequest, api_params);

        qDebug() << "Verifying licence...";
                
        //QObject::connect(reply, &PaddleApi::error, this, &ProductLicense::error);
        QObject::connect(reply, &PaddleReply::response, [=](const QVariantMap& result) {
            status_ = reply->status();
            
            if (status_.isOk()) {
                licence_[VerifyDate] = QDateTime::currentDateTimeUtc().addSecs(VERIFICATION_PERIOD);
                qDebug() << "License Verified";
                restartIntegrity();
            }
            else {
                qDebug() << "Verification error: " << status_.message();
                licence_.clear();
                emit activated(false);
                emit error(&status_);
            }
            save();
                        
            reply->deleteLater();
        });

    }
}

void paddle::ProductLicence::blockActivation(int activation_index) {
    QString uuid = QString("%1_locker_%2")
        .arg(getMachineUUID())
        .arg(activation_index);

    QVariantMap api_params = {  { LicenseEmail, licence_[LicenseEmail] },
                                { LicenseKey, licence_[LicenseKey] },
                                { MachineID, uuid } };
    api_params.insert(credentials_->licenseCredentials());
        
    PaddleReply* reply = api_->post(ActivateRequest, api_params);
    // QObject::connect(reply, &PaddleApi::error, this, &PaddleProduct::error);
    QObject::connect(reply, &PaddleReply::response, [=](const QVariantMap& result) {
        status_ = reply->status();
            
        if (result.contains(ActivationId)) {
            qDebug() << "Blocked activation " << activation_index;
            //licence_[BlockId] = result[ActivationId];
            blockActivation(activation_index+1);

        }

        if (!status_.isOk())
            emit error(&status_);

        reply->deleteLater();
    });
}


//void paddle::ProductLicence::unblockActivation() {
//
//  QVariantMap api_params = { { ActivationId, licence_[BlockId] } };
//  api_params.unite(credentials_->licenseCredentials());
//
//  PaddleReply* reply = api_->post(DeactivateRequest, api_params);
//
//  QObject::connect(reply, &PaddleReply::response, [=](const QVariantMap& result) {
//      status_ = reply->status();
//      if (status_.isOk()) {
//          qDebug() << "License Deactivated";
//          licence_.remove(ActivationId);
//          licence_.remove(BlockId);
//          save();
//          restartIntegrity();
//      }
//      else
//          emit error(&status_);
//
//      reply->deleteLater();
//  });
//}


void ProductLicence::restartIntegrity()
{
    if (licence_.contains(ActivationId)) {
        qint64 expiration_time = licence_[ExpireDate].toDateTime().toMSecsSinceEpoch()
            - QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

        qint64 verification_time = licence_[VerifyDate].toDateTime().toMSecsSinceEpoch()
            - QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

        if (expiration_time <= 0) {
            qDebug() << "License Expired!";
            deactivate();
            blockActivation();
        }
        else {
            timer_.start(qMax<qint64>(verification_time, 0));
        }
    }
    else
        timer_.stop();
}

void ProductLicence::load()
{
    
    QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle", QSettings::NativeFormat);
    QByteArray      data = settings.value(credentials_->productId()).toByteArray();
      
    SimpleCrypt crypto(CRYPTO_KEY(  credentials_->vendorId().toLongLong(), 
                                    credentials_->productId().toLongLong()));
    QByteArray decrypted_data = crypto.decryptToByteArray(data);
    if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
        QDataStream stream(&decrypted_data, QIODevice::ReadOnly);
        stream >> licence_;
    }
    else {
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> licence_;
    }

}

void ProductLicence::save()
{
    //setup our objects
    SimpleCrypt crypto(CRYPTO_KEY(  credentials_->vendorId().toLongLong(),
                                    credentials_->productId().toLongLong()));
    crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
    crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash); //properly protect the integrity of the 

    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << licence_;

    QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle", QSettings::NativeFormat);

    QByteArray encrypted_data = crypto.encryptToByteArray(data);

    if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
        settings.setValue(credentials_->productId(), encrypted_data);
    }
    else {
        settings.setValue(credentials_->productId(), data);
    }

}


QString paddle::ProductLicence::email() const {
    return licence_[LicenseEmail].toString();
}

QString paddle::ProductLicence::license_key() const {
    return licence_[LicenseKey].toString();
}
