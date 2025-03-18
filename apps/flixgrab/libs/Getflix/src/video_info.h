#pragma once

#include <QtCore>

enum class PlaylistKind {
    kPlaylist = 0,
    kSeason,
    kEmptyPlaylist, //youtube case
	kPlaylistPending,
};

struct VideoInfo {
    QString                         video_title;
    QString                         artwork_url;
    int                             duration = 0;               //Duration in seconds;
	bool							is_live_content = false;
    QString                         url_type;
	QString							playlist_id;
    QList<QPair<int, QList<QPair<QString, QString>>>> playlist; //QList<QPair<season_muber, QList<QPair<video_title + video_duration, video_url>>>>
    PlaylistKind                    playlist_kind = PlaylistKind::kSeason;
    bool                            enable_metadata = false;
    QString                         metadata_artwork_url;       //maximum size of albumart image (128KB)
    QString                         metadata_title;
    QString                         metadata_album;
    QString                         metadata_track;
    QString                         metadata_year;
    QString                         metadata_comment;
    QString                         metadata_genre;             //audio only
    QString                         metadata_artist;            //audio only
    QString                         metadata_album_artist;      //audio only
};
Q_DECLARE_METATYPE(VideoInfo)