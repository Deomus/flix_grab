#include "video_item_delegate.h"

#include <product_info.h>

#include "video_item_controller.h"


VideoItemDelegate::VideoItemDelegate(VideoItemController* controller) : controller_(controller)
, video_(controller->video())
, QObject(controller) {
    Q_ASSERT(video_);
}

void VideoItemDelegate::selectSaveAsTracks(const QList<int>& tracks) {
    auto limited = tracks;

    for (auto& t : limited)
        t = limitSaveAsTrack(t);

    if (!limited.isEmpty()) {
        if (video_->portal() == EPortal::kNetflix
            || video_->portal() == EPortal::kDisney
            || video_->portal() == EPortal::kAmazon
            || video_->portal() == EPortal::kHBO
            || video_->portal() == EPortal::kHulu) {
            controller_->fillBtnSettingsMenu(kShowMenu_VideoAndAudio);
        }
        else if (limited.contains(kPreset_OriginalVideo)) {
            controller_->fillBtnSettingsMenu(kShowMenu_Video);
            controller_->selectAudioTrack();
        }
        else {
            controller_->fillBtnSettingsMenu(kShowMenu_Audio);
        }
        controller_->updateOutputPath(QFileInfo(video_->outputMediaPath()).absolutePath(), limited[0]);
    }

    video_->setSaveAsIds(limited);
}

void VideoItemDelegate::selectVideoTracks(const QList<int>& tracks) {
    auto limited = tracks;

    for (auto& t : limited) {
        t = limitVideoTrack(t);
        //if (video_->tracksInfo().video_tracks[t].is_fake_track)
        //    controller_->updateOutputPath(QFileInfo(video_->outputMediaPath()).absolutePath(), kPreset_OriginalAudio);
    }

    video_->setVideoIds(limited);
}

void VideoItemDelegate::selectAudioTracks(const QList<int>& tracks) {
    auto limited = tracks;

    for (auto& t : limited)
        t = limitAudioTrack(t);

    video_->setAudioIds(limited);
    
    if (!selectedSaveAsTracks().isEmpty()) {
        int preset = selectedSaveAsTracks().first();
        if (preset != kPreset_OriginalVideo)
            controller_->updateOutputPath(QFileInfo(video_->outputMediaPath()).absolutePath(), preset);
    }
}

void VideoItemDelegate::selectImageTracks(const QList<int>& tracks) {
    auto limited = tracks;

    for (auto& t : limited)
        t = limitImageTrack(t);

    video_->setImageIds(limited);
}

void VideoItemDelegate::selectSubtitleTracks(const QList<int>& tracks) {
    auto limited = tracks;

    for (auto& t : limited)
        t = limitSubtitleTrack(t);

    video_->setSubtitleIds(limited);
}

void VideoItemDelegate::selectSaveSubtitlesAsTracks(const QList<int>& tracks) {
    auto limited = tracks;

    for (auto& t : limited)
        t = limitSaveSubtitlesAsTrack(t);

    video_->setSaveSubtitlesAsIds(limited);
}

void VideoItemDelegate::updateItemDesk() {
//saveas section
    QString content_type;
    int preset = DEFAULTPRESET;
    if (!selectedSaveAsTracks().isEmpty()) {
        preset = selectedSaveAsTracks().first();
        content_type = saveAsInfo(preset);
    }
//video section
    QString video;
    if (VIDEOSELECTORENABLED) {
        if (!selectedVideoTracks().isEmpty()
            && videoTracksCount() > 0) {

            bool enable_video = false;
            const VideoTrackInfo& track = video_->tracksInfo().video_tracks[selectedVideoTracks().first()];

            if (SAVEASSELECTORENABLED) {
                if (!selectedSaveAsTracks().isEmpty()
                    && selectedSaveAsTracks().first() < kPreset_MP3Original)
                    enable_video = true;
            }
            else {
                if (track.is_fake_track) {
                    if (!video_->tracksInfo().audio_tracks.isEmpty()
                        && !video_->tracksInfo().audio_tracks[selectedAudioTracks().first()].is_fake_track)
                        content_type = saveAsInfo(kPreset_MP3Original);
                    else
                        content_type.clear();
                }
                else {
                    enable_video = true;
                }
            }

            if (enable_video) {
                if (content_type.isEmpty()
                    && !video_->videoInfo().url_type.isEmpty()) {
                    
                    content_type = video_->videoInfo().url_type;
                    content_type.replace(0, 1, content_type[0].toUpper());
                    content_type.append(":");
                }

                bool add_quality = (video_->portal() != EPortal::kNetflix
                    && video_->portal() != EPortal::kDisney
                    && video_->portal() != EPortal::kAmazon
                    && video_->portal() != EPortal::kHBO
                    && video_->portal() != EPortal::kHulu
                    && video_->portal() != EPortal::kYoutube);

                if (content_type.isEmpty())
                    video.append(videoQualityInfo(track, add_quality));
                else
                    video.append(videoQualityInfo(track, add_quality).toLower());

                video.append(addSeparator(video, videoStreamInfo(track), true));

                if (track.height > 0)
                    height_item_for_sort_ = track.height;
                if (height_item_for_sort_ == 0
                    && track.width > 0)
                    height_item_for_sort_ = track.width;
            }
        }
    }
//audio section
    QString audio;
    if (AUDIOSELECTORENABLED) {
        if (!selectedAudioTracks().isEmpty()
            && audioTracksCount() > 0) {

            if (!selectedSaveAsTracks().isEmpty()
                && selectedSaveAsTracks().first() != kPreset_OriginalVideo) {

                const AudioTrackInfo& track = video_->tracksInfo().audio_tracks[selectedAudioTracks().first()];
                bool add_quality = (video_->portal() != EPortal::kYoutube);
                if (content_type.isEmpty())
                    audio.append(audioQualityInfo(track, add_quality));
                else
                    audio.append(audioQualityInfo(track, add_quality).toLower());

                audio.append(addSeparator(audio, audioBitrateInfo(track), true));
                
                if (video_->portal() != EPortal::kYoutube) {
                    QString info = audioStreamInfo(track);
                    QString delim = " (";
                    if (info.contains(delim))
                        info = info.split(delim).first();
                    audio.append(addSeparator(audio, info, true));
                }
            }
        }
    }
//audio language section
    QString audio_lang;
    if (AUDIOLANGSELECTORENABLED) {
        
        auto ids_lang = selectedAudioLanguagesTracks();
        auto ids_aud = selectedAudioTracks();
        
        if (!ids_lang.isEmpty()
            && !ids_aud.isEmpty()
            && audioLanguagesCount() > 0) {

            if (ids_lang.size() > 1) {
                if (ids_aud.size() > 1)
                    audio_lang = QString::number(ids_lang.size() * ids_aud.size()) + "x " + tr("audio");
                else
                    audio_lang = QString::number(ids_lang.size()) + "x " + tr("audio");
            }
            else {
                QString desk = audioLanguageDesk(ids_lang.first(), false).split(" ").first();
                if (ids_aud.size() > 1)
                    audio_lang = QString::number(ids_aud.size()) + "x " + desk;
                else
                    audio_lang = desk;
            }
        }
    }

//image section
    QString image;
    if (IMAGESELECTORENABLED) {
        if (!selectedImageTracks().isEmpty()
            && imageTracksCount() > 0) {

            const ImageTrackInfo& track = video_->tracksInfo().image_tracks[selectedImageTracks().first()];
            if (content_type.isEmpty())
                image.append(imageQualityInfo(track, true));
            else
                image.append(imageQualityInfo(track, true).toLower());
            image.append(addSeparator(image, imageStreamInfo(track), true));

            if (height_item_for_sort_ == 0) {
                if (track.height > 0)
                    height_item_for_sort_ = track.height;

                if (height_item_for_sort_ == 0
                    && track.width > 0)
                    height_item_for_sort_ = track.width;
            }
        }
    }

//subtitles language section
    QString subtitles_lang; //none video and none audio cases
    if (video.isEmpty()
        && audio.isEmpty()
        && image.isEmpty()
        && audio_lang == tr("None")) {

        auto ids_lang = selectedSubtitleTracks();

        if (!ids_lang.isEmpty()
            && subtitleTracksCount() > 0) {

            if (ids_lang.size() > 1) {
                subtitles_lang = QString::number(ids_lang.size()) + "x " + tr("subtitles");
            }
            else {
                subtitles_lang = subtitleTrackDesc(ids_lang.first()).split(" ").first();
            }

            if (subtitles_lang.isEmpty())
                content_type = tr("None");
        }
    }

//size section
    int langs_count = selectedAudioLanguagesTracks().size();
    if (langs_count == 0)
        langs_count = 1;
    
    qint64 video_size = 0;
    qint64 audio_size = 0;

    if (preset < kPreset_MP3Original) {
        video_size = selectedVideoTracksSize();
    }
    
    if (video_->portal() == EPortal::kNetflix
        || video_->portal() == EPortal::kDisney
        || video_->portal() == EPortal::kAmazon
        || video_->portal() == EPortal::kHBO
        || video_->portal() == EPortal::kHulu
        || preset != kPreset_OriginalVideo) {
        audio_size = selectedAudioTracksSize();
    }
    size_item_for_sort_ = video_size + audio_size * langs_count + selectedImageTracksSize() + selectedSubtitleTracksSize();
    QString size = sizeDesk(size_item_for_sort_);
    
    QString desc;
    desc.append(content_type);
    desc.append(addSeparator(desc, video, true));
    desc.append(addSeparator(desc, audio, true));
    if (!subtitles_lang.isEmpty())
        desc.append(addSeparator(desc, subtitles_lang, true));
    else
        desc.append(addSeparator(desc, audio_lang, true));
    
    //instagram collection case
    if (imageTracksCount() > 0
        && (videoTracksCount() > 0)) {
        if (video_->tracksInfo().image_tracks.last().image_number >= 0)
            desc.append("|" + tr("images") + ": ").append(image);
        else
            desc.append("|" + tr("image") + ": ").append(image);
    }
    else {
        desc.append(addSeparator(desc, image, true));
    }
    desc.append(addSeparator(desc, size, true));

    qDebug() << "item desc: " << desc;

    controller_->updateInfo(desc);
}

QString VideoItemDelegate::saveAsInfo(const int preset) const {
    QString info;

    if (preset == kPreset_OriginalVideo)
        info = tr("Video");
    else if (preset == kPreset_JPG)
        info = tr("Image");
    else
        info = tr("Audio");
    
    //instagram case
    if (imageTracksCount() > 0) {
        if (videoTracksCount() > 0) {
            info = tr("Collection");
        }
        else {
            info = tr("Image");

            if (video_->tracksInfo().image_tracks.last().image_number >= 0)
                info = tr("Images");
        }
    }

    info.append(":");

    return info;
}

int VideoItemDelegate::checkAudioTrack(int track) {
    return limitAudioTrack(track);
}

bool VideoItemDelegate::checkAudioLanguageTrack(const int id, const QList<QAction*>& subactions) {
    bool result = false;
    if (checkDefaultSettingsEnabled()) {
        if (!isActivated()) {
            for (auto it : subactions) {
                if (it->isChecked()
                    && it->data().toInt() != id
                    && it->data().toInt() > 0) { // 0 - none case

                    result = true;
                    break;
                }
            }
        }
    }

    return result;
}

int VideoItemDelegate::checkSaveAsTrack(int track) {
    return limitSaveAsTrack(track);
}

int VideoItemDelegate::limitSaveAsTrack(int id) {
    if (checkDefaultSettingsEnabled()) {
        if (!isActivated()) {
            if (forcedConversionToLowQualityEnabled()) {
                if (id != kPreset_MP3LowQuality)
                    id = kPreset_MP3LowQuality;
            }
            else {
                if (id != DEFAULTPRESET)
                    id = DEFAULTPRESET;
            }
        }
    }

    return id;
}

int VideoItemDelegate::checkVideoTrack(int track) {
    return limitVideoTrack(track);
}

int VideoItemDelegate::checkSubtitleTrack(int track) {
    return limitSubtitleTrack(track);
}

int VideoItemDelegate::checkSubtSaveSubtitlesAsTrack(int track) {
    return limitSaveSubtitlesAsTrack(track);
}

int VideoItemDelegate::checkImageTrack(int track) {
    return limitImageTrack(track);
}

void VideoItemDelegate::activate() {
    if (!isActivated()) {
        ProductController::instance()->execActivation();

        if (isActivated())
            controller_->updateState();
    }

    if (loadingSlowdownEnabled())
        video_->setSlowdownFactor(isActivated());

    if (contentLengthLimitEnabled())
        video_->setLimitDuration(isActivated());
}

bool VideoItemDelegate::isActivated() const {
    return ProductController::instance()->isActivated();
}

void VideoItemDelegate::removeAndDelete() {
    /*delete_file_ = true;
    remove();*/
    controller_->removeAndDelete();
}
