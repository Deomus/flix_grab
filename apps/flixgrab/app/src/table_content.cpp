#include "table_content.h"
#include <functional>


#define         DEFAULT_CONTENT_COLUMNS                 (3)
#define         DEFAULT_CONTENT_ROWS                    (3)


static const int    MAX_OPERATIONS_PER_CALL = 4;

inline  int   divCeil(int x, int y){
    return qCeil((qreal)x / (qreal)y);
}

inline  QSize   divCeil(const QSize& x, const QSize& y) {
    return QSize(divCeil(x.width(), y.width()), 
        divCeil(x.height(), y.height()));
}

inline  QSize   mul(const QSize& x, const QSize& y) {
    return QSize(x.width()* y.width(),
        x.height()* y.height());
}


TableContent::TableContent(QWidget* parent /*= nullptr*/) : QFrame(parent)
    , items_(nullptr)
    , from_(0)
    , to_(0)
    , view_tab_(DEFAULT_CONTENT_COLUMNS, DEFAULT_CONTENT_ROWS)
    , content_tab_(DEFAULT_CONTENT_COLUMNS, DEFAULT_CONTENT_ROWS)
    , is_selecting_(false) {

    check_parent_width_ = 0;
    rubber_ = new QRubberBand(QRubberBand::Rectangle, this);
    this->setAcceptDrops(true);
}

void TableContent::handleResize() {
    if (parentWidget())
        resizeContent(parentWidget()->size());
}

void TableContent::setItems(AbstractList* list) {   
    //Optimization: find first item address different from previous list 
    //and hide only new;
    invalidateContent();

    items_ = list;
    updateContent();
}

void TableContent::setItemGeometry(QSize const& item_size, QSize const& margin_size, QSize const& border_size, int size_width_vertical_scroll) {
    geom_size_ = item_size + margin_size;
    margin_size_ = margin_size;
    border_size_ = border_size;
    border_size_right_ = QSize(border_size.width() - size_width_vertical_scroll, border_size.height());
    check_parent_width_ = 0;
    this->setAcceptDrops(true);
    updateContent();
    resize_timer_ = new QTimer(this);
    resize_timer_->setSingleShot(true);
    QObject::connect(resize_timer_, &QTimer::timeout, this, &TableContent::handleResize);
    if (parentWidget())
        parentWidget()->installEventFilter(this);
}

QRect TableContent::itemRect(int i) {
    return QRect(positionFromIndex(i), geom_size_);
}

void TableContent::resizeContent(QSize size) {
    if (!size.isEmpty()
        && !geom_size_.isEmpty()) {
        view_tab_ = size - border_size_ - border_size_right_ + margin_size_ - sensitivity_;
        view_tab_ = divCeil(view_tab_, geom_size_);
        updateContent(TypeUpdateItem::kAll);
    }
}

void TableContent::updateContent(TypeUpdateItem type) {
    if (items_) {
        if (parentWidget()) {
            content_tab_ = view_tab_;
            content_tab_.rheight() = qMax(divCeil(items_->count(), content_tab_.width()), content_tab_.height());
               
            QSize widget_size = sizeHint();

            int width = widget_size.width();
            int height = widget_size.height();

            int parent_width = parentWidget()->size().width();
            int parent_height = parentWidget()->size().height();

            widget_size = widget_size.expandedTo(parentWidget()->size());

            if (this->size() != widget_size) {
                this->setFixedSize(widget_size);
                //invalidateContent();
            }

            if (width - parent_width >= 5) {
                int new_width = content_tab_.width() - 1;
                if (new_width > 2) {
                    content_tab_.setWidth(new_width);
                    if (content_tab_.width() * content_tab_.height() < items_->count()) {
                        while (content_tab_.width() * content_tab_.height() < items_->count())
                            content_tab_.setHeight(content_tab_.height() + 1);

                        QSize widget_size = sizeHint();
                        widget_size = widget_size.expandedTo(parentWidget()->size());
                        if (this->size() != widget_size) {
                            this->setFixedSize(widget_size);
                            invalidateContent();
                        }
                    }
                }
            }

            if (content_tab_.width() != check_parent_width_) {
                invalidateContent();
                check_parent_width_ = content_tab_.width();
            }

            scrollContent(type);
        }
    }
}

void TableContent::scrollContent(QRect viewport, TypeUpdateItem type) {
    if (items_) {
        QSize offset = border_size_right_ + border_size_ - margin_size_;
        viewport.translate(0, -offset.height());
        int view_from = content_tab_.width() * (viewport.top() / geom_size_.height());
        //int view_to = columns_ * (viewport.bottom() / geom_size_.height() + 1);
        int view_to = content_tab_.width() * divCeil(viewport.bottom(), geom_size_.height());// ((viewport.bottom() - 2 * border_size_.height() + margin_size_.height()) / (float)geom_size_.height());

        if (is_selecting_) {
            QPoint pt = this->mapFromGlobal(QCursor::pos());
            selection_rect_.setBottomRight(pt);
            updateSelection();
        }

        switch (type) {
            case TypeUpdateItem::kCurrentBlock:
                //Fixes ASSERT in updateContent
                view_from = std::min(view_from, items_->count());
                view_to = std::min(view_to, items_->count());
                break;
            case TypeUpdateItem::kLast:
                view_from = std::min(view_from, items_->count());
                view_to = std::min(view_to, items_->count());
                break;
            case TypeUpdateItem::kAll:
                view_from = 0;
                view_to = items_->count();
                break;
            default:
                break;
        }

        //Fixes ASSERT in updateContent
        view_from = std::min(view_from, items_->count());
        view_to = std::min(view_to, items_->count());

        updateItems(view_from, view_to);
    }
}

void TableContent::scrollContent(TypeUpdateItem type) {
    if (parentWidget()) {
        QRect viewport(this->mapFromParent(QPoint()), parentWidget()->size());
        scrollContent(viewport, type);
    }
}

QSize TableContent::sizeHint() const {
    QSize widget_size = mul(content_tab_, geom_size_) + border_size_right_ + border_size_ - margin_size_;

    return widget_size.expandedTo(QSize());
}

QSize TableContent::get_height_to_resize() {
    view_tab_ = parentWidget()->size() - border_size_right_ - border_size_ + margin_size_ - sensitivity_;
    view_tab_ = divCeil(view_tab_, geom_size_);

    QSize widget_size = mul(view_tab_, geom_size_) + border_size_right_ + border_size_ - margin_size_;

    return widget_size.expandedTo(QSize());
}

AbstractItem* TableContent::itemAtPoint(QPoint pt) {
    if (items_ == nullptr)
        return nullptr;

    QPoint content_pt = pt - QPoint(border_size_.width(), border_size_.height());
    int column = content_pt.x() / geom_size_.width();
    int row = content_pt.y() / geom_size_.height();

    content_pt.rx() -= column * geom_size_.width();
    content_pt.ry() -= row * geom_size_.height();

    QSize item_size = geom_size_ - margin_size_;

    if (content_pt.x() >= 0
        && content_pt.x() <= item_size.width()
        && content_pt.y() >= 0
        && content_pt.y() <= item_size.height()) {

        int index = row * content_tab_.width() + column;
        if (index < items_->count())
            return items_->item(index);
    }

    return nullptr;
}

AbstractItem* TableContent::itemNearAtPoint(QPoint pt) {
    if (items_ == nullptr)
        return nullptr;

    QPoint content_pt = pt - QPoint(border_size_.width(), border_size_.height());

    int column = content_pt.x() / geom_size_.width();

    if (column < content_tab_.width()
        && column > 0)
        column += 1;

    int row = content_pt.y() / geom_size_.height();

    int index = row * content_tab_.width() + column;

    if (index < items_->count())
        return items_->item(index);

    if (column > 0) { column -= 1; }

    index = row * content_tab_.width() + column;

    if (index < items_->count())
        return items_->item(index);

    index = items_->count() - 1;

    if (index > 0)
        return items_->item(index);

    return nullptr;
}

void TableContent::invalidateContent(int from) {
    if (items_) {
        //qDebug() << "Deactivate 0 from " << from_ << " to " << to_;
        int new_from = qMin(qMax(from_, from), items_->count());
        int new_to = qMin(qMax(to_, from), items_->count());

        for (int i = new_from; i < new_to; ++i) {
            if (AbstractItem* item = items_->item(i))
                //  item->deactivate(); 
                item->hide(this); //nov
        }

        from_ = to_ = 0;
    }
}

//Filtering viewport resize event;
bool TableContent::event(QEvent * event) {
    /*if (event->type() == QEvent::ParentChange) {
        if (parentWidget())
            parentWidget()->installEventFilter(this);
    }

    if (event->type() == QEvent::ParentAboutToChange) {
        if (parentWidget())
            parentWidget()->removeEventFilter(this);
    }*/

    return QFrame::event(event);
}

//Resize of viewport;
bool TableContent::eventFilter(QObject *o, QEvent *e) {
    if (o == parentWidget()
        && e->type() == QEvent::Resize) {

        resize_timer_->stop();
        resize_timer_->start(500);
        //QResizeEvent* re = static_cast<QResizeEvent*>(e);
        //resizeContent(re->size());
    }

    return QFrame::eventFilter(o, e);
}

void TableContent::moveEvent(QMoveEvent *event) {
    scrollContent(TypeUpdateItem::kAll);
    QFrame::moveEvent(event);
}

void TableContent::contextMenuEvent(QContextMenuEvent *event) {
    if (items_) {
        QList<AbstractItem*>    selected_items;
        for (int i = 0; i < items_->count(); ++i) {
            auto item = items_->item(i);
            if (item->isSelected())
                selected_items.push_back(item);
        }

        if (!selected_items.isEmpty())
            emit contextMenuRequested(event->pos(), selected_items);
    }

    QFrame::contextMenuEvent(event);
}

void TableContent::cleanSelection(void) {
    if (items_) {
        for (int i = 0; i < items_->count(); ++i)
            items_->item(i)->setSelected(false);

        prev_selection_rect_ = QRect();
    }
}

void TableContent::updateSelection(QRect rect, ESelectionType type) {
    if (!rect.isValid())
        rect = rect.normalized();

    //Crop Rect To Widget Size;
    rect = rect.intersected(QRect(0, 0, QWidget::width(), QWidget::height()));
    //    m_nRowSize * m_szGeometry.width(), m_nColumnSize*geometry.height()));

    rect.translate(-border_size_.width(), -border_size_.height());

    QRect current(QPoint(rect.left() / geom_size_.width(), rect.top() / geom_size_.height()),
        QPoint(rect.right() / geom_size_.width(), rect.bottom() / geom_size_.height()));

    QRect prev = prev_selection_rect_;
    QRect inter = current.intersected(prev);

    std::function<void(AbstractItem*)>  opCurFunc[] = {
        nullptr,
        [](AbstractItem* p) { p->setSelected(true); },
        [](AbstractItem* p) { p->setSelected(false); },
        [](AbstractItem* p) { p->setSelected(!p->isSelected()); },
    };

    std::function<void(AbstractItem*)>  opPrevFunc[] = {
        nullptr,
        [](AbstractItem* p) { p->setSelected(false); },
        [](AbstractItem* p) { p->setSelected(true); },
        [](AbstractItem* p) { p->setSelected(!p->isSelected()); },
    };

    //Caching previous position when dragging;
    if (prev != inter) {
        for (int j = prev.top(); j <= prev.bottom(); ++j) {
            for (int i = prev.left(); i <= prev.right(); ++i) {
                int pos = j * content_tab_.width() + i;

                if (!inter.contains(i, j, false)
                    && pos < items_->count())
                    opPrevFunc[type](items_->item(pos));
                //if (AbstractItem* item = items_->item(j*row_size_ + i))
                    //item->setSelected(opPrevFunc[type](item));
            }
        }
    }

    if (current != inter) {
        for (int j = current.top(); j <= current.bottom(); ++j) {
            for (int i = current.left(); i <= current.right(); ++i) {
                int pos = j * content_tab_.width() + i;

                if (!inter.contains(i, j, false)
                    && pos < items_->count())
                    opCurFunc[type](items_->item(pos));
                //if (AbstractItem* item = items_->item(j*row_size_ + i))
                    //item->setSelected(opCurFunc[type](item));
            }
        }
    }

    prev_selection_rect_ = current;
}

void TableContent::updateSelection() {
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        updateSelection(selection_rect_, kSelectionType_Inverse);
    else
        updateSelection(selection_rect_, kSelectionType_Add);

    QRect rubber_rect = selection_rect_.intersected(rect()).normalized();
    rubber_->setGeometry(rubber_rect);
    //m_rcPrevSelection = rect;
}

void TableContent::mouseReleaseEvent(QMouseEvent * event) {
    if (is_selecting_) {
        selection_rect_.setBottomRight(event->pos());
        updateSelection();
    }

    is_selecting_ = false;
    rubber_->hide();
    //Move Event To Parent (Scroll Area)
    event->ignore();
}

void TableContent::mousePressEvent(QMouseEvent * event) {
    auto item = itemAtPoint(event->pos());

    if (event->button() == Qt::LeftButton
        && !QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)
        && !QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {

        if (item
            && item->isSelected()) {

            item_start_ = item;
            drag_ = new QDrag(this);
            QMimeData* mimeData = new QMimeData;
            drag_->setMimeData(mimeData);

            QPixmap pixmap = QPixmap(geom_size_);
            drag_->setPixmap(pixmap);

            drag_->exec(Qt::MoveAction);
            return;
        }
    }

    selection_rect_ = QRect(event->pos(), event->pos());
    prev_selection_rect_ = QRect();
    is_selecting_ = true;

    if ((!item || !item->isSelected())
        && !QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)
        && !QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {

        cleanSelection();
    }

    /*if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)
        && !QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)
        && event->button() != Qt::RightButton) {
        cleanSelection();
    }*/

    updateSelection();
    //TODO: Use Only Left Key;
    rubber_->show();
    //Move Event To Parent (Scroll Area)
    event->ignore();
}

void TableContent::mouseMoveEvent(QMouseEvent * event) {
    if (is_selecting_) {
        selection_rect_.setBottomRight(event->pos());
        updateSelection();
    }
    //Move Event To Parent (Scroll Area)
    event->ignore();
}

void TableContent::dragEnterEvent(QDragEnterEvent* event) {
    //QStringList formats = event->mimeData()->formats();
    //event->acceptProposedAction();
    event->accept();
}

void TableContent::dropEvent(QDropEvent* event) {
    auto item_pos = itemAtPoint(event->pos());

    QList<AbstractItem*>    items_drag;
    QList<AbstractItem*>    items_drag_is_selected;

    if (items_
        && items_->count() > 1) {

        if (item_pos == nullptr) {
            item_pos = itemNearAtPoint(event->pos());
            
            if (item_pos == nullptr)
                return;
        }

        for (int i = 0; i < items_->count(); ++i) {
            auto item = items_->item(i);

            if (!item->isSelected()
                || item == item_pos)
                items_drag.push_back(item);

            if (item->isSelected())
                items_drag_is_selected.push_back(item);
        }

        int index_for_insert = items_drag.indexOf(item_pos);
        auto item_last = items_->item(items_->count() - 1);

        if (item_last == item_pos)
            index_for_insert = items_drag.count();

        if (item_pos != item_start_) {
            if (item_pos->isSelected())
                items_drag.replace(index_for_insert, item_start_);
            else
                items_drag.insert(index_for_insert, item_start_);
        }

        for (int i = 0; i < items_drag_is_selected.count(); ++i) {
            auto item = items_drag_is_selected.at(i);
            
            if (item != item_start_) {
                if (index_for_insert < items_->count())
                    index_for_insert += 1;

                items_drag.insert(index_for_insert, item);
            }
        }

        emit updateItemsOnDragItems(items_drag, item_start_, item_pos);
    }

    event->accept();
}


QPoint TableContent::positionFromIndex(int nIndex) const {
    int i = nIndex % content_tab_.width();
    int j = nIndex / content_tab_.width();
    return QPoint(i*geom_size_.width() + border_size_.width(),
        j*geom_size_.height() + border_size_.height());
}

void TableContent::updateItems(int from, int to) {
    //qDebug() << "Update Items: (" << from << "-" << to << ") cached (" << from_ << "_" << to_ << ")";
    to = std::min(to, items_->count());
    to_ = std::min(to_, items_->count());
       
    Q_ASSERT(from <= to);
    if (from > from_) {
        //qDebug() << "Deactivate 1 from " << from_ << " to " << std::min(from, to_);
        for (int i = from_; i < std::min(from, to_); ++i)
            deactivations_.append(i);
    }

    if (to_ > to) {
        //qDebug() << "Deactivate 2 from " << std::max(to, from_) << " to " << to_;
        for (int i = std::max(to, from_); i < to_; ++i)
            deactivations_.append(i);
    }
    
    if (from < from_) {
        //qDebug() << "Activate 1 from " << from << " to " << std::min(from_, to);
        for (int i = from; i < std::min(from_, to); ++i)
            activations_.append(i);
    }

    if (to_ < to) {
        //qDebug() << "Activate 2 from " << std::max(to_, from) << " to " << to;
        for (int i = std::max(to_, from); i < to; ++i)
            activations_.append(i);
    }

    from_ = from;
    to_ = to;

    applyVisibleChanges();
}

Q_INVOKABLE void TableContent::applyVisibleChanges() {
    int ops = MAX_OPERATIONS_PER_CALL;

    while (ops-- >= 0
        && !deactivations_.empty()) {

        if (AbstractItem* item = items_->item(deactivations_.takeFirst()))
            item->hide(this);
    }

    while (ops-- >= 0
        && !activations_.empty()) {

        int i = activations_.takeFirst();
        if (AbstractItem* item = items_->item(i))
            item->activate(this, positionFromIndex(i));
    }
       
    //Add new invoke to loop;
    if (!activations_.empty() || 
        !deactivations_.empty())
        QMetaObject::invokeMethod(this, "applyVisibleChanges", Qt::QueuedConnection);
}
