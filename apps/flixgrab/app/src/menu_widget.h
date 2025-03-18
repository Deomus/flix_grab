#pragma once
#include <QtWidgets>


class MenuWidget : public QFrame {
    Q_OBJECT

public:

    MenuWidget(QWidget* parent = nullptr) : QFrame(parent) {
        
    }


public:
    QAction*                action() { 
        return action_; 
    }

    QAction*                requestAction() {
        if (!action_)
            action_ = createAction();
        return action_;
    }

    void                    setMouseFiltering(bool filtering) { mouse_filtering_ = filtering; }


 signals:
    void                    aboutToShow();
    void                    aboutToHide();
  
protected:
    virtual QWidgetAction*  createAction() {
        QWidgetAction* wa = new QWidgetAction(this);

        wa->setDefaultWidget(this);
        return wa;

    }

    virtual void             enterHover() {

    }

    virtual void             leaveHover() {

    }

    virtual bool             event(QEvent * e) override {

        bool result = QFrame::event(e);
        if (this->isEnabled())
            switch (e->type()) {
            case QEvent::HoverEnter:
                //Remove Hover on over Actions in menu;
                if (QMenu* menu = qobject_cast<QMenu*>(this->parentWidget())) {
                    menu->setActiveAction(0);
                }
                enterHover();
                break;

            case QEvent::HoverLeave:
                leaveHover();
                break;

            case QEvent::MouseButtonRelease:
            case QEvent::MouseButtonPress:

                if (mouse_filtering_) {
                    e->accept();
                    result = true;
                }
               
                break;
                //case QEvent::MouseMove:
                //    //e->ignore();

                //    result = true;
                //    break;

            default:
                break;
            }
        return result;
    }

    void                    showEvent(QShowEvent* event) override {
        leaveHover();
        emit aboutToShow();
        QFrame::showEvent(event);
    }

    void hideEvent(QHideEvent *event) override {

        emit aboutToHide();
        //FIX: remove hover after hide;
        QEvent leaveEvent(QEvent::Leave);
        QCoreApplication::sendEvent(this, &leaveEvent);
        QFrame::hideEvent(event);
    }

private:
    QWidgetAction*                  action_ = nullptr;
    bool                            mouse_filtering_ = false;
};

