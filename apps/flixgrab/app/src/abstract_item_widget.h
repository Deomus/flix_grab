#pragma once
#include <QtGui>
#include <QtWidgets>

class AbstractItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractItemWidget(QWidget* parent = 0) : QWidget(parent) {}
    
    virtual ~AbstractItemWidget() {}

    //Show Selection On Item
    virtual         void    showSelection(bool bSelected = false) = 0;

    virtual         void    release() = 0;
    
};