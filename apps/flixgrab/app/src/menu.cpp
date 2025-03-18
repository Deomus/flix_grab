#include "menu.h"
#include "menu_p.h"

#include <qevent.h>

#include <QScreen>
#include <QtWidgets>
#include <QtWidgets/private/qmenu_p.h>
#include <QWidgetAction>
#include <QScrollBar>
#include <QStyle>
#include <QStyleOption>
#include <QAccessible>
#include <QTimer>
#include <QApplication>
#include <QPainter>
#include <QMenuBar>


Menu* MenuPrivate::mouseDown = nullptr;

MenuPrivate::MenuPrivate(Menu *menu) :
    menu(menu),
    scrollArea(new MenuScrollArea(this, menu)),
    tabWidth(0),
    hasCheckableItems(false),
    maxIconWidth(0),
    mouseEntered(false),
    currentAction(nullptr),
    delayedPopupTimerId(0),
    delayedPopupAction(nullptr) {
}

MenuScrollArea::MenuScrollArea(MenuPrivate *d, QWidget *parent) : QAbstractScrollArea(parent), d(d) {
    setFrameShape(QAbstractScrollArea::NoFrame);
}

void MenuScrollArea::paintEvent(QPaintEvent *e) {
    QStyle *style = d->menu->style();
    QPainter p(viewport());
    QRegion emptyArea = QRegion(rect());

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(d->menu);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
#ifdef QT6
	menuOpt.reservedShortcutWidth = 0;
#else // QT5
    menuOpt.tabWidth = 0;
#endif

    style->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &p, d->menu);

    //draw the items that need updating..
    const auto actions = d->menu->actions();
    for (int i = 0; i < actions.count(); ++i) {
        QAction *action = actions.at(i);
        const QRect &actionRect = d->actionRects.at(i);
        if (!e->rect().intersects(actionRect)
            || d->widgetItems.value(action))
            continue;
        //set the clip region to be extra safe (and adjust for the scrollers)
        emptyArea -= QRegion(actionRect);

        QStyleOptionMenuItem opt;
        d->menu->initStyleOption(&opt, action);
        opt.rect = actionRect;
        style->drawControl(QStyle::CE_MenuItem, &opt, &p, d->menu);
    }

    //finally the rest of the spaces
    p.setClipRegion(emptyArea);
    menuOpt.state = QStyle::State_None;
    menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.rect = rect();
    menuOpt.menuRect = rect();
    style->drawControl(QStyle::CE_MenuEmptyArea, &menuOpt, &p, d->menu);
}

void MenuScrollArea::resizeEvent(QResizeEvent *event) {

}

Menu::Menu(QWidget *parent) :
    QMenu(parent),
    d(nullptr) {

    init();
}

Menu::Menu(const QString &title, QWidget *parent) :
    QMenu(title, parent),
    d(nullptr) {

    init();
}

Menu::~Menu() {
    if (!d->widgetItems.isEmpty()) {  // avoid detach on shared null hash
        QHash<QAction *, QWidget *>::iterator it = d->widgetItems.begin();
        for (; it != d->widgetItems.end(); ++it) {
            if (QWidget *widget = it.value()) {
                QWidgetAction *action = static_cast<QWidgetAction *>(it.key());
                action->releaseWidget(widget);
                *it = 0;
            }
        }
    }

    if (QEventLoop *eventLoop = QMenuPrivate::get(this)->eventLoop)
        eventLoop->exit();

    delete d;
}

QSize Menu::contentSizeHint() const {
    updateActionRects();
    if (!d->actionRects.isEmpty()) {
        QRect &firstItemRect = d->actionRects.front();
        QRect &lastItemRect = d->actionRects.back();
        return firstItemRect.united(lastItemRect).size();
    } 
    else {
        return QSize();
    }
}

QSize Menu::sizeHint() const {
    QSize s = contentSizeHint();
    QMargins margins = d->scrollArea->contentsMargins();
    return QSize(std::max(d->header.rect.width(), margins.left() + margins.right() + s.width()),
                 d->header.rect.height() + margins.top() + margins.bottom() + s.height());
}

QAction *Menu::currentAction() const {
    return d->currentAction;
}

QAction *Menu::activeAction() const {
    return d->currentAction;
}

Menu *Menu::activeMenu() const {
    return d->activeMenu;
}

QAction *Menu::headerAction() const {
    return d->header.action;
}

void Menu::setHeader(QAction *action) {
    if (!d->header.action.isNull()) {
        d->header.action->deleteLater();
        d->header.action.clear();
    }
    if (!d->header.actionWidget.isNull()) {
        d->header.actionWidget->deleteLater();
        d->header.actionWidget.clear();
    }
    if (!d->header.separator.isNull()) {
        d->header.separator->deleteLater();
        d->header.separator.clear();
    }

    if (action) {
        Q_ASSERT(!action->isSeparator());
        if (action->isSeparator()) {
            // так как в updateHeaderRects это не учитывается...
            qWarning("Menu::setHeader: Header action is separator");
            action->setSeparator(false);
        }

        d->header.action = action;
        action->setParent(this);
        if (QWidgetAction *wa = qobject_cast<QWidgetAction *>(action)) {
            QWidget *widget = wa->requestWidget(this);
            if (widget) {
                pSetMouseTracking(widget);
                d->header.actionWidget = widget;
            }
        }

        d->header.separator = new QAction(this);
        d->header.separator->setSeparator(true);
    }

    if (isVisible())
        adjustContent();
}

void Menu::popup(const QPoint &pos) {
    ensurePolished(); // Get the right font
    emit aboutToShow();

    resize(sizeHint());
    move(pos);
    show();
}

void Menu::correctSizePosFromDisplay() {
    QSize qSize = sizeHint();

    QPoint cursorPos = QCursor::pos();

    int heightMenu = qSize.height();
    int widthMenu = qSize.width();

    int topMenu = geometry().top();
    int leftMenu = geometry().left();

    QRect rectMenu = QRect();
    rectMenu.setRect(leftMenu, topMenu, widthMenu, heightMenu);

    QRect recW = QGuiApplication::primaryScreen()->geometry(); // nov    
    int heightW = recW.height();
    int widthW = recW.width();
    int delta = 10;

    bool cursor_in_menu = 
        type_menu_ == TypeMenu::PlayList
        //&& widthMenu > 0.20 * widthW
        //&& heightMenu > 0.6 * heightW
        && cursorPos.x() > rectMenu.left()
        && cursorPos.x() < rectMenu.right()
        && cursorPos.y() > rectMenu.top()
        && cursorPos.y() < rectMenu.bottom();

    QRect rectW = QRect();
    rectW.setRect(delta, delta, widthW - 2 * delta, heightW - 2 * delta);

    heightMenu = qSize.height();
    widthMenu = qSize.width();
   
    if (cursor_in_menu)
        topMenu = btn_rect_.bottom();

    leftMenu = geometry().left();

    rectMenu.setRect(leftMenu, topMenu, widthMenu, heightMenu);

    if (rectMenu.width() > rectW.width())
        widthMenu = rectW.width();
    setFixedWidth(widthMenu);

    if (rectMenu.height() > rectW.height())
        heightMenu = rectW.height();
    setFixedHeight(heightMenu);

    rectMenu.setRect(leftMenu, topMenu, widthMenu, heightMenu);

    QPoint pos = QPoint();

    if (rectMenu.left() < rectW.left())
        pos.setX(rectW.left());
    else if (rectMenu.right() > rectW.right())
        pos.setX(rectW.right() - rectMenu.width());
    else
        pos.setX(rectMenu.left());

    if (rectMenu.top() < rectW.top()) {
        pos.setY(rectW.top());
    }
    else if (rectMenu.bottom() > rectW.bottom()) {
        pos.setY(rectMenu.top());
        setFixedHeight(rectW.bottom() - rectMenu.top());
    }
    else {
        pos.setY(rectMenu.top());
    }

    if (cursor_in_menu)
        move(pos);
}

void Menu::changeEvent(QEvent *e) {
    if (e->type() == QEvent::StyleChange
        || e->type() == QEvent::FontChange
        || e->type() == QEvent::LayoutDirectionChange) {

        bool mouseTracking = style()->styleHint(QStyle::SH_Menu_MouseTracking, 0, this);
        setMouseTracking(mouseTracking);
        d->scrollArea->setMouseTracking(mouseTracking);

        if (e->type() == QEvent::StyleChange)
            d->scrollArea->setPalette(palette());
        else if (e->type() == QEvent::FontChange)
            d->scrollArea->setFont(font());

        if (isVisible())
            resize(sizeHint());
    }

    QWidget::changeEvent(e);
}

void Menu::keyPressEvent(QKeyEvent *e) {
    return QWidget::keyPressEvent(e);
}

void Menu::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);

    d->mouseDown = this;

    QAction *action = actionAt(e->pos());
    setCurrentAction(action, 20);
    update();
}

void Menu::mouseReleaseEvent(QMouseEvent *e) {
    QWidget::mouseReleaseEvent(e);

    if (d->mouseDown != this) {
        d->mouseDown = nullptr;
        return;
    }

    d->mouseDown = nullptr;
    QAction *action = actionAt(e->pos());

    if (action && action == d->currentAction) {

        if (!action->menu()) {
#if defined(Q_OS_WIN)
            //On Windows only context menus can be activated with the right button
            if (e->button() == Qt::LeftButton)
#endif
                activateAction(action, QAction::Trigger);
        }
    }
}

void Menu::mouseMoveEvent(QMouseEvent *e) {
    if (!isVisible())
        return;

    if ((!d->currentAction
        || !d->currentAction->menu()
        || !d->currentAction->menu()->isVisible())) {

        QAction *action = actionAt(e->pos());
        setCurrentAction(action);
    }

    if (e->buttons())
        d->mouseDown = this;

    if (d->activeMenu)
        d->activeMenu->setCurrentAction(nullptr);
}

void Menu::wheelEvent(QWheelEvent *) {

}

#ifdef QT6
void Menu::enterEvent(QEnterEvent*) {
#else // QT5
void Menu::enterEvent(QEvent*) {
#endif
    reloadScrollStyleSheet(true);
}

void Menu::leaveEvent(QEvent *) {
    reloadScrollStyleSheet(false);
}

void Menu::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);
    d->scrollArea->verticalScrollBar()->setValue(0);
    d->scrollArea->horizontalScrollBar()->setValue(0);
    resize(sizeHint());
    correctSizePosFromDisplay();
}

void Menu::hideEvent(QHideEvent *) {
    emit aboutToHide();
    if (d->mouseDown == this)
        d->mouseDown = nullptr;
    if (QEventLoop *eventLoop =  QMenuPrivate::get(this)->eventLoop)
        eventLoop->exit();
}

void Menu::paintEvent(QPaintEvent *e) {
    // Draw a header only. Other items will be drawn in MenuScrollArea::paintEvent
    const MenuPrivate::Header &header = d->header;
    QStyle *style = this->style();
    QPainter p(this);
    QRegion emptyArea = QRegion(rect());

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
#ifdef QT6
	menuOpt.reservedShortcutWidth = 0;
#else // QT5
	menuOpt.tabWidth = 0;
#endif
    style->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &p, this);

    if (!header.action.isNull()) {
        //Draw header
        if (e->rect().intersects(header.actionRect)
            && header.actionWidget.isNull()) {
            //set the clip region to be extra safe (and adjust for the scrollers)
            emptyArea -= QRegion(header.actionRect);

            QStyleOptionMenuItem opt;
            d->menu->initStyleOption(&opt, header.action);
            opt.rect = header.actionRect;
            style->drawControl(QStyle::CE_MenuItem, &opt, &p, this);
        }

        if (!header.separator.isNull()
            && e->rect().intersects(header.separatorRect)) {
            //set the clip region to be extra safe (and adjust for the scrollers)
            emptyArea -= QRegion(header.separatorRect);

            QStyleOptionMenuItem opt;
            d->menu->initStyleOption(&opt, header.separator);
            opt.rect = header.separatorRect;
            style->drawControl(QStyle::CE_MenuItem, &opt, &p, this);
        }
    }

    //finally the rest of the spaces
    p.setClipRegion(emptyArea);
    menuOpt.state = QStyle::State_None;
    menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.rect = rect();
    menuOpt.menuRect = rect();
    style->drawControl(QStyle::CE_MenuEmptyArea, &menuOpt, &p, this);
}

void Menu::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    adjustContent();
}

void Menu::actionEvent(QActionEvent *e) {
    setAttribute(Qt::WA_Resized, false);

    if (e->type() == QEvent::ActionAdded) {
        if (QWidgetAction *wa = qobject_cast<QWidgetAction *>(e->action())) {
            QWidget *widget = wa->requestWidget(d->scrollArea->viewport());
            if (widget) {
                pSetMouseTracking(widget);
                d->widgetItems.insert(wa, widget);
            }
        }

        if (isVisible())
            adjustContent();
    }
    else if (e->type() == QEvent::ActionRemoved) {
        if (e->action() == d->currentAction)
            d->currentAction = nullptr;

        if (QWidgetAction *wa = qobject_cast<QWidgetAction *>(e->action())) {
            if (QWidget *widget = d->widgetItems.value(wa)) {
#ifdef Q_OS_OSX
                QWidget *p = widget->parentWidget();
                if (p != this && qobject_cast<QMacNativeWidget *>(p)) {
                    // This widget was reparented into a native Mac view
                    // (see QMenuPrivate::moveWidgetToPlatformItem).
                    // Reset the parent and delete the native widget.
                    widget->setParent(this);
                    p->deleteLater();
                }
#endif
                wa->releaseWidget(widget);
            }
        }
        d->widgetItems.remove(e->action());

        if (isVisible())
            adjustContent();
    }
}

void Menu::timerEvent(QTimerEvent *e) {
    if (e->timerId() == d->delayedPopupTimerId) {
        stopDelayPopup();
        internalDelayedPopup();
    }
}

bool Menu::event(QEvent *e) {
    return QWidget::event(e);
}

bool Menu::focusNextPrevChild(bool next) {
    setFocus();
    QKeyEvent ev(QEvent::KeyPress, next ? Qt::Key_Tab : Qt::Key_Backtab, Qt::NoModifier);
    keyPressEvent(&ev);
    return true;
}

void Menu::verticalSliderValueChanged(int /*sliderValue*/) {
    updateActionRects();
}

void Menu::horizontalSliderValueChanged(int /*sliderValue*/) {
    updateActionRects();
}

int Menu::getLastVisibleAction() const {
    //let's try to get the last visible action
    const auto actions = this->actions();
    int lastVisibleAction = actions.count() - 1;

    for (;lastVisibleAction >= 0; --lastVisibleAction) {
        const QAction *action = actions.at(lastVisibleAction);
        if (action->isVisible()) {
            //removing trailing separators
            if (action->isSeparator() /*&& collapsibleSeparators*/)
                continue;
            break;
        }
    }

    return lastVisibleAction;
}

void Menu::init() {
    Q_ASSERT(d == nullptr);
    d = new MenuPrivate(this);

    QScrollBar *verticalScrollBar = d->scrollArea->verticalScrollBar();
    QScrollBar *horizontalScrollBar = d->scrollArea->horizontalScrollBar();

    d->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    d->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);

    bool mouseTracking = style()->styleHint(QStyle::SH_Menu_MouseTracking, 0, this);
    setMouseTracking(mouseTracking);
    d->scrollArea->setMouseTracking(mouseTracking);

    d->mouseEntered = true; // чтобы можно было вызвать reloadStyleSheet
    reloadScrollStyleSheet(false);

    connect(verticalScrollBar, &QScrollBar::valueChanged, this, &Menu::verticalSliderValueChanged);
    connect(horizontalScrollBar, &QScrollBar::valueChanged, this, &Menu::horizontalSliderValueChanged);
}

void Menu::reloadScrollStyleSheet(bool mouseEntered) {
    if (d->mouseEntered == mouseEntered)
        return;

    d->mouseEntered = mouseEntered;

    QString s;
    {
        QString color = mouseEntered ? "#39A5F0" : "#353945";
        s = QString("QScrollBar:vertical{border: none;background-color: #2A2E3A;width: 7px;margin: 0 0 0 0;}\
                    QScrollBar::handle:vertical{border-radius: 3px;border-color: %1;border-width: 1px; border-style: solid; background-color: %1; min-height: 30px;}\
                    QScrollBar::handle:vertical:hover{border-radius: 3px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-height: 30px;}\
                    QScrollBar::add-line:vertical{width: 0px; height: 0px;}\
                    QScrollBar::sub-line:vertical{width: 0px; height: 0px;}\
                    QScrollBar::add-page:vertical{background-color: transparent;}\
                    QScrollBar::sub-page:vertical{background-color: transparent;}")
                    .arg(color);
    }

    QScrollBar *verticalScrollBar = d->scrollArea->verticalScrollBar();
    if (verticalScrollBar->styleSheet() != s)
        verticalScrollBar->setStyleSheet(s);
}

void Menu::adjustContent() {
    // contentSizeHint вызовет updateActionRects
    QSize needSize = contentSizeHint();

    QMargins margins = d->scrollArea->contentsMargins();
    QSize viewportSize = d->scrollAreaRect.size();
    viewportSize.setWidth(viewportSize.width() - margins.left() - margins.right());
    viewportSize.setHeight(viewportSize.height() - margins.top() - margins.bottom());

    int needHeight = needSize.height();
    int needWidth = needSize.width();

    if (needWidth < viewportSize.width()
        || needHeight < viewportSize.height()) {

        int w = needWidth < viewportSize.width() ? needWidth : viewportSize.width();
        int h = needHeight < viewportSize.height() ? needHeight : viewportSize.height();
        resize(w, h);

        return;
    }

    QScrollBar *verticalScrollBar = d->scrollArea->verticalScrollBar();
    verticalScrollBar->setSingleStep(std::max(viewportSize.height() / 60, 1));
    verticalScrollBar->setPageStep(viewportSize.height() / 3);
    verticalScrollBar->setRange(0, needHeight - viewportSize.height());

    QScrollBar *horizontalScrollBar = d->scrollArea->horizontalScrollBar();
    horizontalScrollBar->setSingleStep(std::max(viewportSize.width() / 60, 1));
    horizontalScrollBar->setPageStep(viewportSize.width() / 3);
    horizontalScrollBar->setRange(0, needWidth - viewportSize.width());

    //correctSizePosFromDisplay();

    update();
}

void Menu::updateActionRects() const {
    MenuPrivate::Header &header = d->header;
    const auto actions = this->actions();
    bool headerActionExists = !header.action.isNull();

    header.actionRect = QRect();
    header.separatorRect = QRect();
    header.rect = QRect();

    d->actionRects.resize(actions.count());
    d->actionRects.fill(QRect());

    int lastVisibleAction = getLastVisibleAction();

    QStyle *style = this->style();
    QStyleOption opt;
    opt.initFrom(this);

    const int icone = style->pixelMetric(QStyle::PM_SmallIconSize, &opt, this);
    int max_column_width = d->header.rect.width();

    d->tabWidth = 0;
    d->hasCheckableItems = false;
    d->maxIconWidth = 0;

    if (headerActionExists) {
        if (!header.action->isSeparator()
            && header.action->isVisible()
            && header.actionWidget.isNull()) {

            QIcon is = header.action->icon();
            if (!is.isNull())
                d->maxIconWidth = qMax<uint>(d->maxIconWidth, icone + 4);
        }

        Q_ASSERT(header.separator);
        Q_ASSERT(header.separator->isSeparator());
        Q_ASSERT(header.separator->isVisible());
    }

    for (int i = 0; i < actions.count(); ++i) {
        QAction *action = actions.at(i);
        if (action->isSeparator()
            || !action->isVisible()
            || d->widgetItems.contains(action))
            continue;
        //..and some members
        d->hasCheckableItems |= action->isCheckable();
        QIcon is = action->icon();
        if (!is.isNull())
            d->maxIconWidth = qMax<uint>(d->maxIconWidth, icone + 4);
    }

    //calculate size
    QFontMetrics qfm = fontMetrics();
    bool previousWasSeparator = true; // this is true to allow removing the leading separators

    if (headerActionExists) {
        {
            // Action header
            //let the style modify the above size..
            QStyleOptionMenuItem opt;
            initStyleOption(&opt, header.action);
            const QFontMetrics &fm = opt.fontMetrics;

            QSize sz;
            if (!header.actionWidget.isNull()) {
                sz = header.actionWidget->sizeHint().expandedTo(header.actionWidget->minimumSize())
                        .expandedTo(header.actionWidget->minimumSizeHint()).boundedTo(header.actionWidget->maximumSize());
            } 
            else {
                QString s = header.action->text();
                int t = s.indexOf(QLatin1Char('\t'));
                if (t != -1)
                    s = s.left(t);

                sz.setWidth(fm.boundingRect(QRect(), Qt::TextSingleLine | Qt::TextShowMnemonic, s).width());
                sz.setHeight(qMax(fm.height(), qfm.height()));

                QIcon is = header.action->icon();
                if (!is.isNull()) {
                    QSize is_sz = QSize(icone, icone);
                    if (is_sz.height() > sz.height())
                        sz.setHeight(is_sz.height());
                }
            }
            sz = style->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, this);

            if (!sz.isEmpty()) {
                max_column_width = qMax(max_column_width, sz.width());
                //update the item
                header.actionRect = QRect(0, 0, sz.width(), sz.height());
            }
        }

        {
            // Header separator

            QStyleOptionMenuItem opt;
            initStyleOption(&opt, header.separator);
            QSize sz(2, 2);
            sz = style->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, this);
            if (!sz.isEmpty()) {
                max_column_width = qMax(max_column_width, sz.width());
                //update the item
                header.separatorRect = QRect(0, 0, sz.width(), sz.height());
            }
            previousWasSeparator = true;
        }
    }

    for(int i = 0; i <= lastVisibleAction; i++) {
        QAction *action = actions.at(i);
        const bool isSection = action->isSeparator()
            && (!action->text().isEmpty()
                || !action->icon().isNull());
        const bool isPlainSeparator = (isSection
            && !style->styleHint(QStyle::SH_Menu_SupportsSections))
                || (action->isSeparator()
                    && !isSection);

        if (!action->isVisible()
            || (/*collapsibleSeparators && */
                previousWasSeparator
                && isPlainSeparator))
            continue; // we continue, this action will get an empty QRect

        previousWasSeparator = isPlainSeparator;

        //let the style modify the above size..
        QStyleOptionMenuItem opt;
        initStyleOption(&opt, action);
        const QFontMetrics &fm = opt.fontMetrics;

        QSize sz;
        if (QWidget *w = d->widgetItems.value(action)) {
            sz = w->sizeHint().expandedTo(w->minimumSize()).expandedTo(w->minimumSizeHint()).boundedTo(w->maximumSize());
        }
        else {
            //calc what I think the size is..
            if (action->isSeparator()) {
                sz = QSize(2, 2);
            } else {
                QString s = action->text();
                int t = s.indexOf(QLatin1Char('\t'));
                if (t != -1) {
                    d->tabWidth = qMax(int(d->tabWidth), qfm.horizontalAdvance(s.mid(t+1)));
                    s = s.left(t);
#ifndef QT_NO_SHORTCUT
                }
                else if (action->isShortcutVisibleInContextMenu()
                    /* || !contextMenu*/) {
                    QKeySequence seq = action->shortcut();
                    if (!seq.isEmpty())
                        d->tabWidth = qMax(int(d->tabWidth), qfm.horizontalAdvance(seq.toString(QKeySequence::NativeText)));
#endif
                }
                sz.setWidth(fm.boundingRect(QRect(), Qt::TextSingleLine | Qt::TextShowMnemonic, s).width());
                sz.setHeight(qMax(fm.height(), qfm.height()));

                QIcon is = action->icon();
                if (!is.isNull()) {
                    QSize is_sz = QSize(icone, icone);
                    if (is_sz.height() > sz.height())
                        sz.setHeight(is_sz.height());
                }
            }

            sz = style->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, this);
        }

        if (!sz.isEmpty()) {
            max_column_width = qMax(max_column_width, sz.width());
            //update the item
            d->actionRects[i] = QRect(0, 0, sz.width(), sz.height());
        }
    }

    max_column_width += d->tabWidth; //finally add in the tab width

    //calculate position
    if (headerActionExists) {
        QMargins scrollAreaMargins = d->scrollArea->contentsMargins();

        // учесть, что заголовок и остальные элементы находятся в разных виджетах
        header.actionRect.setWidth(max_column_width + scrollAreaMargins.left() + scrollAreaMargins.right());

        if (!header.actionWidget.isNull()) {
            header.actionWidget->setGeometry(header.actionRect);
            header.actionWidget->setVisible(header.action->isVisible());
        }

        // Header separator rect
        header.separatorRect = QRect(0, header.actionRect.y() + header.actionRect.height(),
                                     header.actionRect.width(), header.separatorRect.height());

        header.rect = header.actionRect.united(header.separatorRect);

        int scrollAreaY = d->header.rect.y() + d->header.rect.height();
        d->scrollAreaRect = QRect(0, scrollAreaY, width(), height() - scrollAreaY);
    }
    else {
        d->scrollAreaRect = QRect(QPoint(0, 0), size());
    }

    d->scrollArea->setGeometry(d->scrollAreaRect);

    // Размер заголовка не учитываются, т.к. это координаты для scrollArea
    int x = -(d->scrollArea->horizontalScrollBar()->value());
    int y = -(d->scrollArea->verticalScrollBar()->value());

    for(int i = 0; i < actions.count(); i++) {
        QRect &rect = d->actionRects[i];
        if (rect.isNull())
            continue;
        rect.translate(x, y);                        //move
        rect.setWidth(max_column_width); //uniform width

        //we need to update the widgets geometry
        if (QWidget *widget = d->widgetItems.value(actions.at(i))) {
            widget->setGeometry(rect);
            widget->setVisible(actions.at(i)->isVisible());
        }

        y += rect.height();
    }
}

void Menu::initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const {
    if (!option
        || !action)
        return;

    option->initFrom(this);
    option->palette = palette();
    option->state = QStyle::State_None;

    if (window()->isActiveWindow())
        option->state |= QStyle::State_Active;
    
    if (isEnabled()
        && action->isEnabled()
            && (!action->menu()
                || action->menu()->isEnabled()))
        option->state |= QStyle::State_Enabled;
    else
        option->palette.setCurrentColorGroup(QPalette::Disabled);

    option->font = action->font().resolve(font());
    option->fontMetrics = QFontMetrics(option->font);

    if (d->currentAction
        && d->currentAction == action
        && !d->currentAction->isSeparator()) {
        option->state |= QStyle::State_Selected
                | (d->mouseDown ? QStyle::State_Sunken : QStyle::State_None);
    }

    option->menuHasCheckableItems = d->hasCheckableItems;
    if (!action->isCheckable()) {
        option->checkType = QStyleOptionMenuItem::NotCheckable;
    }
    else {
        option->checkType = (action->actionGroup()
            && action->actionGroup()->isExclusive()) ? QStyleOptionMenuItem::Exclusive : QStyleOptionMenuItem::NonExclusive;
        option->checked = action->isChecked();
    }
    
    if (action->menu())
        option->menuItemType = QStyleOptionMenuItem::SubMenu;
    else if (action->isSeparator())
        option->menuItemType = QStyleOptionMenuItem::Separator;
    else if (/*d->defaultAction == action*/ false)
        option->menuItemType = QStyleOptionMenuItem::DefaultItem;
    else
        option->menuItemType = QStyleOptionMenuItem::Normal;
    
    if (action->isIconVisibleInMenu())
        option->icon = action->icon();
    QString textAndAccel = action->text();
#ifndef QT_NO_SHORTCUT
    if ((action->isShortcutVisibleInContextMenu() || !isContextMenu())
            && textAndAccel.indexOf(QLatin1Char('\t')) == -1) {
        QKeySequence seq = action->shortcut();
        if (!seq.isEmpty())
            textAndAccel += QLatin1Char('\t') + seq.toString(QKeySequence::NativeText);
    }
#endif
    option->text = textAndAccel;    
#ifdef QT6
	option->reservedShortcutWidth = d->tabWidth;
#else // QT5
	option->tabWidth = d->tabWidth;
#endif
    option->maxIconWidth = d->maxIconWidth;
    option->menuRect = rect();
}

bool Menu::isContextMenu() const {
    return qobject_cast<const QMenuBar *>(topCausedWidget()) == nullptr;
}

void Menu::updateAction(QAction *act) {
    //QMenu в этом методе ещё зачем-то пересчитывает координаты
    //updateActionRects();

    if (!d->header.action.isNull() && d->header.action.data() == act) {
        update(d->header.actionRect);
    } else {
        int index = actions().indexOf(act);
        if (index != -1)
            d->scrollArea->update(d->actionRects.at(index));
    }
}

QAction *Menu::actionAt(QPoint p) const {
    if (!d->header.action.isNull()) {
        if (d->header.actionRect.contains(p))
            return d->header.action.data();
    }

    p.setX(p.x() - d->scrollAreaRect.x());
    p.setY(p.y() - d->scrollAreaRect.y());

    for(int i = 0; i < d->actionRects.count(); i++) {
        if (d->actionRects.at(i).contains(p))
            return actions().at(i);
    }

    return nullptr;
}

QWidget *Menu::topCausedWidget() const {
    QWidget* top = d->causedPopup.widget;
    while (Menu* m = qobject_cast<Menu *>(top))
        top = m->d->causedPopup.widget;

    return top;
}

QVector<QPointer<QWidget> > Menu::calcCausedStack() const {
    QVector<QPointer<QWidget> > ret;

    for(QWidget *widget = d->causedPopup.widget; widget; ) {
        ret.append(widget);
        if (Menu *menu = qobject_cast<Menu*>(widget))
            widget = menu->d->causedPopup.widget;
        else
            break;
    }

    return ret;
}

void Menu::hideUpToMenuBar() {
    bool fadeMenus = style()->styleHint(QStyle::SH_Menu_FadeOutOnHide);
    QWidget *caused = d->causedPopup.widget;
    hideMenu(this); //hide after getting causedPopup
    while(caused) {
        if (Menu *m = qobject_cast<Menu*>(caused)) {
            caused = m->d->causedPopup.widget;
            hideMenu(m);
            if (!fadeMenus) // Mac doesn't clear the action until after hidden.
                m->setCurrentAction(nullptr);
        } else {
            caused = nullptr;
        }
    }

    setCurrentAction(nullptr);
}

void Menu::activateAction(QAction *action, QAction::ActionEvent action_e, bool self) {
    if (!action
        || !isEnabled()
        || (action_e == QAction::Trigger
            && (action->isSeparator()
                || !action->isEnabled())))
        return;

    const QVector<QPointer<QWidget> > causedStack = calcCausedStack();

    if (action_e == QAction::Trigger) {
        if (testAttribute(Qt::WA_DontShowOnScreen)) {
            hideUpToMenuBar();
        }
        else {
            for (QWidget *widget = QApplication::activePopupWidget(); widget;) {
                if (Menu *menu = qobject_cast<Menu*>(widget)) {
                    if (menu == this)
                        hideUpToMenuBar();
                    widget = menu->d->causedPopup.widget;
                }
                else {
                    break;
                }
            }
        }
    }

    activateCausedStack({}, action, action_e, self);
}

void Menu::activateCausedStack(const QVector<QPointer<QWidget> > &causedStack,
    QAction *action, QAction::ActionEvent action_e, bool self) {

    if (self)
        action->activate(action_e);

    for(int i = 0; i < causedStack.size(); ++i) {
        QPointer<QWidget> widget = causedStack.at(i);
        if (!widget)
            continue;
        //fire
        if (Menu *menu = qobject_cast<Menu*>(widget)) {
            widget = menu->d->causedPopup.widget;
            if (action_e == QAction::Trigger)
                emit menu->triggered(action);
            else if (action_e == QAction::Hover)
                emit menu->hovered(action);
        }
    }
}

// popup == -1 means do not popup, 0 means immediately, others mean use a timer
void Menu::setCurrentAction(QAction *action, int popup, bool activateFirst) {
    if (action
            && (action->isSeparator()
                || (!action->isEnabled()
                    && !style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled, 0, this))))
        action = nullptr;

    if (Menu *menu = qobject_cast<Menu*>(d->causedPopup.widget)) {
        if (d->causedPopup.action
            && menu->activeMenu() == this) {

            if (menu->currentAction() != d->causedPopup.action)
                menu->setCurrentAction(d->causedPopup.action, 0, false);
        }
    }

    if (d->currentAction)
        updateAction(d->currentAction);

    Menu *hideActiveMenu = d->activeMenu;
    QAction *previousAction = d->currentAction;

    d->currentAction = action;
    if (action) {
        if (!action->isSeparator()) {
            activateAction(action, QAction::Hover);

            if (popup != -1) {
                // if the menu is visible then activate the required action,
                // otherwise we just mark the action as currentAction
                // and activate it when the menu will be popuped.
                if (isVisible())
                    popupAction(d->currentAction, popup, activateFirst);
            }

            updateAction(action);
        }
    }

    if (hideActiveMenu
        && previousAction != d->currentAction) {

        if (popup == -1)
            hideMenu(hideActiveMenu);
    }
}

void Menu::popupAction(QAction *action, int delay, bool /*activateFirst*/) {
    if (action) {
        if (action->isEnabled()) {
            if (!delay)
                internalDelayedPopup();
            else if (action->menu()
                && !action->menu()->isVisible())
                startDelayPopup(delay, action);
            else if (!action->menu())
                stopDelayPopup();
        }
    }
    else if (Menu *menu = d->activeMenu) {  //hide the current item
        hideMenu(menu);
    }
}

void Menu::hideMenu(Menu *menu) {
    if (!menu)
        return;

    if (d->activeMenu == menu)
        d->activeMenu = nullptr;

    menu->d->causedPopup.action = nullptr;
    menu->close();
    menu->d->causedPopup.widget = nullptr;
}

void Menu::internalDelayedPopup() {
    //hide the current item
    if (Menu *menu = d->activeMenu) {
        if (d->activeMenu->menuAction() != d->currentAction)
            hideMenu(menu);
    }

    if (!d->currentAction
        || !d->currentAction->isEnabled()
        || !d->currentAction->menu()
        || !d->currentAction->menu()->isEnabled()
        || d->currentAction->menu()->isVisible())
        return;

    //setup
    d->activeMenu = qobject_cast<Menu*>(d->currentAction->menu());
    if (!d->activeMenu) {
        qWarning("Menu::internalDelayedPopup: It not is Menu");
        return;
    }

    d->activeMenu->d->causedPopup.widget = this;
    d->activeMenu->d->causedPopup.action = d->currentAction;

    QPoint subMenuPos(mapToGlobal(QPoint(0, 0)));

    d->activeMenu->popup(subMenuPos);
}

void Menu::startDelayPopup(int delay, QAction *toStartAction) {
    if (!toStartAction) {
        if (d->delayedPopupTimerId)
            return stopDelayPopup();
    }

    if (toStartAction == d->delayedPopupAction) {
        if (d->delayedPopupTimerId)
            return;
    }
    else if (d->delayedPopupTimerId) {
        stopDelayPopup();
    }

    d->delayedPopupAction = toStartAction;
    d->delayedPopupTimerId = startTimer(delay);
}

void Menu::stopDelayPopup() {
    if (d->delayedPopupTimerId) {
        killTimer(d->delayedPopupTimerId);
        d->delayedPopupTimerId = 0;
    }

    d->delayedPopupAction = nullptr;
}

void Menu::pSetMouseTracking(QWidget *w) {
    w->setMouseTracking(true);
    for (QObject *c : w->children()) {
        if (c->isWidgetType())
            pSetMouseTracking(static_cast<QWidget*>(c));
    }
}

void Menu::click_button(QAction* action) {
    setCurrentAction(action, 20);
    update();
}
