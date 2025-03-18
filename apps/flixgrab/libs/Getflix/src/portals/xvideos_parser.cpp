#include "xvideos_parser.h"

#include "get_utils.h"


XVideosParser::XVideosParser(QObject* parent /*= 0*/) : QObject(parent) {
    qRegisterMetaType<VideoInfo>();
}

void XVideosParser::getTracksData(const QString& data, TracksInfo& tracks_info, const VideoInfo& video_info) {

    QRegExp expression = QRegExp("html5player.setVideoHLS(.*)[\r\n]");
    expression.setMinimal(true);
    if (expression.indexIn(data) != -1) {

        //Get title
        QString hls_url = expression.cap(1).split(QRegExp("[\r\n]")).first();
        hls_url.remove(QRegExp("[();']"));
        
        QString hls_content;
        if (downloadToString(hls_url, hls_content) && !hls_content.isEmpty()) {
            QRegularExpression re_url("(hls-(.+)[^\n])*");
            QRegularExpression re_data("(#EXT-X-STREAM-INF(.+)[^\n])*");

            QRegularExpressionMatchIterator i = re_url.globalMatch(hls_content);
            QRegularExpressionMatchIterator j = re_data.globalMatch(hls_content);


            QList<QString> quality_urls;

            while (i.hasNext()) {
                QRegularExpressionMatch match_url = i.next();
                QString m3u8_part_url = match_url.captured(1);

                if (!m3u8_part_url.isEmpty()) {
                    QString m3u8_content;
                    if (downloadToString(hls_url.split("hls.m3u8").first() + m3u8_part_url, m3u8_content) && !m3u8_content.isEmpty()) {
                        QRegularExpressionMatchIterator k = re_url.globalMatch(m3u8_content);
                        QString url;
                        while (k.hasNext()) {

                            QRegularExpressionMatch match_video_url = k.next();
                            QString video_part_url = match_video_url.captured(1);

                            if (!video_part_url.isEmpty()) {
                                url.append(hls_url.split("hls.m3u8").first() + video_part_url + "|");
                            }
                        }

                        quality_urls << url;
                    }
                }
            }
            if (!quality_urls.isEmpty()) {
                int i = 0;
                while (j.hasNext()) {
                    QRegularExpressionMatch match_data = j.next();
                    QString video_data = match_data.captured(1);

                    if (!video_data.isEmpty()) {
                        VideoTrackInfo video_track;

                        QString resolution = video_data.split("NAME").first().split("RESOLUTION").last();
                        QString width = resolution.remove(QRegExp("[\\s=,]")).split("x").first();
                        video_track.width = width.toInt();

                        QString height = resolution.remove(QRegExp("[=,]")).split("x").last();
                        video_track.height = height.toInt();

                        QString bitrate = video_data.split("RESOLUTION").first().split("BANDWIDTH").last();
                        bitrate.remove(QRegExp("[\\s=,]"));
                        video_track.bitrate = bitrate.toInt();

                        qint64 track_size = 0;

                        if (quality_urls[i].contains("|")) {
                            QList<QString> urls = quality_urls[i].split("|");
                            urls.removeAll(QString(""));
                            foreach(QString str, urls)
                                video_track.urls.push_back(str);
                            video_track.is_chunked = true;
                        }
                        else {
                            video_track.urls.append(quality_urls[i]);
                        }
                        i++;

                        normalizeTrack(video_track);
                        tracks_info.calculateTrackSize(video_track, video_info);
                        normalizeVideoTrack(video_track);
                        tracks_info.video_tracks.append(video_track);
                    }
                }
            }
        }
    }

    //Below low speed quality urls
    if (tracks_info.video_tracks.isEmpty()) {
        QRegularExpression re("(html5player.setVideoUrl(.+)[\r\n])*");
        QRegularExpressionMatchIterator i = re.globalMatch(data);

        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();

            QString video_url = match.captured(1);
            if (!video_url.isEmpty()) {

                video_url.remove(QRegExp("[();'\\s]"));
                video_url = "http" + video_url.split("http").last();

                VideoTrackInfo video_track;
                video_track.eval_type = ManualEvaluation::Normal;

                if (video_url.contains("3gp"))
                    video_track.eval_type = ManualEvaluation::Low;

                video_track.urls.append(video_url);

                normalizeTrack(video_track);
                normalizeVideoTrack(video_track);
                tracks_info.video_tracks.append(video_track);
            }
        }
    }

    qDebug() << "Parsed JS Tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size();

}

Status XVideosParser::parsePage(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;

    QRegExp expression = QRegExp("html5player.setVideoTitle(.*)[\r\n]");
    expression.setMinimal(true);
    
    //get title
    QString title;

    if (expression.indexIn(data) != -1) {
        title = expression.cap(1).split(".").first();
        title.remove(QRegExp("[();']"));
    }    

    //obtain basic info
    if (!title.isEmpty()) {
        video_info.video_title = normalizeTitle(title);

        //get thumbnail
        expression = QRegExp("html5player.setThumbUrl169(.*)[\r\n]");
        if (expression.indexIn(data) != -1) {
            video_info.artwork_url = expression.cap(1).split(QRegExp("[\r\n]")).first();
            video_info.artwork_url.remove(QRegExp("[();']"));
        }

        //get duration in seconds
        expression = QRegExp("<meta property=\"og:duration\" content=\"(.*)[\r\n]");
        if (expression.indexIn(data) != -1) {
            QString duration = expression.cap(1).split(QRegExp("[\r\n]")).first();
            duration.remove(QRegExp("[\\s//();'\"/<>]"));
            video_info.duration = duration.toInt();
        }

        getTracksData(data, tracks_info, video_info);
        if (tracks_info.video_tracks.isEmpty())
            status.update(EStatusCode::ParseError, "Empty tracks info.");
    }
    else {
        status.update(EStatusCode::ParseError, "Could not retrieve video title.");
    }
    return status;

}

Status XVideosParser::parseHTML(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;

    status = parsePage(data, video_info, tracks_info);

    return status;
}