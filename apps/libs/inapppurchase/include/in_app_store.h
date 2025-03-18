
#pragma once

#include <QtCore>
#include <QtWidgets>

#include "in_app_status.h"
#include "in_app_product.h"
#include "in_app_license.h"

class InAppStoreRuntime;

//MUST be created in main thread;
class InAppStore : public QObject {
    Q_OBJECT

public:
    InAppStore(QObject* parent = nullptr);

    ~InAppStore();

public:
    void                        initializeUi(QWidget* hwnd);
    
public:
    
    
    
    InAppProduct*               addon_product(const QString& id);
    InAppProduct*               product();
    QList<InAppProduct*>        addon_products();


    InAppLicense*               license();
    InAppLicense*               addon_license(const QString& id);
    QList<InAppLicense*>        addon_licenses();

public:
    //Updates Methods
    bool                        isUpdatesAvailable();
    void                        downloadUpdates();
    void                        installUpdates();

    void                        requestReview();

signals:
    void                        licenseChanged();
    void                        updatesAvailable();

    void                        updatesProgressChanged(float total_progress);
    //Emited when downloading or Installing updates;
    void                        updatesDownloadFinished(const InAppStatus&);
    void                        updatesInstallFinished(const InAppStatus&);

    void                        ratingReviewResponse(const InAppStatus&);
    
private:
    

private:
    QScopedPointer<QThread>                 wrt_thread_;

    QScopedPointer<InAppStoreRuntime>       runtime_;
   
};