#pragma once

#include <QtCore>
#include <QtNetwork>

#include "abortable.h"


class MutableCookies : public QNetworkCookieJar {
    Q_OBJECT;

public:
    explicit MutableCookies(QObject *parent = nullptr)
        : QNetworkCookieJar(parent)
        , mtx_() {
    };

    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const override {
        QMutexLocker locker(&mtx_);
        return QNetworkCookieJar::cookiesForUrl(url);
    }
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) override {
        QMutexLocker locker(&mtx_);
        return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
    }
    virtual bool insertCookie(const QNetworkCookie &cookie) override {
        QMutexLocker locker(&mtx_);
        return QNetworkCookieJar::insertCookie(cookie);
    }
    virtual bool updateCookie(const QNetworkCookie &cookie) override {
        QMutexLocker locker(&mtx_);
        return QNetworkCookieJar::updateCookie(cookie);
    }
    virtual bool deleteCookie(const QNetworkCookie &cookie) override {
        QMutexLocker locker(&mtx_);
        return QNetworkCookieJar::deleteCookie(cookie);
    }

    MutableCookies* clone(QObject* parent = nullptr) const {
        QMutexLocker locker(&mtx_);
        MutableCookies* cookies = new MutableCookies(parent);
        cookies->setAllCookies(this->allCookies());
        return cookies;
    }

private:
    mutable QRecursiveMutex                  mtx_;
};

bool        downloadToFile(const QString &url_str, const QString &path, Abortable* abortable = nullptr);
bool        downloadToString(const QString& url_str, QString& response, Abortable* abortable = nullptr);
bool        downloadToStringExtended(const QString &url_str, QString &response, const QList<QPair<QByteArray, QByteArray>>& headers = QList<QPair<QByteArray, QByteArray>>(), 
				const QList<QPair<QByteArray, QByteArray >>& url_query_params = QList<QPair<QByteArray, QByteArray >>(), QNetworkCookieJar* cookies = nullptr, Abortable* abortable = nullptr);
bool        downloadToStringPost(const QString &url_str, const QByteArray& post_data, QString &response, const QList<QPair<QByteArray, QByteArray >> &headers = QList<QPair<QByteArray, QByteArray >> (), 
	const QList<QPair<QByteArray, QByteArray >> &url_query_params = QList<QPair<QByteArray, QByteArray >> (), Abortable* abortable = nullptr);
long        getTrackSize(const QString& url);