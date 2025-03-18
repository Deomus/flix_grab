#include "paddle.h"
#include "paddle_credentials.h"
#include "paddle_api.h"
#include "product_licence.h"
#include "product_trial.h"


using namespace paddle;

Paddle::Paddle(QString api_key, QString vendor_id, QString product_id, QObject* parent)
    : QObject(parent)
    , credentials_(new Credentials(api_key, vendor_id, product_id, this))
    , licence_(new ProductLicence(credentials_))
	, trial_(new ProductTrial(credentials_))
    , info_(new ProductInfo(credentials_)){

    api_ = new PaddleApi(this);

    QObject::connect(licence_, &ProductLicence::activated, this, &Paddle::licenceChanged);
    QObject::connect(licence_, &ProductLicence::error, this, &Paddle::licenceError);

	QObject::connect(trial_, &ProductTrial::trialChanged, this, &Paddle::trialChanged);
}

paddle::Paddle::~Paddle() {
	qDebug() << "~Paddle";
}

void Paddle::activateLicence(QString email, QString licence_key) {
    licence_->activate(email, licence_key);
	trial_->stop();
}

void Paddle::deactivateLicence() {
    licence_->deactivate();
}


bool Paddle::isActivated() const {
    return licence_->isActivated();
}

QString Paddle::licenceEmail() const {
    return licence_->email();
}

QString Paddle::licenceKey() const {
    return licence_->license_key();
}

void paddle::Paddle::startTrial(qint64 seconds) {
	trial_->start(seconds);
}

void paddle::Paddle::stopTrial() {
	trial_->stop();
}

bool paddle::Paddle::isTrial() const {
	return trial_->isTrial();
}
