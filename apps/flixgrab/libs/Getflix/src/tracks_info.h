#pragma once

#include <QtCore>

#include <common_config.h>
#include <product_config.h>
#include <portals.h>
#include <languages.h>

#include "video_info.h"
#include "get_utils.h"

enum ManualEvaluation {
    Auto = 0,
    Low,
    Normal,
    High,
    UnrecVideo,
    UnrecAudio,
    UnrecImage
};

template<typename T>
void normalizeTrack(T& track) {
    if (track.size <= 0) {
        if (track.is_chunked) {                                 // query size from http..
            qint64 size;
            if (track.urls.size() < 11) {
                for (const auto& url : track.urls) {
                    size = abs(getTrackSize(url));
                    if (size > 100)
                        track.size += size;
                }
            }
        }
        else {
            track.size = abs(getTrackSize(track.urls.front()));
        }
        
        if (track.size <= 0) {
            track.size = 100;                                   //HACK to show progress for unknown size
            track.is_calc_size = false;
            track.is_fake_size = true;

            if (track.is_chunked)
                track.size = track.size * track.urls.size();
        }
    }
}

template<typename T>
void normalizeVideoTrack(T& track) {
    if (track.bitrate < 0)
        track.bitrate = 0;
    track.bitrate = track.bitrate / 1000;

    if (track.framerate > 1000)
        track.framerate = track.framerate / 1000;

    if (track.width <= 0
        && track.height <= 0
        && track.eval_type == ManualEvaluation::Auto)
        track.eval_type = ManualEvaluation::UnrecVideo;
}

template<typename T>
void normalizeAudioTrack(T& track) {
    if (track.bitrate <= 0) {
        track.bitrate = 0;
        if (track.eval_type == ManualEvaluation::Auto)
            track.eval_type = ManualEvaluation::UnrecAudio;
    }

    track.bitrate = track.bitrate / 1000;

    if (track.bitrate > 123
        && track.bitrate < 133)
        track.bitrate = 128;
}

template<typename T>
void normalizeImageTrack(T& track) {
    if (track.width <= 0
        && track.height <= 0
        && track.eval_type == ManualEvaluation::Auto)
        track.eval_type = ManualEvaluation::UnrecImage;
}

enum VideoProfile {
    UNKNOWN = 0,
    AV1,
    WEBM,
    AVC,
    HVC
};

enum AudioProfile {
    UnknownProfile = 0,
    MP3,
    OGG,
    AAC,
    AAC_HEAACv2,            //amazon case
    AAC_HEAAC,              //amazon case
    AAC_MP4A,               //amazon case
    DolbyDigital20,         //netflix case
    AAC51,                  //netflix case
    DolbyDigital,           //netflix, disney cases
    DolbyDigitalPlus,       //netflix case
    DolbyDigitalPlusAtmos   //netflix case
};

struct VideoTrackInfo {
    ManualEvaluation        eval_type = ManualEvaluation::Auto;
    int                     width = 0;
    int                     height = 0;
    int                     bitrate = 0;
    int                     framerate = 30;
    qint64                  size = 0;
    bool                    is_encrypted = false;
    QList<QString>          urls;
    bool                    contains_audio = true;
    bool                    is_chunked = false;
    VideoProfile            profile = VideoProfile::AVC;
    QString                 extended_desk = "";
    QString                 cench_pssh;
    QList<QString>          keys;

    bool                    is_calc_size = false;
    bool                    is_fake_size = false;
    bool                    is_fake_track = false;  //none video case

    //VideoTrackInfo() : width(0), height(0), bitrate(0), size(0), is_encrypted(false) {}
};

struct AudioTrackInfo {
    ManualEvaluation        eval_type = ManualEvaluation::Auto;
    int                     channels = 0;
    //bcp47 standart tag;
    QString                 language_tag;
    QString                 language;
    int                     bitrate = 0;
    qint64                  size = 0;
    bool                    is_assistive = false; //true - Audio for the blind
    bool                    is_chunked = false;
    bool                    is_encrypted = false;
    bool                    from_video_stream = false;
    AudioProfile            profile = AudioProfile::UnknownProfile;
    QString                 extended_desk = "";
    QList<QString>          urls;
    QString                 audioTrackId;
    QString                 cench_pssh;
    QList<QString>          keys;

    bool                    is_calc_size = false;
    bool                    is_fake_size = false;
    bool                    is_fake_track = false; //none audio case

    //AudioTrackInfo() : channels(0), bitrate(0), size(0), is_encrypted(false) {}
};


struct ImageTrackInfo {
    ManualEvaluation        eval_type = ManualEvaluation::Auto;
    int                     image_number = -1;
    int                     width = 0;
    int                     height = 0;
    qint64                  size = 0;
    QString                 extension = "jpg";
    bool                    is_chunked = false;
    QList<QString>          urls;

    bool                    is_calc_size = false;
    bool                    is_fake_size = false;
    bool                    is_fake_track = false; //none image case
};

struct SubtitleTrackInfo {
    enum SubtitleProfile {
        Unknown = 0,
        SRT,                    //instagram
        WEBVTT,                 //vimeo, yotube
        SRV1,                   //yotube
        SRV2,                   //yotube
        SRV3,                   //yotube
        XML,                    //hbo
        SimpleSdh,              //ttml
        LessSimpleSdh,          //ttml + formatting;
        NflxCmisc               //Images ttml.zip
    };

    SubtitleProfile         profile = SubtitleProfile::Unknown;
    QString                 language_tag; //bcp47 standart tag;
    QString                 language;
    qint64                  size = 0;
    QSize                   image_size;             //netflix only - base size of image
    bool                    is_additional = false;  //forced subtitles - youtube, netflix, disneyplus, hbo
    bool                    is_closed_captions = false;  //netflix, amazon, disneyplus
    bool                    is_chunked = false;     //disneyplus, hbo
    QList<QString>          urls;

    bool                    is_calc_size = false;
    bool                    is_fake_size = false;
    bool                    is_fake_track = false; //none subtitle case
};

struct TracksInfo
{
    QList<VideoTrackInfo>                   video_tracks;
    QList<AudioTrackInfo>                   audio_tracks;
    QList<AudioTrackInfo>                   all_audio_tracks;
    QList<SubtitleTrackInfo>                subtitle_tracks;
    QList<ImageTrackInfo>                   image_tracks;
    QList<ImageTrackInfo>                   all_image_tracks;

    QList<QString>                          audio_languages;
    QList<QString>                          all_audio_languages;

    bool                                    req_decrypt = false;
    
    bool                                    only_audio = false; // true if only audio

    //Decryption Key Id;
    QByteArray                              key_id;
    QString                                 pssh; // cenc pssh
    //QList<QString>                          keys;

    //Duration in milliseconds;
    qint64                                  duration = 0;

    int getDefaultVideoTrack(const QList<VideoTrackInfo>& tracks, const EPortal portal) {
        int id = 0;

        for (const auto& track : tracks) {
            if (!track.is_fake_track) {
                if (track.height > 0) {
                    if (track.height <= normalQualityVideoHeight(portal))
                        break;
                }
                else {
                    if (track.eval_type <= ManualEvaluation::Normal
                        && track.eval_type != ManualEvaluation::Auto)
                        break;
                }
            }

            id++;
        }

        if (tracks.size() > 1
            && id == 0
            && tracks[id].is_fake_track)
            id = 1;

        return id;
    }

    int getDefaultAudioTrack(const QList<AudioTrackInfo>& tracks, const EPortal portal) {
        int id = 0;

        for (const auto& track : tracks) {
            if (!track.is_fake_track) {
                if (track.bitrate > 0) {
                    if (track.bitrate <= normalQualityAudioBitrate(portal))
                        break;
                }
                else {
                    if (track.eval_type <= ManualEvaluation::Normal
                        && track.eval_type != ManualEvaluation::Auto)
                        break;
                }
            }

            id++;
        }

        if (tracks.size() > 1
            && id == 0
            && tracks[id].is_fake_track)
            id = 1;

        return id;
    }

    int getDefaultImageTrack(const QList<ImageTrackInfo>& tracks, const EPortal portal) {
        int id = 0;

        for (const auto& track : tracks) {
            if (!track.is_fake_track) {
                if (track.width > 0) {
                    if (track.width <= normalQualityImageWidth(portal))
                        break;
                }
                else {
                    if (track.eval_type <= ManualEvaluation::Normal
                        && track.eval_type != ManualEvaluation::Auto)
                        break;
                }
            }

            id++;
        }

        if (tracks.size() > 1
            && id == 0
            && tracks[id].is_fake_track)
            id = 1;

        return id;
    }

    QList<int> getDefaultSubtitleTracks(const QList<SubtitleTrackInfo>& tracks) {
        QList<int> default_tracks;
        
        if (!tracks.isEmpty()) {
            for (int i = 0; i < tracks.size(); ++i) {
                if (tracks[i].language_tag == "en") {
                    default_tracks << i;
                }
                else if (tracks[i].language_tag.split("-").first() == "en") {
                    default_tracks << i;
                }
            }
        }
        if (default_tracks.isEmpty())
            default_tracks << 1;

        return default_tracks;
    }

    int getDefaultAudioLanguageTrack(const QList<QString>& tracks) {
        int default_track = 1;

        if (!tracks.isEmpty()) {
            for (int i = 0; i < tracks.size(); ++i) {
                if (tracks[i] == "en") {
                    default_track = i;
                    break;
                }
                else if (tracks[i].split("-").first() == "en") {
                    default_track = i;
                    break;
                }
            }
        }

        return default_track;
    }

    void sortVideoTracks() {
        if (!video_tracks.isEmpty()) {
            //Sort tracks;
            std::sort(video_tracks.begin(), video_tracks.end(), [](const VideoTrackInfo& t1, const VideoTrackInfo& t2)->bool {
                if (t1.is_fake_track == t2.is_fake_track) {
                    if (t1.eval_type == ManualEvaluation::Auto && t2.eval_type == ManualEvaluation::Auto) {
                        if (t1.height == t2.height) {
                            if (t1.bitrate == t2.bitrate
                                && t1.size > 0
                                && t2.size > 0)
                                return (t1.size > t2.size);
                            else
                                return (t1.bitrate > t2.bitrate);
                        }
                        else {
                            return (t1.height > t2.height);
                        }
                    }
                    else {
                        return (t1.eval_type > t2.eval_type);
                    }
                }
                else {
                    return (t1.is_fake_track < t2.is_fake_track);
                }
            });

            for (int i = 0; i < video_tracks.size(); i++) {
                if (video_tracks[i].is_encrypted) {
                    req_decrypt = true;
                    break;
                }
            }
        }
    }

    void sortAudioTracks() {
        if (!audio_tracks.isEmpty()) {
            //Sort tracks;
            std::sort(audio_tracks.begin(), audio_tracks.end(), [](const AudioTrackInfo& t1, const AudioTrackInfo& t2)->bool {
                if (t1.is_fake_track == t2.is_fake_track) {
                    if (t1.eval_type == ManualEvaluation::Auto && t2.eval_type == ManualEvaluation::Auto) {
                        if (t1.profile == t2.profile) {
                            if (t1.bitrate == t2.bitrate)
                                return (t1.size > t2.size);
                            else
                                return (t1.bitrate > t2.bitrate);
                        }
                        else {
                            return (t1.profile > t2.profile);
                        }
                    }
                    else {
                        return (t1.eval_type > t2.eval_type);
                    }
                }
                else {
                    return (t1.is_fake_track < t2.is_fake_track);
                }
            });

            for (int i = 0; i < audio_tracks.size(); i++) {
                if (audio_tracks[i].is_encrypted) {
                    req_decrypt = true;
                    break;
                }
            }
        }
    }

    void sortAudioLanguages() {
        if (!audio_languages.isEmpty()) {
            bool add_none_item = false;
            if (audio_languages.contains(kLanguagesAndCodes[0][1])) {
                audio_languages.removeAll(kLanguagesAndCodes[0][1]);
                add_none_item = true;
            }

            std::sort(audio_languages.begin(), audio_languages.end(), [](const QString& t1, const QString& t2)->bool {
                return QLocale::languageToString(QLocale(t1).language()).toLower() < QLocale::languageToString(QLocale(t2).language()).toLower();
            });

            if (add_none_item)
                audio_languages.push_front(kLanguagesAndCodes[0][1]);
        }
    }

    void sortSubtitlesTracks() {
        if (!subtitle_tracks.isEmpty()) {
            std::sort(subtitle_tracks.begin(), subtitle_tracks.end(), [](const SubtitleTrackInfo& t1, const SubtitleTrackInfo& t2)->bool {
                if (t1.is_fake_track == t2.is_fake_track) {
                    if (QLocale::languageToString(QLocale(t1.language_tag).language()).toLower() == QLocale::languageToString(QLocale(t2.language_tag).language()).toLower()) {
                        if (QLocale::countryToString(QLocale(t1.language_tag).country()).toLower() == QLocale::countryToString(QLocale(t2.language_tag).country()).toLower()) {
                            if (t1.is_additional == t2.is_additional)
                                return (t1.profile < t2.profile);
                            else
                                return (t1.is_additional < t2.is_additional);
                        }
                        else {
                            return QLocale::countryToString(QLocale(t1.language_tag).country()).toLower() < QLocale::countryToString(QLocale(t2.language_tag).country()).toLower();
                        }
                    }
                    else
                        return QLocale::languageToString(QLocale(t1.language_tag).language()).toLower() < QLocale::languageToString(QLocale(t2.language_tag).language()).toLower();
                }
                else {
                    return (t1.is_fake_track > t2.is_fake_track);
                }
            });
        }
    }

    void sortImagesTracks() {
        if (!image_tracks.isEmpty()) {
            //Sort tracks by bitrate;
            std::sort(image_tracks.begin(), image_tracks.end(), [](const ImageTrackInfo& t1, const ImageTrackInfo& t2)->bool {
                if (t1.eval_type == ManualEvaluation::Auto || t2.eval_type == ManualEvaluation::Auto) {
                    if (t1.width == t2.width)
                        return (t1.height > t2.height);
                    else
                        return (t1.width > t2.width);
                }
                else {
                    return (t1.eval_type > t2.eval_type);
                }
            });
        }
    }

    void fillAudioLanguages() {
        audio_languages.clear();
        for (int i = 0; i < audio_tracks.size(); i++) {
            if (!audio_tracks[i].language_tag.isEmpty()
                && !audio_languages.contains(audio_tracks[i].language_tag))

                audio_languages << audio_tracks[i].language_tag;
        }
    }

    void sortTracks() {
        if (!video_tracks.isEmpty())
            sortVideoTracks();

        if (!audio_tracks.isEmpty())
            sortAudioTracks();

        if (!subtitle_tracks.isEmpty())
            sortSubtitlesTracks();

        if (!image_tracks.isEmpty())
            sortImagesTracks();
    }

    void setPrivateKeys(const QList<QString>& kidKeys, const QString pssh) {
        qDebug() << "SetPrivateKeys ...";

        for (int i = 0; i < video_tracks.size(); i++) {
            if (video_tracks[i].cench_pssh == pssh || this->pssh == pssh) {
                video_tracks[i].keys = kidKeys;
                qDebug() << " privateKeys for video " << video_tracks[i].bitrate << "kbps";
            }
        }

        for (int i = 0; i < audio_tracks.size(); i++) {
            if (audio_tracks[i].cench_pssh == pssh || this->pssh == pssh) {
                audio_tracks[i].keys = kidKeys;
                qDebug() << " privateKeys for audio " << audio_tracks[i].bitrate << "kbps";
            }
        }
    }

    void Clear() {
        video_tracks.clear();
        audio_tracks.clear();
        all_audio_tracks.clear();
        audio_languages.clear();
        all_audio_languages.clear();
        subtitle_tracks.clear();
        image_tracks.clear();
        all_image_tracks.clear();
    }
    
    void calculateTrackSize(VideoTrackInfo& track, const VideoInfo& info) {
        if (track.is_fake_size
            && track.height > 0
            && track.width > 0
            && track.bitrate > 0
            && info.duration > 0) {

            double h264_coef = 0.40;

            if (track.height >= 1260)
                h264_coef = 0.03;
            else if (track.height < 1260 && track.height >= 900)
                h264_coef = 0.07;
            else if (track.height < 900 && track.height >= 630)
                h264_coef = 0.18;
            else if (track.height < 630 && track.height >= 510)
                h264_coef = 0.3;
            else if (track.height < 510 && track.height >= 420)
                h264_coef = 0.40;
            else if (track.height < 420 && track.height >= 305)
                h264_coef = 0.75;
            else if (track.height < 305)
                h264_coef = 1.5;

            double height = static_cast<double>(track.height);
            double width = static_cast<double>(track.width);
            double bitrate = static_cast<double>(track.bitrate);
            double duration = static_cast<double>(info.duration);
            double framerate = static_cast<double>(track.framerate);

            double calc_size = height * width * (bitrate / 1000) * duration * 3 * framerate * (h264_coef / 100000.0);
            if (calc_size > 0) {
                track.size = static_cast<qint64>(calc_size);
                track.is_fake_size = false;
                track.is_calc_size = true;
            }
        }
    }
};

inline QString getSubtitleTrackExt(SubtitleTrackInfo::SubtitleProfile profile) {
    QString extension = "sub";

    if (profile == SubtitleTrackInfo::SubtitleProfile::SimpleSdh
        || profile == SubtitleTrackInfo::SubtitleProfile::LessSimpleSdh)
        extension = "ttml";
    else if (profile == SubtitleTrackInfo::SubtitleProfile::NflxCmisc)
        extension = "ttml.zip";
    else if (profile == SubtitleTrackInfo::SubtitleProfile::WEBVTT)
        extension = "vtt";
    else if (profile == SubtitleTrackInfo::SubtitleProfile::SRV1)
        extension = "srv1";
    else if (profile == SubtitleTrackInfo::SubtitleProfile::SRV2)
        extension = "srv2";
    else if (profile == SubtitleTrackInfo::SubtitleProfile::SRV3)
        extension = "srv3";
    else if (profile == SubtitleTrackInfo::SubtitleProfile::XML)
        extension = "xml";

    return extension;
}

inline QString normalizeTitle(const QString& data, int sting_size = 217) {
    QString title = data;
    title = title.remove(QRegularExpression("[<>:\"\/\\|?*']"));
    title = title.replace("\\", "");
    title = title.simplified();
    
    if (title.isEmpty())
        return title;

    title.replace(0, 1, title[0].toUpper());

    if (title.size() > sting_size) {
        title.truncate(sting_size);
        title.append("...");
    }

    title = title.trimmed();

    return title;
}

Q_DECLARE_METATYPE(VideoTrackInfo)
Q_DECLARE_METATYPE(AudioTrackInfo)
Q_DECLARE_METATYPE(ImageTrackInfo)
Q_DECLARE_METATYPE(TracksInfo)


//class TracksInfo
//{
//
//public:
//
//    TracksInfo() : default_video_track_(0), default_audio_track_(0) {}
//
//
//    inline const QList<VideoTrackInfo>&     video_tracks() const { return video_tracks_; }
//    inline const QList<AudioTrackInfo>&     audio_tracks() const { return audio_tracks_; }
//
//    inline const VideoTrackInfo&           default_video_track() const { return video_tracks_[default_video_track_]; }
//    inline const AudioTrackInfo&           default_audio_track() const { return audio_tracks_[default_audio_track_]; }
//
//    inline int                             default_video_index() const { return default_video_track_; }
//    inline int                             default_audio_index() const { return default_audio_track_; }
//
//protected:
//
//    QList<VideoTrackInfo>                   video_tracks_;
//    QList<AudioTrackInfo>                   audio_tracks_;
//
//    int                                     default_video_track_;
//    int                                     default_audio_track_;
//
//};
//
//
//class TracksBuilder : public TracksInfo
//{
//public:
//    void                            append(const VideoTrackInfo& info) { video_tracks_.append(info); }
//    void                            append(const AudioTrackInfo& info) { audio_tracks_.append(info); }
//
//    void                            sortTracks();
//
//};
