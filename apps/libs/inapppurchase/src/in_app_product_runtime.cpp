#include "in_app_product_runtime.h"
#include "in_app_store_runtime.h"

InAppProductRuntime::InAppProductRuntime(InAppStoreRuntime* runtime, const QString &price, const QString &currency, const QString &title, const QString &description, const QString &uri, const QString &id, QObject *parent /*= Q_NULLPTR*/)
    : InAppProduct(price, currency, title, description, uri, id, parent)
    , runtime_(runtime)
{
   
}

Q_INVOKABLE void InAppProductRuntime::purchase() {
    QMetaObject::invokeMethod(runtime_, "purchaseProduct", Qt::QueuedConnection, Q_ARG(QString, this->identifier()));
}
