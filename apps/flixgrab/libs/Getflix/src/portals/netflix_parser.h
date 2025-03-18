#pragma once

#include <QtGui>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


namespace parser {
    Status          parseMetadata(const QString& metadata, VideoInfo& info, const QString& base_url);
    Status          parseManifest(const QJsonObject& manifest, TracksInfo& info);
};
