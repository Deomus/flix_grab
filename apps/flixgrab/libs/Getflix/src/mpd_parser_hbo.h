#pragma once

#include <QtCore>

#include <languages.h>
#include <utils/qs_utils.h>

#include "tracks_info.h"


class MpdParserHBO : public QObject {

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
        if (tracks_info_.video_tracks.size() || tracks_info_.audio_tracks.size()) {
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
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"Period");

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"AdaptationSet")
                readAdaptationSet();
            else
                reader_.skipCurrentElement();
        }
    }

    void            readAdaptationSet(){
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"AdaptationSet");

        QString contentType = reader_.attributes().hasAttribute("contentType") ? reader_.attributes().value("contentType").toString().toLower() : "";
        QString lang_tag = reader_.attributes().hasAttribute("lang") ? reader_.attributes().value("lang").toString().toLower() : "";
        AudioTrackInfo audio_track;
        SubtitleTrackInfo subtitle_track;
        
        QString pssh;

        bool encrypted = false;
        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"ContentProtection") {
                if (reader_.attributes().hasAttribute("value")
                    && reader_.attributes().value("value").toString() == "cenc") {
                    if (reader_.attributes().hasAttribute("_:default_KID")) {
                        QString kid = reader_.attributes().value("_:default_KID").toString();
                        tracks_info_.key_id = QByteArray::fromHex(kid.toLatin1());
                        if (!tracks_info_.key_id.isEmpty())
                            encrypted = true;
                    }
                }
                if (reader_.attributes().hasAttribute("schemeIdUri")) {
                    QString cenc_pssh;
                    auto schemeIdUri = reader_.attributes().value("schemeIdUri").toString();
                    if (schemeIdUri == "urn:uuid:edef8ba9-79d6-4ace-a3c8-27dcd51d21ed") { // widevine
                        if (reader_.readNextStartElement()) {
                            if (reader_.name().contains(L"pssh")) {
                                pssh = reader_.readElementText();
                                encrypted = true;
                                //tracks_info_.pssh = reader_.readElementText();
                            }
                        }
                    }
                }

                reader_.skipCurrentElement();
            }
            else if (reader_.name() == L"Role") {
                if (reader_.attributes().hasAttribute("value")) {
                    QString type = reader_.attributes().value("value").toString();
                    if (contentType == "audio") {
                        if (type == "alternate")
                            audio_track.is_assistive = true;
                    }
                    else if (contentType == "text") {
                        if (type == "caption")
                            subtitle_track.is_closed_captions = true;
                        else if (type == "forced-subtitle")
                            subtitle_track.is_additional = true;
                    }
                }

                reader_.skipCurrentElement();
            }
            else if (reader_.name() == L"Representation"
                && (contentType == "audio" || contentType == "video" || contentType == "text")) {

                QString mimeType = reader_.attributes().hasAttribute("mimeType") ? reader_.attributes().value("mimeType").toString().toLower() : "";
                if (mimeType.contains(L"audio")) {
                    audio_track.is_encrypted = encrypted;
                    readRepresentation(&audio_track);

                    if (reader_.attributes().hasAttribute("codecs"))
                        audio_track.profile = getAudioProfile(reader_.attributes().value("codecs").toString());
                    
                    audio_track.cench_pssh = pssh;
                    audio_track.language_tag = SS2Q(normalizeLangTag(QS2S(lang_tag)));
                    if (audio_track.urls.size() > 0)
                        tracks_info_.audio_tracks.append(audio_track);
                }
                else if (mimeType.contains(L"video")) {
                    VideoTrackInfo track;
                    track.is_encrypted = encrypted;
                    track.cench_pssh = pssh;
                    readRepresentation(&track);
                    if (track.urls.size() > 0) {
                        if (reader_.attributes().hasAttribute("frameRate"))
                            track.framerate = reader_.attributes().value("frameRate").toString().split("/").first().toInt();

                        tracks_info_.video_tracks.append(track);
                    }
                }
                else if (mimeType.contains(L"text")) {
                    subtitle_track.language_tag = SS2Q(normalizeLangTag(QS2S(lang_tag)));
                    if (mimeType.contains(L"vtt"))
                        subtitle_track.profile = SubtitleTrackInfo::WEBVTT;

                    readRepresentation(&subtitle_track);
                    if (subtitle_track.urls.size() > 0)
                        tracks_info_.subtitle_tracks.append(subtitle_track);
                }
                else {
                    reader_.skipCurrentElement();
                }
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readRepresentation(AudioTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"Representation");

        if (reader_.attributes().hasAttribute("bandwidth"))
            info->bitrate = reader_.attributes().value("bandwidth").toInt();

        if (reader_.attributes().hasAttribute("codecs"))
            info->profile = getAudioProfile(reader_.attributes().value("codecs").toString());

        //TODO: Fill size value;

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"BaseURL") {
                info->urls.append(reader_.readElementText());
            }
            else if (reader_.name() == L"AudioChannelConfiguration") {
                QString value = reader_.attributes().value("value").toString().toLower();
                info->channels = getAudioChannelsCount(value);
                reader_.skipCurrentElement();
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readRepresentation(VideoTrackInfo* info){
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"Representation");

        if (reader_.attributes().hasAttribute("bandwidth"))
            info->bitrate = reader_.attributes().value("bandwidth").toInt();
        if (reader_.attributes().hasAttribute("width"))
            info->width = reader_.attributes().value("width").toInt();
        if (reader_.attributes().hasAttribute("height"))
            info->height = reader_.attributes().value("height").toInt();

        QString codec = reader_.attributes().hasAttribute("codecs") ? reader_.attributes().value("codecs").toString() : "";
        if (codec.startsWith("hvc"))
            info->profile = VideoProfile::HVC;

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"BaseURL"
                && (codec.contains(L"avc1") /*|| codec.startsWith(L"hvc")*/)) {
                info->urls.append(reader_.readElementText());
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readRepresentation(SubtitleTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"Representation");

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"BaseURL") {
                info->urls.append(reader_.readElementText());
            }
            else if (reader_.name() == L"SegmentTemplate") {
                readSegments(info);
            }
            else {
                reader_.skipCurrentElement();
            }
        }
    }

    void            readSegments(SubtitleTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement() && reader_.name() == L"SegmentTemplate");

        const QString media = reader_.attributes().hasAttribute("media") ? reader_.attributes().value("media").toString() : "";

        if (!media.isEmpty()) {
            while (reader_.readNextStartElement()) {
                if (reader_.name() == L"SegmentTimeline") {
                    int segments_count = 0;
                    while (reader_.readNextStartElement()) {
                        //qDebug() << "SegmentTimeline reader_.name(): " << reader_.name();
                        if (reader_.name() == L"S")
                            segments_count++;
                        else
                            break;
                    }

                    reader_.skipCurrentElement();
                    //qDebug() << "SegmentTimeline out reader_.name(): " << reader_.name() << " segments_count: " << segments_count;

                    for (int i = 1; i <= segments_count; i++) {
                        QString it = media;
                        it = it.replace("$Number$", QString::number(i));
                        info->urls << it;
                    }
                    if (info->urls.size() > 1)
                        info->is_chunked = true;
                }
                else {
                    reader_.skipCurrentElement();
                }
            }
        }
        else {
            reader_.skipCurrentElement();
        }
    }

    int getDuration(const QString& data) {
        int hour = 0;
        int min = 0;
        int sec = 0;
        if (data.contains(L"H"))
            hour = data.split("T").last().split("H").first().toInt() * 60 * 60;
        if (data.contains(L"M")) {
            if (data.contains(L"H"))
                min = data.split("H").last().split("M").first().toInt() * 60;
            else
                min = data.split("T").last().split("M").first().toInt() * 60;
        }
        if (data.contains(L"M"))
            sec = data.split("M").last().split("S").first().split(".").first().trimmed().toInt();
        else
            sec = data.split("T").last().split("S").first().split(".").first().trimmed().toInt();

        int time = hour + min + sec;

        return time;
    }

    int getAudioChannelsCount(const QString& value) {
        int channels = 2;

        if (value =="4000"
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

        if (codec.contains(L"mp4a.40.29"))
            profile = AAC_HEAACv2;
        else if (codec.contains(L"mp4a.40.5"))
            profile = AAC_HEAAC;
        else if (codec.contains(L"mp4a.40.2"))
            profile = AAC_MP4A;
        else if (codec.contains(L"ac-3"))
            profile = DolbyDigital;
        else if (codec.contains(L"ec-3"))
            profile = DolbyDigitalPlus;

        return profile;
    }

private:
    TracksInfo                  tracks_info_;
    QXmlStreamReader            reader_;
};