#pragma once

#include <QtCore>
#include <QtGui>
#include <QtNetwork>


//TODO: delete old files;
class AppUpdate : public QObject {
    Q_OBJECT
public:
    AppUpdate( QObject* parent = NULL);
    ~AppUpdate();

public slots:
    void                check();
    bool                download();
    bool                install();
    bool                isDownloaded() const;
    bool                isAvailable() const { return is_available_; }
    const QString&      version() const { return app_ver_; }

public:
    QString             downloadPath() const;
   
signals:
    void                available();
    void                downloadFinished();
    void                downloadProgress(float progress);
    void                error(QString);

private:
    void                clearOldDownloads() {
        //TODO:
    }
    void                loadConfig();
    void                saveConfig() const;
   // static int      compareVersions(const QString& ver1, const QString& ver2);

private:
    QNetworkAccessManager*                  nam_ = nullptr;

    bool                                    is_available_ = false;
    bool                                    is_downloading_ = false;
    QString                                 downloaded_;
    QString                                 app_ver_;
    QString                                 app_url_;
    QDateTime                               check_;
	QElapsedTimer							elapsed_;
};
