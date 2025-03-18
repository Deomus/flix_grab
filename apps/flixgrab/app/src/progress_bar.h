#pragma once

#include <QtWidgets>



class ProgressBarStyle : public QProxyStyle {
public:
    ProgressBarStyle(QStyle *style = nullptr) : QProxyStyle(style) {}

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = Q_NULLPTR) const override {
        if (element == CE_ProgressBarLabel) {
            if (const QStyleOptionProgressBar *bar = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
                QRect leftRect;
                QRect rect = bar->rect;
                QColor textColor = option->palette.text().color();
                QColor alternateTextColor = QColor("black");

                painter->save();
                bool vertical = false, inverted = false;
                vertical = (bar->orientation == Qt::Vertical);
                inverted = bar->invertedAppearance;
                if (vertical)
                    rect = QRect(rect.left(), rect.top(), rect.height(), rect.width()); // flip width and height
                const auto totalSteps = qMax(Q_INT64_C(1), qint64(bar->maximum) - bar->minimum);
                const auto progressSteps = qint64(bar->progress) - bar->minimum;
                const auto progressIndicatorPos = progressSteps * rect.width() / totalSteps;
                if (progressIndicatorPos >= 0 && progressIndicatorPos <= rect.width())
                    leftRect = QRect(rect.left(), rect.top(), progressIndicatorPos, rect.height());
                if (vertical)
                    leftRect.translate(rect.width() - progressIndicatorPos, 0);

                bool flip = (!vertical && (((bar->direction == Qt::RightToLeft) && !inverted) ||
                    ((bar->direction == Qt::LeftToRight) && inverted)));

                QRegion rightRect = rect;
                rightRect = rightRect.subtracted(leftRect);
                painter->setClipRegion(rightRect);
                painter->setPen(flip ? alternateTextColor : textColor);
                painter->drawText(rect, bar->text, QTextOption(Qt::AlignAbsolute | Qt::AlignHCenter | Qt::AlignVCenter));
                if (!leftRect.isNull()) {
                    painter->setPen(flip ? textColor : alternateTextColor);
                    painter->setClipRect(leftRect);
                    painter->drawText(rect, bar->text, QTextOption(Qt::AlignAbsolute | Qt::AlignHCenter | Qt::AlignVCenter));

                }
                painter->restore();

            }
        }
        else
            QProxyStyle::drawControl(element, option, painter, widget);
    }
};


class ProgressBar : public QProgressBar {

public:

    explicit ProgressBar(QWidget *parent = nullptr) : QProgressBar(parent) {
        
    }

    void changeEvent(QEvent* e) override
    {
        if (e->type() == QEvent::StyleChange) {
            //if (proxy_)
            //    proxy_->deleteLater();
            //// Style has been changed.
            //proxy_ = new ProgressBarStyle(style());
        }

        QWidget::changeEvent(e);
    }

    //void paintEvent(QPaintEvent* e) override {

    //    QPainter painter(this);
    //    QStyleOptionProgressBar opt;
    //    initStyleOption(&opt);
    //    if (!proxy_)
    //        proxy_ = new ProgressBarStyle(style());
    //    if (proxy_)
    //        proxy_->drawControl(QStyle::CE_ProgressBar, &opt, &painter, this);
    //    /*QStylePainter paint(this);
    //    QStyleOptionProgressBar opt;
    //    initStyleOption(&opt);
    //    paint.drawControl(QStyle::CE_ProgressBar, opt);*/
    //    
    //}

private:
    ProgressBarStyle*                   proxy_ = nullptr;

};



