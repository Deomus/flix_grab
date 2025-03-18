#pragma once

#include <QtCore>
#include "status.h"


namespace vending {
           
    class Product : public QObject {
        friend class ProductPrivate;
        Q_OBJECT
    public:
        explicit Product(const QString& product_name, const QString& company_name, const QString& company_settings_path, const QString& company_vending_url, QObject* parent = nullptr);
        ~Product();

    public:
        const QString&      product_name() const;
        const QString&      product_id() const;
        const QString&      price() const;
        const QString&      currency() const;
        const QString&      product_url() const;
        const QString&      purchase_url() const;

        QString             formattedPrice() const;

        void                purchase();
        void                openProductPage(const QString& company_url);

        Status              status() const;

    public:
        void                downloadUpdate();
        void                installUpdate();
        void                downloadProductData();

        bool                isUpdateDownloaded() const;
        bool                isUpdateAvailable() const;

    signals:
        void                updateAvailable(bool available);

        void                downloadFinished();
        void                downloadProgress(float progress);
        void                productDataNetError();
        void                productDataError();
        void                updateError();
        
    private:
        Q_DISABLE_COPY(Product);
        QScopedPointer<ProductPrivate>           private_;
    };
}

