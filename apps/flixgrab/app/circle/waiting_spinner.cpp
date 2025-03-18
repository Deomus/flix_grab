#include "waiting_spinner.h"

// Standard includes
#include <cmath>
#include <algorithm>

// Qt includes
#include <QPainter>
#include <QTimer>

WaitingSpinner::WaitingSpinner(QWidget *parent,
                                           bool centerOnParent,
                                           bool disableParentWhenSpinning)
    : QWidget(parent),
      center_on_parent_(centerOnParent),
      disable_parent_(disableParentWhenSpinning) {
    initialize();
}

WaitingSpinner::WaitingSpinner(Qt::WindowModality modality,
                                           QWidget *parent,
                                           bool centerOnParent,
                                           bool disableParentWhenSpinning)
    : QWidget(parent, Qt::Dialog | Qt::FramelessWindowHint),
      center_on_parent_(centerOnParent),
      disable_parent_(disableParentWhenSpinning){
    initialize();

    // We need to set the window modality AFTER we've hidden the
    // widget for the first time since changing this property while
    // the widget is visible has no effect.
    setWindowModality(modality);
    setAttribute(Qt::WA_TranslucentBackground);
}

void WaitingSpinner::initialize() {
    color_ = Qt::black;
   
    is_spinning_ = false;

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));

    //_timer->setInterval(200);
    //updateSize();
    updateTimer();
    hide();
    time_.start();
}

void WaitingSpinner::paintEvent(QPaintEvent *) {
    updatePosition();
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing, true);
       
    qreal half_pen = line_width_ / 2.f;
    painter.setPen(QPen(color_, line_width_, Qt::SolidLine, Qt::FlatCap));
    QRectF rect = this->rect();
    rect  = rect.marginsRemoved(QMarginsF(half_pen, half_pen, half_pen, half_pen));
    
    first_pos_ += time_.elapsed() * first_speed_ /1000.f;
    second_pos_ += time_.elapsed() * second_speed_ /1000.f;

    qreal first_real_pos = first_pos_;
    qreal second_real_pos = second_pos_;
    qreal delta_pos = second_pos_ - first_pos_;

    if (second_speed_ > first_speed_) {
        //Solid sector;
        if (delta_pos >= (360 - solid_sector_)) {
            second_real_pos = 360 - solid_sector_ + first_pos_;
        }
        if (delta_pos >= 360) {
            //Change speed of points;
            second_speed_ = slow_speed_;
            first_speed_ = fast_speed_;
            second_pos_ = second_real_pos;
        }
    }
    else {
        if (delta_pos <= solid_sector_) {
            first_real_pos = -solid_sector_ + second_pos_;
        }

        if (delta_pos <= 0) {
            //Change speed of points;
            first_speed_ = slow_speed_;
            second_speed_ = fast_speed_;
            first_pos_ = first_real_pos;
        }
    }

    if (first_pos_ > 360) {
        first_pos_ -= 360;
        second_pos_ -= 360;
    }
                       
    time_.restart();
    
    painter.drawArc(rect, -first_real_pos * 16.0, -16.0 * (second_real_pos - first_real_pos));
   
}

void WaitingSpinner::start() {
    updatePosition();
    is_spinning_ = true;
    show();

    first_speed_ = slow_speed_;
    second_speed_ = fast_speed_;

    if(parentWidget() && disable_parent_) {
        parentWidget()->setEnabled(false);
    }

    if (!timer_->isActive()) {
        timer_->start();
        
    }
}

void WaitingSpinner::stop() {
    is_spinning_ = false;
    hide();

    if(parentWidget() && disable_parent_) {
        parentWidget()->setEnabled(true);
    }

    if (timer_->isActive()) {
        timer_->stop();
        
    }
}


QColor WaitingSpinner::color() {
    return color_;
}

void WaitingSpinner::setSpeeds(qreal slow, qreal fast) {
    slow_speed_ = slow;
    fast_speed_ = fast;
    updateTimer();
}

bool WaitingSpinner::isSpinning() const {
    return is_spinning_;
}

void WaitingSpinner::setColor(QColor color) {
    color_ = color;
}


void WaitingSpinner::updateTimer() {
    double max_angle = 180.0 / ( M_PI * qMax(width(), height())); //3px angle
    int interval = qMax(20, qCeil(1000.0 *max_angle / fast_speed_));
    timer_->setInterval(interval);

    //_timer->setInterval(1000 / (_numberOfLines * _revolutionsPerSecond));
}

void WaitingSpinner::updatePosition() {
    if (parentWidget() && center_on_parent_) {
        move(parentWidget()->width() / 2 - width() / 2,
             parentWidget()->height() / 2 - height() / 2);
    }
}
