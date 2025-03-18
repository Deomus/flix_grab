#pragma once

#include <QtWidgets>

#include "menu_widget.h"
#include "ui_menu_checkbox.h"


class MenuCheckbox : public MenuWidget {
    Q_OBJECT

public:

    MenuCheckbox(QWidget* parent = nullptr) : MenuWidget(parent) {
        ui_.setupUi(this);

        setAttribute(Qt::WA_Hover);

        /*QObject::connect(action_, &QAction::triggered, this, [this](){

        });*/
        //QObject::connect(action_, &QAction::triggered, this, &SubmenuWidget::popupSubmenu);

    }

public:
    void                    translate() { ui_.retranslateUi(this); }
    void                    setName(const QString& name) { 
        ui_.cbValue->setText(name); 

        if (action())
            action()->setText(name);
    }

    const QString&          name() const { return ui_.cbValue->text(); }

    void                    setChecked(bool checked) {
        ui_.cbValue->setChecked(checked);
    }

public:
    bool checked_ = false;

protected:
    virtual QWidgetAction*  createAction() {
        QWidgetAction* wa = MenuWidget::createAction();
        wa->setCheckable(true);

        QObject::connect(ui_.cbValue, &QCheckBox::clicked, this, [=](bool state) {
            Q_UNUSED(state);
            qDebug() << "Checkbox Clicked " << state;
            //wa->setChecked(state == Qt::Checked);
            ui_.cbValue->setChecked(wa->isChecked());
            wa->trigger();
            checked_ = !checked_;
        }, Qt::DirectConnection);

        QObject::connect(wa, &QAction::toggled, this, [=](bool checked) {
            if (checked != ui_.cbValue->isChecked())
                ui_.cbValue->setChecked(wa->isChecked());

        }, Qt::DirectConnection);
              
        wa->setText(ui_.cbValue->text());
        return wa;
    }

private:
    Ui::MenuCheckbox                ui_;
};

