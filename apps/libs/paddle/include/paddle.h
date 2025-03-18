#pragma once

#include <QtCore>

#include "paddle_status.h"
#include "product_info.h"


namespace paddle {
    class ProductLicence;
	class ProductTrial;
    class PaddleApi;

    class Paddle : public QObject {
        Q_OBJECT
    public:
        explicit Paddle(QString api_key, QString vendor_id, QString product_id, QObject* parent = nullptr);
		~Paddle();

    public:
        void            activateLicence(QString email, QString license_key);
        void            deactivateLicence();
        
        bool            isActivated() const;

        QString         licenceEmail() const;
        QString         licenceKey() const;

	public:
		void		    startTrial(qint64 seconds);
		void		    stopTrial();
		bool		    isTrial() const;

    public:
        ProductInfo *   productInfo() { return info_; }
    
    signals:
		void		    trialChanged(bool is_active);
        void            licenceChanged(bool is_active);
        void            licenceError(const Status*);


    private:
        Credentials*                    credentials_;
        ProductInfo*                    info_;
        ProductLicence*                 licence_;
		ProductTrial*					trial_;
        PaddleApi*                      api_;
    };
}