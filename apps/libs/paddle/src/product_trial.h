#pragma once

#include <QtCore>
#include <QtNetwork>

#include "paddle_status.h"
#include "paddle_credentials.h"

namespace paddle {
	
	class ProductTrial : public QObject
	{
		Q_OBJECT

	public:
		//Creates a new PaddleProduct instance for managing the product.  Credentials needed to communicate with the Paddle API for this product.  The data store to persist data back to.
		ProductTrial(Credentials* credentials);
		~ProductTrial();


	signals:
		void            trialChanged(bool is_activated);
	

	public:
		// The information we have on this product.

		bool            isTrial() const;


		void            start(qint64 seconds);
		void            stop();

		

	private:

		

		void            load();
		void            save() const;

		void            restartIntegrity();

	private:
		Credentials*						credentials_;
		//Issues the given request batch to Calq's API servers "endpoint" > The API endpoint to call
		QVariantMap                         trial_;
		QTimer                              timer_;

				
	};

}
