#include "submenu_widget.h"


SubmenuWidget::SubmenuWidget(QWidget* parent /*= nullptr*/) : QFrame(parent) {
    ui_.setupUi(this);
    hideCheckBox();
    this->setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);

    action_ = new QWidgetAction(this);

    action_->setDefaultWidget(this);

    QObject::connect(action_, &QAction::triggered, this, &SubmenuWidget::triggered);
    QObject::connect(action_, &QAction::triggered, this, &SubmenuWidget::popupSubmenu);
    
}

SubmenuWidget::SubmenuWidget(const QString& name, const QString& value, const bool& icon_need, QWidget* parent /*= nullptr*/) : SubmenuWidget(parent) {
    hideCheckBox();
    this->setName(name); 
    this->setValue(value);
    this->setIconNeed(icon_need);
}

void SubmenuWidget::createSubmenu(const QList<QAction*>& subactions) {
    clearSubmenu();

    submenu_ = new QMenu(this);
    QString  menuStyle(
        "QMenu{border: none;}"
        "QMenu::item{"
        "background-color: #2A2E3A;"
        "color: #F2F3F3;"
        "padding: 6 18 6 18;"
        "font-family: \"Tahoma\";"
        "font-size: 9pt;"
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

    submenu_->setStyleSheet(menuStyle);
    group_ = new QActionGroup(submenu_);

    back_ = submenu_->addAction(name());
    submenu_->addSeparator();
    QObject::connect(back_, &QAction::triggered, this, &SubmenuWidget::popupMenu);

    QObject::connect(group_, &QActionGroup::triggered, this, &SubmenuWidget::submenuTriggered);
    //QObject::connect(group_, &QActionGroup::triggered, this, &SubmenuWidget::onActionTriggered);

    //group_->clear();
    for (auto sa : subactions) {
        sa->setParent(submenu_);
        submenu_->addAction(sa);
        group_->addAction(sa);
        sa->setCheckable(true);
    }
        
   /* QObject::connect(submenu_, &QMenu::aboutToShow, [=]() {
        
        QHoverEvent* event = new QHoverEvent(QEvent::Enter, submenu_->mapFromGlobal(QCursor::pos()), QPoint(-1, -1));
        QCoreApplication::postEvent(submenu_, event);
    });*/
}

void SubmenuWidget::clearSubmenu() {
    if (submenu_) {
        //submenu_->removeEventFilter(this);
        submenu_->deleteLater();
        group_->deleteLater();
    }
    group_ = nullptr;
    submenu_ = nullptr;
}

void SubmenuWidget::triggerAction(int index) {
    if (group_ && group_->actions().size() > index) {
        group_->actions().at(index)->trigger();
    }
}

void SubmenuWidget::selectAction(int index) {
    if (group_ && group_->actions().size() > index) {
        auto action = group_->actions().at(index);
        this->setValue(action->text());
        action->setChecked(true);

    }
}

bool SubmenuWidget::triggerActionByData(const QVariant& data) {
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

bool SubmenuWidget::selectActionByData(const QVariant& data) {
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
}

QList<QAction*> SubmenuWidget::submenuActions() const {
    if (group_) {
        return group_->actions();
    }
    return QList<QAction*>();
}

void SubmenuWidget::setName(QString name) {
    ui_.lblName->setText(name);
    this->adjustSize();
    updateAction();
    //this->updateGeometry();
}

void SubmenuWidget::setValue(QString value) {
    ui_.lblValue->setText(value);
    this->adjustSize();
    updateAction();
    //this->updateGeometry();
}

void SubmenuWidget::setIconNeed(bool icon_need) {
    if (icon_need) {
        ui_.pbnIcon->show();
        ui_.pbnIcon->setProperty("state", "empty");
        ui_.pbnIcon->style()->polish(ui_.pbnIcon);
        ui_.pbnIcon->update();
    }
    else {
        ui_.pbnIcon->hide();
    }

    this->adjustSize();
    updateAction();
}


void SubmenuWidget::popupSubmenu() {
    if (submenu_) {
        if (QMenu* parent_menu = qobject_cast<QMenu*>(this->parentWidget())) {
            QMetaObject::invokeMethod(this, "hoverActionFromMenu", Qt::QueuedConnection, Q_ARG(QMenu*, submenu_));
            submenu_->exec(parent_menu->pos());
        }
    }
}

void SubmenuWidget::popupMenu() {

    if (submenu_) {
        if (QMenu* parent_menu = qobject_cast<QMenu*>(this->parentWidget())) {
            QMetaObject::invokeMethod(this, "hoverActionFromMenu", Qt::QueuedConnection, Q_ARG(QMenu*, parent_menu));
            parent_menu->exec(submenu_->pos());
        }
    }
    
    //switchMenu(submenu_, qobject_cast<QMenu*>(this->parentWidget()));
}

void SubmenuWidget::updateAction() {
    if (QMenu* menu = qobject_cast<QMenu*>(this->parentWidget())) {
        QActionEvent e(QEvent::ActionChanged, action_);
        QApplication::sendEvent(menu, &e);
    }
}

void SubmenuWidget::hoverActionFromMenu(QMenu* menu) {
    if (QAction* at = menu->actionAt(menu->mapFromGlobal(QCursor::pos()))) {
        menu->setActiveAction(at);
    }
}

void SubmenuWidget::showEvent(QShowEvent* e) {
    Q_UNUSED(e)
    //Update value;
    if (group_) {
        if (auto action = group_->checkedAction())
            this->setValue(action->text());
    }
}

void SubmenuWidget::hideEvent(QHideEvent *event) {
    //FIX: remove hover after hide;
    QEvent leaveEvent(QEvent::Leave);
    QCoreApplication::sendEvent(this, &leaveEvent);
    setHovered(false);
    QFrame::hideEvent(event);
}

bool SubmenuWidget::event(QEvent * e) {
    if (this->isEnabled())
        switch (e->type()) {
            case QEvent::HoverEnter:
                setHovered(true);
                break;
            case QEvent::HoverLeave:
                setHovered(false);
                break;
            default:
                break;
        }

    return QFrame::event(e);
}

void SubmenuWidget::switchMenu(QMenu* menu_old, QMenu* menu_new) {
    if (menu_old && menu_new) {
        menu_new->exec(menu_old->pos());
      
    }
}

void SubmenuWidget::setHovered(bool hovered /*= true*/) {
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

int SubmenuWidget::getNormalizeNameWidth(const QList<SubmenuWidget*>& widgets) {
    int max_width = 0;

    for (auto w : widgets)
        max_width = qMax(max_width, w->ui_.lblName->width());
    
    return max_width;
}

void SubmenuWidget::normalizeNameWidth(const QList<SubmenuWidget*>& widgets, int min_width) {
    for (auto w : widgets)
        w->ui_.lblName->setMinimumWidth(min_width);
}

