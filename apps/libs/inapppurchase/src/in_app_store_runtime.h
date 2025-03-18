#pragma once

#include "in_app_store.h"
#include "in_app_status.h"
#include "in_app_product.h"
#include "in_app_license.h"

class InAppStoreRuntimePrivate;
class InAppStoreRuntime : public QObject {
    Q_OBJECT;

    friend class InAppProductRuntime;
    friend class InAppStoreRuntimePrivate;
public:
    InAppStoreRuntime(InAppStore* p);

    ~InAppStoreRuntime();

public:

    Q_INVOKABLE bool                initialize();

    Q_INVOKABLE bool                initializeUi(WId hwdlg);


    Q_INVOKABLE void                deinitialize();
       
    Q_INVOKABLE void                requestReview();
  
    Q_INVOKABLE InAppProduct*           addon_product(const QString& id);
    Q_INVOKABLE InAppProduct*           product();
    Q_INVOKABLE QList<InAppProduct*>    addon_products();


    Q_INVOKABLE InAppLicense*           license();
    Q_INVOKABLE QList<InAppLicense*>    addon_licenses();
    Q_INVOKABLE InAppLicense*           addon_license(const QString& id);

public:
    Q_INVOKABLE bool                    isUpdatesAvailable();
    Q_INVOKABLE void                    downloadUpdates();
    Q_INVOKABLE void                    installUpdates();

        

    bool                            isInitialized() const { return is_initialized_; }


private:
    Q_INVOKABLE void                purchaseProduct(const QString& id);
    Q_INVOKABLE InAppProduct*       product(const QString& id);
                

signals:
        
    void                            ratingReviewResponse(const InAppStatus&);
    
    void                            licenseChanged();

    void                            updatesAvailable();

    void                            updatesProgressChanged(float total_progress);
    //Emited when downloading or Installing updates;
    void                            updatesDownloadFinished(const InAppStatus&);
    void                            updatesInstallFinished(const InAppStatus&);

private:
    QScopedPointer<InAppStoreRuntimePrivate>    private_;

    InAppProduct*                               product_ = nullptr;
    InAppLicense*                               license_ = nullptr;

    bool                                        is_initialized_ = false;


    QMap<QString, InAppProduct*>                addon_products_;
    QMap<QString, InAppLicense*>                addon_licenses_;
        
    
private:
    InAppStore * q_ptr;


    Q_DECLARE_PUBLIC(InAppStore)
    Q_DISABLE_COPY(InAppStoreRuntime)
};

Q_DECLARE_METATYPE(WId);


