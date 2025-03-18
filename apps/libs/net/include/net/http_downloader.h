#pragma once 

#include <QtCore>
#include <QtNetwork>


namespace net {
    
    class HttpDownloader : public QObject {
        Q_OBJECT
    public:
        enum FileContent {
            Continue = 0,
            Append,
            Truncate
        };

        const static qint64    DefaultChunkSize = 1024 * 1024 * 2;

    public:

        HttpDownloader(QString const& path, QObject* parent = nullptr);
        ~HttpDownloader();

        qint64      progress() const;
        qint64      size() const;

        qint64      chunkSize() const;
        void        setChunkSize(qint64 size = DefaultChunkSize);

        int         httpStatus() const;
        QString     errorString() const;

        bool        isOk() const;
        bool        isDownloading() const;
        bool        isFinished() const;

        void        download(QUrl const& url,
            FileContent content = Continue,
            const QByteArray& post_data = NULL);

        void        pause();
        void        resume();

    signals:
        void        progress(qint64 received, qint64 total);
        void        error();
        void        finished();

    protected slots:
        void        dataProgress(qint64 received, qint64 total);
        void        handleFinishedReply();
        void        continueDownload();

    private:
        QNetworkAccessManager                   nam_;

        QByteArray                              post_data_;
        QNetworkRequest                         request_;

        QNetworkReply*                          reply_ = nullptr;

        QFile                                   file_;

        QElapsedTimer                           timer_;
        qint64                                  speed_ = 0;

        //qint64                                  size_ = 0;
        qint64                                  file_start_pos_ = 0;
        qint64                                  progress_ = 0;
        qint64                                  current_progress_ = 0;
        qint64                                  current_size_ = 0;
        qint64                                  requested_size_ = 0;
        qint64                                  chunk_size_ = 0;

    private:
        Q_DISABLE_COPY(HttpDownloader)
    };
}

