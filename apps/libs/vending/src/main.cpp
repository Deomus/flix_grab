
#include <QtWidgets>

#include "vending/product.h"
#include "vending/license.h"

//#include "test_form.h"

//#define PADDLE_VENDOR_ID                "19321"
//#define PADDLE_PRODUCT_ID               "514362"

#define PADDLE_VENDOR_ID                "1"
#define PADDLE_PRODUCT_ID               "1"

#define FLIXGRAB_PRODUCT_ID             VENDING_PRODUCT_ID
//#define FLIXGRAB_PUBLIC_KEY             "ABYBDQELSESqCyka15VaiQ6SAIYxic4IPcShJv6oRpSnuKxBgQA="
#define FLIXGRAB_PUBLIC_KEY             VENDING_PUBLIC_KEY

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    vending::Product  product(FLIXGRAB_PRODUCT_ID);
    vending::License  license(FLIXGRAB_PUBLIC_KEY);


    qDebug() << "Product price:" << product.formattedPrice();

   // product.purchase();


    license.clear();

    QEventLoop loop;
    QObject::connect(&license, &vending::License::finished, [&]() {
    

        QEventLoopLocker  locker(&loop);
        qDebug() << "Request Finished:" << license.isValid() << " Expire Date:" << license.expirationDate().toString(Qt::SystemLocaleShortDate);
    });

    QObject::connect(&license, &vending::License::changed, [&]() {
                
        qDebug() << "License Changed:" << license.isValid() << " Expire Date:" << license.expirationDate().toString(Qt::SystemLocaleShortDate);
    });

    license.clear();

    qDebug() << "License Valid:" << license.isValid();

	//QString lic = license.generateKey("test", VENDING_PRODUCT_ID, VENDING_PRIVATE_KEY);
	//license.activate("test", lic);
	license.activate("", "NPWU4A-AAWDR2-DPARKL-8Z5UM6-KNWSMK");

			//license.activate("freegrabapp@gmail.com", "NPWU4A-AAWDR2-DPARKL-8Z5UM6-KNWSMK");
	//license.activate("test", "NPWU4A-AAWDR2-DPARKL-8Z5UM6-KNWSMK");
	//license.activate("test", "NPWU4A-AAWDR2-DPARKL-8Z5UM6-KNWSMK");
	
	//license.activate("freegrabapp@gmail.com", "NPWU4A-AAWDR2-DPARKL-8Z5UM6-KNWSMK");
	//vending::License::makeTestPaddleLicense(PADDLE_VENDOR_ID, PADDLE_PRODUCT_ID);

    //license.migrateFromPaddle(PADDLE_VENDOR_ID, PADDLE_PRODUCT_ID);
    //license.reactivate();
    //license.activate("test@email");
    //license.reactivate();
   // license.reactivate();
  

    loop.exec();
   /* QElapsedTimer timer;
    for (int i = 0; i < 3; ++i) {
        timer.restart();
        license.activate("test@email");
        qDebug() << "Elapsed time:" << timer.elapsed() << "ms";
    }*/
    //license.clear();

    qDebug() << "License Valid:" << license.isValid();
   
    

    //TestForm w;

    //w.show();

    return 0;
    return a.exec();
}
