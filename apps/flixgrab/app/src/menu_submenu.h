#pragma once


#include <QtWidgets>

#include "menu_widget.h"

#include "ui_menu_action.h"


//TODO: remove
#include "trans_menu.h"

class MenuSubmenu : public MenuWidget {
    Q_OBJECT

public:

    MenuSubmenu(QWidget* parent = nullptr) : MenuWidget(parent) {
        ui_.setupUi(this);

        QString  menuStyle(
            /*"QMenu {"
                "menu-scrollable: 1;"
                "icon-size: 50px 50px;"
            "}"

            "QMenu::QScrollBar:vertical {"
                "border: none;background-color: #39A5F0;width: 4px;margin: 0 0 0 0;"
            "}"
            "QMenu::QScrollBar:vertical:hover {"
                "border: none;background-color: #39A5F0;width: 4px;margin: 0 0 0 0;"
            "}"
            "QMenu::QScrollBar::handle:vertical {"
                "border-radius: 1px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #3D404F; min-height: 32px;"
            "}"
            "QMenu::QScrollBar::handle:vertical:hover {"
                "border-radius: 1px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-height: 32px;"
            "}"
            "QMenu::QScrollBar::add-line:vertical {"
                "width: 0px; height: 0px;"
            "}"
            "QMenu::QScrollBar::sub-line:vertical {"
                "width: 0px; height: 0px;"
            "}"
            "QMenu::QScrollBar::add-page:vertical {"
                "background-color: transparent;"
            "}"
            "QMenu::QScrollBar::sub-page:vertical {"
                "background-color: transparent;"
            "}"*/
            "QMenu {"
            "icon-size: 17px 32px;"
            "padding: 5 18 5 18;"
            "font-family: \"Tahoma\";"
            "font-size: 9pt; "
            "}"
            "QMenu::icon{"
            "padding-left: 20;"
            "}"
            "QMenu::item{"
            "padding: 7 6 6 5;"
            "background:transparent;"
            "color: #F2F3F3;"
            "}"

            "QMenu::item:hover{"
            "background-color: #414755;"
            "}"
            "QMenu::item:selected{"
            "background-color: #414755;"
            "}"
            "QMenu::separator{"
            "background-color: #414755;"
            "height: 1px;"
            "}"
            "QMenu::separator:hover{"
            "background-color: #414755;"
            "}"
        );

        submenu_ = new TransMenu(this);

        group_ = new QActionGroup(submenu_);

        requestAction()->setMenu(submenu_);

        back_ = submenu_->addBack(name());
        //submenu_->addSeparator();
        submenu_->setStyleSheet(menuStyle);

        QObject::connect(group_, &QActionGroup::triggered, this, [=](QAction* action) {
            emit subactionTriggered(action);

            if (group_->isExclusive())
                submenu_->hide();
            
        });

        QObject::connect(submenu_, &QMenu::aboutToShow, this, &MenuSubmenu::submenuAboutToShow);
        QObject::connect(submenu_, &QMenu::aboutToHide, this, &MenuSubmenu::submenuAboutToHide);
    }
    MenuSubmenu(const QString& name, const QString& value, const bool& icon_need = TRUE, QWidget* parent = nullptr) : MenuSubmenu(parent) {
        this->setName(name);
        this->setValue(value);
        this->setIconNeed(icon_need);
    }

public:
    void                    translate() { ui_.retranslateUi(this); }
    QString                 name() const { return ui_.lblName->text(); }
    QString                 value() const { return ui_.lblValue->text(); }

   /* QActionGroup*           subGroup() const {return group_;}
 
    TransMenu*              subMenu() const {return submenu_;}*/

    void                    createSubmenu(const QList<QAction*>& subactions, bool exclusive = true) {
        ui_.cbxSelected->setChecked(false);
        hideCheckBox();

        clearSubmenu();

        /*submenu_ = new TransMenu(this);
        requestAction()->setMenu(submenu_);

        group_ = new QActionGroup(submenu_);*/
        group_->setExclusive(exclusive);
             
		for (int i = 0; i < subactions.count(); i++) {
			QAction* sa = subactions[i];
            //sa->setParent(submenu_);
            submenu_->addAction(sa);
            group_->addAction(sa);
            //sa->setCheckable(true);

			if (i % 10 == 0) qApp->processEvents();
        }
    }

    void                    setSignalButton(TransMenu* menu) {
        QObject::connect(ui_.pbnIcon, &QPushButton::clicked, this, [=] {
            menu->click_button(action());
        });
    }

    void clearSubmenu() {

        auto acts = group_->actions();

        for (auto a : acts) {
            group_->removeAction(a);
            submenu_->removeAction(a);
        }

        //if (submenu_) {
        //    //submenu_->removeEventFilter(this);
        //    submenu_->deleteLater();
        //    group_->deleteLater();

        //}

        //if (back_)
        //    back_->deleteLater();

        //group_ = nullptr;
        //submenu_ = nullptr;
        //back_ = nullptr;
    }

    bool                isExclusive() const {
        return group_->isExclusive();
    }

    QList<QAction*>     subactions() const {
        return group_->actions();
    }

    QAction*            checkedSubaction() const {
        return group_->checkedAction();
    }

    //void triggerAction(int index) {
    //    if (group_ && group_->actions().size() > index) {
    //        group_->actions().at(index)->trigger();
    //    }
    //}

    //void selectAction(int index) {
    //    if (group_ && group_->actions().size() > index) {
    //        auto action = group_->actions().at(index);
    //        this->setValue(action->text());
    //        action->setChecked(true);

    //    }
    //}

    QAction* actionByData(const QVariant& data) {
        for (auto a : group_->actions()) {
            if (a->data() == data)
                return a;
        }
        return nullptr;
    }

    static int getNormalizeNameWidth(const QList<MenuSubmenu*>& widgets) {
        int max_width = 0;

        for (auto w : widgets)
            max_width = qMax(max_width, w->ui_.lblName->width());

        return max_width;
    }

    static void normalizeNameWidth(const QList<MenuSubmenu*>& widgets, int max_width) {
        for (auto w : widgets) {
            w->ui_.lblName->setMinimumWidth(max_width);
            w->ui_.lblName->setMaximumWidth(max_width);
        }
    }

    /*bool triggerActionByData(const QVariant& data) {
        if (group_) {
            for (auto a : group_->actions()) {
                if (a->data() == data) {
                    a->trigger();
                    return true;
                }
            }
        }
        return false;
    }

    bool selectActionByData(const QVariant& data) {
        if (group_) {
            for (auto a : group_->actions()) {
                if (a->data() == data) {
                    this->setValue(a->text());
                    a->setChecked(true);
                    return true;
                }
            }
        }
        return false;
    }*/

    void                    showCheckBox() {
        ui_.cbxSelected->show();
        ui_.lblName->setStyleSheet("border:none; padding: 6 2 6 0; color: #F2F3F3; font: normal 9pt \"Tahoma\";");
    }

    QCheckBox* getCheckBoxSelected() {
        return ui_.cbxSelected;
    }

    void                    hideCheckBox() {
        ui_.cbxSelected->hide();
        ui_.lblName->setStyleSheet("border:none; padding: 6 2 6 11; color: #F2F3F3; font: normal 9pt \"Tahoma\";");
    }

signals:
    void                    subactionTriggered(QAction*);
    void                    submenuAboutToShow();
    void                    submenuAboutToHide();

public slots:
    void                    setName(QString name) {
        ui_.lblName->setText(name);
        this->adjustSize();
        updateAction();
    }

    void                    setValue(QString value) {
        ui_.lblValue->setText(value);
        this->adjustSize();
        updateAction();
    }

    void                    setIconNeed(bool icon_need) {
        if (icon_need)
            ui_.pbnIcon->show();
        else
            ui_.pbnIcon->hide();
        this->adjustSize();
        updateAction();
    }

private slots:
   

    void                    updateAction() {
        if (QMenu* menu = qobject_cast<QMenu*>(this->parentWidget())) {
            QActionEvent e(QEvent::ActionChanged, action());
            QApplication::sendEvent(menu, &e);
        }
    }
       
protected:
    void                    showEvent(QShowEvent* e) override {
        if (auto action = group_->checkedAction())
            this->setValue(action->text());

        back_->setText(name());
        MenuWidget::showEvent(e);
    }

    //void                    hideEvent(QHideEvent* e) override {
    //    //Q_UNUSED(e);
    //    //Update value;
    //    qDebug() << "Hide Submenu";
    //    MenuWidget::hideEvent(e);
    //}

    //virtual QWidgetAction*  createAction() {
    //    QWidgetAction* wa = MenuWidget::createAction();
    //    //QObject::connect(wa, &QAction::triggered, this, &MenuSubmenu::popupSubmenu);
    //    return wa;
    //}

    virtual void            enterHover() override {
        setHovered(true);
    }

    virtual void            leaveHover() override {
        setHovered(false);
    }

private:
   
    
    void                    setHovered(bool hovered /*= true*/) {
        ui_.cbxSelected->setProperty("hovered", hovered);
        ui_.cbxSelected->style()->polish(ui_.cbxSelected);

        ui_.lblName->setProperty("hovered", hovered);
        ui_.lblName->style()->polish(ui_.lblName);
        
        ui_.lblValue->setProperty("hovered", hovered);
        ui_.lblValue->style()->polish(ui_.lblValue);

        ui_.pbnIcon->setProperty("hovered", hovered);
        ui_.pbnIcon->style()->polish(ui_.pbnIcon);

        update();
    }

private:
    Ui::MenuAction                  ui_;

private:
    
    TransMenu*                      submenu_ = nullptr;
    QAction*                        back_ = nullptr;
    QActionGroup*                   group_ = nullptr;

};

