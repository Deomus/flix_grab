#pragma once

#include <QtCore>
#include "abstract_item.h"

class AbstractList : public QObject
{
    Q_OBJECT

public:
    AbstractList(QObject* parent = nullptr) : QObject(parent) {}


    //Count Items;
    virtual int                 count(void) const = 0;
    //Get Item;
    virtual AbstractItem*       item(int index) = 0;


};