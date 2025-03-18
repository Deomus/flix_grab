#include "in_app_license_runtime.h"
#include "in_app_store_runtime.h"

InAppLicenseRuntime::InAppLicenseRuntime(InAppStoreRuntime* runtime, const QDateTime &exp, bool is_active, bool is_trial, const QString &id, QObject *parent /*= Q_NULLPTR*/)
    : InAppLicense(exp, is_active, is_trial, id, parent)
    , runtime_(runtime)
{

}

Q_INVOKABLE InAppProduct* InAppLicenseRuntime::product() {
    InAppProduct* product = nullptr;
    QMetaObject::invokeMethod(runtime_, "product", Qt::BlockingQueuedConnection, Q_RETURN_ARG(InAppProduct*, product), Q_ARG(QString, this->identifier()));
    return product;
}
