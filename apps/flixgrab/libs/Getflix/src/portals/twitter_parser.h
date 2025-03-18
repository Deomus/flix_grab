#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"


class TwitterParser : public QObject
{
    Q_OBJECT
public:
    TwitterParser(QNetworkAccessManager* nam, QObject* parent = 0);

    Status                          parseHTML(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url);

private:
    Status                          getRequest(QNetworkRequest, QByteArray&);
    Status                          postRequest(QNetworkRequest request, QByteArray &data, const QByteArray& body = "");
    Status                          loadToken(QString& token);

    //video
    Status                          loadVideo(const QString& video_id, const QString& token, VideoInfo& video_info, TracksInfo& tracks_info);
    Status                          loadM3U8(const QString& url, TracksInfo& tracks_info);
    Status                          parseM3U8(const QString& data, const QString& url, TracksInfo& tracks_info);
    Status                          loadVmap(const QString& url, VideoInfo& video_info, TracksInfo& tracksInfo);

    //broadcast
    Status                          loadBroadcast(const QString& broadcast_id, const QString& token, VideoInfo& video_info, TracksInfo& tracks_info);
    Status                          loadBroadcastStream(const QString& mediaKey, const QString& token, TracksInfo& tracks_info);

    //twit блочится не используется оставил на всякий
    Status                          loadTwit(QString id);

private:
    QNetworkAccessManager*          nam_ = nullptr;
    const QString                   user_agent_ = "Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0";

    const QString                   bearer_ = "Bearer AAAAAAAAAAAAAAAAAAAAAPYXBAAAAAAACLXUNDekMxqa8h%2F40K4moUkGsoc%3DTYfbDKbT3jJPCEVnMYqilB28NHfOPqkca3qaAxGfsyKCs0wRbw";
    const QString                   token_url_ = "https://api.twitter.com/1.1/guest/activate.json";

    //video
    QString                         video_info_url_ = "https://api.twitter.com/1.1/videos/tweet/config/%1.json";
    const QString                   video_re_str_ = "^https://video.twimg.com/.*\\.m3u8.*";

    //broadcast
    QString                         broadcast_info_url_ = "https://api.twitter.com/1.1/broadcasts/show.json?ids=%1";
    const QString                   broadcast_re_str_ = "^https://twitter.com/i/broadcasts/(.+)$";

    //twit блочится не используется оставил на всякий
    QString                         twit_info_url_ = "https://api.twitter.com/1.1/statuses/show.json?id=%1";
};