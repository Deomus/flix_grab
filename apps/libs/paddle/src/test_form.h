#pragma once

#include <QtWidgets>

#include "paddle.h"
#include "ui_activate_license.h"


class TestForm : public QDialog {
    Q_OBJECT
public:

    TestForm() {
        ui_.setupUi(this);
        //paddle_ = new paddle::Paddle("a7145dc81561fa0d638617949e2d8d8e", "19321", "516585");
        paddle_ = new paddle::Paddle("a7145dc81561fa0d638617949e2d8d8e", "19321", "514362");
        
        ui_.edtEmail->setText(paddle_->licenceEmail());
        ui_.edtActivate->setText(paddle_->licenceKey());
        

        if (paddle_->isActivated()) {
            ui_.Status->setText("Activated");
        }
        else {
            ui_.Status->setText("Deactivated");
        }

        QObject::connect(paddle_, &paddle::Paddle::licenceActive, [=](bool is) {
            if (is) {
                ui_.Status->setText("Activated");
            }
            else {
                ui_.Status->setText("Deactivated");
            }
        });

        QObject::connect(paddle_, &paddle::Paddle::licenceError, [=](const paddle::Status* status) {
            ui_.Message->setText(status->message());
        });


        QObject::connect(paddle_, &paddle::Paddle::productInfo, [=](const paddle::ProductInfo* info) {
            
            ui_.ProductName->setText(info->productName());
            QString productUrl = info->paymentUrl();
            QObject::connect(ui_.btnBuy, &QPushButton::clicked, [=]() {

                QDesktopServices::openUrl(productUrl);

            });
           
        });

        QObject::connect(ui_.btnActivate, &QPushButton::clicked, [=]() {
            ui_.Message->setText("");
            paddle_->activateLicence(ui_.edtEmail->text(), ui_.edtActivate->text());
           
        });

        QObject::connect(ui_.btnDeactivate, &QPushButton::clicked, [=]() {
            ui_.Message->setText("");
            paddle_->deactivateLicence();

        });
        paddle_->requestInfo();

    }




private:
    Ui::ActivateForm            ui_;
    paddle::Paddle*             paddle_;
};
