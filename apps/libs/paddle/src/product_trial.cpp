#include "product_trial.h"
#include "simplecrypt.h"

#define CRYPTO_KEY(a,b)  (quint64)(a*b*0x10ee0f)

paddle::ProductTrial::ProductTrial(Credentials* credentials) : QObject(credentials)
, credentials_(credentials)
{


	//	QObject::connect(&timer_, &QTimer::timeout, this, &ProductLicence::verify);
	timer_.setSingleShot(true);
	load();
	//restartIntegrity();

	if (!trial_.contains(TrialFinished)) {
		trial_[TrialFinished] = false;
	}

	QObject::connect(&timer_, &QTimer::timeout, [=]() {
		/*trial_[TrialFinished] = true;
		trial_.remove(TrialExpire);
		save();*/
		stop();
		emit trialChanged(false);
	});

	restartIntegrity();
}

paddle::ProductTrial::~ProductTrial() {
	//save();
}

bool paddle::ProductTrial::isTrial() const
{
	if (!trial_[TrialFinished].toBool()) {
		return trial_.contains(TrialExpire);
	}
	return false;
}

void paddle::ProductTrial::start(qint64 seconds)
{
	if (!trial_[TrialFinished].toBool()) {
		trial_[TrialExpire] = QDateTime::currentDateTimeUtc().addSecs(seconds);
		save();
		emit trialChanged(true);
		restartIntegrity();
	}
}

void paddle::ProductTrial::stop()
{
	if (!trial_[TrialFinished].toBool()) {
		trial_.remove(TrialExpire);
		trial_[TrialFinished] = true;
		save();
		restartIntegrity();
	}
}

void paddle::ProductTrial::load()
{
	QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle\\Trial", QSettings::NativeFormat);
	QByteArray      data = settings.value(credentials_->productId()).toByteArray();

	SimpleCrypt crypto(CRYPTO_KEY(credentials_->vendorId().toLongLong(),
		credentials_->productId().toLongLong()));
	QByteArray decrypted_data = crypto.decryptToByteArray(data);
	if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
		QDataStream stream(&decrypted_data, QIODevice::ReadOnly);
		stream >> trial_;
	}
	else {
		QDataStream stream(&data, QIODevice::ReadOnly);
		stream >> trial_;
	}
}

void paddle::ProductTrial::save() const {
	//setup our objects
	SimpleCrypt crypto(CRYPTO_KEY(credentials_->vendorId().toLongLong(),
		credentials_->productId().toLongLong()));
	crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
	crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash); //properly protect the integrity of the 

	QByteArray data;

	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << trial_;

	QSettings       settings("HKEY_CURRENT_USER\\Software\\Paddle\\Trial", QSettings::NativeFormat);

	QByteArray encrypted_data = crypto.encryptToByteArray(data);

	if (crypto.lastError() == SimpleCrypt::ErrorNoError) {
		settings.setValue(credentials_->productId(), encrypted_data);
	}
	else {
		settings.setValue(credentials_->productId(), data);
	}
}

void paddle::ProductTrial::restartIntegrity()
{
	if (trial_.contains(TrialExpire)) {
		qint64 expiration_time = trial_[TrialExpire].toDateTime().toMSecsSinceEpoch()
			- QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

		timer_.start(qMax<qint64>(expiration_time, 0));
	}
	else
		timer_.stop();
}
