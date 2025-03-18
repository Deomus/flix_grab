#include "in_app_license.h"

class InAppLicensePrivate
{
public:
    InAppLicensePrivate(const QDateTime &exp, bool is_active, bool is_trial, const QString &id)
        : local_expiration(exp)
        , local_is_active(is_active)
        , local_is_trial(is_trial)
        , identifier(id)
    {
    }

    bool local_is_active;
    bool local_is_trial;
    QDateTime local_expiration;
    QString identifier;
};

/*!
\class QInAppProduct
\inmodule QtPurchasing
\brief A product registered in the store

QInAppProduct encapsulates a product in the external store after it has been registered in \c QInAppStore
and confirmed to exist. It has an identifier which matches the identifier of the product in the external
store, it has a price which is retrieved from the external store, and it has a product type.

The product type can be either \c Consumable or \c Unlockable. The former type of products can be purchased
any number of times as long as each transaction is finalized explicitly by the application. The latter type
can only be purchased once.
*/

/*!
* \internal
*/
InAppLicense::InAppLicense(const QDateTime &exp, bool is_active, bool is_trial, const QString &identifier, QObject *parent)
    : QObject(parent)
{
    private_ = QSharedPointer<InAppLicensePrivate>(new InAppLicensePrivate(exp, is_active, is_trial, identifier));

    qDebug() << "License for product:" << identifier << ",active:" << is_active << ",trial:" << is_trial << ",expiration:" << exp.toString(); // exp.toString(Qt::SystemLocaleShortDate);
}

/*!
* \internal
*/
InAppLicense::~InAppLicense()
{
}

/*!

This property holds the price of the product as reported by the external store. This is the full
price including currency, usually in the locale of the current user.
*/
QDateTime InAppLicense::expirationDate() const
{
    return private_->local_expiration;
}

bool InAppLicense::isActive() const
{
    return private_->local_is_active;
}

bool InAppLicense::isTrial() const
{
    return private_->local_is_trial;
}
/*!

This property holds the identifier of the product. It matches the identifier which is registered in
the external store.
*/
QString InAppLicense::identifier() const
{
    return private_->identifier;
}

