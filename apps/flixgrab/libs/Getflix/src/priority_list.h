#pragma once
#include <QtCore>

//List of items with fixed priority;
template<typename T>
class PriorityList {

public:
    inline int			size() const { return priority_list_.size(); }

    //Insert item with priority, return index of item;
    inline int			insert(int priority, const T& v) {
        int back_index = 0;
        auto it_insert = priority_list_.rbegin();
        for (; it_insert != priority_list_.rend(); ++it_insert, ++back_index) {
            //Check if priority detected;
            if (it_insert->second >= priority)
                break;
        }
        back_index = priority_list_.size() - back_index;
        //priority_list_.insert(it_insert, video);
        priority_list_.insert(it_insert.base(), qMakePair(v, priority));
        return back_index;
    }


    //Remove item, return previous index of item or -1;
    inline int			remove(const T& v) {
        int index = 0;
        auto it = priority_list_.begin();
        for (; it != priority_list_.end(); ++it, ++index) {
            if (it->first == v)
                break;
        }

        if (it != priority_list_.end())
            it = priority_list_.erase(it);
        else
            index = -1;

        return index;
    }

    //clear all list;
    inline void         clear() {
        priority_list_.clear();
    }

    //Check if item contains;
    inline bool         contains(const T& v) const {
        for (auto p : priority_list_) {
            if (p.first == v)
                return true;
        }
        return false;
    }

    //List of items by indexes;
    inline QList<T>	    get(int from, int count) {
        QList<T>	lst;
        if (priority_list_.size() > from) {
            auto it = priority_list_.begin() + from;
            count = qMin(priority_list_.size() - from, count);
            lst.reserve(count);
            for (int i = 0; i < count; i++, ++it) {
                lst.push_back(it->first);
            }
        }
        
        return lst;
    }

    //Single item;
    T			        get(int index) {
        if (index < priority_list_.size()) {
            auto it = priority_list_.begin() + index;
            return it->first;
        }
        return nullptr;
    }

private:
    QList<QPair<T, int>>              priority_list_;
};

