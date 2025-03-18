#pragma once

#include <QtCore>
#include <QtConcurrent>
#include <QtNetwork>

#include "status.h"

//Http content of web;
class WebContent : public QObject {
    Q_OBJECT

public:
    WebContent(QObject* parent = nullptr);
    virtual ~WebContent();

public:

    const QUrl&             url() const { return url_; }

    const QString&          data() const { return data_; }

    const QByteArray&       raw_data() const { return raw_data_; }

    virtual void            invalidate() { valid_ = false; }
    bool                    isValid() const { return valid_; }

    Status                  status() const { return status_; }

    void                    setNetworkAccess(QNetworkAccessManager* nam) { nam_ = nam; }
    QNetworkAccessManager*  networkAccess() const { return nam_; }

    void                    fetch(const QUrl& url);

    void                    get(const QUrl& url, const QList<QPair<QByteArray, QByteArray>>& headers = QList<QPair<QByteArray, QByteArray>>());

    void                    post(QUrl const& url, const QByteArray& post_data, const char* content_type = "", int method = 1);

    void                    reset() { valid_ = false; url_.clear(); };

    void                    add_header(const QString& name, const QString& val);

    void                    abort() { aborted_ = true; if (fetching_ && reply_) reply_->abort(); }

signals:
    void                    finished();
    

protected:
    virtual Status          modifyContent();
    virtual bool            isModifyAsync() const;

	void waitForFinished();

    QString&                mutableData() {
        return data_;
    }

private:
    Q_INVOKABLE void        fetchInternal();

    void                    initModification();
    void                    runModification();

private:
    bool                            valid_ = false;
    QString                         urs_str_;
    QUrl                            url_;
    QString                         data_;
    QByteArray                      raw_data_;
    QByteArray                      post_data_;
    QString                         content_type_;
    int                             method_ = 0; // 0 = get, 1 = post, 2 = put
    std::map<QString, QString>      custom_headers_;
    Status                          status_;
    bool                            fetching_ = false;
    bool                            aborted_ = false;
    QNetworkReply*                  reply_ = nullptr;

    //QPointer<QNetworkAccessManager> nam_;
    QNetworkAccessManager*          nam_ = nullptr;

    QFutureWatcher<Status>          watcher_;

};
