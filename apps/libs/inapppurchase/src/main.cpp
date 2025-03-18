
#include <QtWidgets>


#include "in_app_store.h"

//////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    //StoreBridge store;

    QApplication a(argc, argv);

    QDialog dialog;

    auto w = dialog.winId();

    dialog.show();
    w = dialog.winId();
   
    InAppStore  app_store(&dialog);

    app_store.initializeUi(&dialog);
    InAppProduct* product = app_store.addon_product("NetflixSubscription");

    auto license = app_store.addon_licenses();
    
    app_store.downloadUpdates();

   // product->purchase();
    //app_store.requestReview();
    
    //bool is_active = app_store.isLicenseActive();

    //app_store.purchaseLicense();
    //app_store.requestReview();
    //store.activate((HWND)dialog.winId());


   /* QInAppStore     store;

        
    store.registerProduct(QInAppProduct::Unlockable, "app");
    store.restorePurchases();*/
  
  //  return 0;
    return a.exec();
}
