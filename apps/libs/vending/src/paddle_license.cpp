#include "paddle_license.h"
#include "simplecrypt.h"

#define LicenseEmail                "email"
#define LicenseKey                  "license_key"
#define ActivationId                "activation_id"
#define ExpireDate                  "expire_date"

#define CRYPTO_KEY(a,b)  (quint64)(a*b*0x4fe0f)

namespace vending {

    PaddleLicense::PaddleLicense(const QString & vendor_id, const QString & product_id)
        : product_id_(product_id)
        , vendor_id_(vendor_id) {

        QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle", QSettings::NativeFormat);
        QByteArray      data = settings.value(product_id).toByteArray();

        SimpleCrypt crypto(CRYPTO_KEY(vendor_id.toLongLong(),
                                      product_id.toLongLong()));
        QByteArray decrypted_data = crypto.decryptToByteArray(data);
        if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
            QDataStream stream(&decrypted_data, QIODevice::ReadOnly);
            stream >> paddle_license_;
        }
        else {
            QDataStream stream(&data, QIODevice::ReadOnly);
            stream >> paddle_license_;
        }
    }

    PaddleLicense::~PaddleLicense() {

        //setup our objects
        if (!paddle_license_.isEmpty()) {
            SimpleCrypt crypto(CRYPTO_KEY(vendor_id_.toLongLong(),
                                          product_id_.toLongLong()));
            crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
            crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash); //properly protect the integrity of the 

            QByteArray data;

            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << paddle_license_;

            QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle", QSettings::NativeFormat);

            QByteArray encrypted_data = crypto.encryptToByteArray(data);

            if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
                settings.setValue(product_id_, encrypted_data);
            }
            else {
                settings.setValue(product_id_, data);
            }
        }
        
    }

    QString PaddleLicense::email() const {
        return paddle_license_[LicenseEmail].toString();
    }

    QString PaddleLicense::key() const {
        return paddle_license_[LicenseKey].toString();
    }

    bool PaddleLicense::isValid() const {

        if (paddle_license_.contains(ActivationId) &&
            paddle_license_.contains(ExpireDate)) {
            return paddle_license_[ExpireDate].toDateTime() > QDateTime::currentDateTimeUtc();
        }
        return false;
    }

    void PaddleLicense::clear() {

        QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle", QSettings::NativeFormat);
        settings.remove(product_id_);
        paddle_license_.clear();
    }

    void PaddleLicense::makeTestLicense() {

        paddle_license_ = { { LicenseEmail, "test@email.org" },
        { LicenseKey, "00000-00000-00000-00000-00000" },
        { ActivationId, 999 },
        { ExpireDate, QDateTime::currentDateTimeUtc().addSecs(60 * 3) } };

    }

}

