#pragma once

#include <QtCore>

#include "paddle_keys.h"

namespace paddle {
    class Credentials : public QObject {
    public:
        explicit Credentials(QString api_key, QString vendor_id, QString product_id, QObject* parent = nullptr)
            : QObject(parent)
			, api_key_(api_key)
            , vendor_id_(vendor_id)
            , product_id_(product_id) { }

        QVariantMap     allCredentials() const {
            return {
                { ApiKey, api_key_ },
                { VendorId, vendor_id_ },
                { ProductId, product_id_ } };
        }

        QVariantMap     licenseCredentials() const {
            return {
                { ApiKey, api_key_ },
                { ProductId, product_id_ } };
        }


        const QString&  apiKey() const { return api_key_; }
        const QString&  vendorId() const { return vendor_id_; }
        const QString&  productId() const { return product_id_; }


    private:
        //The credentials needed to talk to the API server.

        QString                 api_key_;
        QString                 vendor_id_;
        QString                 product_id_;
    };

}

