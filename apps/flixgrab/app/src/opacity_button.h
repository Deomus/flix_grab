#pragma once

#include <QtWidgets>


class OpacityButton : public QPushButton {
    Q_OBJECT
public:
    Q_PROPERTY(bool inverted READ inverted WRITE setInverted);

public:
    explicit OpacityButton(QWidget *parent = nullptr) : QPushButton(parent) {}
    explicit OpacityButton(const QString &text, QWidget *parent = Q_NULLPTR) : QPushButton(text, parent) {}

    void            setOpacity(qreal opacity) {
        opacity_ = opacity;
        update();
    }

    qreal           opacity() const { return opacity_; }

    bool            inverted() const { return is_inverted_; }
    void            setInverted(bool is_inverted = true) { 

        if (is_inverted_ != is_inverted) {
            is_inverted_ = is_inverted;

            if (icon_original_.isNull())
                invertIcon();

            if (is_inverted_)
                setIcon(icon_inverted_);
            else
                setIcon(icon_original_);

            this->style()->polish(this);
            update();
        }
    }
    

private:
    void            invertIcon() {
        icon_original_ = this->icon();

        QImage inverted_image = icon_original_.pixmap(128).toImage();
        inverted_image.invertPixels();
        icon_inverted_.addPixmap(QPixmap::fromImage(inverted_image));
    }

protected:
    void            initPainter(QPainter *painter) const override {
        QWidget::initPainter(painter);
        painter->setOpacity(opacity_);
    }

private:

    qreal               opacity_ = 1.0f;

    bool                is_inverted_ = false;
    QIcon               icon_original_;
    QIcon               icon_inverted_;
};
