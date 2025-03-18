#include "get_utils.h"

#include <QtNetwork>
#include <QSslSocket>

#include <utils/http_headers.h>
#include <utils/http.h>

#include "abortable.h"


bool downloadToFile(const QString &url_str, const QString &path, Abortable *abortable) {
    QUrl                    url(url_str);

	if (abortable && abortable->isAborted()) {
		return false;
	}

    QNetworkAccessManager   nam;

    if (!QSslSocket::supportsSsl()) {
        qDebug("SSL initialization error!");
        return false;
    }

    QNetworkRequest         request(url);
    QNetworkReply*          reply = nam.get(request);
    QEventLoop              loop;
    bool                    result = false;
	QMetaObject::Connection aborted_connection;

	if (abortable) {
		aborted_connection = QObject::connect(abortable, &Abortable::aborted, [=] {
			reply->abort();
			});
	}

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //Check if Error;
    if (QNetworkReply::NoError == reply->error()) {
        QString player_script(reply->readAll());

        QFile file_out(path);
        if (file_out.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream_injected(&file_out);
            stream_injected << player_script;
            result = true;
        }
    }
    else {
        qDebug() << "Download data using get to file " << url_str << " error: " << reply->errorString() << " file path: " << path;
    }

	QObject::disconnect(aborted_connection);

	delete reply;

    qApp->processEvents();

    return result;
}

bool downloadToString(const QString& url_str, QString& response, Abortable* abortable) {

	if (abortable && abortable->isAborted()) {
		return false;
	}

    QUrl                    url(url_str);
    QNetworkAccessManager   nam;

    if (!QSslSocket::supportsSsl()) {
        qDebug("SSL initialization error!");
        return false;
    }

    QNetworkRequest         request(url);
    QNetworkReply* reply = nam.get(request);
    QEventLoop              loop;
    bool                    result = false;
	QMetaObject::Connection aborted_connection;

	if (abortable) {
		aborted_connection = QObject::connect(abortable, &Abortable::aborted, [=] {
			reply->abort();
			});
	}

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //Check if Error;
    if (QNetworkReply::NoError == reply->error()) {
        response = reply->readAll();
        result = true;

    }
    else {
        qDebug() << "Download script " << url_str << " error: " << reply->errorString();
    }

	QObject::disconnect(aborted_connection);

	delete reply;

    qApp->processEvents();

    return result;
}

bool downloadToStringExtended(const QString &url_str, QString &response, const QList<QPair<QByteArray, QByteArray>>& headers, const QList<QPair<QByteArray, QByteArray>>& url_query_params, 
	QNetworkCookieJar* cookies, Abortable* abortable) {

	if (abortable && abortable->isAborted()) {
		return false;
	}

    QNetworkAccessManager   nam;

    if (!QSslSocket::supportsSsl()) {
        qDebug("SSL initialization error!");
        return false;
    }

    if (cookies) {
        nam.setCookieJar(cookies);
        cookies->setParent(0);
    }

    QNetworkRequest request;
    for (int i = 0; i < headers.size(); i++) {
        QPair<QByteArray, QByteArray> pair = headers.at(i);
        request.setRawHeader(pair.first, pair.second);
    }

    QUrlQuery params;
    for (int i = 0; i < url_query_params.size(); i++) {
        QPair<QByteArray, QByteArray> pair = url_query_params.at(i);
        params.addQueryItem(pair.first, pair.second);
    }

    QUrl url(url_str);
    url.setQuery(params.query());
    request.setUrl(url);

    QNetworkReply*          reply = nam.get(request);
    QEventLoop              loop;
    bool                    result = false;
	QMetaObject::Connection aborted_connection;

	if (abortable) {
		aborted_connection = QObject::connect(abortable, &Abortable::aborted, [=] {
			reply->abort();
			});
	}

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //Check if Error;
    if (QNetworkReply::NoError == reply->error()) {
        response = reply->readAll();
#if _DEBUG
        qDebug() << "response: " << response;
#endif
        result = true;
    }
    else {
        qDebug() << "Download data using get " << url_str << " error: " << reply->errorString();
    }

	QObject::disconnect(aborted_connection);

	delete reply;

    qApp->processEvents();

    return result;
}

bool downloadToStringPost(const QString &url_str, const QByteArray& post_data, QString &response, const QList<QPair<QByteArray, QByteArray>>& headers, 
	const QList<QPair<QByteArray, QByteArray>>& url_query_params, Abortable* abortable) {

	if (abortable && abortable->isAborted()) {
		return false;
	}

    QNetworkAccessManager   nam;

    if (!QSslSocket::supportsSsl()) {
        qDebug("SSL initialization error!");
        return false;
    }

    QNetworkRequest request;
    for (int i = 0; i < headers.size(); i++) {
        QPair<QByteArray, QByteArray> pair = headers.at(i);
        request.setRawHeader(pair.first, pair.second);
    }

    QUrlQuery params;
    for (int i = 0; i < url_query_params.size(); i++) {
        QPair<QByteArray, QByteArray> pair = url_query_params.at(i);
        params.addQueryItem(pair.first, pair.second);
    }

    QUrl url(url_str);
    url.setQuery(params.query());
    request.setUrl(url);

    QNetworkReply*          reply = nam.post(request, post_data);
    QEventLoop              loop;
    bool                    result = false;
	QMetaObject::Connection aborted_connection;

	if (abortable) {
		aborted_connection = QObject::connect(abortable, &Abortable::aborted, [=] {
			reply->abort();
			});
	}

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //Check if Error;
    if (QNetworkReply::NoError == reply->error()) {
        response = reply->readAll();
#if _DEBUG
        qDebug() << response;
#endif
        result = true;
    }
    else {
        qDebug() << "Download data using post " << url_str << " error: " << reply->errorString();
    }

	QObject::disconnect(aborted_connection);

	delete reply;

    qApp->processEvents();

    return result;
}

long getTrackSize(const QString& url) {
    // get http-file size ..
    qApp->processEvents();
    try {
        utils::HttpHeaders headers;
        auto http = std::make_unique<utils::Http>();
        http->Open(url.toLocal8Bit().data());
        http->Headers(&headers);
        return headers.ContentLength();
    }
    catch (...)
    {
    }
    return -1;
}
