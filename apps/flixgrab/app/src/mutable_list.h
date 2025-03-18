#pragma once
#include <QtCore>

#include "abstract_list.h"
#include "abstract_item.h"

#include "video_item_controller.h"
#include "table_view.h"

class MutableList : public AbstractList
{
    Q_OBJECT

public:

    MutableList(TableContent* content, QObject* parent = nullptr)
        : AbstractList(parent)
        , content_(content) {
    }

    
    int count(void) const { return items_.size(); };
    //Get Item;
    AbstractItem*           item(int index) {
        /*Q_ASSERT(index < items_.count());
        return items_.at(index);*/
        return index < items_.count() ? items_.at(index) : nullptr;
    }

    QList<AbstractItem*>    items() const { return items_; }


public slots:
    //Add Item
    void addItem(AbstractItem* item, int index = -1) {
        
        index = index < 0 ? items_.size() : index;
        content_->invalidateContent(index);
        items_.insert(index, item);
        
        content_->updateContent(TypeUpdateItem::kLast);
    }

    void removeItem(int index) {
        Q_ASSERT(index < items_.count());
        content_->invalidateContent(index);
        items_.removeAt(index);
        content_->updateContent();
    }

    void removeItem(AbstractItem* item) {
        int index = items_.indexOf(item);
        if (index != -1)
            removeItem(index);
    }

    void setItems(const QList<AbstractItem*>& list) {    
        content_->invalidateContent();
        items_ = list;
        content_->updateContent();

        ////Trying to cache items;
        //items_.reserve(list.size());

        //if (list.size() < items_.size()) {
        //    items_.erase(items_.begin() + list.size(), items_.end());
        //}

        //int changed_pos = items_.size();

        //for (int i = 0; i < items_.size(); ++i) {
        //    if (list[i] != items_[i]) {
        //        items_[i] = list[i];

        //        //TODO: too many connections!!!
        //        //QObject::connect(items_[i], &AbstractItem::removed, this, &ListController::removeSender);
        //        if (changed_pos > i)
        //            changed_pos = i;
        //    }
        //}

        //for (int i = items_.size(); i < list.size(); ++i) {
        //    items_.push_back(list[i]);
        //    //QObject::connect(items_.back(), &AbstractItem::removed, this, &ListController::removeSender);
        //}

        //qDebug() << "Changed pos " << changed_pos;
        //view_->updateContent();
    }

protected slots:
    void removeSender() {
        if (AbstractItem* item = qobject_cast<AbstractItem*>(sender()))
            removeItem(item);
    }

private:
    QList<AbstractItem*>                            items_;
    TableContent*                                   content_;
};