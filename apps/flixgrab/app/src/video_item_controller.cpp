#include "video_item_controller.h"
#include "video_item_widget.h"

#include "utils.h"
#include "settings.h"

#include "video_item_delegate.h"

#include "product_controller.h"


const static int            MAX_INDEX_AT_FILE = 64;

inline bool isCached(const QString& key, const QVariant& value, QVariantMap& cache) {
    auto it = cache.find(key);

    if (it != cache.end()) {
        if (*it == value)
            return true;
        *it = value;
    }
    else {
        cache.insert(key, value);
    }
    return false;
}

VideoItemController::VideoItemController(VideoHandle* video, QObject* parent /*= nullptr*/) : AbstractItem(parent)
, widget_(nullptr)
, widget_state_(kWidgetState_Enqueued)
, video_(video) {
    delegate_ = new VideoItemDelegate(this);

    //QObject::connect(video_, &VideoHandle::described, this, &VideoItemController::applySettings);

    QObject::connect(video_, &VideoHandle::ready, this, [this]() {
        applySettings();
    });
    QObject::connect(video_, &VideoHandle::analyzingPlaylist, this, [this]() {
		updatePlaylistProgress(true);
    });
    QObject::connect(video_, &VideoHandle::playlistReady, this, [this]() {
        updatePlaylist();
		updatePlaylistProgress(false);
    });

    //  ui_.progressBar->setAlignment(Qt::AlignCenter);
    QObject::connect(video_, &VideoHandle::progressChanged, this, &VideoItemController::updateProgress);
    QObject::connect(video_, &VideoHandle::cancelling, this, &VideoItemController::markToDelete);
    QObject::connect(video_, &VideoHandle::cancelled, this, &VideoItemController::cleanUp);
    createStateMachine();

    //qRegisterMetaType<EVideoItemStates>("EVideoItemStates");

    //Test: Set Widget State
    //setWidgetState(kVideoItemState_SettingUp);
}

VideoItemController::~VideoItemController() {
    qDebug() << "~VideoItemController()";
    releaseWidget();
}

void VideoItemController::processWidgetState() {
    switch (widget_state_) {
        case kWidgetState_SettingUp:
        case kWidgetState_Paused:
            video_->download();
            break;
        case kWidgetState_Downloading:
        case kWidgetState_Pending:
            video_->pause();
            break;
        case kWidgetState_Completed:
            playVideo();
            break;
    }
}

void VideoItemController::updateProgress(float progress) {
    if (widget_ 
        && widget_state_ == kWidgetState_Downloading)
        widget_->UpdateProgress(progress);
    //setWidgetState(widget_state_);
    //updateState(kVideoItemState_Downloading);
}

void VideoItemController::updateInfo(const QString& info) {
    if (widget_)
        widget_->UpdateInfo(info);
}

void VideoItemController::fillBtnSettingsMenu(EShowMenu show_menu) {
    if (widget_)
        widget_->Control()->fillBtnSettingsMenu(show_menu);
}

void VideoItemController::updatePlaylistProgress(bool is_in_progress) {
	if (widget_)
		widget_->updatePlaylistProgress(is_in_progress);
}

void VideoItemController::updatePlaylist() {
	if (widget_)
		widget_->Control()->setupPlaylist(delegate_);
}

void VideoItemController::selectSaveAsTrack(const int& preset) {
    if (delegate_)
        delegate_->selectSaveAsTracks({ preset });
}

void VideoItemController::selectVideoTrack() {
    if (video_
        && delegate_) {

        EVideoSelector selector = settings_.value(settings::kVideoStream).value<EVideoSelector>();
        auto tracks = video_->tracksInfo();
        if (!tracks.video_tracks.isEmpty()) {

            int id = tracks.video_tracks.size() - 1;
            if (id < 0)
                id = 0;

            if (tracks.video_tracks[id].is_fake_track
                && tracks.video_tracks.size() > 1)
                id = tracks.video_tracks.size() - 2;

            switch (selector) {
            case kVideoSelector_Default:
                id = tracks.getDefaultVideoTrack(tracks.video_tracks, video_->portal());
                break;
            case kVideoSelector_HighQuality:
                id = 0;
                break;
            }

            delegate_->selectVideoTracks({ id });
        }
        else {
            delegate_->selectVideoTracks({});
        }
    }
}

void VideoItemController::selectAudioTrack() {
    if (video_
        && delegate_) {

        EAudioSelector selector = settings_.value(settings::kAudioStream).value<EAudioSelector>();
        delegate_->setAudioSelector(selector);
        auto tracks = video_->tracksInfo();
        if (!tracks.audio_tracks.isEmpty()) {

            int id = tracks.audio_tracks.size() - 1;
            if (id < 0)
                id = 0;

            if (tracks.audio_tracks[id].is_fake_track
                && tracks.audio_tracks.size() > 1)
                id = tracks.audio_tracks.size() - 2;

            switch (selector) {
            case kAudioSelector_Default:
                id = tracks.getDefaultAudioTrack(tracks.audio_tracks, video_->portal());
                break;
            case kAudioSelector_HighQuality:
                id = 0;
                break;
            }

            delegate_->selectAudioTracks({ id });
        }
        else {
            delegate_->selectAudioTracks({});
        }
    }
}

void VideoItemController::filterImageTracks() {
    if (video_
        && delegate_) {

        auto tracks_info = video_->tracksInfo();

        //find image_number with max count of image heights
        QMap<int, int> image_heights_count;
        int max_number = 0;
        int image_numbers = 0;
        for (auto& it : tracks_info.image_tracks) {
            if (it.image_number > image_numbers)
                image_numbers = it.image_number;
        }

        for (int i = 0; i <= image_numbers; ++i) {
            int count = 0;
            for (auto& it : tracks_info.image_tracks) {
                if (it.image_number == i) {
                    count++;
                }
            }
            image_heights_count.insert(i, count);
        }

        if (!image_heights_count.isEmpty()) {
            auto it = std::max_element(image_heights_count.begin(), image_heights_count.end());
            max_number = it.key();
        }

        //filter image_tracks by image_number with max count of image heights
        QMutableListIterator<ImageTrackInfo> it(tracks_info.image_tracks);
        while (it.hasNext()) {
            if (it.next().image_number != max_number)
                it.remove();
        }

        video_->updateTracksInfo(tracks_info);
        video_->sortImagesTracks();
    }
}

void VideoItemController::selectImageTrack() {
    if (video_
        && delegate_) {

        EImageSelector selector = settings_.value(settings::kImageStream).value<EImageSelector>();
        delegate_->setImageSelector(selector);
        auto tracks = video_->tracksInfo();
        if (!tracks.image_tracks.isEmpty()) {

            int id = tracks.image_tracks.size() - 1;
            if (id < 0)
                id = 0;

            if (tracks.image_tracks[id].is_fake_track
                && tracks.image_tracks.size() > 1)
                id = tracks.image_tracks.size() - 2;

            switch (selector) {
            case kImageSelector_Default:
                id = tracks.getDefaultImageTrack(tracks.image_tracks, video_->portal());
                break;
            case kImageSelector_HighQuality:
                id = 0;
                break;
            }

            delegate_->selectImageTracks({ id });
        }
        else {
            delegate_->selectImageTracks({});
        }
    }
}

void VideoItemController::selectSubtitleTracks(const QList<QString>& lang_tags) {
    if (video_
        && delegate_) {

        QList<int> tracks_ids;
        auto tracks = video_->tracksInfo();
        if (!tracks.subtitle_tracks.isEmpty()) {
            for (auto it : lang_tags) {
                QList<int> ids;
                for (int i = 0; i < tracks.subtitle_tracks.size(); ++i) {
                    if (tracks.subtitle_tracks[i].language_tag == it) {
                        if (!ids.contains(i)
                            && !tracks_ids.contains(i))
                            ids << i;
                    }
                }

                if (ids.isEmpty()) {
                    for (int i = 0; i < tracks.subtitle_tracks.size(); ++i) {
                        if (tracks.subtitle_tracks[i].language_tag.split("-").first() == it) {
                            if (!ids.contains(i)
                                && !tracks_ids.contains(i))
                                ids << i;
                        }
                    }
                    for (int i = 0; i < tracks.subtitle_tracks.size(); ++i) {
                        if (tracks.subtitle_tracks[i].language_tag.split("-").first() == it.split("-").first()) {
                            if (!ids.contains(i)
                                && !tracks_ids.contains(i))
                                ids << i;
                        }
                    }
                }

                if (!ids.isEmpty())
                    tracks_ids << ids;
            }
            if (tracks_ids.isEmpty())
                tracks_ids << 0; //no subtitles
        }

        delegate_->selectSubtitleTracks(tracks_ids);
    }
}

void VideoItemController::selectSaveSubtitlesAsTracks(const QList<int>& tracks) {
    if (video_
        && delegate_)
        delegate_->selectSaveSubtitlesAsTracks(tracks);
}

void VideoItemController::filterAssistiveTracks() {
    if (video_
        && delegate_) {

        bool is_assistive = settings_.value(settings::kAudioType).toBool();
        auto tracks_info = video_->tracksInfo();

        if (!tracks_info.all_audio_tracks.isEmpty()) {

            tracks_info.audio_tracks.clear();

            for (int i = 0; i < tracks_info.all_audio_tracks.size(); ++i) {
                if (tracks_info.all_audio_tracks[i].is_assistive == is_assistive)
                    tracks_info.audio_tracks.push_back(tracks_info.all_audio_tracks[i]);
            }


            tracks_info.audio_languages.clear();

            for (int i = 0; i < tracks_info.all_audio_tracks.size(); ++i) {
                if (tracks_info.all_audio_tracks[i].is_assistive == is_assistive
                    && !tracks_info.all_audio_tracks[i].language_tag.isEmpty()
                    && !tracks_info.audio_languages.contains(tracks_info.all_audio_tracks[i].language_tag))

                    tracks_info.audio_languages << tracks_info.all_audio_tracks[i].language_tag;
            }
        }

        video_->updateTracksInfo(tracks_info);
        video_->sortAudioLanguages();
    }
}

void VideoItemController::filterAudioTracks(const QList<QString>& languages) {
    if (video_
        && delegate_) {

        auto tracks_info = video_->tracksInfo();

        //find lang with max count of audio qualities
        QMap<QString, int> audio_tracks_count;
        QString lang;

        for (auto& it : languages) {
            lang.clear();

            if (!tracks_info.audio_languages.isEmpty()) {
                for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                    if (tracks_info.audio_languages[i] == it) {
                        lang = tracks_info.audio_languages[i];
                        break;
                    }
                }
                if (lang.isEmpty()) {
                    for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                        if (tracks_info.audio_languages[i].split("-").first() == it) {
                            lang = tracks_info.audio_languages[i];
                            break;
                        }
                    }
                }
                if (lang.isEmpty()) {
                    for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                        if (tracks_info.audio_languages[i].split("-").first() == it.split("-").first()) {
                            lang = tracks_info.audio_languages[i];
                            break;
                        }
                    }
                }
            }

            if (!lang.isEmpty()) {
                int count = 0;
                for (int i = 0; i < tracks_info.audio_tracks.size(); ++i) {
                    if (tracks_info.audio_tracks[i].language_tag == lang) {
                        count++;
                    }
                }
                audio_tracks_count.insert(lang, count);
            }
        }

        if (!audio_tracks_count.isEmpty()) {
            auto it = std::max_element(audio_tracks_count.begin(), audio_tracks_count.end());
            lang = it.key();
        }
        //try find english or get first language
        else {
            for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                lang = tracks_info.audio_languages[0];
                if (tracks_info.audio_languages.size() > 1)
                    lang = tracks_info.audio_languages[1];

                if (tracks_info.audio_languages[i] == "en") {
                    lang = tracks_info.audio_languages[i];
                    break;
                }
                else if (tracks_info.audio_languages[i].split("-").first() == "en") {
                    lang = tracks_info.audio_languages[i];
                    break;
                }
            }
        }

        //filter audio_tracks by lang with max count of audio qualities
        QMutableListIterator<AudioTrackInfo> it(tracks_info.audio_tracks);
        while (it.hasNext()) {
            if (it.next().language_tag != lang)
                it.remove();
        }

        video_->updateTracksInfo(tracks_info);
        video_->sortAudioTracks();
    }
}

void VideoItemController::selectAudioLanguageTracks(const QList<QString>& lang_tags) {
    if (video_
        && delegate_) {

        auto tracks_info = video_->tracksInfo();

        if (tracks_info.audio_languages.isEmpty())
            return;

        QList<int> ids;
        QString lang;

        for (auto& it : lang_tags) {
            lang.clear();

            for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                if (tracks_info.audio_languages[i] == it) {
                    lang = tracks_info.audio_languages[i];
                    break;
                }
            }
            if (lang.isEmpty()) {
                for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                    if (tracks_info.audio_languages[i].split("-").first() == it) {
                        lang = tracks_info.audio_languages[i];
                        break;
                    }
                }
            }
            if (lang.isEmpty()) {
                for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                    if (tracks_info.audio_languages[i].split("-").first() == it.split("-").first()) {
                        lang = tracks_info.audio_languages[i];
                        break;
                    }
                }
            }

            if (!lang.isEmpty()) {
                for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                    if (tracks_info.audio_languages[i] == lang) {
                        if (!ids.contains(i)) {
                            ids.append(i);
                            break;
                        }
                    }
                }
            }
        }

        if (ids.isEmpty()) {
            lang = tracks_info.audio_languages[0];
            if (tracks_info.audio_languages.size() > 1)
                lang = tracks_info.audio_languages[1];

            for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                if (tracks_info.audio_languages[i] == "en") {
                    lang = tracks_info.audio_languages[i];
                    break;
                }
                else if (tracks_info.audio_languages[i].split("-").first() == "en") {
                    lang = tracks_info.audio_languages[i];
                    break;
                }
            }

            if (!lang.isEmpty()) {
                for (int i = 0; i < tracks_info.audio_languages.size(); ++i) {
                    if (tracks_info.audio_languages[i] == lang) {
                        if (!ids.contains(i)) {
                            ids.append(i);
                            break;
                        }
                    }
                }
            }
        }

        delegate_->selectAudioLanguageTracks({ ids });

    }
}

QString VideoItemController::getContentExtension(int preset) {
    QString extension = "mp4";
    int id = -1;
    if (delegate_
        && !delegate_->selectedAudioTracks().isEmpty())
        id = delegate_->selectedAudioTracks()[0];

    switch (preset) {
        case kPreset_OriginalVideo:
            extension = "mp4";
            break;
        case kPreset_OriginalAudio:
            extension = "m4a";
            if (id > -1
                && !video_->tracksInfo().audio_tracks.isEmpty()) {
                if (video_->tracksInfo().audio_tracks[id].profile == AudioProfile::OGG)
                    extension = "ogg";
                if (video_->tracksInfo().audio_tracks[id].profile == AudioProfile::MP3)
                    extension = "mp3";
            }
            break;
        case kPreset_MP3Original:
        case kPreset_MP3LowQuality:
        case kPreset_MP3LameR3mix:
        case kPreset_MP3LameStandard:
        case kPreset_MP3LameExtreme:
        case kPreset_MP3LameInsane:
            extension = "mp3";
            break;
        case kPreset_JPG:
            extension = "jpg";
            break;
    }

    return "." + extension;
}

QString VideoItemController::getOutputPath(int preset) {
    QString file_path;
    
    int id = -1;
    
    if (delegate_
        && !delegate_->selectedAudioTracks().isEmpty())
        id = delegate_->selectedAudioTracks()[0];

    QString extension = getContentExtension(preset);

    switch (preset) {
        case kPreset_OriginalVideo:
            file_path = QFileDialog::getSaveFileName(nullptr, tr("Save video"), video_->outputMediaPath(), tr("Video files") + " (*" + extension + ")");
            break;
        case kPreset_OriginalAudio:
        case kPreset_MP3Original:
        case kPreset_MP3LowQuality:
        case kPreset_MP3LameR3mix:
        case kPreset_MP3LameStandard:
        case kPreset_MP3LameExtreme:
        case kPreset_MP3LameInsane:
            file_path = QFileDialog::getSaveFileName(nullptr, tr("Save audio"), video_->outputMediaPath(), tr("Audio files") + " (*" + extension + ")");
            break;
        case kPreset_JPG:
            file_path = QFileDialog::getSaveFileName(nullptr, tr("Save image"), video_->outputMediaPath(), tr("Image files") + " (*" + extension + ")");
            break;

        default:
            file_path = QFileDialog::getSaveFileName(nullptr, tr("Save video"), video_->outputMediaPath(), tr("Video files") + " (*" + extension + ")");
            break;
    }

    file_path = file_path.left(file_path.lastIndexOf("."));

    return file_path;
}

QWidget* VideoItemController::getWidget(QWidget* parent) {
    if (widget_ == nullptr) {
        
        //Try to reuse hidden widgets;
        if ((widget_ = getHiddenChild<VideoItemWidget>(parent)) == nullptr) {
            widget_ = new VideoItemWidget(parent);
        }
      
        //connect(widget_->control(), &VideoControl::audioChanged, this, &VideoItemController::changeAudioTrackWithOffer);
        //QObject::connect(widget_->Control(), &VideoControl::audioChanged, this, [this]() {
        //    //changeAudioTrack(track, true);
        //});

        ////connect(widget_->control(), &VideoControl::videoChanged, this, &VideoItemController::changeVideoTrackWithOffer);
        //QObject::connect(widget_->Control(), &VideoControl::videoChanged, this, [this](int track) {
        //    changeVideoTrack(track, true);
        //});
        ////connect(widget_->control(), &VideoControl::subtitleChanged, this, &VideoItemController::changeSubtitleTrackWithOffer);
        //QObject::connect(widget_->Control(), &VideoControl::subtitleChanged, this, [this](int track) {
        //    changeSubtitleTrack(track, true);
        //});

        connect(widget_->Control(), &VideoControl::itemToRemove, this, &VideoItemController::remove);
        connect(widget_->Control(), &VideoControl::stateToProcess, this, &VideoItemController::processWidgetState);
        connect(widget_->Control(), &VideoControl::signalshowOptions, this, &VideoItemController::showOptions);
        //connect(widget_->control(), &VideoControl::priorityChanged, this, &VideoItemController::changePriority);

        //connect(widget_->Control(), &VideoControl::outputFolderToShow, this, &VideoItemController::showOutputFolder);
        connect(widget_->Control(), SIGNAL(openPlaylist(const QString&)), this, SIGNAL(openPlaylist(const QString&)), Qt::UniqueConnection);

        connect(widget_, &VideoItemWidget::offerClicked, this, &VideoItemController::tryActivate);

        //Widget Destroyed before VideoItem Deleted;
        connect(widget_, &QWidget::destroyed, this, &VideoItemController::clearDestroyedWidget);
           
        widget_->UpdateWidget(widget_state_, delegate_);
        /*if (video_)
            widget_->updateState(widget_state_, video_);*/
        widget_->ShowSelection(isSelected());
    }

    widget_->show();

    return widget_;
}

void VideoItemController::releaseWidget(void) {
    qDebug() << "Release Widget: " << widget_;
    if (widget_ != nullptr) {
        widget_->Control()->removeItem();
        disconnect(widget_, QMetaMethod(), this, QMetaMethod());
        disconnect(widget_->Control(), QMetaMethod(), this, QMetaMethod());
        widget_->Release();
        widget_->hide();
        widget_ = nullptr;
    }
}

void VideoItemController::showSelection(bool is_selected /*= true*/) {
    if (widget_)
        widget_->ShowSelection(is_selected);
}

void VideoItemController::translateItem() {
    if (video_
        && delegate_) {
        if (!isCached(settings::kLanguage, settings_.value(settings::kLanguage).toString(), settings_cached_)) {
            if (widget_) {
                widget_->translate();
                widget_->Control()->translate();
            }
            delegate_->updateItemDesk();
        }
    }
}

void VideoItemController::applySettings() {
    if (video_ 
        && delegate_) {
        int save_as = settings_.value(settings::kSaveAs).toInt();
        if (!isCached(settings::kSaveAs, settings_.value(settings::kSaveAs), settings_cached_)) {
            applySettings();

            selectSaveAsTrack(save_as);
            selectAudioTrack();
            if (save_as == kPreset_OriginalVideo)
                selectVideoTrack();

            delegate_->updateItemDesk();
        }

        QString output_path = settings_.value(settings::kOutputFolder).toString();
        if (!isCached(settings::kOutputFolder, output_path, settings_cached_)) {
            if (!QDir(output_path).exists())
                QDir().mkpath(output_path);
            updateOutputPath(output_path, save_as);
        }

        if (VIDEOSELECTORENABLED) {
            if (!isCached(settings::kVideoStream, settings_.value(settings::kVideoStream).value<EVideoSelector>(), settings_cached_)) {
                selectVideoTrack();
                delegate_->updateItemDesk();
            }
        }

        //first operation
        if (AUDIOLANGSELECTORENABLED) {
            QList<QString> a_languages = settings_.value(settings::kAudioLanguage).value<QList<QString>>();

            if (!isCached(settings::kAudioType, settings_.value(settings::kAudioType), settings_cached_)
                || !isCached(settings::kAudioLanguage, settings_.value(settings::kAudioLanguage), settings_cached_)) {
                
                if (!video_->tracksInfo().audio_languages.isEmpty()) {
                    filterAssistiveTracks();
                    filterAudioTracks(a_languages);
                    selectAudioLanguageTracks(a_languages);
                    selectAudioTrack();

                    if (widget_) {
                        widget_->Control()->fillAudioMenu();
                        widget_->Control()->fillAudioLanguageMenu();
                    }

                    delegate_->updateItemDesk();
                }
            }
        }

        //second operation
        if (AUDIOSELECTORENABLED) {
            if (!isCached(settings::kAudioStream, settings_.value(settings::kAudioStream).value<EAudioSelector>(), settings_cached_)) {
                selectAudioTrack();
                delegate_->updateItemDesk();
            }
        }

        QList<QString> subtitle_languages = settings_.value(settings::kSubtitleLanguage).value<QList<QString>>();
        if (!isCached(settings::kSubtitleLanguage, settings_.value(settings::kSubtitleLanguage), settings_cached_)) {
            selectSubtitleTracks(subtitle_languages);
            delegate_->updateItemDesk();
        }

        if (SUBTITLESELECTORENABLED) {
            QList<int> save_subtitles_as = settings_.value(settings::kSaveSubtitlesAs).value<QList<int>>();
            if (!isCached(settings::kSaveSubtitlesAs, settings_.value(settings::kSaveSubtitlesAs), settings_cached_))
                selectSaveSubtitlesAsTracks(save_subtitles_as);
        }
        
        if (IMAGESELECTORENABLED) {
            EImageSelector image_selector = settings_.value(settings::kImageStream).value<EImageSelector>();
            if (!isCached(settings::kImageStream, image_selector, settings_cached_)) {
                if (!video_->tracksInfo().image_tracks.isEmpty()) {
                    filterImageTracks();
                    selectImageTrack();

                    if (widget_)
                        widget_->Control()->fillImageMenu();

                    delegate_->updateItemDesk();
                }
            }
        }
    }
}

void VideoItemController::updateOutputPath(const QString& output_path, int preset) {
    if (video_
        && delegate_) {

        if (!output_path.isEmpty()) {
            QFileInfo fi(output_path);

            QString output_path_in = output_path;
            QString output_file_name = makeFilename(video_->videoInfo().video_title);

            if (fi.isDir()) {
                if (!QDir(output_path).exists())
                    QDir().mkpath(output_path);
            }
            else {
                output_path_in = fi.path();
                output_file_name = fi.fileName();
                widget_->UpdateTitle(output_file_name);
            }
            video_->correctOutputName(output_path_in, output_file_name);

            QString extension = getContentExtension(preset);
            
            if (!video_->tracksInfo().image_tracks.isEmpty())   //instagram case
                if (video_->tracksInfo().video_tracks.isEmpty()
                    || (delegate_ && !delegate_->isActivated()
                        && !video_->tracksInfo().video_tracks.isEmpty()))
                    extension = getContentExtension(EPreset::kPreset_JPG);

            video_->setOutputPath(output_path_in, output_file_name, extension);
        }
    }
}

void VideoItemController::showOutputFolder() {
    if (video_
        && delegate_) {

        if (state_ == kItemState_Completed) {
            showInGraphicalShell(video_->outputMediaPath());
        }
        else {
            int preset = DEFAULTPRESET;
            if (!delegate_->selectedSaveAsTracks().isEmpty())
                preset = delegate_->selectedSaveAsTracks()[0];

            updateOutputPath(getOutputPath(preset), preset);
        }
    }
}

void VideoItemController::cleanUp() {
    qDebug() << "cleanUp";
    if (video_
        && delegate_) {

        Q_ASSERT(video_);
        if (delete_file_)
            video_->deleteOutputs();

        AbstractItem::remove();
        video_->deleteLater();
        video_ = nullptr;
    }
}

void VideoItemController::updateState() {
    if (widget_)
        widget_->UpdateWidget(widget_state_, delegate_);
}

QState* VideoItemController::createVideoState(EWidgetState widget_state, QState* parent /*= nullptr*/) {
    QState* machine_state = new QState(parent);
    machine_state->assignProperty(this, "widgetState", widget_state);
    return machine_state;
}

QState* VideoItemController::createVideoState(EWidgetState widget_state, EItemState item_state, QState* parent /*= nullptr*/) {
    QState* machine_state = createVideoState(widget_state, parent);
    machine_state->assignProperty(this, "itemState", item_state);
    return machine_state;
}

void VideoItemController::createStateMachine() {
    QState* system_state = new QState();
    QState* operation_state = new QState(system_state);
    QState* error_state = createVideoState(kWidgetState_Failed, kItemState_Failed, system_state);
    QState* trans_remove_state = createVideoState(kWidgetState_Transitioning);
    QState* removed_state = createVideoState(kWidgetState_Cancelled, kItemState_Cancelled);
    //QFinalState* removed_state = new QFinalState();
    

    //Operational states;
    QState* enqueued_state = createVideoState(kWidgetState_Enqueued, kItemState_NotReady, operation_state);
    QState* analyzing_state = createVideoState(kWidgetState_Analyzing, kItemState_NotReady, operation_state);
    QState* described_state = createVideoState(kWidgetState_Described, kItemState_NotReady, operation_state);
    QState* settingup_state = createVideoState(kWidgetState_SettingUp, kItemState_SettingUp, operation_state);
    QState* pending_download_state = createVideoState(kWidgetState_Pending, kItemState_Pending, operation_state);
    QState* pending_resume_state = createVideoState(kWidgetState_Pending, kItemState_Pending, operation_state);
    QState* trans_download_state = createVideoState(kWidgetState_Transitioning, kItemState_Downloading, operation_state);
    QState* downloading_state = createVideoState(kWidgetState_Downloading, kItemState_Downloading, operation_state);
    QState* trans_pause_state = createVideoState(kWidgetState_Transitioning, kItemState_Paused, operation_state);
    QState* paused_state = createVideoState(kWidgetState_Paused, kItemState_Paused, operation_state);
    QState* completed_state = createVideoState(kWidgetState_Completed, kItemState_Completed, operation_state);

    //Table of switching states;
    enqueued_state->addTransition(video_, &VideoHandle::analyzing, analyzing_state);
    analyzing_state->addTransition(video_, &VideoHandle::described, described_state);
    described_state->addTransition(video_, &VideoHandle::ready, settingup_state);
    settingup_state->addTransition(video_, &VideoHandle::pending, pending_download_state);
    settingup_state->addTransition(video_, &VideoHandle::starting, trans_download_state);
    pending_download_state->addTransition(video_, &VideoHandle::ready, settingup_state);
    pending_download_state->addTransition(video_, &VideoHandle::paused, settingup_state);
    pending_download_state->addTransition(video_, &VideoHandle::starting, trans_download_state);
    trans_download_state->addTransition(video_, &VideoHandle::progressChanged, downloading_state);
    trans_download_state->addTransition(video_, &VideoHandle::paused, paused_state);
    downloading_state->addTransition(video_, &VideoHandle::paused, paused_state);
    downloading_state->addTransition(video_, &VideoHandle::pausing, trans_pause_state);

    trans_pause_state->addTransition(video_, &VideoHandle::paused, paused_state);
    trans_pause_state->addTransition(video_, &VideoHandle::pending, pending_resume_state);
    paused_state->addTransition(video_, &VideoHandle::pending, pending_resume_state);
    paused_state->addTransition(video_, &VideoHandle::starting, downloading_state);
    pending_resume_state->addTransition(video_, &VideoHandle::paused, paused_state);
    pending_resume_state->addTransition(video_, &VideoHandle::resumed, downloading_state);

    downloading_state->addTransition(video_, &VideoHandle::finished, completed_state);
    trans_download_state->addTransition(video_, &VideoHandle::finished, completed_state);
    //External ring:
    operation_state->addTransition(video_, &VideoHandle::error, error_state);
    system_state->addTransition(video_, &VideoHandle::cancelling, trans_remove_state);
    trans_remove_state->addTransition(video_, &VideoHandle::cancelled, removed_state);
    trans_remove_state->addTransition(video_, &VideoHandle::error, removed_state);
    system_state->addTransition(video_, &VideoHandle::cancelled, removed_state);

    operation_state->setInitialState(enqueued_state);
    system_state->setInitialState(operation_state);

    machine_.addState(system_state);
    machine_.addState(trans_remove_state);
    machine_.addState(removed_state);
    machine_.setInitialState(system_state);
            
    machine_.start();
}

void VideoItemController::setWidgetState(EWidgetState state) {
    if (widget_) {
        widget_->ShowSelection(isSelected());
        if (video_)
            widget_->UpdateWidget(state, delegate_);
    }

    if (widget_state_ != state) {
        widget_state_ = state;
        emit stateChanged();
    }
}

void VideoItemController::setItemState(EItemState state) {
    if (state_ != state) {
       emit stateAboutToChange(state, std::exchange(state_, state));
    }
}

void VideoItemController::playVideo() {
    //QDesktopServices::openUrl(QUrl::fromLocalFile(QDir(output_path_).filePath(output_file_)));
    QDesktopServices::openUrl(QUrl::fromLocalFile(video_->outputMediaPath()));
}

void VideoItemController::tryActivate() {
    if (delegate_)
        delegate_->activate();
}
