#include "table_view.h"

#define     SCROLL_UPDATE_TIME              (60) //ms


TableView::TableView(QWidget *parent) 
    : QScrollArea(parent)
    //, content_(nullptr)

    , auto_scroll_dist_(0.1f)
    , max_scroll_step_(100)
{
    // QApplication::instance()->installEventFilter(this);
    /*if (parent)
    {
    parent->setMouseTracking(true);
    parent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    parent->setAutoFillBackground(false);
    parent->setAttribute(Qt::WA_TranslucentBackground);
    parent->setAttribute(Qt::WA_OpaquePaintEvent);
    parent->setAttribute(Qt::WA_NoSystemBackground);
    }*/
    parent->setStyleSheet("QScrollBar:vertical{border: none;background-color: #353945;width: 7px;margin: 0 0 0 0;}\
    QScrollBar:vertical:hover{border: none;background-color: #353945;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:vertical{border-radius: 3px;border-color: #353945;border-width: 1px; border-style: solid; background-color: #353945; min-height: 30px;}\
    QScrollBar::handle:vertical:hover{border-radius: 3px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-height: 30px;}\
    QScrollBar::add-line:vertical{width: 0px; height: 0px;}\
    QScrollBar::sub-line:vertical{width: 0px; height: 0px;}\
    QScrollBar::add-page:vertical{background-color: transparent;}\
    QScrollBar::sub-page:vertical{background-color: transparent;}\
    QScrollBar:horizontal{border: none;background-color: #353945;height: 7px;margin: 0 0 0 0;}\
    QScrollBar:horizontal:hover{border: none;background-color: #353945;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:horizontal{border-radius: 3px;border-color: #353945;border-width: 1px; border-style: solid; background-color: #353945; min-width: 30px;}\
    QScrollBar::handle:horizontal:hover{border-radius: 3px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-width: 30px;}\
    QScrollBar::add-line:horizontal{width: 0px; width: 0px;}\
    QScrollBar::sub-line:horizontal{width: 0px; width: 0px;}\
    QScrollBar::add-page:horizontal{background-color: transparent;}\
    QScrollBar::sub-page:horizontal{background-color: transparent;}\
    QAbstractScrollArea::corner {background-color: #353945;}");
}


void TableView::mouseReleaseEvent(QMouseEvent * event) {
    //Q_UNUSED(event)
    auto_scroll_timer_.stop();
    QScrollArea::mouseReleaseEvent(event);
}

void TableView::mousePressEvent(QMouseEvent * event) {
    //Q_UNUSED(event)
    auto_scroll_timer_.start(SCROLL_UPDATE_TIME, this);
    scroll_step_ = 0;
    QScrollArea::mousePressEvent(event);
}

void TableView::mouseMoveEvent(QMouseEvent * event) {
    setupAutoScroll(event->pos());
    QScrollArea::mouseMoveEvent(event);
}

void TableView::timerEvent(QTimerEvent *event) {
    if (event->timerId() == auto_scroll_timer_.timerId()) {
        //Auto-scrolling Enabled;
        updateAutoScroll();
    }
}

QSize TableView::sizeHint() const {
    if (widget()) {
        QSize sz = widget()->sizeHint();

        if (this->verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOn)
            sz.setWidth(sz.width() + this->verticalScrollBar()->sizeHint().width());
        if (this->horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOn)
            sz.setHeight(sz.height() + this->horizontalScrollBar()->sizeHint().height());
        return sz;
    }

    return QScrollArea::sizeHint();
    
}

void TableView::setupAutoScroll(QPoint pos) {
    //Check if We in Scroll Distance From Viewport;
    QSize   view_size = viewport()->size();
    QSize   auto_size(view_size.width() * auto_scroll_dist_, view_size.height() * auto_scroll_dist_);

    QRect   bound_rect(QPoint(auto_size.width(), auto_size.height()),
        QPoint(view_size.width() - auto_size.width(), view_size.height() - auto_size.height()));

    scroll_step_ = 0;

    if (bound_rect.top() > pos.y()) {
        //Move Up;
        scroll_step_ = max_scroll_step_ * std::max(-1.f, (float)(pos.y() - bound_rect.top()) / (float)auto_size.height());
    }
    else if (bound_rect.bottom() < pos.y()) {
        //Move Down;
        scroll_step_ = max_scroll_step_ * std::min(1.f, (float)(pos.y() - bound_rect.bottom()) / (float)auto_size.height());
    }
}

void TableView::updateAutoScroll(void) {
    QSize   widget_size = widget()->size();

    int value = verticalScrollBar()->value() + scroll_step_;
    //Setup Bounds;
    value = std::max(0, std::min(widget_size.height(), value));

    verticalScrollBar()->setValue(value);
}
