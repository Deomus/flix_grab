#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


class TwitchParser : public QObject
{
    Q_OBJECT
public:
    TwitchParser(QObject* parent = 0);

    Status                          parseHTML(const QString& html, VideoInfo& video_info, TracksInfo& tracks_info, const QString& html_url);

private:
    void                            timeToDuration(const QString& time, VideoInfo& video_info);
    void                            parseVmap(const QString& data, TracksInfo& tracks_info, VideoInfo& video_info);

private:
    //video
    Status                          loadVideoInfo(const QString& id, VideoInfo& video_info);
    Status                          loadVideoToken(const QString& video_id, const QString& client_id, QString &sig, QString &token);
    Status                          loadM3U8Video(const QString& vidoe_id, const QString& sig, const QString& token, TracksInfo& tracks_info, VideoInfo& video_info);
    //Status                          getTracksData(const QString& content, TracksInfo& tracks_info, VideoInfo& video_info);

    //clip
    Status                          loadClipInfo(const QString& clip_id, VideoInfo& video_info);
    Status                          loadClipTracks(const QString& clip_id, const QString& client_id, TracksInfo& tracks_info);
    
    //stream
    Status                          loadStreamId(const QString& login, QString& stream_id);
    Status                          loadStreamInfo(const QString& stream_id, VideoInfo& video_info);
    Status                          loadStreamToken(const QString& login, const QString& client_id, QString &sig, QString &token);
    Status                          loadM3U8Stream(const QString& login, const QString& sig, const QString& token, TracksInfo& tracks_info, VideoInfo& video_info);

private:
    //const QByteArray                api_key_ = "zb8f68630nmr79pqz7tdj2mhu518sk";  from Yahsa
    const QByteArray                api_key_ = "kimne78kx3ncx6brgo4mv6wki5h1ko";  //from ytb-dl
    const QByteArray                client_id_ = "kimne78kx3ncx6brgo4mv6wki5h1ko"; //from ytb-dl
    const QString                   user_agent_ = "Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0";
    
    //video
    QString                         video_info_url_ = "https://api.twitch.tv/kraken/videos/%1";
    QString                         video_token_url_ = "https://api.twitch.tv/api/vods/%1/access_token?oauth_token=undefined&need_https=true&platform=web&player_type=site&player_backend=mediaplayer";
    QString                         M3U8_video_url_ = "https://usher.ttvnw.net/vod/%1.m3u8?allow_source=true&sig=%2&token=%3";
    
    //clip
    QString                         clip_info_url_ = "https://api.twitch.tv/kraken/clips/%1";
    QString                         clip_post_url_ = "https://gql.twitch.tv/gql";
    
    //stream
    QString                         stream_id_url_ = "https://api.twitch.tv/kraken/users?login=%1";
    QString                         stream_info_url_ = "https://api.twitch.tv/kraken/streams/%1";
    QString                         stream_token_url_ = "https://api.twitch.tv/api/channels/%1/access_token?oauth_token=undefined&need_https=true&platform=web&player_type=site&player_backend=mediaplayer";
    QString                         M3U8_stream_url_ = "https://usher.ttvnw.net/api/channel/hls/%1.m3u8?allow_source=true&sig=%2&token=%3";
};