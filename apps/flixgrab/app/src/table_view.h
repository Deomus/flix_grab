#pragma once
#include <QFrame>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include "abstract_list.h"
#include "table_content.h"


class TableView : public QScrollArea
{
	Q_OBJECT

public:
    TableView(QWidget *parent);

public:
    void                mouseReleaseEvent(QMouseEvent * event);
    void                mousePressEvent(QMouseEvent * event);
    void                mouseMoveEvent(QMouseEvent * event);
    void                timerEvent(QTimerEvent *event);

protected:
    QSize               sizeHint() const override;
   
    void                setupAutoScroll(QPoint pos);
    void                updateAutoScroll(void);

private:
  //  TableContent*               content_;

    //Minimal Auto Scrolling Distance;
    float                       auto_scroll_dist_;
    int                         max_scroll_step_;

    int                         scroll_step_;
    //Timer For Autoscroll;
    QBasicTimer                 auto_scroll_timer_;
};
