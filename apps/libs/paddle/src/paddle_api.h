#pragma once

#include <QtCore>
#include <QtNetwork>

#include "paddle_status.h"
#include "paddle_reply.h"

namespace paddle {
    class PaddleReply;

    class PaddleApi : public QObject {
        Q_OBJECT
    public:
        explicit PaddleApi(QObject* parent = nullptr);
        
        PaddleReply*        post(QString endpoint, const QVariantMap& api_params);


    private slots :
        void                onFinished(QNetworkReply* reply);

    private:
        QNetworkAccessManager           nat_;
        QQueue<PaddleReply*>            queue_;
    };

}

