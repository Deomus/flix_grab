#pragma once

#include <QtWidgets>


//Checkbox With TExt alignment to Left (or Right)
class Checkbox : public QCheckBox {

public:
    explicit Checkbox(QWidget *parent = Q_NULLPTR) : QCheckBox(parent){};
    explicit Checkbox(const QString &text, QWidget *parent = Q_NULLPTR) : QCheckBox(text, parent) {};


    bool    isTransAction() const { return trans_action_; }
    void    setTransAction( bool trans = true) { trans_action_ = trans; }

protected:
    bool hitButton(const QPoint &pos) const {
        Q_UNUSED(pos)
        //Full Box of widget is used;
        return true;

    }

    void mouseMoveEvent(QMouseEvent* e) override {
        
        QCheckBox::mouseMoveEvent(e);
        if (trans_action_)
            e->ignore();
                    
    }

    void mousePressEvent(QMouseEvent* e) override {

        QCheckBox::mousePressEvent(e);
        if (trans_action_)
            e->ignore();

    }

    void mouseReleaseEvent(QMouseEvent* e) override {
        
        QCheckBox::mouseReleaseEvent(e);
        if (trans_action_)
            e->ignore();

    }

    void paintEvent(QPaintEvent *) override {
        QStylePainter p(this);
        QStyleOptionButton opt;
        initStyleOption(&opt);
        opt.text.clear();
        
        p.drawControl(QStyle::CE_CheckBox, opt);

        opt.rect = style()->subElementRect(QStyle::SE_CheckBoxContents, &opt, this);
        opt.direction = (Qt::LayoutDirection)(opt.direction ^ (Qt::LeftToRight | Qt::LeftToRight));
        opt.text = text();
        p.drawControl(QStyle::CE_CheckBoxLabel, opt);
    }

private:
    bool            trans_action_ = false;

};
