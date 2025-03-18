#pragma once

#include <QtWidgets>
#include <QScreen>

#include "menu.h"

class TransMenu : public Menu {
    Q_OBJECT

public:
    TransMenu(QWidget* parent = nullptr) : Menu(parent) {
        setMaximumHeight(625);
    }

    QAction* addBack(const QString& text) {
        QAction* action = new QAction(text, this);

        Q_ASSERT(!headerAction()
            || back_actions_.contains(headerAction()) == false);

        QIcon leftIcon;
        leftIcon.addFile(":/img/arrow_left.png", QSize(10, 18), QIcon::Normal);
        leftIcon.addFile(":/img/arrow_left_hover.png", QSize(10, 18), QIcon::Active);

        action->setIcon(leftIcon);
        setHeader(action);
        back_actions_.push_back(action);
        
        return action;
    }

protected:
    void showEvent(QShowEvent *event) override {
        //qDebug() << "Show Menu:" << this;
             
        prev_menu_ = nullptr;
        if (menuAction()) {
            //Find previous menu of exists;
            for (QWidget* w : menuAction()->associatedWidgets()) {
                if (TransMenu* menu = qobject_cast<TransMenu*>(w)) {
                    if (menu->activeAction() == menuAction()) {
                        prev_menu_ = menu;

                        auto const rec = QGuiApplication::primaryScreen()->geometry();
                        auto const height = rec.height();
                        auto const width = rec.width();

                        QPoint pos_menu_lower_left_corner = prev_menu_->parentWidget()->mapToGlobal(QPoint(0, prev_menu_->parentWidget()->height()))
                            + QPoint(this->width(), this->height());

                        QPoint pos = prev_menu_->parentWidget()->mapToGlobal(QPoint(0, prev_menu_->parentWidget()->height()));
                        if (width < pos_menu_lower_left_corner.x()
                            && height > pos_menu_lower_left_corner.y()) {

                            pos = prev_menu_->parentWidget()->mapToGlobal(QPoint(prev_menu_->parentWidget()->width(), prev_menu_->parentWidget()->height()))
                                - QPoint(this->width(), 0);
                        }
                        else if (width < pos_menu_lower_left_corner.x()
                            && height < pos_menu_lower_left_corner.y()) {

                            QPoint pos_2 = prev_menu_->parentWidget()->mapToGlobal(QPoint(prev_menu_->parentWidget()->width(), 0))
                                - QPoint(this->width(), this->height());

                            if (pos_2.y() > 0)
                                pos = pos_2;
                            else if (width < pos_menu_lower_left_corner.x())
                                pos = prev_menu_->parentWidget()->mapToGlobal(QPoint(prev_menu_->parentWidget()->width(), prev_menu_->parentWidget()->height()))
                                - QPoint(this->width(), 0);
                        }
                        else if (width > pos_menu_lower_left_corner.x()
                            && height < pos_menu_lower_left_corner.y()) {

                            QPoint pos_2 = prev_menu_->parentWidget()->mapToGlobal(QPoint(0, 0)) - QPoint(0, this->height());
                            if (pos_2.y() > 0)
                                pos = pos_2;
                            else if (width < pos_menu_lower_left_corner.x())
                                pos = prev_menu_->parentWidget()->mapToGlobal(QPoint(prev_menu_->parentWidget()->width(), prev_menu_->parentWidget()->height()))
                                    - QPoint(this->width(), 0);
                        }

                        move(pos);

                        menu->hide();

                        //qDebug() << "Detected Previous Menu: " << prev_menu_;
                        break;
                    }
                }
            }
        }

        QMouseEvent move_event(QEvent::MouseMove, this->mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
        QApplication::sendEvent(this, &move_event);

       // if (QAction* at = this->actionAt(this->mapFromGlobal(QCursor::pos()))) {
       //     qDebug() << "Set Active Action " << this;

       //     QMouseEvent new_e(QEvent::MouseMove, this->mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
       //     QApplication::sendEvent(this, &new_e);

       //     bool a = this->isVisible();
       //     //this->setActiveAction(at);

       ///*     at->hovered();
       //     auto r = this->actionGeometry(at);
       //     this->update(this->actionGeometry(at));*/
       // }
       // else {
       //     qDebug() << "Active Action Not Set " << QCursor::pos();
       // }
        Menu::showEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) {
        //    qDebug() << "QtListWidget::mousePressEvent";
        if (event->button() == Qt::LeftButton)
            mouse_pressed_ = true;

        Menu::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) {
        if (!mouse_pressed_)   // disable click+drag
            Menu::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (mouse_pressed_)
            Menu::mouseReleaseEvent(event);

        if (event->button() == Qt::LeftButton)
            mouse_pressed_ = false;
    }

    void hideEvent(QHideEvent *event) override {
        //qDebug() << "Hide Menu:" << this;

        if (!activeAction()
            || !activeAction()->menu()) {
            //qDebug() << "Close menu" << this;

            bool back_action = back_actions_.contains(activeAction());
            Menu::hideEvent(event);

            if (prev_menu_) {
                if (back_action) {
                    //qDebug() << "Popup Previous Menu: " << prev_menu_;
                    //Clear action to prevent reshow this menu;
                    prev_menu_->setActiveAction(0);
                    //Prevent sending releaseMouseEvent;
                    QMetaObject::invokeMethod(prev_menu_, "show", Qt::QueuedConnection);
                }
                else {
                    //Finish hiding;
                    prev_menu_->dispatchHideEvent();
                }
            }
        }
        else {
            emit this->aboutToHide();
        }
    }

    void    actionEvent(QActionEvent *e) override {
        Menu::actionEvent(e);
        
        if (e->type() == QEvent::ActionRemoved)
            back_actions_.removeAll(e->action());
    }

    void    dispatchHideEvent() {
        //qDebug() << "Finish Menu: " << this;
        QHideEvent event;
        Menu::hideEvent(&event);

        if (prev_menu_)
            prev_menu_->dispatchHideEvent();
    }

private:
    TransMenu*              prev_menu_ = nullptr;
    QList<QAction*>         back_actions_;
    bool                    mouse_pressed_;
};
