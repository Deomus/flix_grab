//paddle_product.h
#pragma once

#include <QtCore>
#include <QtNetwork>

#include "paddle_status.h"

namespace paddle {
    class Credentials;
    class PaddleApi;

    class ProductLicence : public QObject
    {
        Q_OBJECT

    public:
        //Creates a new PaddleProduct instance for managing the product.  Credentials needed to communicate with the Paddle API for this product.  The data store to persist data back to.
        ProductLicence(Credentials* credentials);
        ~ProductLicence();


    signals:
        void            activated(bool is_activated);
        void            error(const Status*);

    public:
        // The information we have on this product.

        bool            isActivated() const;
        const Status&   status() const { return status_; }


        void            activate(QString email, QString license_key);

        //Calls the API to deactive this product's licence.
        void            deactivate();
        void            verify();

        QString         email() const;

        QString         license_key() const;


    private:

		void			blockActivation(int activation_index = 0);
		//void			unblockActivation(int activation_index);

        void            restartIntegrity();


        void            load();
        void            save();



    private:
        Credentials*                        credentials_;
        //Issues the given request batch to Calq's API servers "endpoint" > The API endpoint to call
        QVariantMap                         licence_;
        QTimer                              timer_;
        Status                              status_;

        PaddleApi*                          api_;
    };

}