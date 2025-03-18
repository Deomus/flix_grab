#pragma once

#include "in_app_store.h"
#include "in_app_license.h"

class InAppStoreRuntime;

class InAppLicenseRuntime : public InAppLicense {

    Q_OBJECT

public:
    explicit InAppLicenseRuntime(InAppStoreRuntime* runtime,
        const QDateTime &exp,
        bool is_active,
        bool is_trial,
        const QString &id, QObject *parent = Q_NULLPTR);

    InAppProduct* product() override;

private:
    Q_DISABLE_COPY(InAppLicenseRuntime)

    InAppStoreRuntime * runtime_;
  
};

