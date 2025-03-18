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
    //hide();
    time_.start();
}



void WaitingSpinner::paintEvent(QPaintEvent *) {
    updatePosition();
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(color_);
    painter.setPen(QPen(color_, line_width_, Qt::SolidLine, Qt::FlatCap));

    QRectF rect = this->rect();

    switch (type_) {
        case kSpinnerType_Analize: {
            compress_ = compress_ + reduce_coef_;
            compress_backflow_ = compress_backflow_ + reduce_coef_backflow_;

            if (compress_ <= 0) {
                reduce_coef_ = 1;
                compress_ = 1;
            }

            if (compress_backflow_ <= 0) {
                reduce_coef_backflow_ = 1;
                compress_backflow_ = 1;
            }

            int parent_width = rect.width() + rect.width() / 20;
            int spacing = parent_width / 3 * 0.3;
            int width = (parent_width - spacing * 2) / 3;
            qreal coef = (parent_width - width) / 2;


            rect = rect.adjusted(coef, coef, -coef, -coef);
            QRectF rect2_backflow = rect.adjusted(-rect.width() - spacing, 0, -rect.width() - spacing, 0);
            QRectF rect3_backflow = rect.adjusted(rect.width() + spacing, 0, rect.width() + spacing, 0);

            qreal coef_w = rect.width() * compress_ / max_value;
            qreal coef_h = rect.height() * compress_ / max_value;
            rect.adjust(coef_w, coef_h, -coef_w, -coef_h);


            qreal coef_w_backflow = rect2_backflow.width() * compress_backflow_ / max_value;
            qreal coef_h_backflow = rect2_backflow.height() * compress_backflow_ / max_value;
            rect2_backflow.adjust(coef_w_backflow, coef_h_backflow, -coef_w_backflow, -coef_h_backflow);
            rect3_backflow.adjust(coef_w_backflow, coef_h_backflow, -coef_w_backflow, -coef_h_backflow);

            if (compress_ >= max_value)
                reduce_coef_ = -1;

            if (compress_backflow_ >= max_value)
                reduce_coef_backflow_ = -1;

            time_.restart();

            painter.drawEllipse(rect);
            painter.drawEllipse(rect2_backflow);
            painter.drawEllipse(rect3_backflow);

            break;
        }
        case kSpinnerType_Pending: {
            qreal coef = rect.width() / 3.8;// 9 - big circle;
            rect = rect.adjusted(coef, coef, -coef, -coef);
            qreal half_pen = line_width_ / 2.f;
            rect = rect.marginsRemoved(QMarginsF(half_pen, half_pen, half_pen, half_pen));

            first_pos_ += time_.elapsed() * first_speed_ / 1000.f;
            second_pos_ += time_.elapsed() * second_speed_ / 1000.f;

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

            break;
        }
    }
}

void WaitingSpinner::start() {
    if (type_ == ESpinnerType::kSpinnerType_Pending) {
        first_speed_ = slow_speed_;
        second_speed_ = fast_speed_;
    }
    
    updatePosition();
    is_spinning_ = true;
    //show();



    if(parentWidget() && disable_parent_) {
        parentWidget()->setEnabled(false);
    }

    if (!timer_->isActive()) {
        timer_->start();
    }
}

void WaitingSpinner::stop() {
    is_spinning_ = false;
    //hide();

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

bool WaitingSpinner::isSpinning() const {
    return is_spinning_;
}

void WaitingSpinner::setColor(QColor color) {
    color_ = color;
}


void WaitingSpinner::updateTimer() {
    int interval = 10;
    if (type_ == kSpinnerType_Pending) {
        double max_angle = 180.0 / ( M_PI * qMax(width(), height())); //3px angle
        interval = qMax(20, qCeil(1000.0 *max_angle / fast_speed_)); 
    }
    timer_->setInterval(interval);
    //_timer->setInterval(1000 / (_numberOfLines * _revolutionsPerSecond));
}

void WaitingSpinner::updatePosition() {
    if (parentWidget() && center_on_parent_) {
        move(parentWidget()->width() / 2 - width() / 2,
             parentWidget()->height() / 2 - height() / 2 + 4);  //4 - HACK
    }
}

//SpinnerType_Pending
void WaitingSpinner::setSpeeds(qreal slow, qreal fast) {
    slow_speed_ = slow;
    fast_speed_ = fast;
    updateTimer();
}
