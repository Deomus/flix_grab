#include "paddle_reply.h"

using namespace paddle;

PaddleReply::PaddleReply(QString endpoint, const QVariantMap& api_params, QNetworkAccessManager* nat) : QObject(nat)
, nat_(nat) {
    init(endpoint, api_params);
}

PaddleReply::~PaddleReply() {
    if (reply_)
        reply_->deleteLater();
}


void PaddleReply::init(QString endpoint, const QVariantMap& api_params) {
    QUrl url = QUrl("https://v3.paddleapi.com/3.1/" + endpoint);

    for (auto it = api_params.constBegin(); it != api_params.constEnd(); ++it) {
        query_.addQueryItem(it.key(), it.value().toString());
    }

    request_.setUrl(url);
    request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
}


void PaddleReply::parse() {
    if (reply_) {
        QJsonDocument result_doc = QJsonDocument::fromJson(reply_->readAll());
        QJsonObject result_obj = result_doc.object();

        if (!result_obj.isEmpty() && result_obj.contains("success")) {
            if (result_obj["success"].toBool()) {
                QJsonObject response_obj = result_obj["response"].toObject();

                result_ = response_obj.toVariantMap();

            }
            else {
                //Check Error;
                QJsonObject error_obj = result_obj["error"].toObject();

                result_ = error_obj.toVariantMap();
                status_ = Status(error_obj["code"].toInt(),
                    error_obj["message"].toString());
                emit error();
            }
        }
        else {
            status_ = Status(reply_->error(),
                reply_->errorString());

            emit error();
        }
        emit response(result_);

        /* reply_->deleteLater();
        reply_ = nullptr;*/
    }
}

void PaddleReply::post() {
    if (reply_ == nullptr) {
        reply_ = nat_->post(request_, query_.query(QUrl::FullyEncoded).toUtf8());
        QObject::connect(reply_, &QNetworkReply::finished, this, &PaddleReply::parse);
    }
}

