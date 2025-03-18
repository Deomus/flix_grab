#include "tracks_info.h"


Q_DECL_CONSTEXPR inline bool operator==(const VideoTrackInfo& lhs, const VideoTrackInfo& rhs) Q_DECL_NOTHROW
{ return lhs.bitrate == rhs.bitrate; }

Q_DECL_CONSTEXPR inline bool operator==(const AudioTrackInfo& lhs, const AudioTrackInfo& rhs) Q_DECL_NOTHROW
{ return lhs.bitrate == rhs.bitrate; }

Q_DECL_CONSTEXPR inline bool operator==(const ImageTrackInfo& lhs, const ImageTrackInfo& rhs) Q_DECL_NOTHROW
{
    return lhs.width == rhs.width;
}

