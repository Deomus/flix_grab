#pragma once

#include <QtCore>
#include "in_app_product.h"

class ProductBridge;

class InAppProductPrivate
{
    
public:
    ~InAppProductPrivate();

    
    QString         title;
    QString         description;
    QString         price;
    QString         currency;
    QString         url;


protected:
    explicit InAppProductPrivate(ProductBridge*);

private:
    
    Q_DISABLE_COPY(InAppProductPrivate)

    QSharedPointer<ProductBridge> d;
};
