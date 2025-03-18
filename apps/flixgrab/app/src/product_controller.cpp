#include "product_controller.h"

#include <company_info.h>

#if PURCHASING_BACKEND == PADDLE_BACKEND

#include <paddle.h>
#include <app_update.h>

#elif PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
#include <in_app_store.h>
#elif PURCHASING_BACKEND == VENDING_BACKEND
#include <vending/product.h>
#include <vending/license.h>
#include <app_update.h>
#endif


#include "alert_dialog.h"
#include "activation_dialog.h"

#if PURCHASING_BACKEND == PADDLE_BACKEND

#pragma comment(lib, "paddle.lib")
#pragma comment(lib, "updater.lib")

//ProductController* ProductController::product_ = nullptr;

class ProductControllerPrivate {

public:

    ProductControllerPrivate(ProductController* p) : public_(p) {
        createLicensing();
        createUpdater();
    }

    void initializeUi(QWidget* hwnd) {
        Q_UNUSED(hwnd);
        //Used only in Windows Store;
    }

    void createLicensing() {
        paddle_ = new paddle::Paddle(PADDLE_API_KEY, PADDLE_VENDOR_ID, PADDLE_PRODUCT_ID, public_);

     /*   QObject::connect(paddle_, &paddle::Paddle::licenceChanged,
            public_, &ProductController::licenceChanged);*/


        QObject::connect(paddle_, &paddle::Paddle::licenceChanged, [=](bool is_active) {
            if (is_active)
                AlertDialog::execDialog(EAlert::kAlert_LicenceActivated);
            else
                AlertDialog::execDialog(EAlert::kAlert_LicenceDeactivated);
        });


        QObject::connect(paddle_, &paddle::Paddle::trialChanged, [=](bool is_active) {
            if (is_active)
                AlertDialog::execDialog(EAlert::kAlert_TrialStarted);
            else
                AlertDialog::execDialog(EAlert::kAlert_TrialFinished);
        });

        createTrial();

    }

    void createUpdater() {
        app_update_ = new AppUpdate(public_);

        QObject::connect(app_update_, &AppUpdate::downloadFinished,
            public_, &ProductController::updateDownloaded);

        if (app_update_->isAvailable()
            && !app_update_->isDownloaded()) {
            app_update_->download();
        }
        else {
            QObject::connect(app_update_, &AppUpdate::available, [=]() {
                if (!app_update_->isDownloaded())
                    app_update_->download();
            });
        }

    }


    void createTrial() {
        auto product_info = paddle_->productInfo();
        if (!paddle_->isActivated()
            && product_info->isTrialSupported()
            && product_info->trialDuration() > 0
            && !paddle_->isTrial()) {
            paddle_->startTrial(product_info->trialDuration() * 24 * 60 * 60);
            //paddle_->startTrial(60);
        }
    }


    bool isActivated() const {
        return paddle_->isActivated();
    }

    void deactivate() {
        QEventLoop wait_reply;
        QObject::connect(paddle_, &paddle::Paddle::licenceChanged,
            &wait_reply, &QEventLoop::quit);
        paddle_->deactivateLicence();
        wait_reply.exec();
    }

    void execPurchase() {
        QDesktopServices::openUrl(QUrl(paddle_->productInfo()->paymentUrl()));
    }

    void execActivation() {
        ActivationDialog dialog(paddle_->productInfo()->toPriceString(), QApplication::activeWindow());
        //Purchase product;
        QObject::connect(&dialog, &ActivationDialog::purchaseClicked, paddle_, [=]() {
            QDesktopServices::openUrl(QUrl(paddle_->productInfo()->paymentUrl()));
        });
        
        QObject::connect(&dialog, &ActivationDialog::activateClicked, 
                         paddle_, &paddle::Paddle::activateLicence);

        QObject::connect(paddle_, &paddle::Paddle::licenceError, &dialog, [&](auto status) {
            dialog.setError(status->message());
        });

        QObject::connect(paddle_, &paddle::Paddle::licenceChanged, &dialog, [&](bool is_active) {
            if (is_active)
                dialog.accept();
        });
        
        dialog.exec();
    }

    //////////////////////////////////////////////////////////////////////////

    bool isUpdateDownloaded() {
        return app_update_->isDownloaded();
    }

    void installUpdate() {

        if (!will_be_updated_) {
            QObject::connect(qApp, &QApplication::aboutToQuit, [=](auto) {
                app_update_->install();
            });
            will_be_updated_ = true;
        }

        QApplication::closeAllWindows();

    }

    bool isTrial() const {
        return paddle_->isTrial();
    }


private:
    paddle::Paddle*                 paddle_;
    AppUpdate*                      app_update_;

    bool                            will_be_updated_ = false;

private:
    ProductController*              public_;

};


#elif PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
#pragma comment(lib, "inapppurchase.lib")

//Properties
#define UPDATE_DOWNLOADED_PROPERTY      "UpdateDownloaded"

//Periodically show Review Request;
#define REVIEW_DATE_PROPERTY            "ReviewDate"


#define REVIEW_REQUEST_PERIOD           (60*60*24) //secs;
#define FIRST_REVIEW_REQUEST_PERIOD     (60*45*1) //secs;

//Debug
#define REVIEW_REQUEST_PERIOD           (60*2) //secs;
#define FIRST_REVIEW_REQUEST_PERIOD     (60*1) //secs;


class ProductControllerPrivate {

public:

    ProductControllerPrivate(ProductController* p) : public_(p) {
        store_ = new InAppStore(public_);
        loadConfig();
        createLicensing();
        createUpdater();
        createReviewRequest();
        
    }

    ~ProductControllerPrivate() {
        saveConfig();
    }


    void createLicensing() {
        Q_ASSERT(store_);

        QObject::connect(store_, &InAppStore::licenseChanged, [=]() {
            auto license = store_->addon_license(STORE_PRODUCT_ID);

            if (license &&
                (license->isActive() || license->isTrial()))
                AlertDialog::execDialog(EAlert::kAlert_LicenceActivated);
            else
                AlertDialog::execDialog(EAlert::kAlert_LicenceDeactivated);

            emit public_->licenseChanged(false);

        });
    }

    void createUpdater() {
        Q_ASSERT(store_);

        QObject::connect(store_, &InAppStore::updatesAvailable, [&]() {
            store_->downloadUpdates();
        });

        QObject::connect(store_, &InAppStore::updatesDownloadFinished, [&](const InAppStatus& status) {
            if (status.isOk()) {
                update_downloaded_ = true;
                emit public_->updateDownloaded();
            }
            else {
                qInfo() << "Download updates error" << status.message();
            }
        });
    }

    void    createReviewRequest() {
        QObject::connect(qApp, &QApplication::aboutToQuit, store_, [this](auto) {
            if (review_date_ < QDateTime::currentDateTime() && !installing_update_) {
                
                InAppStatus status;
                QEventLoop loop;

               QObject::connect(store_, &InAppStore::ratingReviewResponse, &loop, [&](const InAppStatus& result) {
                    QEventLoopLocker locker(&loop);
                    qDebug() << "Review status: " << result.message();
                    status = result;
                });
                
                store_->requestReview();
                loop.exec();
                //FIX: hover sometimes not removed;
                if (ui_)
                    ui_->update();
                review_date_ = status.isOk()? QDateTime::currentDateTime().addYears(1) : 
                    QDateTime::currentDateTime().addSecs(REVIEW_REQUEST_PERIOD);
                                
                qDebug() << "Updated Review date" << review_date_;
                saveConfig();
            }
        });
        
    }

    void initializeUi(QWidget* hwnd) {
       
        ui_ = hwnd;
        store_->initializeUi(hwnd);
    }

    bool isActivated() const {
        auto license = store_->addon_license(STORE_PRODUCT_ID);
        return license &&
            (license->isActive() || license->isTrial());
    }

    void execPurchase() {
        InAppStatus     status;
        QEventLoop      loop;

        InAppProduct* product = store_->addon_product(STORE_PRODUCT_ID);

        if (product) {
            QObject::connect(product, &InAppProduct::purchasingStatus, &loop, [&](const InAppStatus& result) {
                qDebug() << "Finished purchase status:" << result.message();
                QEventLoopLocker locker(&loop);
                status = result;

            });
            //QTimer::singleShot(10, product, &InAppProduct::purchase);
            product->purchase();
            loop.exec();
            //FIX: hover sometimes not removed;
            if (ui_)
                ui_->update();
            qDebug() << "Finished purchasing" << status.message();
        }
    }
    
    void execActivation() {
        execPurchase();
    }

    //////////////////////////////////////////////////////////////////////////

    bool isUpdateDownloaded() {
        return update_downloaded_;
    }

    void installUpdate() {
        QObject::connect(qApp, &QApplication::aboutToQuit, [=](auto) {
            InAppStatus status;
            QEventLoop loop;
            QObject::connect(store_, &InAppStore::updatesInstallFinished, &loop, [&](const InAppStatus& result) {
                QEventLoopLocker locker(&loop);
                qDebug() << "Install status: " << result.message();
                status = result;
            }, Qt::QueuedConnection);

            store_->installUpdates();
            loop.exec();
            //FIX: hover sometimes not removed;
            if (ui_)
                ui_->update();
            update_downloaded_ = !status.isOk();

        });

        installing_update_ = true;
        QApplication::closeAllWindows();
    }

    bool isTrial() const {
        //No trial for windows store;
        return false;
    }

private:
    void loadConfig() {
        QSettings settings;
        
        update_downloaded_ = settings.value(UPDATE_DOWNLOADED_PROPERTY, false).toBool();

        review_date_ = settings.value(
            REVIEW_DATE_PROPERTY, QDateTime::currentDateTime().addSecs(FIRST_REVIEW_REQUEST_PERIOD)).toDateTime();

        //Debug;
        //review_date_ = QDateTime::currentDateTime();

        qDebug() << "Load Review date" << review_date_;
    }

    void saveConfig() const {
        QSettings settings;

        settings.setValue(UPDATE_DOWNLOADED_PROPERTY, update_downloaded_);
        settings.setValue(REVIEW_DATE_PROPERTY, review_date_);
    }


private:
    InAppStore*                     store_;
    bool                            update_downloaded_ = false;
    QDateTime                       review_date_;

    bool                            installing_update_ = false;
    QWidget*                        ui_ = nullptr;
    
private:
    ProductController*              public_;
};

#elif PURCHASING_BACKEND == VENDING_BACKEND
#pragma comment(lib, "vending.lib")


class ProductControllerPrivate {

public:
    ProductControllerPrivate(ProductController* p) : public_(p) {
        createVending();
    }

    void initializeUi(QWidget* hwnd) {
        Q_UNUSED(hwnd);
        //Used only in Windows Store;
    }

    void createVending() {
        product_ = new vending::Product(PRODUCT_NAME, COMPANY_NAME, COMPANY_SETTINGS_PATH, COMPANY_VENDING_URL, public_);
        license_ = new vending::License(VENDING_PRODUCT_ID, VENDING_PUBLIC_KEY, COMPANY_SETTINGS_PATH, public_);
        //license_->migrateFromPaddle();

        QObject::connect(license_, &vending::License::changed, [=](bool forced) {
            if (license_->isValid()
                && license_->expirationDate() >= QDateTime::currentDateTimeUtc()) {
                activated_ = true;
                if (!forced)
                    AlertDialog::execDialog(EAlert::kAlert_LicenceActivated);
            }
            else {
                activated_ = false;
                if (license_->status().code() == license_->status().LicenseExpired) {
                    AlertDialog::execDialog(EAlert::kAlert_LicenceDeactivated);
                }
                else if (license_->productId().toInt() != -1
                    && license_->status().code() == license_->status().NetError) {
                    AlertDialog::execDialog(EAlert::kAlert_LicenseNetworkError);
                }
                else if (forced) {
                    qDebug() << "forced license deleting";
                }
                else {
                    AlertDialog::execDialog(EAlert::kAlert_License, EPortal::kUnknown, license_->status().message());
                }
            }

            emit public_->licenseChanged(forced);
        });

        //Updater
        QObject::connect(product_, &vending::Product::productDataNetError, public_, &ProductController::productDataNetError);
        QObject::connect(product_, &vending::Product::productDataError, public_, &ProductController::productDataError);
        QObject::connect(product_, &vending::Product::downloadFinished, public_, &ProductController::updateDownloaded);
        QObject::connect(product_, &vending::Product::updateError, public_, &ProductController::updateError);
        QObject::connect(product_, &vending::Product::updateAvailable, [=](bool available) {
            emit public_->updateAvailable(available);
            if (available
                && !product_->isUpdateDownloaded())
                product_->downloadUpdate();
        });
        QObject::connect(product_, &vending::Product::downloadProgress, [=](float progress) {
            emit public_->updateProgress(progress);
        });

        if (license_->isValid()
            && license_->expirationDate() >= QDateTime::currentDateTimeUtc())
            verify(true);
    }
    
    bool isActivated() const {
        return activated_;
    }

    void firstTryEnded() {
        license_->firstTryEnded();
    }

    void verify(bool forced_activate) {
        license_->verify(forced_activate);
    }

    void deactivate() {
        activated_ = false;
        license_->deactivate();
    }

    void openProductPage() {
        if (product_ != nullptr) {
            QString company_url = COMPANY_URL;
            product_->openProductPage(company_url);
        }
    }

    void execPurchase() {
        if (product_ != nullptr)
            product_->purchase();
    }

    void execActivation() {
        if (product_ != nullptr) {
            ActivationDialog dialog(product_->formattedPrice(), QApplication::activeWindow());

            if (product_->purchase_url().isEmpty())
                dialog.enablePurchase(false);

            QObject::connect(&dialog, &ActivationDialog::activateClicked,
                [=](const QString& email, const QString& key) {
                    license_->activate(email, key);
            });

            //Purchase product;
            QObject::connect(&dialog, &ActivationDialog::purchaseClicked, product_, &vending::Product::purchase);

            QObject::connect(license_, &vending::License::error, &dialog, [&]() {
                dialog.setError(license_->status().message());
            });

            //QObject::connect(license_, &vending::License::changed, &dialog, [&]() {
            //    if (license_->isValid())
            //        dialog.accept();
            //});

            QObject::connect(license_, &vending::License::finished, &dialog, [&]() {
                dialog.accept();
            });

            //QObject::connect(&dialog, &ActivationDialog::cancelClicked, &dialog, [&]() {
            //    if (!license_->isValid()) {
            //        //Activate Trial
            //        license_->activate(FLIXGRAB_TIRAL_EMAIL, FLIXGRAB_TIRAL_KEY);
            //    }
            //        
            //});

            dialog.exec();
        }
    }

    QDateTime expirationDate() const {
        return license_->expirationDate();
    }


    QString registeredEmail() const {
        return license_->registeredEmail();
    }

    QString registeredKey() const {
        return license_->registeredKey();
    }

    //////////////////////////////////////////////////////////////////////////

    bool isUpdateAvailable() {
        if (product_ != nullptr)
            return product_->isUpdateAvailable();
        else
            return false;
    }

    bool isUpdateDownloaded() {
        if (product_ != nullptr)
            return product_->isUpdateDownloaded();
        else
            return false;
    }

    void installUpdate() {

        if (!will_be_updated_) {
            QObject::connect(qApp, &QApplication::aboutToQuit, [=](auto) {
                product_->installUpdate();
            });
            will_be_updated_ = true;
        }

        QApplication::closeAllWindows();
    }

    void downloadProductData() {
        if (product_ != nullptr)
            product_->downloadProductData();
    }

    bool isFirstTry() const {
        return license_->isFirstTry();
    }

    bool isTrial() const {
        //Not supporting yet;
        return false;
    }

private:
    vending::Product*               product_;
    vending::License*               license_;
    bool                            activated_ = false;
 
    bool                            will_be_updated_ = false;

private:
    ProductController*              public_;
};

QDateTime ProductController::expirationDate() const {
    return private_->expirationDate();
}

QString ProductController::registeredEmail() const {
    return private_->registeredEmail();
}

QString ProductController::registeredKey() const {
    return private_->registeredKey();
}

void ProductController::downloadProductData() {
    private_->downloadProductData();
}

void ProductController::openProductPage() {
    private_->openProductPage();
}

void ProductController::verify(bool forced_activate) {
    private_->verify(forced_activate);
}

void ProductController::deactivate() {
    private_->deactivate();
}

bool ProductController::isFirstTry() const {
    return private_->isFirstTry() && !private_->isActivated() && !private_->isTrial();
}

void ProductController::firstTryEnded() {
    private_->firstTryEnded();
}
#endif

ProductController* ProductController::ProductController::instance() {
    static ProductController   controller;
    return &controller;
}

ProductController::ProductController() {
    private_.reset(new ProductControllerPrivate(this));
}

ProductController::~ProductController() {
    qDebug() << "~ProductController";
}

void ProductController::initializeUi(QWidget* hwnd) {
    private_->initializeUi(hwnd);
}

bool ProductController::isActivated() const {
//#ifdef _DEBUG
//    return true;
//#endif
    return private_->isActivated();
}

void ProductController::execPurchase() {
    private_->execPurchase();
}

void ProductController::execActivation() {
    activation_dialog_clicked_ = true;
    private_->execActivation();
}

//////////////////////////////////////////////////////////////////////////
//bool ProductController::isUpdateAvailable() {
//    return private_->isUpdateAvailable();
//}

bool ProductController::isUpdateDownloaded() {
    return private_->isUpdateDownloaded();
}

void ProductController::installUpdate() {
    private_->installUpdate();
}

bool ProductController::isTrial() const {
    return private_->isTrial();
}

QString ProductController::statusDesc(const EStatusCode& code, QString& subcode_) const {
    if (subcode_ == "0")
        subcode_.clear();

    QString desc = QApplication::translate("ProductController", "Unknown error %1").arg(subcode_);

    switch (code) {
        case Success:
            desc = "Success";
            break;
        case NetError:
            desc = QApplication::translate("ProductController", "Network error");
            break;
        case ParseError:
            desc = QApplication::translate("ProductController", "Page parse error");
            break;
        case WrongUrl://
            desc = QApplication::translate("ProductController", "Wrong URL address");
            break;
        case RegistrationFailed:
            desc = QApplication::translate("ProductController", "Registration failed");
            break;
        case CheckProfileFailed:
            desc = QApplication::translate("ProductController", "Check profile failed");
            break;
        case ParseTimeout:
            desc = QApplication::translate("ProductController", "Engine response timeout");
            break;
        case StreamError:
            desc = QApplication::translate("ProductController", "DASH stream error %1").arg(subcode_);
            break;
        case EncoderError:
            desc = QApplication::translate("ProductController", "Encoder error %1").arg(subcode_);
            break;
        case CdmError:
            desc = QApplication::translate("ProductController", "Cdm error %1").arg(subcode_);
            break;
        case PxError:
            desc = QApplication::translate("ProductController", "Internal error %1").arg(subcode_);
            break;
        case HttpError:
            desc = QApplication::translate("ProductController", "Http error %1").arg(subcode_);
            break;
        case NoMemory:
            desc = QApplication::translate("ProductController", "No memory error %1").arg(subcode_);
            break;
        case FileError:
            desc = QApplication::translate("ProductController", "IO File error %1").arg(subcode_);
            break;
        case ConvError:
            desc = QApplication::translate("ProductController", "Conversion error %1").arg(subcode_);
            break;
        case ConvSubtitlesError:
            desc = QApplication::translate("ProductController", "Conversion subtitles error %1").arg(subcode_);
            break;
        case RecodeSubtitlesError:
            desc = QApplication::translate("ProductController", "Processing subtitles error %1").arg(subcode_);
            break;
        case DecompressSubtitlesError:
            desc = QApplication::translate("ProductController", "Decompress subtitles error %1").arg(subcode_);
            break;
        case WriteMetadataError:
            desc = QApplication::translate("ProductController", "Writing metadata error %1").arg(subcode_);
            break;
        case LiveStream:
            desc = QApplication::translate("ProductController", "Live streams are not supported");
            break;
        case VideoBlockedInLocation:
            desc = QApplication::translate("ProductController", "Video is unavailable in your location");
            break;
        case PrivateVideo:
            desc = QApplication::translate("ProductController", "Video is private");
            break;
        case ProtectedByPassword:
            desc = QApplication::translate("ProductController", "Video is protected by a password");
            break;
        case VideoUnavailable:
            desc = QApplication::translate("ProductController", "Video is unavailable");
            break;
        case ForcedReloadPage:                      //Amazon, HBO
            desc = QApplication::translate("ProductController", "Forcing reload page");
            break;
        case EmptyData:
            desc = QApplication::translate("ProductController", "Data is empty. Restart");
            break;
        case PaidVideo:
            desc = QApplication::translate("ProductController", "Paid video");
            break;
        case YouTubeMovies:
            desc = QApplication::translate("ProductController", "YouTube Movies are not supported");
            break;
        case StopPage:
            desc = QApplication::translate("ProductController", "Stop load page error %1").arg(subcode_);
            break;
        default:
            break;
    }

    desc.append(" (" + QString::number(code) + ")");

    return desc;
}