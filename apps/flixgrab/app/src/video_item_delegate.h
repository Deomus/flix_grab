#pragma once

#include <QtCore>

#include <languages.h>
#include <utils/qs_utils.h>
#include <product_config.h>
#include <limitations.h>

#include "video_handle.h"
#include "product_controller.h"


class VideoItemController;

class VideoItemDelegate : public QObject {
    Q_OBJECT
public:
    VideoItemDelegate(VideoItemController* controller);

    const EPortal& portal() const {
        return video_->portal();
    }

    qint64 get_size_item_for_sort() {
        return size_item_for_sort_;
    }

    int get_height_item_for_sort() {
        return height_item_for_sort_;
    }

    void set_date_in_milliseconds_now_for_sort(const int date_in_milliseconds_now) {
        date_in_milliseconds_now_ = date_in_milliseconds_now;
    }

    int get_date_in_milliseconds_now_for_sort() {
        return date_in_milliseconds_now_;
    }

    bool isSlowdownMode() const {
        return video_->slowdown() > 0;
    }
   
    QString videoUrl() const {
        return video_->url().toString();
    }

    const VideoInfo& videoInfo() const {
        return video_->videoInfo();
    }

    double progress() const {
        return video_->progress();
    }

    VideoItemController* controller() const {
        return controller_;
    }

    void selectPriority(int priority) {
        Q_ASSERT(controller_);
        video_->setPriority(priority);
    }

    int selectedPriority() const {
        return video_->priority();
    }

    QString outputName() const {
        return video_->outputName();
    }

    QString outputPath() const {
        return video_->outputPath();
    }
    
    QString outputMediaPath() const {
        return video_->outputMediaPath();
    }

    void correctOutputName(const QString& output_dir, QString& file_name) const {
        video_->correctOutputName(output_dir, file_name);
    }

    void setOutputPath(const QString& output_dir, const QString& file_name) const {
        video_->setOutputPath(output_dir, file_name);
    }

    EStatusCode statusCode() const {
        return video_->status().code();
    }

    QString statusMessage() const {
        return video_->status().message();
    }

    QString statusDesc() const {
        QString subcode = QString::number(video_->status().subcode());
        return ProductController::instance()->statusDesc(video_->status().code(), subcode);
    }

    bool isDownloadable() const {
        return video_->isDownloadable();
    }

    void            updateItemDesk();

    void            setAudioSelector(const EAudioSelector& selector) const {
        video_->setAudioSelector(selector);
    }

    int             audioTracksCount() const {
        return video_->tracksInfo().audio_tracks.size();
    }

    void            selectAudioTracks(const QList<int>& tracks);

    const QList<int>& selectedAudioTracks() const {
        return video_->audioIds();
    }

    int             audioLanguagesCount() const {
        return video_->tracksInfo().audio_languages.size();
    }

    void            selectAudioLanguageTracks(const QList<int>& tracks) {
        video_->setAudioLanguageIds(tracks);
    }

    const QList<int>& selectedAudioLanguagesTracks() const {
        return video_->audioLanguageIds();
    }

    const QList<QString> selectedAudioLanguages(const QList<int>& tracks) const {
        QList<QString> langs;
        
        for (int index : tracks)
            langs << video_->tracksInfo().audio_languages[index];
        
        return langs;
    }

    void            selectSaveAsTracks(const QList<int>& tracks);

    const QList<int>& selectedSaveAsTracks() const {
        return video_->saveAsIds();
    }

    int             videoTracksCount() const {
        return video_->tracksInfo().video_tracks.size();
    }

    void            selectVideoTracks(const QList<int>& tracks);

    const QList<int>& selectedVideoTracks() const { 
        return video_->videoIds(); 
    }

    int             subtitleTracksCount() const {
        return video_->tracksInfo().subtitle_tracks.size();
    }

    void            selectSubtitleTracks(const QList<int>& tracks);

    const QList<int>&   selectedSubtitleTracks() const {
        return video_->subtitleIds();
    }

    void            selectSaveSubtitlesAsTracks(const QList<int>& tracks);

    const QList<int>& selectedSaveSubtitlesAsTracks() const {
        return video_->saveSubtitlesAsIds();
    }

    void            setImageSelector(const EImageSelector& selector) const {
        video_->setImageSelector(selector);
    }

    int             imageTracksCount() const {
        return video_->tracksInfo().image_tracks.size();
    }

    void            selectImageTracks(const QList<int>& tracks);

    const QList<int>& selectedImageTracks() const { 
        return video_->imageIds();
    }

    QString         saveAsInfo(const int preset) const;

    QString         videoQualityInfo(const VideoTrackInfo& video_track, bool add_quality = true) {
        QString info;

        if (video_track.height > 0)
            info = getVideoQualityEvaluationDesc(video_track.height);
        else
            info = getManualEvaluationDesc(video_track.eval_type);

        if (add_quality 
            && !info.isEmpty())
            info.append(" ").append(tr("quality"));

        return info;
    }

    QString         videoStreamInfo(const VideoTrackInfo& video_track) const {
        QString info;

        if (video_track.height > 0) {
            QString framerate_desk;
            if (video_track.framerate > 49)
                framerate_desk = "60";
            info = QString::number(video_track.height).append("p").append(framerate_desk);
        }

        return info;
    }

    QString         videoResolutionInfo(const VideoTrackInfo& video_track) const {
        QString info;

        if (video_track.height > 0
            && video_track.width > 0)
            info = "(" + QString::number(video_track.width) + "x" + QString::number(video_track.height) + ")";

        return info;
    }

    QString         videoBitrateInfo(const VideoTrackInfo& video_track) const {
        QString info;

        if (video_track.bitrate > 0) {

            info = QString::number(video_track.bitrate);

            if (!info.isEmpty())
                info.append("k");
        }

        return info;
    }

    QString         videoSizeInfo(const VideoTrackInfo& video_track) const {
        QString info;

        if (video_track.size > 0
            && !video_track.is_fake_size) {

            if (video_track.is_calc_size)
                info = "~" + info;

            info.append(sizeDesk(video_track.size));
        }

        return info;
    }

    QString         audioLanguageInfo(const QString& audio_language) const {
        QString info;

        if (!audio_language.isEmpty()) {
            info = SS2Q(getLangNameByLangTag(QS2S(audio_language)));

            if (info.isEmpty()) {
                QString lang = audio_language;
                if (lang.contains("-")) {
                    QString last_part = lang.split("-").last();
                    lang = lang.split("-").first() + "-" + last_part.replace(0, 1, last_part[0].toUpper());
                }

                QLocale locale(lang);
                QString country;

                if (audio_language.contains("-"))
                    country = " " + locale.countryToString(locale.country());

                info = QString("%1%2").arg(QLocale::languageToString(locale.language())).arg(country);
            }
        }

        return info;
    }

    QString         audioQualityInfo(const AudioTrackInfo& audio_track, bool add_quality = false) {
        QString info;

        if (audio_track.bitrate > 0)
            info = getAudioQualityEvaluationDesc(audio_track.bitrate);
        else
            info = getManualEvaluationDesc(audio_track.eval_type);

        if (add_quality 
            && !info.isEmpty())
            info.append(" ").append(tr("quality"));

        return info;
    }

    QString         audioSizeInfo(const AudioTrackInfo& audio_track) const {
        QString info;

        if (audio_track.size > 0 
            && !audio_track.is_fake_size)
            info = sizeDesk(audio_track.size);

        return info;
    }

    QString         audioBitrateInfo(const AudioTrackInfo& audio_track) const {
        QString info;

        if (audio_track.bitrate > 0) {

            info = QString::number(audio_track.bitrate);

            if (!info.isEmpty())
                info.append("k");
        }

        return info;
    }

    QString         audioStreamInfo(const AudioTrackInfo& audio_track) const {
        QString info;

        switch (audio_track.profile) {
        case AudioProfile::UnknownProfile: info = ""; break;
        case AudioProfile::OGG: info = "ogg"; break;
        case AudioProfile::AAC: info = "aac"; break;
        case AudioProfile::AAC_HEAACv2: info = "aac (he-aac v2)"; break;
        case AudioProfile::AAC_HEAAC: info = "aac (he-aac)"; break;
        case AudioProfile::AAC_MP4A: info = "aac (mp4a)"; break;
        case AudioProfile::DolbyDigital20: info = "dd"; break;
        case AudioProfile::AAC51: info = "aac 5.1"; break;
        case AudioProfile::DolbyDigital: info = "dd 5.1"; break;
        case AudioProfile::DolbyDigitalPlus: info = "dd+ 5.1"; break;
        case AudioProfile::DolbyDigitalPlusAtmos: info = "datmos"; break;
        default:
            switch (audio_track.channels) {
            case 0: info = ""; break;
            case 1: info = tr("mono"); break;
            case 2: info = tr("stereo"); break;
            case 6: info = "5.1"; break;
            }
        }

        return info;
    }

    QString         imageQualityInfo(const ImageTrackInfo& image_track, bool add_quality = false) {
        QString info;

        if (image_track.width > 0)
            info = getImageQualityEvaluationDesc(image_track.width);
        else
            info = getManualEvaluationDesc(image_track.eval_type);

        if (add_quality 
            && !info.isEmpty())
            info.append(" ").append(tr("quality"));

        return info;
    }

    QString         imageStreamInfo(const ImageTrackInfo& image_track) const {
        QString info;

        if (image_track.width > 0 
            && image_track.height > 0)
            info = QString::number(image_track.width).append("x").append(QString::number(image_track.height));

        return info;
    }

    QString         imageSizeInfo(const ImageTrackInfo& image_track) const {
        QString info;

        if (image_track.size > 0
            && !image_track.is_fake_size)
            info = sizeDesk(image_track.size);

        return info;
    }
    
    QString         subtitleLanguageInfo(const SubtitleTrackInfo& subtitle_track) const {
        QString info;

        if (!subtitle_track.language_tag.isEmpty()) {
            info = SS2Q(getLangNameByLangTag(QS2S(subtitle_track.language_tag)));

            if (info.isEmpty()) {
                QString lang = subtitle_track.language_tag;
                if (lang.contains("-")) {
                    QString last_part = lang.split("-").last();
                    lang = lang.split("-").first() + "-" + last_part.replace(0, 1, last_part[0].toUpper());
                }

                QLocale locale(lang);
                QString country;
                if (subtitle_track.language_tag.contains("-"))
                    country = " " + locale.countryToString(locale.country());

                QString language = QLocale::languageToString(locale.language());

                if (language == "C")
                    language = subtitle_track.language;

                info = QString("%1%2").arg(language).arg(country);
            }
        }

        return info;
    }

    QString         subtitleStreamInfo(const SubtitleTrackInfo& subtitle_track) const {
        QString info;

        switch (subtitle_track.profile) {
        case SubtitleTrackInfo::SimpleSdh:
            info = "TTML";
            break;
        case SubtitleTrackInfo::LessSimpleSdh:
            info = "TTML-dfxp";
            break;
        case SubtitleTrackInfo::NflxCmisc:
            info = "TTML2-images";
            break;
        case SubtitleTrackInfo::WEBVTT:
            info = "WebVTT";
            break;
        case SubtitleTrackInfo::SRV1:
            info = "SRV1";
            break;
        case SubtitleTrackInfo::SRV2:
            info = "SRV2";
            break;
        case SubtitleTrackInfo::SRV3:
            info = "SRV3";
            break;
        case SubtitleTrackInfo::XML:
            info = "XML";
            break;
        }

        return info;
    }

    QString         subtitleSizeInfo(const SubtitleTrackInfo& subtitle_track) const {
        QString info;

        if (subtitle_track.size > 0)
            info = sizeDesk(subtitle_track.size);

        return info;
    }

    QString         videoTrackDesc(int id) {
        QString desc;

        if (videoTracksCount() > 0) {
            const VideoTrackInfo& track = video_->tracksInfo().video_tracks[id];

            desc.append(videoQualityInfo(track));
            desc.append(addSeparator(desc, videoStreamInfo(track)));
            desc.append(addSeparator(desc, videoResolutionInfo(track)));
            desc.append(addSeparator(desc, videoBitrateInfo(track)));
            desc.append(addSeparator(desc, videoSizeInfo(track)));
            desc.append(addSeparator(desc, track.extended_desk));
        }

        if (desc.isEmpty())
            desc = tr("No video");

        return desc;
    }

    QString         audioTrackDesc(int id) {
        QString desc;
        
        if (audioTracksCount() > 0) {
            const AudioTrackInfo& track = video_->tracksInfo().audio_tracks[id];

            desc.append(audioQualityInfo(track, true));
            desc.append(addSeparator(desc, audioBitrateInfo(track)));
            desc.append(addSeparator(desc, audioSizeInfo(track)));
            desc.append(addSeparator(desc, audioStreamInfo(track)));
            desc.append(addSeparator(desc, track.extended_desk));
        }

        if (desc.isEmpty())
            desc = tr("No audio");

        return desc;
    }

    QString         imageTrackDesc(int id) {
        QString desc;
        
        if (imageTracksCount() > 0) {
            const ImageTrackInfo& track = video_->tracksInfo().image_tracks[id];

            desc.append(imageQualityInfo(track, true));
            desc.append(addSeparator(desc, imageStreamInfo(track)));
            desc.append(addSeparator(desc, imageSizeInfo(track)));
        }

        if (desc.isEmpty())
            desc = tr("No images");

        return desc;
    }

    QString         audioLanguageDesk(int id, bool add_country = true) const {
        QString desc;

        if (audioLanguagesCount() > 0) {
            QString track = video_->tracksInfo().audio_languages[id];

            if (track == "zxx") {  //netflix case
                desc = tr("Original");
            }
            else {
                desc = SS2Q(getLangNameByLangTag(QS2S(track)));
                
                if (desc.isEmpty()) {
                    QString lang = track;

                    if (lang.contains("-")) {
                        QString last_part = lang.split("-").last();
                        lang = lang.split("-").first() + "-" + last_part.replace(0, 1, last_part[0].toUpper());
                    }

                    QLocale locale(lang);

                    QString country;
                    if (add_country &&
                        track.contains("-"))
                        country = " " + locale.countryToString(locale.country());
                }
            }
        }

        return desc;
    }

    QString         subtitleTrackDesc(int id) const {
        QString desc;
        
        if (subtitleTracksCount() > 0) {
            const SubtitleTrackInfo& track = video_->tracksInfo().subtitle_tracks[id];

            desc.append(subtitleLanguageInfo(track));
            if (track.is_closed_captions)
                desc.append(addSeparator(desc, subtitlesCCInfo(video_->portal())));
            if (track.is_additional)
                desc.append(addSeparator(desc, subtitlesAdditionalInfo(video_->portal()).toLower()));
            desc.append(addSeparator(desc, subtitleStreamInfo(track)));
            desc.append(addSeparator(desc, subtitleSizeInfo(track)));
        }

        return desc;
    }

    QString        selectedVideoTracksDesc() {
        QString desc;

        if (!selectedVideoTracks().isEmpty()
            && videoTracksCount() > 0) {

            auto ids = selectedVideoTracks();

            desc.append(QString::number(ids.size()) + "x");
            desc.append(addSeparator(desc, sizeDesk(selectedVideoTracksSize())));

            desc = ids.size() > 1 ? desc :
                videoTrackDesc(ids.first());
        }

        if (desc.isEmpty())
            desc = tr("No video");

        return desc;
    }

    QString        selectedAudioTracksDesc() {
        QString desc;
        
        if (!selectedAudioTracks().isEmpty()
            && audioTracksCount() > 0) {

            auto ids = selectedAudioTracks();

            desc.append(QString::number(ids.size()) + "x");
            desc.append(addSeparator(desc, sizeDesk(selectedAudioTracksSize())));

            desc = ids.size() > 1 ? desc :
                audioTrackDesc(ids.first());
        }

        if (desc.isEmpty())
            desc = tr("No audio");

        return desc;
    }

    QString        selectedImageTracksDesc() {
        QString desc;

        if (!selectedImageTracks().isEmpty()
            && imageTracksCount() > 0) {

            auto ids = selectedImageTracks();

            desc.append(QString::number(ids.size()) + "x");
            desc.append(addSeparator(desc, sizeDesk(selectedImageTracksSize())));

            desc = ids.size() > 1 ? desc :
                imageTrackDesc(ids.first());
        }

        if (desc.isEmpty())
            desc = tr("No images");

        return desc;
    }

    QString         selectedAudioLanguagesDesc() const {
        QString desc;

        if (!selectedAudioLanguagesTracks().isEmpty()
            && audioLanguagesCount() > 0) {

            auto ids = selectedAudioLanguagesTracks();

            if (ids.size() == 1) {
                desc = audioLanguageDesk(ids.first());
            }
            else if (ids.size() < 4) {
                for (int index : ids) {
                    const QString& track = video_->tracksInfo().audio_languages[index];

                    if (!desc.isEmpty())
                        desc += ", ";

                    desc += audioLanguageInfo(track);
                }
            }
            else {
                desc = QString::number(ids.size()) + "x " + tr("languages");
            }
        }

        if (desc.isEmpty())
            desc = tr("None");

        return desc;
    }

    QString         selectedSubtitlesTracksDesc() const {
        QString desc;

        if (!selectedSubtitleTracks().isEmpty()
            && subtitleTracksCount() > 0) {

            auto ids = selectedSubtitleTracks();
            if (ids.size() == 1) {
                desc = subtitleTrackDesc(ids.first());
            }
            else if (ids.size() < 4) {
                for (int index : ids) {
                    const SubtitleTrackInfo& track = video_->tracksInfo().subtitle_tracks[index];

                    if (!desc.isEmpty())
                        desc += ", ";

                    desc.append(subtitleLanguageInfo(track));
                    if (track.is_closed_captions)
                        desc.append(addSeparator(desc, subtitlesCCInfo(video_->portal())));
                    if (track.is_additional) {
                        QString addit_info = subtitlesAdditionalInfoShort(portal());
                        if (!addit_info.isEmpty()) {
                            addit_info = "(" + addit_info + ")";
                            desc.append(addSeparator(desc, addit_info));
                        }
                    }
                }
                desc.append(addSeparator(desc, sizeDesk(selectedSubtitleTracksSize())));
            }
            else {
                desc = QString::number(ids.size()) + "x " + tr("languages");
            }
        }

        if (desc.isEmpty())
            desc = tr("None");

        return desc;
    }

    qint64          selectedVideoTracksSize() const {
        qint64 size = 0;

        if (!selectedVideoTracks().isEmpty()
            && videoTracksCount() > 0) {

            auto ids = selectedVideoTracks();
            for (int index : ids) {
                auto track = video_->tracksInfo().video_tracks[index];
                if (!track.is_fake_size)
                    size += track.size;
            }
        }

        return size;
    }

    qint64          selectedAudioTracksSize() const {
        qint64 size = 0;

        if (!selectedAudioTracks().isEmpty()
            && audioTracksCount() > 0) {

            auto ids = selectedAudioTracks();
            for (int index : ids) {
                auto track = video_->tracksInfo().audio_tracks[index];
                if(!track.is_fake_size)
                    size += track.size;
            }
        }

        return size;
    }

    qint64          selectedImageTracksSize() const {
        qint64 size = 0;

        if (!selectedImageTracks().isEmpty()
            && imageTracksCount() > 0) {

            auto ids = selectedImageTracks();
            for (int index : ids) {
                auto track = video_->tracksInfo().image_tracks[index];
                if (!track.is_fake_size)
                    size += track.size * track.urls.count();
            }
        }

        return size;
    }

    qint64          selectedSubtitleTracksSize() const {
        qint64 size = 0;

        if (!selectedSubtitleTracks().isEmpty()
            && subtitleTracksCount() > 0) {

            auto ids = selectedSubtitleTracks();
            for (int index : ids) {
                auto track = video_->tracksInfo().subtitle_tracks[index];
                    size += track.size;
            }
        }

        return size;
    }

    QString         presetTrackDesk(const EPreset preset) { //todo move to video_handle
        QString desk;

        switch (preset) {
            case kPreset_OriginalVideo:
                desk = tr("Original video");
                break;
            case kPreset_MP3Original:
                desk = "MP3 " + tr("original quality");
                break;
            case kPreset_MP3LameInsane:
                desk = "MP3 " + tr("high quality") + " (320 Kbps)";
                break;
            case kPreset_MP3LameExtreme:
                desk = "MP3 " + tr("high quality") + " (~230 Kbps)";
                break;
            case kPreset_MP3LameStandard:
                desk = "MP3 " + tr("normal quality") + " (~190 Kbps)";
                break;
            case kPreset_MP3LameR3mix:
                desk = "MP3 " + tr("normal quality") + " (~160 Kbps)";
                break;
            case kPreset_MP3LowQuality:
                desk = "MP3 " + tr("low quality") + " (32 Kbps)";
                break;
            case kPreset_OriginalAudio:
                desk = tr("Original audio (no conversion)");
                break;
        }

        return desk;
    }

    int             checkAudioTrack(int track);
    bool            checkAudioLanguageTrack(const int track, const QList<QAction*>& subactions);
    int             checkSaveAsTrack(int track);
    int             checkVideoTrack(int track);
    int             checkSubtitleTrack(int track);
    int             checkSubtSaveSubtitlesAsTrack(int track);
    int             checkImageTrack(int track);

    void            activate();
    bool            isActivated() const;
 
    //Remove Right Button Pressed
    void            remove() {
        video_->cancel();
    }
    void            removeAndDelete();

private:
    int             limitSaveAsTrack(int id);

    int             limitAudioTrack(int check_id) {
        int id = audioTracksCount() - 1;
        if (id < 0)
            id = 0;

        if (checkDefaultSettingsEnabled()) {
            if (!isActivated()) {
                for (; id > 0; id--) {
                    const AudioTrackInfo& track = video_->tracksInfo().audio_tracks[id];
                    if (!track.is_fake_track) {
                        if (track.eval_type == ManualEvaluation::Auto) {
                            if (track.profile <= AudioProfile::AAC
                                && track.language_tag == track.language_tag
                                && track.bitrate <= lowQualityAudioBitrate(video_->portal())) {
                                if (check_id < id)
                                    check_id = id;

                                return check_id;
                            }

                            if (track.bitrate <= lowQualityAudioBitrate(video_->portal())) {
                                if (check_id < id)
                                    check_id = id;

                                return check_id;
                            }
                        }
                        else {
                            if (limitManualEvaluation(track.eval_type)) {
                                if (check_id < id)
                                    check_id = id;

                                return check_id;
                            }
                        }
                    }
                }

                if (audioTracksCount() > 1
                    && video_->tracksInfo().audio_tracks[audioTracksCount() - 1].is_fake_track)
                    id = audioTracksCount() - 2;

                if (check_id < id)
                    check_id = id;
            }
        }

        return check_id;
    }

    bool limitManualEvaluation(const ManualEvaluation& eval_type) {
        return (eval_type <= ManualEvaluation::Low);
    }

    int             limitVideoTrack(int check_id) {
        int id = videoTracksCount() - 1;
        if (id < 0)
            id = 0;

        if (checkDefaultSettingsEnabled()) {
            if (!isActivated()) {
                for (; id > 0; id--) {
                    const VideoTrackInfo& track = video_->tracksInfo().video_tracks[id];
                    if (!track.is_fake_track) {
                        if (track.eval_type == ManualEvaluation::Auto) {
                            if (track.height <= lowQualityVideoHeight(video_->portal())) {
                                if (check_id < id)
                                    check_id = id;

                                return check_id;
                            }
                        }
                        else {
                            if (limitManualEvaluation(track.eval_type)) {
                                if (check_id < id)
                                    check_id = id;

                                return check_id;
                            }
                        }
                    }
                }

                if (videoTracksCount() > 1
                    && video_->tracksInfo().video_tracks[videoTracksCount() - 1].is_fake_track)
                    id = videoTracksCount() - 2;

                if (check_id < id)
                    check_id = id;
            }
        }

        return check_id;
    }

    int             limitSubtitleTrack(int id) {
        if (!isActivated())
            return 0;

        return id;
    }

    int             limitSaveSubtitlesAsTrack(int id) {
        if (!isActivated())
            return kSaveSubtitlesAs_Original;

        return id;
    }

    int             limitImageTrack(int check_id) {
        int id = videoTracksCount() - 1;
        if (id < 0)
            id = 0;

        if (!isActivated()) {
            for (; id > 0; id--) {
                const VideoTrackInfo& track = video_->tracksInfo().video_tracks[id];
                if (!track.is_fake_track) {
                    if (track.eval_type == ManualEvaluation::Auto) {
                        if (track.width <= lowQualityImageWidth(video_->portal())) {
                            if (check_id < id)
                                check_id = id;

                            return check_id;
                        }
                    }
                    else {
                        if (limitManualEvaluation(track.eval_type)) {
                            if (check_id < id)
                                check_id = id;

                            return check_id;
                        }
                    }
                }
            }

            if (videoTracksCount() > 1
                && video_->tracksInfo().video_tracks[videoTracksCount() - 1].is_fake_track)
                id = videoTracksCount() - 2;

            if (check_id < id)
                check_id = id;
        }

        return check_id;
    }

    QString addSeparator(const QString& first, const QString& second, bool wide_separator = false) const {
        QString desc;
        QString separator = " ";
        if (wide_separator)
            separator = "  ";
        
        if (!second.isEmpty()) {
            if (!first.isEmpty())
                desc = separator + second;
            else
                desc = second;
        }
        
        return desc;
    }

    QString sizeDesk(const qint64 size) const {
        QString desc;

        if (size > 0) {
            qint64 size_in_mb = size / (1024 * 1024);
            qint64 size_in_kb = size / 1024;

            if (size_in_mb == 0) {
                if (size_in_kb != 0)
                    desc = QString::number(size_in_kb);
            }
            else {
                desc = QString::number(size_in_mb);
            }

            if (!desc.isEmpty()) {
                if (size_in_mb == 0) {
                    if (size_in_kb != 0)
                        desc.append("Kb");
                }
                else {
                    desc.append("Mb");
                }
            }
        }
        
        return desc;
    }

    QString getVideoQualityEvaluationDesc(const int height) {
        QString desk;

        if (height > normalQualityVideoHeight(video_->portal()))
            desk = tr("High");
        else if (height > lowQualityVideoHeight(video_->portal()) && height <= normalQualityVideoHeight(video_->portal()))
            desk = tr("Normal");
        else if (height > 0 && height <= lowQualityVideoHeight(video_->portal()))
            desk = tr("Low");

        return desk;
    }

    QString getAudioQualityEvaluationDesc(const int audio_bitrate) {
        QString desk;

        if (audio_bitrate > normalQualityAudioBitrate(video_->portal()))
            desk = tr("High");
        else if (audio_bitrate >= lowQualityAudioBitrate(video_->portal()) && audio_bitrate <= normalQualityAudioBitrate(video_->portal()))
            desk = tr("Normal");
        else if (audio_bitrate > 0 && audio_bitrate < lowQualityAudioBitrate(video_->portal()))
            desk = tr("Low");

        return desk;
    }

    QString getImageQualityEvaluationDesc(const int width) {
        QString desk;

        if (width > normalQualityImageWidth(video_->portal()))
            desk = tr("High");
        else if (width > lowQualityImageWidth(video_->portal()) && width <= normalQualityImageWidth(video_->portal()))
            desk = tr("Normal");
        else if (width > 0 && width <= lowQualityImageWidth(video_->portal()))
            desk = tr("Low");

        return desk;
    }

    QString getManualEvaluationDesc(const ManualEvaluation& eval_type) {
        QString desk;

        if (eval_type == ManualEvaluation::High)
            desk = tr("High");
        else if (eval_type == ManualEvaluation::Normal)
            desk = tr("Normal");
        else if (eval_type == ManualEvaluation::Low)
            desk = tr("Low");
        else if (eval_type == ManualEvaluation::UnrecVideo)
            desk = tr("Unknown");
        else if (eval_type == ManualEvaluation::UnrecAudio)
            desk = tr("Unknown");
        else if (eval_type == ManualEvaluation::UnrecImage)
            desk = tr("Unknown");

        return desk;
    }
    
private:
    VideoHandle*                    video_ = nullptr;
    VideoItemController*            controller_ = nullptr;
    qint64                          size_item_for_sort_ = 0;
    int                             height_item_for_sort_ = 0;
    int                             date_in_milliseconds_now_;
};

