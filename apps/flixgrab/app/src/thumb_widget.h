#pragma once

#include <QtWidgets>

#include "image_downloader.h"

class ThumbWidget : public QWidget
{
    Q_OBJECT
public:
    ThumbWidget(QWidget* parent) 
        : QWidget(parent) {
        setDefaultBackground();
    }

    ~ThumbWidget() {
        
    }

public:
    //void            setOpacity(qreal opacity) {
    //    opacity_ = opacity;
    //    update();
    //}

    //qreal           opacity() const { return opacity_; }

    void     setBackground(const QString& artwork_path) {
        artwork_path_ = artwork_path;
    }

public slots:
    void    loadImage(const QString& url) {
        if (url == url_)
            return;

        image_downloader_.reset(ImageDownloader::download(url));
        setDefaultBackground();
        QObject::connect(image_downloader_.data(), &ImageDownloader::imageFinished, [=](QPixmap pixmap) {
            palette_.setBrush(QPalette::Window, QBrush(pixmap.scaled(this->size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
            this->setAutoFillBackground(true);
            this->setPalette(palette_);
            artwork_path_.clear();
            //this->setScaledContents(true);
            //this->setPixmap(pixmap);

            //image_downloader_->deleteLater();
            //freeHandle();
        });

        QObject::connect(image_downloader_.data(), &ImageDownloader::imageError, [=]() {
            if (!artwork_path_.isEmpty()) {
                palette_.setBrush(QPalette::Window, QBrush(QPixmap(artwork_path_)));
                this->setAutoFillBackground(true);
                this->setPalette(palette_);

                artwork_path_.clear();
            }
        });

        url_ = url;
    }

    void    cancel() {
     
        image_downloader_.reset();
        
        //palette_.setBrush(QPalette::Background, QColor("#4E5164"));
        //this->setAutoFillBackground(true);
        //this->setPalette(palette_);
        //this->setStyleSheet("#" + this->objectName() + "{border: 1 solid #757577;}");
        //this->clear();
        //setDefault();
        url_.clear();
    }

//protected:
//    void            initPainter(QPainter *painter) const override {
//        QWidget::initPainter(painter);
//        painter->setOpacity(opacity_);
//    }

private:
    void     setDefaultBackground() {
        if (!artwork_path_.isEmpty()) {
            QPixmap pixmap(artwork_path_);
            palette_.setBrush(QPalette::Window, QBrush(pixmap.scaled(this->size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
            this->setAutoFillBackground(true);
            this->setPalette(palette_);
        }
        else {
            this->setAutoFillBackground(true);
            palette_.setBrush(QPalette::Window, QColor("#4E5164"));
            this->setPalette(palette_);
        }
    }

private:
    QScopedPointer<ImageDownloader>     image_downloader_;
    QString                             url_;
    QPalette                            palette_;
    QString                             artwork_path_;
    //qreal                               opacity_ = 1.0f;

};


