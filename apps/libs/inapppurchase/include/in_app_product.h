#pragma once 
#include <QtCore>
#include "in_app_status.h"

class InAppProductPrivate;
class InAppProduct : public QObject
{
    Q_OBJECT
        //Q_ENUMS(ProductType)
        //Q_PROPERTY(QString identifier READ identifier CONSTANT)
        ////Q_PROPERTY(ProductType productType READ productType CONSTANT)
        //Q_PROPERTY(QString price READ price CONSTANT)
        //Q_PROPERTY(QString title READ title CONSTANT)
        //Q_PROPERTY(QString description READ description CONSTANT)

public:
    enum ProductType
    {
        Consumable,
        Unlockable
    };

    ~InAppProduct();

    QString identifier() const;
    
    QString price() const;
    QString currency() const;

    QString title() const;
    QString description() const;
    QString uri() const;

    Q_INVOKABLE virtual void purchase() = 0;

signals:
    void    purchasingStatus(const InAppStatus&);

protected:
    explicit InAppProduct(  const QString &price, 
                            const QString &currency, 
                            const QString &title, 
                            const QString &description, 
                            const QString &uri, 
                            const QString &id, QObject *parent = Q_NULLPTR);

private:
    friend class InAppStoreRuntime;
    Q_DISABLE_COPY(InAppProduct)

    QSharedPointer<InAppProductPrivate> d;
};
