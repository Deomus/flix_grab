#pragma once

#include <QtWidgets>

enum EPosition {
    kPosition_TopLeft = 0,
    kPosition_TopRight,
};


class NotificationButton : public QPushButton {
    Q_OBJECT
public:
    explicit NotificationButton(QWidget *parent = nullptr);

    explicit NotificationButton(const QString &text, QWidget *parent = nullptr);

    void        setPosition(EPosition pos) { pos_ = pos; }
    EPosition   position() const { return pos_; }

    void        setOverlapping(float factor) { overlapping_factor_ = factor; }
    float       overlapping() const { return overlapping_factor_; }

    void        setNotificationText(const QString& text) {
        int count = text.toInt();
        offset_ = 138;

        if (count >= 1000000)
            offset_ = offset_ - 28;
        else if (count >= 10000)
            offset_ = offset_ - 23;
        else if (count >= 1000)
            offset_ = offset_ - 21;
        else if (count >= 100)
            offset_ = offset_ - 14;
        else if (count > 10)
            offset_ = offset_ - 7;

        if (!notification_->isVisible())
            notification_->show();

        notification_->setText(text);
        notification_->adjustSize();
        updatePos(this->pos(), this->size());
    }

    void        moveEvent(QMoveEvent *event) override {
        updatePos(event->pos(), this->size());

        QPushButton::moveEvent(event);
    }

    void        resizeEvent(QResizeEvent *event) override {

        updatePos(this->pos(), event->size());

        QPushButton::resizeEvent(event);
    }


    void        showNotification() {
        notification_->show();
    }
    void        hideNotification() {
        notification_->hide();
    }

protected:

    void        updatePos(const QPoint& button_pos, const QSize& button_size);


private:
    QLabel*                 notification_;
    EPosition               pos_ = kPosition_TopLeft;
    float                   overlapping_factor_ = 0.3f;
    int                     offset_ = 138;
};
