#include "../include/app_update.h"

#include <QtCore>
#include <QtNetwork>
#include <QApplication>

#include <company_info.h>

#define SETTINGS_GROUP                          "Update"
#define VERSION_PROPERTY                        "Version"
#define URL_PROPERTY                            "Url"
#define CHECK_DATE_PROPERTY                     "CheckDate"
#define DOWNLOADED_PROPERTY                     "DownloadedPath"

#define UPDATE_PERIOD                           (60*60*1) //Every 1 hour check version;

//Test Purposes;
//#define UPDATE_PERIOD                           (2*60)



inline int      compareVersions(const QString& ver1, const QString& ver2) {
    //Only Major.Minor.Micro compare;
    QStringList ver1_nums = ver1.split('.', Qt::SkipEmptyParts);
    QStringList ver2_nums = ver2.split('.', Qt::SkipEmptyParts);

    int size = qMin(ver1_nums.size(), ver2_nums.size());

    for (int i = 0; i < qMin(size, 3); ++i) {
        int v1_num = ver1_nums[i].toInt();
        int v2_num = ver2_nums[i].toInt();
        if (v1_num < v2_num)
            return -1;
        else if (v1_num > v2_num)
            return 1;
    }

    return 0;
}

AppUpdate::AppUpdate(QObject* parent /*= NULL*/) 
    : QObject(parent) {
    nam_ = new QNetworkAccessManager(this);
    loadConfig();

    qDebug() << "Last remote version: " << app_ver_;

    qint64 expiration_time = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()
        - check_.toMSecsSinceEpoch();

    if (expiration_time >= 0)
        check();
}

AppUpdate::~AppUpdate() {
    saveConfig();
}

void AppUpdate::check() {
    check_ = QDateTime::currentDateTimeUtc().addSecs(UPDATE_PERIOD);
    QStringList ver_numbers = QApplication::applicationVersion().
        split('.', Qt::SkipEmptyParts);

    QString update_url = COMPANY_VENDING_URL;
    QUrl url = QUrl(update_url);
    QNetworkRequest request_version(url);
    QNetworkReply* reply = nam_->get(request_version);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            const auto& content = reply->readAll();
            if (!content.isEmpty()) {
                const auto doc = QJsonDocument::fromJson(content).object();
                const auto app = doc.value(QApplication::applicationName()).toObject();
                if (!app.isEmpty()) {
                    app_ver_ = app.value("version").toString();
                    qDebug() << "Checked remote version: " << app_ver_;

                    app_url_ = app.value("download_url").toString();
                    is_available_ = compareVersions(QApplication::applicationVersion(), app_ver_) < 0;

                    if (is_available_)
                        emit available();

                    saveConfig();
                }
            }

            if (!isDownloaded()
                && !downloaded_.isEmpty())
                QFile(downloaded_).remove();
        }
        else {
            qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        }

        reply->deleteLater();
    });

    saveConfig();
}

bool AppUpdate::download() {

    if (app_ver_.isEmpty())
        return false;

    if (is_downloading_)
        return false;

    QString app_path = downloadPath();

   
    QFile* app_file = new QFile(app_path, this);
    if (app_file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QNetworkRequest request_app(app_url_);

        QNetworkReply* reply = nam_->get(request_app);
        elapsed_.start();
        is_downloading_ = true;
        connect(reply, &QNetworkReply::finished, [=]() {

            app_file->close();

            downloaded_.clear();
            
            if (reply->error() == QNetworkReply::NoError) {
                downloaded_ = app_file->fileName();
                emit downloadFinished();
            }
            else {
                qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                app_file->remove();
                emit error(reply->errorString());
            }
            clearOldDownloads();
            saveConfig();

            app_file->deleteLater();
            reply->deleteLater();
            is_downloading_ = false;
        });

        connect(reply, &QNetworkReply::readyRead, [=]() {
            app_file->write(reply->readAll());
        });

        connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 bytesTotal) {
            if (elapsed_.hasExpired(300)) {
                elapsed_.start();
                if (bytesTotal > 0) {
                    int progress = (int)(bytesRead * 1000.0 / (double)bytesTotal);

                    float progress_float = progress / 10.f;
                    qDebug() << "Update: " << progress_float << "% : " << bytesRead / 1000000.0 << "/" << bytesTotal / 1000000.0 << " MB";
                    emit downloadProgress(progress_float);
                }
                else
                    qDebug() << "Update Downloading: " << bytesRead / 1000000.0 << " MB";
            }

        });

        return true;

    }
        /*else {
            qDebug() << "Error in" << app_path << ":" << app_file->errorString();
            emit error(app_file->errorString());
        }*/
  

    return false;
}

bool AppUpdate::install() {
    if (isDownloaded())
        return QProcess::startDetached(QString("%1 /silent").arg(downloadPath()));
    return false;
}

bool AppUpdate::isDownloaded() const {
    QString update_path = downloadPath();
    return (update_path == downloaded_) && QFile::exists(update_path);
}

QString AppUpdate::downloadPath() const {
    QString  update_dir = QString("%1/updates").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QDir dir(update_dir);

    if (!dir.exists())
        dir.mkpath(update_dir);

    return dir.filePath(QString("%1.exe").arg(app_ver_));
}

void AppUpdate::loadConfig() {
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP);
    app_ver_ = settings.value(
        VERSION_PROPERTY,
        QApplication::applicationVersion()).toString();
    app_url_ = settings.value(URL_PROPERTY).toString();

    check_ = settings.value(
        CHECK_DATE_PROPERTY,
        QDateTime::currentDateTimeUtc()).toDateTime();

    downloaded_ = settings.value(
        DOWNLOADED_PROPERTY, false).toString();

           
    settings.endGroup();

    is_available_ = compareVersions(QApplication::applicationVersion(), app_ver_) < 0;

    if (!is_available_ || (!downloaded_.isEmpty()
        && !QFile::exists(downloaded_)))
        downloaded_.clear();
}

void AppUpdate::saveConfig() const {
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP);
    settings.setValue(VERSION_PROPERTY, app_ver_);
    settings.setValue(URL_PROPERTY, app_url_);
    settings.setValue(CHECK_DATE_PROPERTY, check_);
    settings.setValue(DOWNLOADED_PROPERTY, downloaded_);
    settings.endGroup();
}
