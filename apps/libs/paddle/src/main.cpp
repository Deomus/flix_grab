
#include <QtWidgets>

#include "paddle.h"
#include "test_form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //16
    //https://vendors.paddle.com/fulfillment/framework/516585/licences/manage/2345423
    //9099FCE9-8A13FD63-E9A8DFB4-DBAC2796-1CFB1C20


//https://vendors.paddle.com/fulfillment/framework/516585/licences/manage/2345368
//8FAF4CDA-B91DBC3A-D7481666-2DD3B5CD-223D3C28


    
    //paddle::Paddle      p("a7145dc81561fa0d638617949e2d8d8e", "19321", "516585");
    //
    ////p.activateLicense("freegrabapp@gmail.com", "6C562445-58DCA8B2-525EB8BD-133BC32C-59150B2F");
    //p.activateLicense("freegrabapp@gmail.com", "6C562445-58DCA8B2-525EB8BD-133BC32C-59150B2");

   
    //QObject::connect(&p, &paddle::Paddle::productInfo, [=](const paddle::ProductInfo* info) {
    //    qDebug() << "Product Name: " << info->productName();

    //});

    //QObject::connect(&p, &paddle::Paddle::licenseActive, [=](bool is_active) {
    //    qDebug() << "License Active: " << is_active;

    //});

    //QObject::connect(&p, &paddle::Paddle::licenseError, [=](const paddle::Status* status) {
    //    qDebug() << "License Error: " << status->code() << " Message: " << status->message();

    //});
    ////prod.activateLicense("freegrabapp@gmail.co", "6C562445-58DCA8B2-525EB8BD-133BC32C-59150B2");
    //p.requestInfo();

    TestForm w;

    w.show();

  //  return 0;
    return a.exec();
}
