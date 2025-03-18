#pragma once

#include <QtCore>
#include <QtNetwork>

#include "paddle_status.h"

namespace paddle {

    class PaddleReply : public QObject {
        friend class PaddleApi;

        Q_OBJECT
    public:
       

        ~PaddleReply();

    public:

        const QVariantMap&  result() const { return result_; }



        const Status&       status() const { return status_; }

        QNetworkReply*      reply() const { return reply_; }

    signals:
        void                response(const QVariantMap& result);
        void                error();

        private slots :
        void                parse();



    private:
        explicit PaddleReply(QString endpoint, const QVariantMap& api_params, QNetworkAccessManager* nat);

        void                init(QString endpoint, const QVariantMap& api_params);
        void                post();

    private:
        QNetworkAccessManager*          nat_ = nullptr;
        QNetworkReply*                  reply_ = nullptr;
        QUrlQuery                       query_;
        QNetworkRequest                 request_;

        QVariantMap                     result_;

        Status                          status_;

    };
}