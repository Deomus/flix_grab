#pragma once

#include <QtWidgets>

#include "ui_paddle_activation.h"

//Forward declaration;
namespace paddle {
    class Paddle;
	class ProductInfo;
	struct Status;
}

class PaddleActivationDialog : public QDialog {
	Q_OBJECT

public:
    explicit PaddleActivationDialog(paddle::Paddle* engine, QWidget *parent = nullptr
        , Qt::WindowFlags f = Qt::WindowFlags());


private:
    void    createDialog();
    void	createPurchase();

private slots:
	   
	
    void	onLicenceError(const paddle::Status* status);
	void	onlicenceChanged(bool);

    

private:
    Ui::PaddleActivation        ui_;
    paddle::Paddle*             paddle_;
        
};
