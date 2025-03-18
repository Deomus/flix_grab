#include "activation_dialog.h"

ActivationDialog::ActivationDialog(
    const QString& formatted_price, 
    QWidget *parent /*= nullptr*/,
    Qt::WindowFlags f /*= Qt::WindowFlags()*/) 
        : QDialog(parent, f) {
    ui_.setupUi(this);
    this->setWindowTitle(QString("%1 %2").arg(QApplication::applicationName()).arg(tr("activation")));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui_.lblError->setVisible(false);

    //Update 
    //QString purchase_text = QString("%1 %2").arg(ui_.btnPurchase->text()).arg(formatted_price);
    QString purchase_text = tr("Buy now");
    ui_.btnPurchase->setText(purchase_text);

    QObject::connect(ui_.btnPurchase, &QPushButton::clicked, this, &ActivationDialog::purchaseClicked);

    ui_.edEmail->installEventFilter(this);
    ui_.edKey->installEventFilter(this);
    QObject::connect(ui_.btnActivate, &QPushButton::clicked, [=]() {
        this->setEnabled(false);
        ui_.lblCaption->setText(tr("Activating..."));
        ui_.lblError->setVisible(false);

        emit activateClicked(ui_.edEmail->text(), ui_.edKey->text());
    });
}

bool ActivationDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        QLineEdit *ed = static_cast<QLineEdit *> (obj);
        if (!ed->text().isEmpty())
            ed->setText(ed->text().replace(" ", ""));
    }

    return false; // We return false to ignore the event and allow the child to recieve the event normally
}

void ActivationDialog::enablePurchase(bool enabled /*= true*/) {
    ui_.btnPurchase->setVisible(enabled);

}

void ActivationDialog::setError(const QString& message) {
    QString msg = message;
    if (!msg.isEmpty())
        msg.replace(0, 1, message[0].toUpper());

    ui_.lblCaption->setText(tr("Get Premium"));

    ui_.lblError->setVisible(true);
    ui_.lblError->setText(msg);
	this->setEnabled(true);
}
