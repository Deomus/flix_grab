#pragma once
#include <QtCore>
//#include <QtGui>
#include <QtWidgets>
#include "abstract_item.h"
#include "abstract_list.h"


enum ESelectionType {
    kSelectionType_None,
    kSelectionType_Add,
    kSelectionType_Remove,
    kSelectionType_Inverse
};

enum TypeUpdateItem {
    kCurrentBlock,
    kLast,
    kAll
};

class TableContent : public QFrame {
    Q_OBJECT

public:
                        TableContent(QWidget* parent = nullptr);

signals:
    void                contextMenuRequested(const QPoint &pos, const QList<AbstractItem*>& items);
    void                updateItemsOnDragItems(const QList<AbstractItem*>& items, AbstractItem* item_start_, AbstractItem* item_pos);

public:
    QSize               sensitivity() const { return sensitivity_; }
    void                setSensitivity(QSize s) { sensitivity_ = s; }
       
    //Content Setup;
    void                setItems(AbstractList* list);
    void                setItemGeometry(QSize const& item_size, QSize const& margin_size, QSize const& border_size, int size_width_vertical_scroll);
    
    QRect               itemRect(int i);

    void                updateContent(TypeUpdateItem type = TypeUpdateItem::kCurrentBlock);
    void                invalidateContent(int from = 0);
    QSize               sizeHint() const override;
    QSize               get_height_to_resize();

    AbstractItem*       itemAtPoint(QPoint pt);
    AbstractItem*       itemNearAtPoint(QPoint pt);

    QSize               get_geom_size_() { return geom_size_; };
    void                handleResize();

public:
    const AbstractList* items() const { return items_; }
       
protected:
    void                resizeContent(QSize size);
    void                scrollContent(TypeUpdateItem type = TypeUpdateItem::kCurrentBlock);
    void                scrollContent(QRect viewport, TypeUpdateItem type = TypeUpdateItem::kCurrentBlock);

    bool                event(QEvent* event) override;
    bool                eventFilter(QObject *o, QEvent *e) override;

    void                moveEvent(QMoveEvent *event) override;
    void                contextMenuEvent(QContextMenuEvent *event) override;
    //Selection Region;
    void                cleanSelection(void);

    void                updateSelection();
    void                updateSelection(QRect rect, ESelectionType type);

    //Mouse Interception;
    void                mouseReleaseEvent(QMouseEvent * event);
    void                mousePressEvent(QMouseEvent * event);
    void                mouseMoveEvent(QMouseEvent * event);
           
protected:
    QPoint              positionFromIndex(int nIndex) const;

    //Update Items By Index;
    void                updateItems(int from, int to);

    //Function That do heavy work of activation/deactivation through EventLoop
    Q_INVOKABLE void    applyVisibleChanges();

    void                dragEnterEvent(QDragEnterEvent* event);
    void                dropEvent(QDropEvent* event);

private:
    AbstractList*               items_;
    QSize                       view_tab_;
    bool                        user_is_resizing_ = true;
    QSize                       content_tab_;
    int                         check_parent_width_;
    //Placed Indexes
    int                         from_, to_;

    QTimer*                     resize_timer_;

    //Selection;
private:
    QRect                       selection_rect_;
    //Handle previous position when dragging selection;
    QRect                       prev_selection_rect_;
    bool                        is_selecting_;

    AbstractItem*               item_start_;

    QDrag*                      drag_;

private:
    QSize                       geom_size_;
    QSize                       margin_size_;
    QSize                       border_size_;
    QSize                       sensitivity_;
    QSize                       border_size_right_;
    
private:
    QRubberBand*                rubber_;

private:
    //Item id in lists to apply visualisation;
    QList<int>                 activations_;
    QList<int>                 deactivations_;
};
