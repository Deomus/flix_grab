#pragma once

#include "in_app_store.h"
#include "in_app_status.h"
#include "in_app_product.h"

class InAppStoreRuntime;

class InAppProductRuntime : public InAppProduct {

    Q_OBJECT
  

public:
    explicit InAppProductRuntime(InAppStoreRuntime* runtime,
        const QString &price,
        const QString &currency,
        const QString &title,
        const QString &description,
        const QString &uri,
        const QString &id, QObject *parent = Q_NULLPTR);

    void purchase() override;

private:
    Q_DISABLE_COPY(InAppProductRuntime)

    InAppStoreRuntime * runtime_;

};
