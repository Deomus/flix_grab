#include "in_app_product.h"

class InAppProductPrivate
{
public:
    InAppProductPrivate(const QString &price, const QString &currency, const QString &title, const QString &description, const QString &uri, const QString &id)
        : local_price(price)
        , local_title(title)
        , local_description(description)
        , local_uri(uri)
        , identifier(id)
    {
    }

    QString local_price;
    QString local_currency;
    QString local_title;
    QString local_description;
    QString local_uri;
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
*/\
InAppProduct::InAppProduct(const QString &price, const QString &currency, const QString &title, const QString &description, const QString &uri, const QString &identifier, QObject *parent)
    : QObject(parent)
{
    d = QSharedPointer<InAppProductPrivate>(new InAppProductPrivate(price, currency, title, description, uri, identifier));
}

/*!
* \internal
*/\
InAppProduct::~InAppProduct()
{
}

/*!

This property holds the price of the product as reported by the external store. This is the full
price including currency, usually in the locale of the current user.
*/
QString InAppProduct::price() const
{
    return d->local_price;
}

QString InAppProduct::currency() const
{
    return d->local_currency;
}

QString InAppProduct::uri() const
{
    return d->local_uri;
}


/*!
*
* This property holds the title of the product as reported by the external store.  This title is returned from the
* store in the locale language if available.
*/
QString InAppProduct::title() const
{
    return d->local_title;
}

/*!
*
* This property holds the description of the product as reported by the external store.  This description is returned
* from the store in the locale language if available.
*/
QString InAppProduct::description() const
{
    return d->local_description;
}

/*!

This property holds the identifier of the product. It matches the identifier which is registered in
the external store.
*/
QString InAppProduct::identifier() const
{
    return d->identifier;
}

