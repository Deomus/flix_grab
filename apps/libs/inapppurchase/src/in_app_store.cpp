#include "in_app_store.h"
#include "in_app_store_runtime.h"

InAppStore::InAppStore( QObject* parent /*= nullptr*/) : QObject(parent)

, wrt_thread_(new QThread)
, runtime_(new InAppStoreRuntime(this)){
    
    qRegisterMetaType<WId>();

    runtime_->moveToThread(wrt_thread_.data());
    //connect(&*wrt_thread_, &QThread::started, this, &InAppStore::initializeInternal, Qt::DirectConnection);
    //connect(&*wrt_thread_, &QThread::finished, this, &InAppStore::deinitializeInternal, Qt::DirectConnection);
    //connect(wrt_thread_, &QThread::finished, this, &QThread::deleteLater);

    connect(&*runtime_, &InAppStoreRuntime::licenseChanged, this, &InAppStore::licenseChanged, Qt::QueuedConnection);

    connect(&*runtime_, &InAppStoreRuntime::updatesAvailable, this, &InAppStore::updatesAvailable, Qt::QueuedConnection);
    connect(&*runtime_, &InAppStoreRuntime::updatesProgressChanged, this, &InAppStore::updatesProgressChanged, Qt::QueuedConnection);
    connect(&*runtime_, &InAppStoreRuntime::updatesDownloadFinished, this, &InAppStore::updatesDownloadFinished, Qt::QueuedConnection);
    connect(&*runtime_, &InAppStoreRuntime::updatesInstallFinished, this, &InAppStore::updatesInstallFinished, Qt::QueuedConnection);
        
    connect(&*runtime_, &InAppStoreRuntime::ratingReviewResponse, this, &InAppStore::ratingReviewResponse, Qt::QueuedConnection);
        
    QObject::connect(&*wrt_thread_, &QThread::started, [this]() {
        runtime_->initialize();
    });

    QObject::connect(&*wrt_thread_, &QThread::finished, [this]() {
        runtime_->deinitialize();
    });
    wrt_thread_->start();
            
}

InAppStore::~InAppStore() {
    wrt_thread_->quit();
    wrt_thread_->wait();
}



void InAppStore::initializeUi(QWidget* hwnd) {
    QMetaObject::invokeMethod(runtime_.data(), "initializeUi", Qt::QueuedConnection, Q_ARG(WId, hwnd->winId()));
}

//
//InAppStatus InAppStore::purchaseProduct() {
//    InAppStatus     status;
//    QEventLoop loop;
//    
//    auto connection = QObject::connect(&*runtime_, &InAppStoreRuntime::purchasingStatus, this, [&](const InAppStatus& result) {
//        qDebug() << "Purchase status: " << result.message();
//        status = result;
//        loop.quit();
//    }, Qt::QueuedConnection);
//    
//    QMetaObject::invokeMethod(runtime_.data(), "purchaseProduct", Qt::QueuedConnection);
//    loop.exec();
//
//    QObject::disconnect(connection);
//    return status;
//}

//InAppStatus InAppStore::requestReview() {
//    InAppStatus     status;
//    QEventLoop loop;
//
//    auto connection = QObject::connect(&*runtime_, &InAppStoreRuntime::ratingReviewResponse, this, [&](const InAppStatus& result) {
//        qDebug() << "Review status: " << result.message();
//        status = result;
//        loop.quit();
//    }, Qt::QueuedConnection);
//       
//    QMetaObject::invokeMethod(runtime_.data(), "requestReview", Qt::QueuedConnection);
//    loop.exec();
//
//    QObject::disconnect(connection);
//    return status;
//}


void InAppStore::requestReview() {
    
    QMetaObject::invokeMethod(runtime_.data(), "requestReview", Qt::QueuedConnection);
}



InAppProduct * InAppStore::addon_product(const QString & id) {
    InAppProduct*   product = nullptr;
    QMetaObject::invokeMethod(runtime_.data(), "addon_product", Qt::BlockingQueuedConnection, Q_RETURN_ARG(InAppProduct*, product), Q_ARG(QString, id));
    return product;
}

InAppProduct* InAppStore::product() {
    InAppProduct*   product = nullptr;
    QMetaObject::invokeMethod(runtime_.data(), "product", Qt::BlockingQueuedConnection, Q_RETURN_ARG(InAppProduct*, product));
    return product;
}

QList<InAppProduct*> InAppStore::addon_products() {
    QList<InAppProduct*>   products;
    QMetaObject::invokeMethod(runtime_.data(), "addon_products", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QList<InAppProduct*>, products));
    return products;
}

InAppLicense * InAppStore::license() {
    InAppLicense*   license = nullptr;
    QMetaObject::invokeMethod(runtime_.data(), "license", Qt::BlockingQueuedConnection, Q_RETURN_ARG(InAppLicense*, license));
    return license;
}

InAppLicense * InAppStore::addon_license(const QString & id) {
    InAppLicense*   license = nullptr;
    QMetaObject::invokeMethod(runtime_.data(), "addon_license", Qt::BlockingQueuedConnection, Q_RETURN_ARG(InAppLicense*, license), Q_ARG(QString, id));
    return license;
}

QList<InAppLicense*> InAppStore::addon_licenses() {
    QList<InAppLicense*>   licenses;
    QMetaObject::invokeMethod(runtime_.data(), "addon_licenses", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QList<InAppLicense*>, licenses));
    return licenses;
}

bool InAppStore::isUpdatesAvailable() {
    bool   result = false;
    QMetaObject::invokeMethod(runtime_.data(), "isUpdatesAvailable", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, result));
    return result;
}

void InAppStore::downloadUpdates() {
    
    QMetaObject::invokeMethod(runtime_.data(), "downloadUpdates", Qt::QueuedConnection);
    
}

void InAppStore::installUpdates() {
    QMetaObject::invokeMethod(runtime_.data(), "installUpdates", Qt::QueuedConnection);
}

