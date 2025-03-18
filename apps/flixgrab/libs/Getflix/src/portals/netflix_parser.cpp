#include "netflix_parser.h"

#include <languages.h>

#include "get_utils.h"
#include "utils/qs_utils.h"

namespace parser {

    Status parseMetadata(const QString& metadata, VideoInfo& info, const QString& base_url) {
        Status status;

#ifdef _DEBUG
        QTextDocument text;
        text.setHtml(metadata);
        QString plain_json = text.toPlainText();
        QFile   metadata_file("netflix_metadata.json");
        if (metadata_file.open(QIODevice::ReadWrite)) {
            QTextStream(&metadata_file) << plain_json << endl;
        }
#endif
        //TODO: rewrite
        QJsonDocument metadata_json = QJsonDocument::fromJson(metadata.toLocal8Bit());

        if (!metadata_json.isNull()) {
            QJsonObject video = metadata_json.object()["video"].toObject();

            info.url_type = video["type"].toString();
            if (info.url_type == "supplemental")
                info.url_type = "more";

            QString title;
            if (video.contains("currentEpisode") &&
                video.contains("seasons")) {
                //Episodes parsing: title, runtime, year, artwork;
                QString episode_id = video["currentEpisode"].toVariant().toString();
                QString base_ep_url = base_url.mid(0, base_url.lastIndexOf("/") + 1);

                for (auto season_ref : video["seasons"].toArray()) {
                    QJsonObject season = season_ref.toObject();
                    
                    QRegExp rx("(\\d+)");
                    QString season_number;
                    int pos = 0;
                    while ((pos = rx.indexIn(season["longName"].toString(), pos)) != -1) {
                        season_number.append(rx.cap(1));
                        pos += rx.matchedLength();
                    }

                    int season_num = season_number.toInt();

                    if (season_num < 10)
                        season_number = "0" + season_number;

                    for (auto episode_ref : season["episodes"].toArray()) {
                        QJsonObject episode = episode_ref.toObject();

                        if (episode["episodeId"].toVariant().toString() == episode_id) {

                            int episode_num = episode["seq"].toVariant().toInt();
                            QString episode_number = QString::number(episode_num);
                            if (episode_num < 10)
                                episode_number = "0" + episode_number;

                            //Episode detected;
                            QString movie_name = episode["title"].toString();
                            QString album_name = video["title"].toString();

                            info.metadata_title = normalizeTitle(movie_name);
                            info.metadata_album = normalizeTitle(album_name);
                            //info.metadata_track = "S" + QString::number(season_num) + ".E" + QString::number(episode_num);
                            info.metadata_track = QString::number(episode_num) + "/" + QString::number(season_num);

                            info.duration = episode["runtime"].toInt();
                            info.metadata_comment = episode["synopsis"].toString();
                            title = "[S" + season_number + ".E" + episode_number + "] " + album_name + " - " + movie_name;
                            info.metadata_year = season["year"].toString();

                            QJsonObject still = episode["stills"].toArray()[0].toObject();
                            info.artwork_url = still["url"].toString();
                        }
                        else if (!base_ep_url.isEmpty()) {
                            info.playlist.append(base_ep_url + episode["episodeId"].toVariant().toString() + ("|"));
                        }
                    }
                }
            }
            else {
                //qint64 id = video["id"].toVariant().toLongLong();
                //Movie or supplemential detected;
                info.duration = video["runtime"].toInt();
                info.metadata_comment = video["synopsis"].toString();
                title = video["title"].toString();
                info.metadata_title = normalizeTitle(title);
                info.metadata_year = video["year"].toString();

                QJsonObject artwork = video["artwork"].toArray()[0].toObject();
                info.artwork_url = artwork["url"].toString();
            }
            
            info.video_title = normalizeTitle(title);

            if (!info.video_title.isEmpty()) {
                qDebug() << "Parsed Title: " << info.url_type << ": '" << info.video_title << "' Year: " << info.metadata_year << " Duration: " << info.duration;
            }
            else {
                qDebug() << "Parsed Title Error ";
                //TODO: parse error;
                status.update(Status::ParseError, "Metadata parse error.");
            }
        }
        else status.update(Status::ParseError, "Wrong video metadata");


        return status;
    }

    static AudioProfile     netflixAudioProfile(const QString& profile_str) {
        const QList<QString>
            profile_strs = { "heaac-2-piff", "heaac-2-dash", "playready-heaac-2-dash", "heaac-2-elem", "heaac-2hq-dash", "heaac-2-dash-alt",
                                "ddplus-2.0-dash",
                                "heaac-5.1-dash",
                                "dd-5.1-elem", "ddplus-5.1-dash", "ddplus-5.1-piff", "ddplus-5.1hq-dash",
                                "ddplus-atmos-dash" };
        const AudioProfile
            profile_ids[] = { AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC,
                                AudioProfile::DolbyDigital20,
                                AudioProfile::AAC51,
                                AudioProfile::DolbyDigital, AudioProfile::DolbyDigitalPlus, AudioProfile::DolbyDigitalPlus, AudioProfile::DolbyDigitalPlus,
                                AudioProfile::DolbyDigitalPlusAtmos };
        int index = profile_strs.indexOf(profile_str);
        return index >= 0 ? profile_ids[index] : AudioProfile::UnknownProfile;
    }

    void parseAudioTracks(const QJsonArray& tracks, TracksInfo& info) {
        for (auto track_ref : tracks) {
            QJsonObject audio_track = track_ref.toObject();

            AudioTrackInfo track;

            track.channels = audio_track["channelsCount"].toInt();
            QString tag = audio_track["bcp47"].toString().toLower();

            if (tag == "es")
                tag = SS2Q(getLangTagByLangName("Spanish Latin"));

            track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));

            if (audio_track["trackType"].toString() == "ASSISTIVE")
                track.is_assistive = true;

            for (auto download_ref : audio_track["downloadables"].toArray()) {
                QJsonObject download = download_ref.toObject();

                track.bitrate = download["bitrate"].toInt();
                track.profile = netflixAudioProfile(download["contentProfile"].toString());
                track.is_encrypted = download["isEncrypted"].toBool();
                track.size = download["size"].toInt();

                track.urls.clear();

                for (auto url_ref : download["urls"].toObject()) {
                    qDebug() << "audio trask url: " << url_ref.toString();
                    track.urls.append(url_ref.toString());
                }

                if (!track.urls.isEmpty()) {
                    //track.size = abs(getTrackSize(track.urls.front()));
                    info.audio_tracks.append(track);
                }
            }
        }
    }

    void parseVideoTracks(const QJsonArray& tracks, TracksInfo& info) {
        for (auto track_ref : tracks) {
            QJsonObject video_track = track_ref.toObject();
            VideoTrackInfo track;
            for (auto download_ref : video_track["downloadables"].toArray()) {
                QJsonObject download = download_ref.toObject();
                
                track.bitrate = download["bitrate"].toInt();
                track.height = download["height"].toInt();
                track.width = download["width"].toInt();
                track.is_encrypted = download["isEncrypted"].toBool();
                track.size = download["size"].toVariant().toLongLong();

                track.contains_audio = false;

                track.urls.clear();

                for (auto url_ref : download["urls"].toObject()) {
                    qDebug() << "video trask url: " << url_ref.toString();
                    track.urls.append(url_ref.toString());
                }

                if (!track.urls.isEmpty()) {
                    //track.size = abs(getTrackSize(track.urls.front()));
                    info.video_tracks.append(track);
                }
            }
        }
    }

    static SubtitleTrackInfo::SubtitleProfile     subtitleProfile(const QString& profile_str) {
        const QList<QString>
            profile_strs = { "simplesdh", "dfxp-ls-sdh", "nflx-cmisc" };
        const SubtitleTrackInfo::SubtitleProfile
            profile_ids[] = { SubtitleTrackInfo::SimpleSdh, SubtitleTrackInfo::LessSimpleSdh, SubtitleTrackInfo::NflxCmisc };
        int index = profile_strs.indexOf(profile_str);
        return index >= 0 ? profile_ids[index] : SubtitleTrackInfo::Unknown;
    }

    void parseTextTracks(const QJsonArray& tracks, TracksInfo& info) {
        QList<QString> language_tags;
        QList<QString> language_tags_additional;
        QList<QString> language_closed_captions;

        for (auto track_ref : tracks) {
            QJsonObject text_track = track_ref.toObject();

            SubtitleTrackInfo track;
            if (!text_track["isNone"].toBool()) {
                QString tag = text_track["bcp47"].toString().toLower();
                if (tag == "es")
                    tag = SS2Q(getLangTagByLangName("Spanish Latin"));
                track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));

                QLocale locale(track.language_tag);
                QString country;
                if (track.language_tag.contains("-"))
                    country = " " + locale.countryToString(locale.country());
                track.language = QString("%1%2").arg(QLocale::languageToString(locale.language())).arg(country);

                track.is_additional = text_track["isForced"].toBool();
                if (!track.is_additional
                    && language_tags.contains(track.language_tag)
                    && !language_closed_captions.contains(track.language_tag))
                    track.is_closed_captions = true;

                int track_priority = INT_MAX;
                for (auto download_ref : text_track["downloadables"].toArray()) {
                    QJsonObject download = download_ref.toObject();

                    //Only one profile are best for getflix;
                    auto profile = subtitleProfile(download["contentProfile"].toString());
                    int downloadables_priority = (int)profile;
                    if (downloadables_priority > 0) {
                        if (download["isImage"].toBool())
                            downloadables_priority += download["pixWidth"].toInt()*
                            download["pixHeight"].toInt();

                        if (downloadables_priority >= track_priority)
                            continue;
                    }
                    else {
                        qDebug() << "Unsupported SUBTITLE profile" << download["contentProfile"].toString();
                        continue;
                    }

                    track_priority = downloadables_priority;
                    track.profile = profile;

                    track.size = 0;
                    track.image_size = QSize();
                    track.urls.clear();

                    if (download["isImage"].toBool()) {
                        track.size = download["offset"].toInt();
                        track.image_size = QSize(download["pixWidth"].toInt(),
                            download["pixHeight"].toInt());
                    }

                    //Parse URLS;
                    for (auto url_ref : download["urls"].toObject()) {
                        qDebug() << "suttitle trask url: " << url_ref.toString();
                        track.urls.append(url_ref.toString());
                    }
                }
                if (!track.urls.isEmpty()) {
                    if (track.is_additional) {
                        if (!language_tags_additional.contains(track.language_tag)) {
                            info.subtitle_tracks.append(track);
                            language_tags_additional << track.language_tag;
                        }
                    }
                    else if (track.is_closed_captions) {
                        if (!language_closed_captions.contains(track.language_tag)) {
                            info.subtitle_tracks.append(track);
                            language_closed_captions << track.language_tag;
                        }
                    }
                    else {
                        if (!language_tags.contains(track.language_tag)) {
                            info.subtitle_tracks.append(track);
                            language_tags << track.language_tag;
                        }
                    }
                }
            }
        }
    }

    Status parseManifest(const QJsonObject& manifest, TracksInfo& info) {

        Status status;
        if (!manifest.isEmpty()) {

            parseAudioTracks(manifest["audioTracks"].toArray(), info);
            parseVideoTracks(manifest["videoTracks"].toArray(), info);
            parseTextTracks(manifest["textTracks"].toArray(), info);

            // QString keyId = manifest.object()["keyId"].toString();

            /* if (manifest_object.contains("psshb64")) {
            QString psshb64 = manifest_object["psshb64"].toArray()[0].toString();
            QByteArray pssh = QByteArray::fromBase64(psshb64.toLatin1());
            std::vector<uint8_t> pssh_vec;
            pssh_vec.assign((uint8_t*)pssh.data(), (uint8_t*)pssh.data() + pssh.size());
            std::vector<std::vector<uint8_t>> keys = ParsePSSHKeys(pssh_vec);
            }*/
            //  tracks_info_.key_id = QByteArray::fromBase64(keyId.toLatin1());
            info.duration = manifest["runtime"].toInt();    //not used now

            if (info.video_tracks.isEmpty()
                || info.audio_tracks.isEmpty())
                status.update(Status::ParseError, "Wrong manifest content.");

        }
        else
            status.update(Status::ParseError, "Wrong manifest json object.");

        return status;
    }

}


