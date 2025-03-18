#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


class VimeoParser : public QObject
{
    Q_OBJECT
public:
    VimeoParser(QObject* parent = 0);

    Status                          parseHTML(const QString& html_in, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url);

private:
    Status                          parsePage(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url);
    void                            getTracksData(const QString& data, TracksInfo& tracks_info, VideoInfo& video_info);
    qint64                          getVideoSize(const QString& data);
    qint64                          getAudioSize(const QString& data, const QList<QString>& audio_data);
};