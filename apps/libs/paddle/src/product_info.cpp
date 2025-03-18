#include "product_info.h"
#include "paddle_credentials.h"
#include "paddle_api.h"
#include "paddle_keys.h"
#include "utils.h"
#include "simplecrypt.h"

#define CRYPTO_KEY(a,b)  (quint64)(a*b*0x1eeff)

//#define INFO_CHECK_PERIOD           (3*24*60*60)
#define INFO_CHECK_PERIOD           (24*60*60)

namespace paddle {
      

	ProductInfo::ProductInfo(Credentials* credentials) 
		: QObject(credentials)
		, credentials_(credentials) {
		api_ = new PaddleApi(this);
        load();
	}

    QString ProductInfo::productName() {
        syncIfExpired();
        return info_.value(ProductName).toString();
    }

    QString ProductInfo::developerName() {
        syncIfExpired();
        return info_.value(DeveloperName).toString();
    }

    QString ProductInfo::currency() {
        syncIfExpired();
        QVariantMap prices = info_.value(Prices).toMap();
        QString currency_key = QLocale::system().currencySymbol(QLocale::CurrencyIsoCode);
        if (!prices.isEmpty()) {
            if (!prices.contains(currency_key)) {
                currency_key = prices.keys().first();
            }
        }
        return currency_key;
    }


    QString ProductInfo::toPriceString() {
        QString symbol = utils::getCurrencySymbolByIso(currency());
        return QLocale::system().toCurrencyString(price(), symbol);
    }

    double ProductInfo::price() {
        syncIfExpired();
        QVariantMap prices = info_.value(Prices).toMap();

        if (!prices.isEmpty())
            return prices[currency()].toReal();

        return -1;
    }
        

    QString ProductInfo::imageUrl() {
        syncIfExpired();
        return info_.value(ImageUrl).toString();
    }

    QString ProductInfo::paymentUrl() {
        syncIfExpired();
        return info_.value(PaymentUrl).toString();
    }

	bool ProductInfo::isTrialSupported() {
        syncIfExpired();
		return info_.contains(Trial);
	}

	QString ProductInfo::trialText() {
        syncIfExpired();
		if (isTrialSupported())
			return info_[Trial].toMap()[TrialText].toString();
		return QString();
	}

	double ProductInfo::trialDuration()	{
        syncIfExpired();
		if (isTrialSupported())
			return info_[Trial].toMap()[TrialDuration].toDouble();
		return 0;
	}


	void ProductInfo::load() {
		QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle\\Info", QSettings::NativeFormat);
		QByteArray      data = settings.value(credentials_->productId()).toByteArray();

		SimpleCrypt crypto(CRYPTO_KEY(credentials_->vendorId().toLongLong(),
			credentials_->productId().toLongLong()));
		QByteArray decrypted_data = crypto.decryptToByteArray(data);
		if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
			QDataStream stream(&decrypted_data, QIODevice::ReadOnly);
			stream >> info_;
		}
		else {
			QDataStream stream(&data, QIODevice::ReadOnly);
			stream >> info_;
		}
	}

	void ProductInfo::save() const {
		//setup our objects
		SimpleCrypt crypto(CRYPTO_KEY(credentials_->vendorId().toLongLong(),
			credentials_->productId().toLongLong()));
		crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
		crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash); //properly protect the integrity of the 

		QByteArray data;

		QDataStream stream(&data, QIODevice::WriteOnly);
		stream << info_;

		QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle\\Info", QSettings::NativeFormat);

		QByteArray encrypted_data = crypto.encryptToByteArray(data);

		if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
			settings.setValue(credentials_->productId(), encrypted_data);
		}
		else {
			settings.setValue(credentials_->productId(), data);
		}
	}


    void ProductInfo::syncIfExpired()
    {
        qint64 expiration_time = 0;
        if (info_.contains(InfoExpire)) {
            expiration_time = info_[InfoExpire].toDateTime().toMSecsSinceEpoch()
                - QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
        }
                
        if (expiration_time <= 0) {
            PaddleReply* reply = api_->post(ProductInfoRequest, credentials_->allCredentials());

            QEventLoop      loop;

            QObject::connect(reply, &PaddleReply::response, [&](const QVariantMap& result) {
                status_ = reply->status();

                info_ = result;
                info_[InfoExpire] = QDateTime::currentDateTimeUtc().addSecs(INFO_CHECK_PERIOD);
                save();

                //restartIntegrity();
                reply->deleteLater();
                loop.quit();
            });

            loop.exec();
        }
                
        
    }

}