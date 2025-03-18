#pragma once

#include <QtGui>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


namespace parser {
    Status          parseMetadata(const QString& metadata, VideoInfo& info, TracksInfo& tracks_info, QList<QString>& audio_tracks, QString& mpd_url);
};
