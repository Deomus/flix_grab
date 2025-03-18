#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


class InstagramParser : public QObject
{
    Q_OBJECT
public:
    InstagramParser(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* parent = 0);

    Status          parseHTML(const QString& html_in, VideoInfo& info, TracksInfo& tracks_info, const QString& url);

private:
    Status                          parsePage(QString& html, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url);
    void                            getTracksData(const QString& html, TracksInfo& tracks_info, VideoInfo& video_info);
    void                            formatTitle(QString& title);

private:
    QNetworkAccessManager*          nam_ = nullptr;
    QWebEnginePage*                 page_ = nullptr;
};