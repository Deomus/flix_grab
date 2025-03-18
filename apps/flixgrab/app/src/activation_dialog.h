#pragma once

#include <QtWidgets>

#include "ui_activation_dialog.h"

class ActivationDialog : public QDialog {
	Q_OBJECT

public:
    explicit ActivationDialog(const QString& formatted_price, QWidget *parent = nullptr
        , Qt::WindowFlags f = Qt::WindowFlags());

    void    enablePurchase(bool enabled = true);

signals:
    void    purchaseClicked();
    void    activateClicked(const QString& email, const QString& key);

    
public slots:
    void	setError(const QString& message);


private:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::ActivationDialog        ui_;
};



