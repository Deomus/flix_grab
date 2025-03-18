#pragma once

#include <QtCore>

#include <languages.h>
#include <utils/qs_utils.h>

#include "tracks_info.h"


class MpdParserHulu : public QObject {

    Q_OBJECT

public:
    bool            read(const QString& mpd_file) {
#ifdef _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("hulu_mpd_" + QString::number(ms) + ".xml");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(mpd_file.toLocal8Bit());
            file.close();
        }
#endif
        tracks_info_ = {};
        video_urls_.clear();
        audio_urls_.clear();
        reader_.clear();
        reader_.addData(mpd_file);

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"MPD") {
                while (reader_.readNextStartElement()) {
                    if (reader_.name() == L"Period"
                        && reader_.attributes().hasAttribute("id")
                        && reader_.attributes().value("id").toString().contains("content")) {
                        readPeriod();
                    }
                    else {
                        reader_.skipCurrentElement();
                    }
                }
            }
            else
                reader_.skipCurrentElement();
        }

        bool result = false;
        if (tracks_info_.video_tracks.size() || tracks_info_.audio_tracks.size())
            result = true;

        return result;
    }

    const TracksInfo& get_tracks() const { return tracks_info_; }

private:
    void            readPeriod() {
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"Period");

        if (reader_.attributes().hasAttribute("duration")) {
            QString duration = reader_.attributes().value("duration").toString();
            tracks_info_.duration = getDuration(duration);                          //PT21M6.304S
            //TODO: parse PT7160.236S
        }

        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"AdaptationSet")
                readAdaptationSet();
            else
                reader_.skipCurrentElement();
        }
    }

    void            readAdaptationSet() {
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"AdaptationSet");
        
        QString mimeType = reader_.attributes().value("mimeType").toString();

        QString label = reader_.attributes().hasAttribute("label") ? reader_.attributes().value("label").toString() : "";

        QString lang_tag = reader_.attributes().hasAttribute("lang") ? reader_.attributes().value("lang").toString().toLower() : "";

        bool encrypted = false;
        while (reader_.readNextStartElement()) {
            if (reader_.name() == L"ContentProtection") {
                if (reader_.attributes().hasAttribute("cenc:default_KID")) {
                    QString kid = reader_.attributes().value("cenc:default_KID").toString();
                    tracks_info_.key_id = QByteArray::fromHex(kid.toLatin1());
                    if (!tracks_info_.key_id.isEmpty())
                        encrypted = true;
                }
                if (mimeType == "video/mp4" && reader_.attributes().hasAttribute("schemeIdUri")) {
                    QString cenc_pssh;
                    auto schemeIdUri = reader_.attributes().value("schemeIdUri").toString();
                    if (schemeIdUri == "urn:uuid:edef8ba9-79d6-4ace-a3c8-27dcd51d21ed") { // widevine
                        //if (reader_.readNextStartElement()) {
                        //    if (reader_.name().contains("pssh"))
                        //        tracks_info_.pssh = reader_.readElementText();
                        //}
                    }
                }
            }
            else if (reader_.name() == L"Representation") {
                if (mimeType.contains("video")) {
                    VideoTrackInfo track;
                    track.is_encrypted = encrypted;
                    readRepresentation(&track);
                    if (track.urls.size() > 0)
                        tracks_info_.video_tracks.append(track);
                    continue;
                }

                else if (mimeType.contains("audio")) {
                    AudioTrackInfo track;
                    track.is_encrypted = encrypted;
                    readRepresentation(&track);

                    track.language_tag = SS2Q(normalizeLangTag(QS2S(lang_tag)));

                    if (label == "description")
                        track.is_assistive = true;

                    if (track.urls.size() > 0)
                        tracks_info_.audio_tracks.append(track);
                    continue;
                }

            }

            reader_.skipCurrentElement();
        }
    }

    void            readRepresentation(VideoTrackInfo* info) {
        Q_ASSERT(reader_.isStartElement()
            && reader_.name() == L"Representation");

        if (reader_.attributes().hasAttribute("bandwidth"))
            info->bitrate = reader_.attributes().value("bandwidth").toInt();

        if (reader_.attributes().hasAttribute("frameRate"))
            info->framerate = reader_.attributes().value("frameRate").toString().split("/").first().toInt();

        info->width = reader_.attributes().value("width").toInt();
        info->height = reader_.attributes().value("height").toInt();

        //TODO: Fill size value;
        bool enable_url = false;
        while (reader_.readNextStartElement()) {
            QString name = reader_.name().toString();
            if (name == "Cdn") {
                if (reader_.readElementText() == "fa")
                    enable_url = true;

                continue;
            }

            if (name == "BaseURL") {
                QString url = reader_.readElementText();
                if (enable_url
                    && !video_urls_.contains(url)) {
                    video_urls_.push_back(url);
                    info->urls.append(url);
                }
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

        if (reader_.attributes().hasAttribute("codecs"))
            info->profile = getAudioProfile(reader_.attributes().value("codecs").toString());

        //TODO: Fill size value;
        bool enable_url = false;
        while (reader_.readNextStartElement()) {
            QString name = reader_.name().toString();
            if (name == "Cdn") {
                if (reader_.readElementText() == "fa")
                    enable_url = true;

                continue;
            }

            if (name == "BaseURL") {
                QString url = reader_.readElementText();
                if (enable_url
                    && !audio_urls_.contains(url)) {
                    audio_urls_.push_back(url);
                    info->urls.append(url);
                }

                continue;
            }
            else if (name == "AudioChannelConfiguration") {
                QString value = reader_.attributes().value("value").toString().toLower();
                info->channels = getAudioChannelsCount(value);
            }

            reader_.skipCurrentElement();
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
    QList<QString>                  video_urls_;
    QList<QString>                  audio_urls_;
};