#include "notification_button.h"

NotificationButton::NotificationButton(QWidget *parent /*= nullptr*/) 
    : NotificationButton("", parent) {

}

NotificationButton::NotificationButton(const QString &text, QWidget *parent /*= nullptr*/) 
    : QPushButton(text, parent){
    notification_ = new QLabel(parent);
    notification_->setStyleSheet("color: #F6F6F6; font: normal 10pt;");
    notification_->setAttribute(Qt::WA_TransparentForMouseEvents);

    hideNotification();
}

//void NotificationButton::updatePos(const QPoint& button_pos, const QSize& button_size) {
//    //auto center = notification_->rect().center();
//    int overlapping = notification_->height() * overlapping_factor_;
//    auto pos = button_pos;
//
//    switch (pos_) {
//    case kPosition_TopLeft:
//        //pos -= overlapping_;
//        pos -= QPoint(overlapping, overlapping);
//        break;
//    case kPosition_TopRight:
//        pos += QPoint(button_size.width() - notification_->size().width() + overlapping, -overlapping);
//        break;
//    }
//
//    notification_->move(pos);
//}


void NotificationButton::updatePos(const QPoint& button_pos, const QSize& button_size) {
    //auto center = notification_->rect().center();
    //int overlapping = notification_->height() * overlapping_factor_;
    auto pos = button_pos;

    pos.ry() += button_size.height() / 2 - notification_->height()/2;
    pos.rx() += offset_;

    //switch (pos_) {
    //case kPosition_TopLeft:
    //    //pos -= overlapping_;
    //    pos -= QPoint(overlapping, overlapping);
    //    break;
    //case kPosition_TopRight:
    //    pos += QPoint(button_size.width() - notification_->size().width() + overlapping, -overlapping);
    //    break;
    //}

    notification_->move(pos);
}

