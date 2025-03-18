#include "paddle_api.h"
#include "paddle_keys.h"
#include "paddle_reply.h"

using namespace paddle;


PaddleApi::PaddleApi(QObject* parent /*= nullptr*/) 
    : QObject(parent) {
    QObject::connect(&nat_, &QNetworkAccessManager::finished, 
                    this, &PaddleApi::onFinished/*, Qt::QueuedConnection*/);
}

void PaddleApi::onFinished(QNetworkReply* reply) {
    Q_ASSERT(reply == queue_.head()->reply());

    if (reply == queue_.dequeue()->reply()
        && !queue_.isEmpty()) {
        queue_.head()->post();
    }
}

PaddleReply* PaddleApi::post(QString endpoint, const QVariantMap& api_params) {
    PaddleReply* reply = new PaddleReply(endpoint, api_params, &nat_);

    queue_.enqueue(reply);

    if (queue_.head() == reply)
        reply->post();
    return reply;
}
