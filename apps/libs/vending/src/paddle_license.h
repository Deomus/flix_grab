#pragma once

#include <QtCore>


namespace vending {

    class PaddleLicense  {
        
    public:
        explicit PaddleLicense(const QString& vendor_id, const QString& product_id);
        ~PaddleLicense();

    public:
        
        QString         email() const;
        QString         key() const;

        bool            isValid() const;

        void            clear();

    public:

        void            makeTestLicense();

    private:
        QVariantMap                         paddle_license_;
        QString                             product_id_;
        QString                             vendor_id_;
    };
}
