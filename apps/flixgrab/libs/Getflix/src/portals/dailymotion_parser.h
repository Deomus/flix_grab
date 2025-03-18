#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


class DailymotionParser : public QObject
{
    Q_OBJECT
public:
    DailymotionParser(QObject* parent = 0);

    Status                          parseHTML(const QString& html, VideoInfo& info, TracksInfo& tracks_info, const QString& url);

private:
    Status                          parsePage(const QString& html, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url);
    Status                          getTracksData(const QString& html, TracksInfo& tracks_info, VideoInfo& video_info, const QString& url);
};