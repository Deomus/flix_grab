#pragma once

#include <QtCore>
#include <QtGui>

#include <error_codes.h>


namespace paddle {
    class Paddle;
}

class AppUpdate;


//Controllling Offers and Features;
class ProductControllerPrivate;

class ProductController : public QObject {
    Q_OBJECT

public:
    ~ProductController();

public:
    static ProductController*   instance();

    void                        initializeUi(QWidget* hwnd);

public:
    //Product Licence;
    bool                        isActivated() const;
#if PURCHASING_BACKEND == VENDING_BACKEND
    void                        verify(bool forced_activate = false);
    void                        deactivate();
    QDateTime                   expirationDate() const;
    bool                        isFirstTry() const;
    void                        firstTryEnded();
#endif

    //Product Update;
    bool                        isUpdateAvailable();
    bool                        isUpdateDownloaded();
    void                        installUpdate();
    void                        downloadProductData();
    void                        openProductPage();
    QString                     statusDesc(const EStatusCode& code, QString& subcode_) const;
public:
    bool                        isTrial() const;

    QString                     registeredEmail() const;
    QString                     registeredKey() const;
    bool                        activationDialogClicked() { return activation_dialog_clicked_; }
    void                        setActivationDialogClicked(bool activation_dialog_clicked = false) { activation_dialog_clicked_ = activation_dialog_clicked; }

public slots:
    void                        execPurchase();
    void                        execActivation();

signals:
    void                        updateError();
    void                        productDataNetError();
    void                        productDataError();
    void                        updateAvailable(bool available);
    void                        updateProgress(float progress);
    void                        updateDownloaded();
    void                        licenseChanged(bool forced = false);

private:
    ProductController();

private:
    Q_DISABLE_COPY(ProductController);

    QScopedPointer<ProductControllerPrivate>            private_;
    bool activation_dialog_clicked_ = false;
};
