#include "alert_dialog.h"

#include <company_info.h>
#include <product_config.h>
#include "product_controller.h"


AlertDialog::AlertDialog(EAlert type, const EPortal& portal, const QString& main_text, QWidget *parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/) : QDialog(parent, f) {
    ui_.setupUi(this);
    QApplication::alert(this);
    setType(type, portal, main_text);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void AlertDialog::setType(EAlert type, const EPortal& portal, const QString& main_text) {
    type_ = type;
    ui_.btnAccept->setVisible(true);
    ui_.btnReject->setVisible(true);
    ui_.btnContinue->setVisible(false);
    
    QObject::disconnect(ui_.btnAccept, 0, ProductController::instance(), 0);
    QObject::disconnect(ui_.btnReject, 0, ProductController::instance(), 0);

    //BUG: not showed then close event;
    //setWindowFlags(windowFlags() & Qt::WindowCloseButtonHint);
    
    //return;

    ui_.lblMainDesk->setVisible(false);

    QString lbl_main_text(main_text);
    if (!lbl_main_text.isEmpty())
        ui_.lblMain->setText(lbl_main_text);


    QString portal_name;
    portalName(portal, portal_name);
    
    QString source_name = tr("playlists");
    if (portal == EPortal::kNetflix
        || portal == EPortal::kAmazon
        || portal == EPortal::kDisney
        || portal == EPortal::kHBO
        || portal == EPortal::kHulu)
        source_name = tr("all episodes");

    int video_limit = durationLimit(portal);
    QString content_type = tr("video");
    if (type == EAlert::kAlert_AudioLimitDuration)
        content_type = tr("audio");

    switch (type) {
        case EAlert::kAlert_Unknown:
            break;
        case EAlert::kAlert_LimitHdAnd5_1:
            //this->setWindowTitle(tr("Limitation"));
            //ui_.lblHeadline->setText(tr("HD Video & Dolby Audio 5.1"));
            ui_.lblCaption->setText(tr("You've reached high-quality limit"));
            this->setWindowTitle(tr("Quality limitation"));
            ui_.lblMain->setText(tr("Downloading high-quality video and audio is available\nin premium version.\nPlease purchase a subscription to download such streams."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;

        case EAlert::kAlert_LimitDownloadSpeed:
            //this->setWindowTitle(tr("Limitation"));
            //ui_.lblHeadline->setText(tr("Downloading speed is limited"));
            ui_.lblCaption->setText(tr("You've reached speed limit"));
            this->setWindowTitle(tr("Speed limitation"));
            ui_.lblMain->setText(tr("Speed was slowed down.\nTo download up to 3 times faster, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_VideoLimitDuration:
        case EAlert::kAlert_AudioLimitDuration:
            this->setWindowTitle(tr("%1 minute limit").arg(QString::number(video_limit)));
            //ui_.lblCaption->setText("");
            ui_.wgtCaption->setVisible(false);
            ui_.btnContinue->setVisible(true);

            QObject::connect(ui_.btnContinue, &QPushButton::clicked, this, &AlertDialog::close);

            if (video_limit > 1)
                ui_.lblMain->setText(tr("The free version can only download the first %1 mins of the %2.\nPlease purchase a subscription to download the full %2.").arg(QString::number(video_limit)).arg(content_type));
            else
                ui_.lblMain->setText(tr("The free version can only download 1 minute of the %1.\nPlease purchase a subscription to download the full %1.").arg(content_type));

            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
            ui_.btnReject->setVisible(false);
#endif
            break;
        case EAlert::kAlert_LimitThreads:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Multithreading limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached multithreading download limit"));
            ui_.lblMain->setText(tr("Downloading multiple streams is available in premium version.\nTo download up to 10 videos simultaneously, please\npurchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitEncodingSpeed:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Encoding speed limitation"));
            //ui_.lblHeadline->setText(tr("Encoding speed is limited"));
            ui_.lblCaption->setText(tr("You've reached encoding speed limit"));
            ui_.lblMain->setText(tr("Encoding at higher speed is available in premium version.\nTo encoding at higher speed, please\npurchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitPresets:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Presets limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached presets limit"));
            ui_.lblMain->setText(tr("All presets are available in premium version.\nTo use any presets, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitAudioTracks:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Multiple audio tracks limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached a limit for downloading multiple audio tracks"));
            ui_.lblMain->setText(tr("Downloading multiple audio tracks is available in premium version.\nTo download multiple audio tracks, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitAudioLanguages:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Multiple audio languages limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached a limit for downloading multiple audio languages"));
            ui_.lblMain->setText(tr("Downloading multiple audio languages is available in premium version.\nTo download multiple audio languages, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitAudioType:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Audio for the blind limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached a limit for downloading audio for the blind"));
            ui_.lblMain->setText(tr("Downloading audio for the blind is available in premium version.\nTo download audio for the blind, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitSubtitle:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Subtitles limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached subtitles download limit"));
            ui_.lblMain->setText(tr("Downloading subtitles is available in premium version.\nTo download subtitles, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitSaveSubtitlesAs:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Subtitles limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached download subtitles in SRT format limit"));
            ui_.lblMain->setText(tr("Downloading subtitles in SRT format is available in premium version.\nTo download subtitles in SRT format, please purchase a subscription."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitVideo:
            this->setWindowTitle(tr("Video limitation"));
            ui_.lblCaption->setText(tr("You've reached video download limit"));
            ui_.lblMain->setText(tr("Downloading %1 videos are available in premium version.\nTo download videos, please purchase a subscription.").arg(portal_name));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitAudio:
            this->setWindowTitle(tr("Audio limitation"));
            ui_.lblCaption->setText(tr("You've reached audio download limit"));
            ui_.lblMain->setText(tr("Downloading %1 audio is available in premium version.\nTo download audio, please purchase a subscription.").arg(portal_name));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitImage:
            this->setWindowTitle(tr("Images limitation"));
            ui_.lblCaption->setText(tr("You've reached images download limit"));
            ui_.lblMain->setText(tr("Downloading %1 images is available in premium version.\nTo download images, please purchase a subscription.").arg(portal_name));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LimitPlaylist:
            this->setWindowTitle(tr("%1 limitation").arg(source_name).replace(0, 1, source_name[0].toUpper()));
            ui_.lblCaption->setText(tr("You've reached %1 download limit").arg(source_name));
            ui_.lblMain->setText(tr("Downloading %1 is available in premium version.\nTo download %1, please purchase a subscription.").arg(source_name));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_Image:
            //this->setWindowTitle(tr("Limitation"));
            this->setWindowTitle(tr("Images limitation"));
            //ui_.lblHeadline->setText(tr("Downloading threads is limited"));
            ui_.lblCaption->setText(tr("You've reached a limit for downloading high resolution images"));
            ui_.lblMain->setText(tr("Downloading high resolution images is available in premium version.\nPlease purchase a subscription to download such images."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;

#if PURCHASING_BACKEND == VENDING_BACKEND
        case EAlert::kAlert_GetProductsData:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("A newer version is available"));
            this->setWindowTitle(tr("Update %1").arg(QApplication::applicationName()));
            ui_.lblCaption->setText(tr("Unable to check for update"));
            ui_.lblMain->setText(tr("Check your internet connection or try to disable your antivirus or firewall and click the Check button again."));
            ui_.btnAccept->setText(tr("Check"));
            ui_.btnReject->setVisible(false);

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::downloadProductData);
            break;

        case EAlert::kAlert_DownloadUpdate:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("A newer version is available"));
            this->setWindowTitle(tr("Update %1").arg(QApplication::applicationName()));
            ui_.lblCaption->setText(tr("Error downloading update"));
            ui_.lblMain->setText(tr("The application cannot download the update.\n Please download and install the update manually."));
            ui_.btnAccept->setText(tr("Update"));
            ui_.btnReject->setVisible(false);

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::openProductPage);
            break;
#endif
        case EAlert::kAlert_MandatoryUpdate:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("A newer version is available"));
            this->setWindowTitle(tr("Update %1").arg(QApplication::applicationName()));
            ui_.lblCaption->setText(tr("Important update available"));
            ui_.lblMain->setText(tr("Please install update to continue.\nDue to update process the application will be closed."));
            ui_.btnAccept->setText(tr("Install"));
            ui_.btnReject->setVisible(false);

            setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::installUpdate/*, Qt::QueuedConnection*/);
            break;

        case EAlert::kAlert_DeleteAccounts:
            this->setWindowTitle(tr("Deleting accounts data"));
            ui_.lblCaption->setText(tr("Application restart is required to delete accounts data"));
            ui_.lblMain->setText(tr("Are you sure you want to restart the application?"));

            ui_.btnAccept->setText(tr("Restart"));
            ui_.btnReject->setText(tr("Cancel"));
            break;

        case EAlert::kAlert_UnexpectedExit:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("Application exit"));
            this->setWindowTitle(tr("Exit %1").arg(QApplication::applicationName()));
            ui_.lblCaption->setText(tr("Some links didn't download"));
            ui_.lblMain->setText(tr("Exit anyway?"));

            ui_.btnAccept->setText(tr("Exit and save links to file"));
            ui_.btnReject->setText(tr("Exit"));
            break;

        case EAlert::kAlert_UnexpectedExitOnDownload:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("Application exit"));
            this->setWindowTitle(tr("Exit %1").arg(QApplication::applicationName()));
            ui_.lblCaption->setText(tr("Download is in progress"));
            ui_.lblMain->setText(tr("Exit anyway?"));

            ui_.btnAccept->setText(tr("Exit and save progress"));
            ui_.btnReject->setText(tr("Exit"));
            break;

        case EAlert::kAlert_UnexpectedExitOnParse:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("Application exit"));
            this->setWindowTitle(tr("Exit %1").arg(QApplication::applicationName()));
            ui_.lblCaption->setText(tr("Parsing playlist is in progress"));
            ui_.lblMain->setText(tr("Exit anyway?"));

            ui_.btnAccept->setText(tr("Continue parsing"));
            ui_.btnReject->setText(tr("Exit"));
            break;

        case EAlert::kAlert_DeleteItems:
            //this->setWindowTitle(tr("Warning"));
            //ui_.lblHeadline->setText(tr("Application exit"));
            this->setWindowTitle(QApplication::applicationName());
            ui_.lblCaption->setText(tr("Deleting files"));
            ui_.lblMain->setText(tr("Are you sure you want to delete the downloaded files?"));

            ui_.btnAccept->setText(tr("Delete"));
            ui_.btnReject->setText(tr("Cancel"));
            break;

        case EAlert::kAlert_LicenceActivated:
            //this->setWindowTitle(tr("Information"));
            //ui_.lblHeadline->setText(tr("Activation successful"));
            this->setWindowTitle(QString("%1 %2 %3").arg(QApplication::applicationName()).arg("-").arg(tr("License")));
            lbl_main_text = tr("Thank you for choosing %1!\nHope you enjoy").arg(COMPANY_NAME);

            ui_.lblCaption->setText(tr("Activation successful"));
            ui_.lblMain->setText(lbl_main_text);
#if PURCHASING_BACKEND == VENDING_BACKEND
            ui_.lblMainDesk->setText(QString("\n%1: %3\n%2: %4").arg(tr("Activated email")).arg(tr("Activated key")).arg(ProductController::instance()->registeredEmail()).arg(ProductController::instance()->registeredKey()));
            //ui_.lblMainDesk->setAlignment(Qt::AlignJustify);
            ui_.lblMainDesk->setVisible(true);
#endif
            ui_.btnAccept->setText(tr("Done"));
            ui_.btnReject->setVisible(false);
            break;

        case EAlert::kAlert_LicenceDeactivated:
            //this->setWindowTitle(tr("Information"));
            //ui_.lblHeadline->setText(tr("Activation successful"));
            this->setWindowTitle(QString("%1 %2 %3").arg(QApplication::applicationName()).arg("-").arg(tr("License")));
            ui_.lblCaption->setText(tr("License expired"));
            ui_.lblMain->setText(tr("Your license has expired.\nPlease purchase a new license to use all the features again."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_LicenseNetworkError:
            this->setWindowTitle(QString("%1 %2 %3").arg(QApplication::applicationName()).arg("-").arg(tr("License check")));
            ui_.lblCaption->setText(tr("Unable to check license"));
            ui_.lblMain->setText(tr("Check your internet connection or try to disable your antivirus or firewall and click the Check license button again."));
            ui_.btnAccept->setText(tr("Check license"));
            ui_.btnReject->setVisible(false);
#if PURCHASING_BACKEND == VENDING_BACKEND
            QObject::connect(ui_.btnAccept, &QPushButton::clicked, []() {
                ProductController::instance()->verify(true);
            });
#endif
            break;

        case EAlert::kAlert_License:
            this->setWindowTitle(QString("%1 %2 %3").arg(QApplication::applicationName()).arg("-").arg(tr("Invalid license")));
            ui_.lblCaption->setText(tr("Invalid license"));
            if (!lbl_main_text.isEmpty())
                lbl_main_text = tr("License is invalid. Please contact technical support.") + "\n\n" + tr("Error") + ": " + lbl_main_text;
            else
                lbl_main_text = tr("License is invalid. Please contact technical support.");
#if PURCHASING_BACKEND == VENDING_BACKEND
            ui_.lblMainDesk->setText(QString("\n%1: %3\n%2: %4").arg(tr("Activated email")).arg(tr("Activated key")).arg(ProductController::instance()->registeredEmail()).arg(ProductController::instance()->registeredKey()));
            //ui_.lblMainDesk->setAlignment(Qt::AlignJustify);
            if (!ProductController::instance()->registeredEmail().isEmpty()
                && !ProductController::instance()->registeredKey().isEmpty())
                ui_.lblMainDesk->setVisible(true);
#endif
            ui_.lblMain->setText(lbl_main_text);

            ui_.btnAccept->setText(tr("Contact support"));
            ui_.btnReject->setText(tr("Cancel"));
            ui_.btnReject->setVisible(false);

            QObject::connect(ui_.btnAccept, &QPushButton::clicked, []() {
                QString bugreport_url = BUGREPORT_URL;
                QDesktopServices::openUrl(QUrl(bugreport_url));
            });
            break;

        case EAlert::kAlert_DeleteLicense:
            this->setWindowTitle(QApplication::applicationName());
            ui_.lblCaption->setText(tr("Deleting license"));
            ui_.lblMain->setText(tr("Are you sure you want to delete the license?"));

            ui_.btnAccept->setText(tr("Delete"));
            ui_.btnReject->setText(tr("Cancel"));
#if PURCHASING_BACKEND == VENDING_BACKEND
            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::deactivate);
#endif
            break;

        case EAlert::kAlert_TrialStarted:
            //this->setWindowTitle(tr("Information"));
            //ui_.lblHeadline->setText(tr("Activation successful"));
            ui_.lblCaption->setText(tr("Trial period started"));
            this->setWindowTitle(QString("%1 %2 %3").arg(QApplication::applicationName()).arg("-").arg(tr("Trial")));
            ui_.lblMain->setText(tr("Thanks for downloading %1 application.\nYou can use all the features for 14 days for free.").arg(COMPANY_NAME));

            ui_.btnAccept->setText(tr("Ok"));
            ui_.btnReject->setVisible(false);
            break;

        case EAlert::kAlert_TrialFinished:
            //this->setWindowTitle(tr("Information"));
            //ui_.lblHeadline->setText(tr("Activation successful"));
            ui_.lblCaption->setText(tr("Trial period expired"));
            this->setWindowTitle(QString("%1 %2 %3").arg(QApplication::applicationName()).arg("-").arg(tr("Trial")));
            ui_.lblMain->setText(tr("Your trial period has expired.\nPlease purchase a license to use all the features again."));
            ui_.btnAccept->setText(tr("Buy now"));

            QObject::connect(ui_.btnAccept, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execPurchase);
#if PURCHASING_BACKEND != WINDOWS_STORE_BACKEND
            ui_.btnReject->setText(tr("Activate key"));
            QObject::connect(ui_.btnReject, &QPushButton::clicked,
                ProductController::instance(), &ProductController::execActivation);
#else
            ui_.btnReject->setText(tr("No, thanks"));
#endif
            break;
        case EAlert::kAlert_ShutdownPC:
            this->setWindowTitle(tr("Shutdown"));
            ui_.lblMain->setText(tr("The system will shut down in %1 seconds").arg(timeout_));
            ui_.wgtCaption->setVisible(false);
            ui_.btnAccept->setText(tr("Cancel"));
            ui_.wgtReject->setVisible(false);
            ui_.wgtContinue->setVisible(false);

            QObject::connect(ui_.btnAccept, &QPushButton::clicked, this, &AlertDialog::close);
            startTimer();
            break;
        default:
            break;
    }
    ui_.wgtContent->adjustSize();
    this->adjustSize();
}

int AlertDialog::execDialog(EAlert type, const EPortal& portal, const QString& main_text, QWidget* parent) {
    AlertDialog dialog(type, portal, main_text, parent);
    //dialog.setWindowModality(Qt::ApplicationModal);
    return dialog.exec();
}

void AlertDialog::startTimer(unsigned long timeout) {
    timeout_ = timeout;
    onTimerTimeout();
}

void AlertDialog::onTimerTimeout() {
    if (timeout_) {
        updateMessage();
        QTimer::singleShot(1000, this, SLOT(onTimerTimeout()));
        --timeout_;
    }
    else {
        emit countdownEnded(type_);
        close();
    }
}

void AlertDialog::updateMessage() {
    switch (type_) {
        case EAlert::kAlert_ShutdownPC:
            ui_.lblMain->setText(QApplication::translate("AlertDialog", "The system will shut down in %1 seconds").arg(timeout_));
            break;
        default:
            break;
    }
}
