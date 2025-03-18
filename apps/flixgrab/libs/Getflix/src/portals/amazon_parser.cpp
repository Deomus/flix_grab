#include "amazon_parser.h"

#include <languages.h>
#include "get_utils.h"
#include "utils/qs_utils.h"

namespace parser {

    Status parseMetadata(const QString& metadata, VideoInfo& video_info, TracksInfo& tracks_info, QList<QString>& audio_tracks, QString& mpd_url){
        Status status;
        QJsonDocument metadata_json = QJsonDocument::fromJson(metadata.toLocal8Bit());
#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("metadata_" + QString::number(ms) + " .json");
        if (file.open(QIODevice::ReadWrite)) {
            file.write(metadata_json.toJson(QJsonDocument::Compact));
        }
#endif
        if (!metadata_json.isNull()) {

            QJsonObject error = metadata_json.object()["error"].toObject();
            if (!error.isEmpty()) {
                qDebug() << "metadata responce: " << error["errorCode"].toString();
                return EStatusCode::ParseError;
            }
            error = metadata_json.object()["errorsByResource"].toObject();
            if (!error.isEmpty()) {
                QJsonDocument doc(error);
                qDebug() << "metadata responce: " << QString(doc.toJson(QJsonDocument::Compact));
                //return Status::ParseError;
            }
            
            QJsonObject catalogMeta = metadata_json.object()["catalogMetadata"].toObject();
            QJsonObject catalog = catalogMeta["catalog"].toObject();
            QJsonObject family = catalogMeta["family"].toObject();
            QJsonObject summary = catalogMeta["playback"].toObject()["summary"].toObject();

            if (!summary.isEmpty()
                && !summary["isPlayable"].toBool())
                return EStatusCode::VideoBlockedInLocation;

            if (video_info.video_title.isEmpty()) {
                video_info.url_type = catalog["entityType"].toString();
                if (video_info.url_type.contains(" "))
                    video_info.url_type = video_info.url_type.split(" ").last().toLower();
                if (!video_info.url_type.isEmpty())
                    video_info.url_type.replace(0, 1, video_info.url_type[0].toUpper());

                //Movie or supplemential detected;
                video_info.duration = catalog["runtimeSeconds"].toInt();
                video_info.metadata_comment = catalog["synopsis"].toString();
                QString title = catalog["title"].toString().replace("\"", "");
                video_info.metadata_title = normalizeTitle(title);
                
                if (!family.isEmpty()) {
                    QJsonArray tvAncestors = family["tvAncestors"].toArray();
                    if (!tvAncestors.isEmpty()) {

                        int i = 0;
                        QString season_number;
                        QString show_title;
                        int season;
                        foreach(const QJsonValue& value, tvAncestors) {
                            QJsonObject catalog_tvAncestors = value.toObject()["catalog"].toObject();
                            if (!catalog_tvAncestors.isEmpty()) {
                                if (i == 0) {
                                    season = catalog_tvAncestors["seasonNumber"].toInt();
                                    season_number = QString::number(season);
                                    if (season < 10)
                                        season_number = "0" + season_number;
                                }
                                else {
                                    show_title = catalog_tvAncestors["title"].toString();
                                }

                                i++;
                            }
                        }

                        int episode = catalog["episodeNumber"].toInt();
                        QString episode_number = QString::number(episode);
                        if (episode < 10)
                            episode_number = "0" + episode_number;

                        video_info.metadata_title = normalizeTitle(title);
                        video_info.metadata_album = normalizeTitle(show_title);
                        if (season > 0
                            && episode > 0)
                            video_info.metadata_track = QString::number(episode) + "/" + QString::number(season);
                            
                        title = "[S" + season_number + ".E" + episode_number + "] " + show_title + " - " + title;
                    }
                }

                video_info.video_title = normalizeTitle(title);
                //? info.metadata_year = video["year"].toString();

                QJsonObject artwork = catalogMeta["images"].toObject()["imageUrls"].toObject();
                video_info.artwork_url = artwork["episode"].toString();
            }

            QJsonObject playbackUrls = metadata_json.object()["playbackUrls"].toObject();
            auto audio_tracks_parsed = playbackUrls["audioTracks"].toArray();
            auto defaultUrlSetId = playbackUrls["defaultUrlSetId"].toString();
            auto urlSets = playbackUrls["urlSets"].toObject();
            auto defaultUrlSet = urlSets[defaultUrlSetId].toObject();
            auto urls = defaultUrlSet["urls"].toObject();
            auto manifest = urls["manifest"].toObject();
            mpd_url = manifest["url"].toString();


            if (!audio_tracks_parsed.isEmpty()) {
                for (auto track_ref : audio_tracks_parsed){
                    auto t = track_ref.toObject()["audioTrackId"].toString();
                    if (!audio_tracks.contains(t))
                        audio_tracks << t; // map audioTrackId -> lang
                }
            }
            if (tracks_info.subtitle_tracks.isEmpty()) {

                auto subtitleUrls = metadata_json.object()["subtitleUrls"].toArray();
                if (!subtitleUrls.isEmpty()) {

                    for (auto track_ref : (QJsonArray)metadata_json.object()["subtitleUrls"].toArray()) {
                        SubtitleTrackInfo track;
                        if (track_ref.toObject()["displayName"].toString().contains("[CC]"))
                            track.is_closed_captions = true;
                        if (track_ref.toObject()["type"].toString().toLower() == "narrative")
                            track.is_additional = true;
                        QString tag = track_ref.toObject()["languageCode"].toString();
                        track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));     // "languageCode":"ru-ru"

                        auto format = track_ref.toObject()["format"].toString(); // "format":"TTMLv2"
                        auto type = track_ref.toObject()["type"].toString(); // "type":"sdh"
                        track.profile = SubtitleTrackInfo::LessSimpleSdh; // .ttml extension
                        auto url = track_ref.toObject()["url"].toString();
                        track.urls.push_back(url);

                        //normalizeTrack(track);
                        tracks_info.subtitle_tracks.push_back(track);
                    }
                }

                auto forcedSubtitleUrls = metadata_json.object()["forcedNarratives"].toArray();
                if (!forcedSubtitleUrls.isEmpty()) {

                    for (auto track_ref : (QJsonArray)metadata_json.object()["forcedNarratives"].toArray()) {
                        SubtitleTrackInfo track;
                        track.is_additional = true;
                        if (track_ref.toObject()["displayName"].toString().contains("[CC]"))
                            track.is_closed_captions = true;
                        QString tag = track_ref.toObject()["languageCode"].toString();
                        track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));     // "languageCode":"ru-ru"

                        auto format = track_ref.toObject()["format"].toString(); // "format":"TTMLv2"
                        auto type = track_ref.toObject()["type"].toString(); // "type":"sdh"
                        track.profile = SubtitleTrackInfo::LessSimpleSdh; // .ttml extension
                        auto url = track_ref.toObject()["url"].toString();
                        track.urls.push_back(url);

                        //normalizeTrack(track);
                        tracks_info.subtitle_tracks.push_back(track);
                    }
                }
            }

            if (!video_info.video_title.isEmpty()) {
                qDebug() << "Parsed Title: " << video_info.url_type << ": '" << video_info.video_title << "' Year: " << video_info.metadata_year << " Duration: " << video_info.duration;
            }
            else {
                qDebug() << "Parsed Title Error ";
                //TODO: parse error;
                status.update(EStatusCode::ParseError, "Metadata parse error.");
            }
        }
        else {
            status.update(EStatusCode::ParseError, "Wrong video metadata");
        }

        return status;
    }
}