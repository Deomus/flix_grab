#ifndef MENU_P_H
#define MENU_P_H

#include <QMenu>
#include <QHash>
#include <QVector>
#include <QPointer>
#include <QAbstractScrollArea>

class Menu;
class MenuScrollArea;

class MenuPrivate
{
public:
    Menu *menu;
    MenuScrollArea *scrollArea;
    QRect scrollAreaRect;
    QVector<QRect> actionRects;
    int tabWidth;
    bool hasCheckableItems;
    int maxIconWidth;
    QHash<QAction *, QWidget *> widgetItems;
    bool mouseEntered;

    QAction *currentAction = nullptr;
    QPointer<Menu> activeMenu;
    int delayedPopupTimerId;
    QAction *delayedPopupAction;

    //used to walk up the popup list
    struct QMenuCaused {
        QPointer<QWidget> widget;
        QPointer<QAction> action;
    };
    QMenuCaused causedPopup;

    struct Header
    {
        QPointer<QAction> action;
        QPointer<QAction> separator;
        QPointer<QWidget> actionWidget;
        QRect actionRect;
        QRect separatorRect;
        QRect rect;
    };
    Header header;

    static Menu *mouseDown;

public:
    MenuPrivate(Menu *menu);
};

class MenuScrollArea : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit MenuScrollArea(MenuPrivate *d, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    MenuPrivate *d;
};


#endif // MENU_P_H
