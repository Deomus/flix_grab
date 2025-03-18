#include "twitch_parser.h"

#include "get_utils.h"


TwitchParser::TwitchParser(QObject* parent /*= 0*/) : QObject(parent) {}

void TwitchParser::timeToDuration(const QString& time, VideoInfo& video_info) {
    QList<QString> time_list = time.split(".");
    QString hour;
    QString min;
    QString sec;
    QString msec;
    if (!time_list.isEmpty()) {
        if (time_list.size() > 3) {
            hour = time_list[0];
            min = time_list[1];
            sec = time_list[2];
            msec = time_list[3];
        }
        else if (time_list.size() > 2) {
            min = time_list[0];
            sec = time_list[1];
            msec = time_list[2];
        }
        else if (time_list.size() == 2) {
            sec = time_list[0];
            msec = time_list[1];
        }
        else if (time_list.size() == 1) {
            sec = time_list[0];
        }
        else {
            msec = time_list[0];
        }
    }
    int add_sec = 0;

    if (msec.toInt() > 500)
        add_sec = 1;

    video_info.duration = hour.toInt() * 60 * 60 + min.toInt() * 60 + sec.toInt() + add_sec;
}

void TwitchParser::parseVmap(const QString& data, TracksInfo& tracks_info, VideoInfo& video_info) {
    QRegExp streamRe("^#EXT-X-STREAM-INF:.*$");
    QRegExp infoRe("^#EXT-X-MEDIA:.*$");
    QRegExp nameRe("NAME=\"(.*)$");
    QRegExp bandWidhtRe("BANDWIDTH=(\\d+),");
    QRegExp widthRe("RESOLUTION=(\\d+)x");
    QRegExp heightRe("x(\\d+),");

    QStringList string_list = data.split("\n");
    bool urlString = false;
    int frame_rate = 30;

    foreach(QString str, string_list) {
        if (infoRe.exactMatch(str)) {
            int pos = nameRe.indexIn(str);
            if (pos > -1) {
                QString str = nameRe.cap(1).split("\"").first();
                frame_rate = str.split("p").last().toInt();
            }
        }
        if (streamRe.exactMatch(str)) {
            tracks_info.video_tracks.push_back(VideoTrackInfo());
            tracks_info.video_tracks.back().framerate = frame_rate;

            int pos = bandWidhtRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().bitrate = bandWidhtRe.cap(1).toInt();

            pos = widthRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().width = widthRe.cap(1).toInt();

            pos = heightRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().height = heightRe.cap(1).toInt();

            normalizeTrack(tracks_info.video_tracks.back());
            tracks_info.calculateTrackSize(tracks_info.video_tracks.back(), video_info);
            normalizeVideoTrack(tracks_info.video_tracks.back());

            urlString = true;
            continue;
        }
        if (urlString) {
            QString data;
            if (downloadToString(str, data)) {
                QString base_url = str.split(str.split("/").last()).first();

                QStringList stringList = data.split("\n");
                foreach(QString str, stringList) {
                    if (str.contains(".ts")) {
                        tracks_info.video_tracks.back().urls.push_back(base_url + str);
                        tracks_info.video_tracks.back().is_chunked = true;
                    }
                }
            }
            else {
                tracks_info.video_tracks.removeLast();
            }

            urlString = false;
        }
    }
}

Status TwitchParser::loadVideoInfo(const QString& video_id, VideoInfo& video_info) {
    Status status;

    QString data;
    QString url = QString(video_info_url_).arg(video_id);
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Accept"), QByteArray("application/vnd.twitchtv.v5+json")));
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(api_key_)));
    if (!downloadToStringExtended(url, data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject json_obj = doc.object();

    QString title = json_obj.find("title")->toString();
    video_info.video_title = normalizeTitle(title);

    QString duration = QString::number(json_obj.find("length")->toDouble());
    if (!duration.isEmpty())
        timeToDuration(duration, video_info);

    QJsonObject preview_obj = json_obj.find("preview")->toObject();
    if (preview_obj.size()) {
        QString thumb = preview_obj.find("medium")->toString();
        if (thumb.isEmpty())
            thumb = preview_obj.begin().value().toString();

        if (!thumb.isEmpty())
            video_info.artwork_url = thumb;
    }

    return status;
}

Status TwitchParser::loadVideoToken(const QString& video_id, const QString& client_id, QString &sig, QString &token) {
    Status status;

    QString url = QString(video_token_url_).arg(video_id);
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(client_id.toLocal8Bit())));
    if (!downloadToStringExtended(url, data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject json_obj = doc.object();
    sig = json_obj.find("sig")->toString();
    if (sig.isEmpty()) {
        status.update(EStatusCode::ParseError, "Data not found");
        return status;
    }
    token = json_obj.find("token")->toString();
    if (token.isEmpty()) {
        status.update(EStatusCode::ParseError, "Data not found");
        return status;
    }
    return status;
}

Status TwitchParser::loadM3U8Video(const QString& vidoe_id, const QString& sig, const QString& token, TracksInfo& tracks_info, VideoInfo& video_info) {
    Status status;

    QString str = QString(M3U8_video_url_).arg(vidoe_id).arg(sig).arg(token);
    str.replace("\"", "%22");

    QString data;
    if (!downloadToString(str, data))
        return status.update(EStatusCode::ParseError, "Data not found");

    parseVmap(data, tracks_info, video_info);
    return status;
}

Status TwitchParser::loadClipInfo(const QString& clip_id, VideoInfo& video_info) {
    Status status;
    QString data;
    QString url = QString(clip_info_url_).arg(clip_id);

    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Accept"), QByteArray("application/vnd.twitchtv.v5+json")));
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(api_key_)));
    if (!downloadToStringExtended(url, data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject json_obj = doc.object();

    QString title = json_obj.find("title")->toString();
    video_info.video_title = normalizeTitle(title);

    QString duration = QString::number(json_obj.find("duration")->toDouble());
    if (!duration.isEmpty())
        timeToDuration(duration, video_info);

    QJsonObject thumbnails = json_obj.find("thumbnails").value().toObject();
    if (thumbnails.size())
        video_info.artwork_url = thumbnails.begin().value().toString();

    return status;
}

Status TwitchParser::loadClipTracks(const QString& clip_id, const QString& client_id, TracksInfo& tracks_info) {
    Status status;

    //взял с браузера post запрос в body json
    QString body = QString("{\"query\":\"\n    query getClipStatus($slug:ID!) {\n        clip(slug: $slug) {\n            creationState\n            videoQualities {\n              frameRate\n              quality\n              sourceURL\n            }\n          }\n    }\n\",\"variables\":{\"slug\":\"%1\"}}").arg(clip_id);
    QJsonDocument doc = QJsonDocument::fromJson(body.toLocal8Bit());

    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(client_id.toLocal8Bit())));
    
    if (!downloadToStringPost(clip_post_url_, doc.toJson(), data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    doc = QJsonDocument::fromJson(data.toLocal8Bit());
    //QString strJson(doc.toJson(QJsonDocument::Compact));
    //QString a = strJson;

    QJsonObject jsonObj = doc.object();
    QJsonObject dataObj = jsonObj.value("data").toObject();
    QJsonObject clipObj = dataObj.value("clip").toObject();
    QJsonArray videoQualitiesArray = clipObj.value("videoQualities").toArray();

    foreach(QJsonValue value, videoQualitiesArray) {
        QJsonObject obj = value.toObject();
        if (obj.contains("sourceURL")) {

            tracks_info.video_tracks.push_back(VideoTrackInfo());
            QString quality = obj.value("quality").toString();
            tracks_info.video_tracks.back().height = quality.toInt();
            tracks_info.video_tracks.back().framerate = obj.value("frameRate").toInt();
            tracks_info.video_tracks.back().urls.push_back(obj.value("sourceURL").toString());

            normalizeTrack(tracks_info.video_tracks.back());
            normalizeVideoTrack(tracks_info.video_tracks.back());
        }
    }

    return status;
}

Status TwitchParser::loadStreamId(const QString& login, QString &stream_id) {
    Status status;
    QString data;
    QString url = QString(stream_id_url_).arg(login);
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Accept"), QByteArray("application/vnd.twitchtv.v5+json")));
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(api_key_)));
    if (!downloadToStringExtended(url, data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject jsonObj = doc.object();
    if (jsonObj.contains("users")) {
        QJsonObject userObj = jsonObj.find("users").value().toArray().first().toObject();
        stream_id = userObj.find("_id")->toString();
    }
    else {
        status.update(EStatusCode::ParseError, "Not valid user");
    }

    return status;
}

Status TwitchParser::loadStreamInfo(const QString& stream_id, VideoInfo& video_info) {
    Status status;
    QString data;
    QString url = QString(stream_info_url_).arg(stream_id);
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Accept"), QByteArray("application/vnd.twitchtv.v5+json")));
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(api_key_)));
    if (!downloadToStringExtended(url, data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject json_obj = doc.object();

    QJsonObject stream_obj = json_obj.find("stream")->toObject();
    if (!stream_obj.isEmpty()) {
        QString title = stream_obj.value("channel").toObject().find("status")->toString();
        video_info.video_title = normalizeTitle(title);
        video_info.artwork_url = stream_obj.value("preview").toObject().begin()->toString();

        return status;
    }
    else {
        status.update(EStatusCode::ParseError, "Stream not active");
        return status;
    }
}

Status TwitchParser::loadStreamToken(const QString& login, const QString& client_id, QString &sig, QString &token) {
    Status status;

    QString url = QString(stream_token_url_).arg(login);
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("Client-ID"), QByteArray(client_id.toLocal8Bit())));
    if (!downloadToStringExtended(url, data, headers))
        return status.update(EStatusCode::ParseError, "Data not found");

    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject jsonObj = doc.object();
    sig = jsonObj.find("sig")->toString();
    if (sig.isEmpty()) {
        status.update(EStatusCode::ParseError, "Data not found");
        return status;
    }
    token = jsonObj.find("token")->toString();
    if (token.isEmpty()) {
        status.update(EStatusCode::ParseError, "Data not found");
        return status;
    }

    return status;
}

Status TwitchParser::loadM3U8Stream(const QString& login, const QString& sig, const QString& token, TracksInfo& tracks_info, VideoInfo& video_info) {
    Status status;

    QString data;
    QString url = QString(M3U8_stream_url_).arg(login).arg(sig).arg(token);
    url.replace("\"", "%22");
    if (!downloadToString(url, data))
        return status.update(EStatusCode::ParseError, "Data not found");

    parseVmap(data, tracks_info, video_info);
    return status;
}

Status TwitchParser::parseHTML(const QString &data, VideoInfo &video_info, TracksInfo &tracks_info, const QString &url) {
    Status status;

    //if (url.contains(".m3u8") &&
    //    !data.contains("BROADCAST-ID"))
    //    status = getTracksData(data, tracks_info, video_info);

    QRegExp streamRe("^https://www.twitch.tv/.*");
    QRegExp clipRe("^https://www.twitch.tv/.*/clip/.*");
    QRegExp clipRe2("^https://www.twitch.tv/.*/clips.*");
    QRegExp clipReEmbed("^https://clips.twitch.tv/embed\\?clip=.*");
    QRegExp videoRe("^https://www.twitch.tv/videos/.*");
    QRegExp videoRe2("^https://www.twitch.tv/.*/v/.*");
    QRegExp videoRe3("^https://www.twitch.tv/.*/video/.*");

    QString client_id;
    QRegExp clientIdRe = QRegExp("\"Client-ID\":\"([a-zA-Z0-9]+)\"");
    int pos = clientIdRe.indexIn(data);
    if (pos > -1)
        client_id = clientIdRe.cap(1).toLocal8Bit();

    if (client_id.isEmpty())
        client_id = client_id_;
    //video
    if (videoRe.exactMatch(url) ||
        videoRe2.exactMatch(url) ||
        videoRe3.exactMatch(url)) {
        QString video_id;
        QRegExp idRe = QRegExp("/(\\d+)");

        int pos = idRe.indexIn(url);
        if (pos > -1) {
            video_id = idRe.cap(1);
        }
        else {
            status.update(EStatusCode::ParseError, "Video id not found");
            return status;
        }

        status = loadVideoInfo(video_id, video_info);
        if (!status)
            return status;

        QString sig, token;
        status = loadVideoToken(video_id, client_id, sig, token);
        if (!status)
            return status;

        return loadM3U8Video(video_id, sig, token, tracks_info, video_info);
    }
    //clip
    else if (clipRe.exactMatch(url)
        || clipRe2.exactMatch(url) 
        || clipReEmbed.exactMatch(url)) {
        
        QString clip_id;
        QRegExp idRe = QRegExp("/clip/([a-zA-Z0-9]+)");
        QRegExp idReEmbed = QRegExp("clip=([a-zA-Z0-9]+)");

        int pos = idRe.indexIn(url);
        if (pos < 0)
            pos = idReEmbed.indexIn(url);

        if (pos > -1) {
            clip_id = idRe.cap(1);
        }
        else {
            status.update(EStatusCode::ParseError, "Clip id not found");
            return status;
        }

        status = loadClipInfo(clip_id, video_info);

        if (!status)
            return status;

        return loadClipTracks(clip_id, client_id, tracks_info);
    }
    //stream
    else if (streamRe.exactMatch(url)) { 
        return status.update(EStatusCode::LiveStream, "Live streams are not supported.");

        QString login;
        QRegExp loginRe = QRegExp("^https://www.twitch.tv/(.+)$");

        int pos = loginRe.indexIn(url);
        if (pos > -1) {
            login = loginRe.cap(1);
        }
        else {
            status.update(EStatusCode::ParseError, "Login not found");
            return status;
        }

        QString stream_id;
        status = loadStreamId(login, stream_id);
        if (!status)
            return status;

        status = loadStreamInfo(stream_id, video_info);
        if (!status)
            return status;

        QString sig, token;
        status = loadStreamToken(login, client_id, sig, token);
        if (!status)
            return status;

        status = loadM3U8Stream(login, sig, token, tracks_info, video_info);
        if (!status)
            return status;

        return status;
    }
    else {
        status.update(EStatusCode::ParseError, "Error url");
        return status;
    }
}

//Status TwitchParser::getTracksData(const QString& content, TracksInfo& tracks_info, VideoInfo& video_info) {
//    Status status;
//
//    if (!content.isEmpty()) {
//        QString hls_content = content;
//
//        QString first_exp_part = "https:";
//        QString last_exp_part = "m3u8";
//        QRegularExpression re_url(first_exp_part + "(.+)" + last_exp_part);
//
//        QRegularExpressionMatchIterator i = re_url.globalMatch(hls_content);
//
//        QList<QString> quality_urls;
//        int duration = 0;
//
//        while (i.hasNext()) {
//            QRegularExpressionMatch match_url = i.next();
//            QString m3u8_url = match_url.captured(1);
//
//            if (!m3u8_url.isEmpty()) {
//                m3u8_url = first_exp_part + m3u8_url + last_exp_part;
//                QNetworkRequest request;
//                request.setUrl(QUrl(m3u8_url));
//                QByteArray data;
//                status = getRequest(request, data);
//
//                if (status && !data.isEmpty()) {
//                    QString video_part_url = m3u8_url.split(m3u8_url.split("/").last()).first();
//
//                    QString url;
//                    QString dur_delim = "TOTAL-SECS:";
//                    QStringList list = QString(data).split(QRegExp("[\r\n]"));
//                    for (int i = 0; i < list.size(); i++) {
//                        if (list.at(i).contains(".ts"))
//                            url.append(video_part_url + list.at(i) + "|");
//
//                        if (list.at(i).contains(dur_delim)) {
//                            duration = list.at(i).split(dur_delim).last().split(".").first().toInt();
//                            QString dur_fract = list.at(i).split(dur_delim).last().split(".").last();
//
//                            if (!dur_fract.isEmpty()) {
//                                QString fract = dur_fract.at(0);
//                                if (fract.toInt() > 5)
//                                    duration = duration + 1;
//                            }
//                        }
//                    }
//
//                    quality_urls << url;
//                }
//            }
//        }
//
//        if (!quality_urls.isEmpty()) {
//            if (duration != 0)
//                video_info.duration = duration;
//
//            QRegularExpression re_data("(#EXT-X-STREAM-INF(.+)[^\n])*");
//            QRegularExpressionMatchIterator j = re_data.globalMatch(hls_content);
//
//            int i = 0;
//            while (j.hasNext()) {
//                QRegularExpressionMatch match_data = j.next();
//                QString video_data = match_data.captured(1);
//
//                if (!video_data.isEmpty()) {
//                    VideoTrackInfo track;
//
//                    QString resolution = video_data.split("VIDEO").first().split("RESOLUTION").last();
//
//                    QString width = resolution.remove(QRegExp("[\"\\s=,]")).split("x").first();
//                    track.width = width.toInt();
//
//                    QString height = resolution.remove(QRegExp("[\"=,]")).split("x").last();
//                    track.height = height.toInt();
//
//                    QString bitrate = video_data.split("CODECS").first().split("BANDWIDTH").last();
//                    bitrate.remove(QRegExp("[\\s=,]"));
//                    track.bitrate = bitrate.toInt();
//                    track.bitrate = track.bitrate;
//                    if (duration != 0) {
//                        QString frame_rate_s = video_data.split("FRAME-RATE").last();
//                        frame_rate_s.remove(QRegExp("[\\s=,]"));
//                        track.frame_rate = frame_rate_s.split(".").first().toInt() + 1;
//                    }
//
//                    track.urls.append(quality_urls[i]);
//                    i++;
//                    normalizeTrack(track);
//                    tracks_info.calculateTrackSize(track, video_info);
//                    normalizeVideoTrack(track);
//                    tracks_info.video_tracks.append(track);
//                }
//            }
//        }
//    }
//
//    qDebug() << "Parsed JS Tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size();
//    if (tracks_info.video_tracks.isEmpty())
//        status.update(Status::ParseError, "Empty tracks info");
//
//    return status;
//}