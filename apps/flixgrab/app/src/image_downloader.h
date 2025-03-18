#pragma once

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#define DEFAULT_CACHE_SIZE      (40 * 1024 * 1024)

class ImageDownloader : public QObject {
    Q_OBJECT
public:

private:
    ImageDownloader(QNetworkReply* reply, QObject* parent = nullptr) 
        : QObject(parent)
        , reply_(reply) {
       
        connect(reply, &QNetworkReply::finished, this, &ImageDownloader::imageLoadFinished);

    }

public:
    ~ImageDownloader() {
        if (reply_) {
            reply_->abort();
            reply_->deleteLater();
        }
    }


private slots:
  
    void imageLoadFinished() {
        if (reply_->error() == QNetworkReply::NoError) {
            QByteArray image_data = reply_->readAll();
            QPixmap pixmap;

            if (pixmap.loadFromData(image_data)){
                emit imageFinished(pixmap);
            }
            else {
                qDebug() << "Cant load image '" << reply_->url();
                emit imageError();
            }
        }
        else {
            qDebug() << "Error in" << reply_->url() << ":" << reply_->errorString();
            emit imageError();
        }

        reply_->deleteLater();
        reply_ = nullptr;
        //cancel();
               
    }

signals:
    void imageFinished(QPixmap image);
    void imageError();
public:

    static ImageDownloader*         download(QString const& url) {
        static QNetworkAccessManager                  nam;

        if (nam.cache() == nullptr) {
            QNetworkDiskCache* disk_cache = new QNetworkDiskCache(&nam);
            QString cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
            disk_cache->setCacheDirectory(QDir::cleanPath(cache_path + QDir::separator() + "image_cache"));
            disk_cache->setMaximumCacheSize(DEFAULT_CACHE_SIZE);
            nam.setCache(disk_cache);

        }
        QUrl qurl(url);
        QNetworkRequest request(qurl);
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

        return new ImageDownloader(nam.get(request), &nam);


    }


private:
    QNetworkReply*              reply_;
        
};