#pragma once
#include <QtCore>
#include <QtWidgets>

//Provider Connect Model Data And Widget View;
class AbstractItem : public QObject {
    Q_OBJECT

public:
    AbstractItem(QObject* parent = 0)
        : QObject(parent)
        , is_selected_(false) {
    }

    virtual ~AbstractItem() {
        
    }

    //Allocate precached item;
    QWidget*            activate(QWidget* parent, const QPoint& pos) {
        QWidget* widget = getWidget(parent);
        widget->move(pos);
        return widget;
    }

    //Return Item To Pool;
    void                hide(QWidget* parent) {
        QWidget* widget = getWidget(parent);
        widget->hide();
    }

    //Return Item To Pool;
    void                deactivate(void) {
        releaseWidget();
    }

public slots:
    virtual void        remove() {
        emit removed();
        deleteLater();
    }

signals:
    //Signal Removed sended to  All lists;
    void    removed();


protected:
    //C-tor,D-tor Methods;
    virtual QWidget*                getWidget(QWidget* pContent) = 0;
    virtual void                    releaseWidget(void) = 0;

    virtual void                    showSelection(bool is_selected = true) = 0;

public:

    void                            setSelected(bool selected = false)
    {
        if (is_selected_ != selected)
        {
            is_selected_ = selected;
            showSelection(is_selected_);
        }
    }

    bool                            isSelected(void) const { return is_selected_; }

private:
    bool                        is_selected_;
};
