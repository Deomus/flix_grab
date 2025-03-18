#pragma once

#include <QtCore>

#include <languages.h>
#include <utils/qs_utils.h>

#include "tracks_info.h"


class MpdParserAmazon : public QObject {

    Q_OBJECT

public:
    bool            read(const QString& mpd_file) {
        tracks_info_ = {};
        reader_.clear();
        reader_.addData(mpd_file);

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"MPD") {

                if (reader_.attributes().hasAttribute("mediaPresentationDuration")) {
                    QString duration = reader_.attributes().value("mediaPresentationDuration").toString(); //PT21M6.304S
                    tracks_info_.duration = getDuration(duration);
                }

                while (reader_.readNextStartElement()) {
                    if (reader_.name() == L"Period")
                        readPeriod();
                    else
                        reader_.skipCurrentElement();
                }
            }
            else {
                reader_.skipCurrentElement();
            }
        }

        bool result = false;
        if (tracks_info_.video_tracks.size()
            || tracks_info_.audio_tracks.size()) {
            result = true;
            emit parsingFinished(tracks_info_);
        }

        return result;
    }

    const TracksInfo&   get_tracks() const { return tracks_info_; }

signals:
    void            parsingFinished(const TracksInfo&);
    

private:
    void            readPeriod(){
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"Period");

        if (reader_.attributes().hasAttribute("duration")) {
            QString duration = reader_.attributes().value("duration").toString();
            tracks_info_.duration = getDuration(duration);
            //TODO: parse PT7160.236S
        }

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"AdaptationSet")
                readAdaptationSet();
            else
                reader_.skipCurrentElement();
        }
    }

    void            readAdaptationSet(){
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"AdaptationSet");

        QString mimeType = reader_.attributes().value("mimeType").toString();
        
        QString audioTrackId = reader_.attributes().hasAttribute("audioTrackId") ?
            reader_.attributes().value("audioTrackId").toString(): ""; // amazon primvideo

        QString lang_tag = reader_.attributes().hasAttribute("lang") ?
            reader_.attributes().value("lang").toString().toLower() : ""; // amazon primvideo

        bool is_encrypted = false;
        QString cench_pssh;
        while (reader_.readNextStartElement()) {
            const auto& n = reader_.name().toString();
            if (reader_.name() == L"ContentProtection") {
                if (reader_.attributes().hasAttribute("cenc:default_KID")) {
                    QString kid = reader_.attributes().value("cenc:default_KID").toString();
                    tracks_info_.key_id = QByteArray::fromHex(kid.toLatin1());
                    if (!tracks_info_.key_id.isEmpty())
                        is_encrypted = true;
                }
                if (reader_.attributes().hasAttribute("schemeIdUri")) {
                    QString cenc_pssh;
                    auto schemeIdUri = reader_.attributes().value("schemeIdUri").toString();
                    if (schemeIdUri == "urn:uuid:edef8ba9-79d6-4ace-a3c8-27dcd51d21ed") { // widevine
                        if (reader_.readNextStartElement()) {
                            if (reader_.name().contains(L"pssh"))
                                cench_pssh = reader_.readElementText();
                        }
                    }
                }
                reader_.skipCurrentElement();
            }
            else if (reader_.name() == L"Representation") {
                if (mimeType.contains("video")) {
                    VideoTrackInfo track;
                    track.is_encrypted = is_encrypted;
                    readRepresentation(&track);
                    if (track.urls.size() > 0)
                        tracks_info_.video_tracks.append(track);
                }
                else if (mimeType.contains("audio")) {
                    AudioTrackInfo track;
                    track.is_encrypted = is_encrypted;
                    track.cench_pssh = cench_pssh;
                    readRepresentation(&track);
                    const auto& x = reader_.name().toString();

                    track.audioTrackId = audioTrackId;
                    QString tag = track.audioTrackId.split("_").first().toLower();
                    if (tag.isEmpty())
                        tag = lang_tag;

                    track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));

                    if (audioTrackId.contains("descriptive"))
                        track.is_assistive = true;

                    if (track.urls.size() > 0)
                        tracks_info_.audio_tracks.append(track);
                }
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readRepresentation(VideoTrackInfo* info){
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"Representation");

        if (reader_.attributes().hasAttribute("bandwidth"))
            info->bitrate = reader_.attributes().value("bandwidth").toInt();

        if (reader_.attributes().hasAttribute("frameRate"))
            info->framerate = reader_.attributes().value("frameRate").toString().split("/").first().toInt();

        info->width = reader_.attributes().value("width").toInt();
        info->height = reader_.attributes().value("height").toInt();

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"BaseURL") {
                info->urls.append(reader_.readElementText());
            }
            else if (reader_.name() == L"SegmentList") {
                readSegments(info);
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readSegments(VideoTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"SegmentList");
        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"Initialization") {
                if (reader_.attributes().hasAttribute("sourceURL"))
                    info->urls.append(reader_.attributes().value("sourceURL").toString());

                if (reader_.attributes().hasAttribute("range")) {
                    QString track_size = reader_.attributes().value("range").toString();
                    info->size = track_size.split("-").last().toLongLong();
                }

                reader_.skipCurrentElement();
            }
            else if (reader_.name() == L"SegmentURL") {
                info->is_chunked = true;

                if (reader_.attributes().hasAttribute("media"))
                    info->urls.append(reader_.attributes().value("media").toString());

                if (reader_.attributes().hasAttribute("mediaRange")) {
                    QString track_size = reader_.attributes().value("mediaRange").toString();
                    info->size = track_size.split("-").last().toLongLong();
                }

                reader_.skipCurrentElement();
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readRepresentation(AudioTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"Representation");

        if (reader_.attributes().hasAttribute("bandwidth"))
            info->bitrate = reader_.attributes().value("bandwidth").toInt();

        if (reader_.attributes().hasAttribute("bitrate"))
            info->bitrate = reader_.attributes().value("bitrate").toInt() * 1000;
        
        if (reader_.attributes().hasAttribute("codecs"))
            info->profile = getAudioProfile(reader_.attributes().value("codecs").toString());

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"BaseURL") {
                info->urls.append(reader_.readElementText());
            }
            else if (reader_.name() == L"AudioChannelConfiguration") {
                QString value = reader_.attributes().value("value").toString().toLower();
                info->channels = getAudioChannelsCount(value);
                reader_.skipCurrentElement();
            }
            else if (reader_.name() == L"SegmentList") {
                readSegments(info);
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readSegments(AudioTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"SegmentList");
        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"Initialization") {
                if (reader_.attributes().hasAttribute("sourceURL"))
                    info->urls.append(reader_.attributes().value("sourceURL").toString());

                if (reader_.attributes().hasAttribute("range")) {
                    QString track_size = reader_.attributes().value("range").toString();
                    info->size = track_size.split("-").last().toLongLong();
                }

                reader_.skipCurrentElement();
            }
            else if (reader_.name() == L"SegmentURL") {
                info->is_chunked = true;
                
                if (reader_.attributes().hasAttribute("media"))
                    info->urls.append(reader_.attributes().value("media").toString());

                if (reader_.attributes().hasAttribute("mediaRange")) {
                    QString track_size = reader_.attributes().value("mediaRange").toString();
                    info->size = track_size.split("-").last().toLongLong();
                }

                reader_.skipCurrentElement();
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    int getDuration(const QString& data) {
        int hour = 0;
        int min = 0;
        int sec = 0;
        if (data.contains("H"))
            hour = data.split("T").last().split("H").first().toInt() * 60 * 60;
        if (data.contains("M")) {
            if (data.contains("H"))
                min = data.split("H").last().split("M").first().toInt() * 60;
            else
                min = data.split("T").last().split("M").first().toInt() * 60;
        }
        if (data.contains("M"))
            sec = data.split("M").last().split("S").first().split(".").first().trimmed().toInt();
        else
            sec = data.split("T").last().split("S").first().split(".").first().trimmed().toInt();

        int time = hour + min + sec;

        return time;
    }

    int getAudioChannelsCount(const QString& value) {
        int channels = 2;

        if (value == "4000"
            || value == "1")
            channels = 1;
        else if (value == "a000"
            || value == "2")
            channels = 2;
        else if (value == "f801"
            || value == "6")
            channels = 6;
        else if (value == "fa01"
            || value == "8")
            channels = 8;

        return channels;
    }

    AudioProfile getAudioProfile(const QString& codec) {
        AudioProfile profile = AAC;

        if (codec.contains("mp4a.40.29"))
            profile = AAC_HEAACv2;
        else if (codec.contains("mp4a.40.5"))
            profile = AAC_HEAAC;
        else if (codec.contains("mp4a.40.2"))
            profile = AAC_MP4A;
        else if (codec.contains("ac-3"))
            profile = DolbyDigital;
        else if (codec.contains("ec-3"))
            profile = DolbyDigitalPlus;

        return profile;
    }
    

private:
    TracksInfo                      tracks_info_;
    QXmlStreamReader                reader_;
};