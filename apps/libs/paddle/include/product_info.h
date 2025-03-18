#pragma once
#include <QtCore>

#include "paddle_status.h"

namespace paddle {
    class Credentials;
    class PaddleApi;

    class ProductInfo : public QObject {
        friend class Paddle;
        Q_OBJECT
        
    public:
       
        QString         productName();
        QString         developerName();
        //Currency Iso Code;
        QString         currency();
               

        QString         toPriceString();

        double          price();
        QString         imageUrl();
        QString         paymentUrl();

		bool			isTrialSupported();
		QString         trialText();
		double			trialDuration();

        const Status&   status() const { return status_; }

	private:
		void			load();
		void			save() const;
    private:
        void            syncIfExpired();

    private:
        explicit ProductInfo(Credentials* credentials);
        ~ProductInfo() {}

    private:
		Credentials *			credentials_;
        QVariantMap             info_;
        paddle::Status          status_;
		PaddleApi*              api_;
    };

}

