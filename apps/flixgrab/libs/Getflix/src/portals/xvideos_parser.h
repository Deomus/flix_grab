#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


class XVideosParser : public QObject
{
    Q_OBJECT
public:
    XVideosParser(QObject* parent = 0);

    Status                          parseHTML(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info);

private:
    Status                          parsePage(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info);
    void                            getTracksData(const QString& data, TracksInfo& tracks_info, const VideoInfo& video_info);
};