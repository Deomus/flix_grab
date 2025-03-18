#pragma once

#include <QtCore>

class InAppProduct;
class InAppLicensePrivate;
class InAppLicense : public QObject
{
    Q_OBJECT


public:
    

    ~InAppLicense();
            

    QDateTime       expirationDate() const;

    bool            isActive() const;

    bool            isTrial() const;

    QString         identifier() const;

    virtual InAppProduct*   product() = 0;

protected:
    explicit InAppLicense(const QDateTime &exp,
        bool is_active,
        bool is_trial,
        const QString &id, QObject *parent = Q_NULLPTR);

private:
    friend class InAppStoreRuntime;
    Q_DISABLE_COPY(InAppLicense)

    QSharedPointer<InAppLicensePrivate> private_;
};
