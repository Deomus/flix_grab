#include "video_handle.h"

#include <purchasing_config.h>
#include <product_config.h>
#include <net/http_downloader.h>

#include "decrypt_controller.h"
#include "getflix.h"


#define MAX_INDEX_AT_FILE                   64


inline bool checkFlag(int value, int flag) {
    return ((value & flag) == flag);
}
 
VideoHandle::VideoHandle(const EPortal& portal, const QUrl& url, Getflix *parent)
    : Abortable(parent)
    , portal_(portal)
    , media_url_(url)
    , video_flag_(kVideoFlag_NotReady)
    , downloader_(nullptr)
    , getflix_(parent) {
    save_as_ids_.clear();
    video_ids_.clear();
    audio_ids_.clear();
    audio_language_ids_.clear();
    subtitle_ids_.clear();
    saveas_subtitle_ids_.clear();
    image_ids_.clear();
    audio_selector_ = EAudioSelector::kAudioSelector_Default;
    image_selector_ = EImageSelector::kImageSelector_Default;
}

VideoHandle::~VideoHandle() {    
    qDebug() << "~VideoHandle Destructor";
}

void VideoHandle::correctOutputName(const QString& output_dir, QString& output_name) {
    qApp->processEvents();
    QFileInfo fi_vid(QDir(output_dir).filePath(output_name));
    qApp->processEvents();
    QList<QFileInfo> file_infos;
    file_infos.append(fi_vid);
    QString output_name_in = output_name;

    for (int i = 1; i < MAX_INDEX_AT_FILE; ++i) {
        for (auto fi : file_infos) {
            output_lock_.reset(new QLockFile(QDir(output_dir).filePath(output_name) + ".lock"));
            qApp->processEvents();
            output_lock_->setStaleLockTime(0);
            bool unlock = !output_lock_->tryLock();
            qApp->processEvents();
            bool exists = fi.exists();
            qApp->processEvents();
            bool isWritable = fi.isWritable();
            qApp->processEvents();
            
            if (unlock
                || (exists && !isWritable))
                output_name = QString("%1 (%2)").arg(output_name_in).arg(i); //Change File_name
            else
                return;

            qApp->processEvents();
        }

        qApp->processEvents();
    }
}

void VideoHandle::setOutputPath(const QString& output_media_path) {
    output_media_path_ = output_media_path;
}

void VideoHandle::setOutputPath(const QString& output_dir, const QString& output_name, QString ext) {
    output_name_ = output_name;
    output_path_ = output_dir;
    if (ext.isEmpty())
        ext = output_media_path_.mid(output_media_path_.lastIndexOf("."));

    output_media_path_ = QDir(output_dir).filePath(output_name_ + ext);
}

void VideoHandle::deleteOutputs() {
    if (isDownloading()) {
        cancel();
        qApp->processEvents();
    }
         
    QFileInfo media_file(output_media_path_);
    if (!media_file.dir().remove(media_file.fileName()))
        qDebug() << "Cant remove file: " << media_file.filePath();

    for (const auto& file : downloader_->audio_file_names_) {
        QFileInfo audio_file(file);
        if (!audio_file.dir().remove(audio_file.fileName()))
            qDebug() << "Cant remove audio file: " << audio_file.filePath();

        qApp->processEvents();
    }

    for (const auto& file : downloader_->subtitle_file_names_) {
        QFileInfo sub_file(file);
        if (!sub_file.dir().remove(sub_file.fileName()))
            qDebug() << "Cant remove subtitle file: " << sub_file.filePath();

        qApp->processEvents();
    }

    for (const auto& file : downloader_->collection_file_names_) {
        QFileInfo img_file(file);
        if (!img_file.dir().remove(img_file.fileName()))
            qDebug() << "Cant remove image file: " << img_file.filePath();

        qApp->processEvents();
    }

    output_lock_.reset();
}

void VideoHandle::setSaveAsIds(const QList<int>& ids) {
    save_as_ids_ = ids;
    emit saveAsIdChanged();
}

void VideoHandle::setVideoIds(const QList<int>& ids) {
    video_ids_ = ids;
    emit videoIdChanged();
}

void VideoHandle::setAudioSelector(const EAudioSelector& selector) {
    audio_selector_ = selector;
}

void VideoHandle::setImageSelector(const EImageSelector& selector) {
    image_selector_ = selector;
}

void VideoHandle::setAudioIds(const QList<int>& ids) {
    audio_ids_ = ids;
    emit audioIdChanged();
}

void VideoHandle::setAudioLanguageIds(const QList<int>& ids) {
    if (ids.isEmpty())
        audio_language_ids_ = { 0 };
    else
        audio_language_ids_ = ids;

    emit audioLanguageIdChanged();
}

void VideoHandle::setSubtitleIds(const QList<int>& ids) {
    subtitle_ids_ = ids;
    emit subtitleIdChanged();
}

void VideoHandle::setSaveSubtitlesAsIds(const QList<int>& ids) {
    saveas_subtitle_ids_ = ids;
    emit saveSubtitlesAsIdChanged();
}

void VideoHandle::setImageIds(const QList<int>& ids) {
    image_ids_ = ids;
    emit imageIdChanged();
}

void VideoHandle::setPriority(int priority /*= kDownloadPriority_Normal*/) {
    if (priority != download_priority_) {
        download_priority_ = priority;
        getflix_->restart(this);
    }
}

bool VideoHandle::isFinished() const {
    if (downloader_)
        return downloader_->isFinished();

    return false;
}

void VideoHandle::setSlowdownFactor(const bool activated) {
    if (activated)
        slowdown_ = 0;
    else
        slowdown_ = portalSlowdownFactor(portal_);

    if (downloader_)
        downloader_->setSlowdown(slowdown_);
}

void VideoHandle::setLimitDuration(const bool activated) {
    if (activated)
        limit_duration_ = 0;
    else
        limit_duration_ = durationLimit(portal_);

    if (downloader_)
        downloader_->setLimitDuration(limit_duration_);
}

void VideoHandle::setEncodingSelector(int encoding_selector) {
    QString encoding_preset = "superfast";

    switch (encoding_selector) {
        case kEncodingSelector_2_pass:
            encoding_preset = "2pass";
            break;
        case kEncodingSelector_Slow:
            encoding_preset = "slow";
            break;
        case kEncodingSelector_Medium:
            encoding_preset = "medium";
            break;
        case kEncodingSelector_Faster:
            encoding_preset = "faster";
            break;
        case kEncodingSelector_Veryfast:
            encoding_preset = "veryfast";
            break;
        case kEncodingSelector_Superfast:
            encoding_preset = "superfast";
            break;
        case kEncodingSelector_Ultrafast:
            encoding_preset = "ultrafast";
            break;
    }

    if (downloader_)
        downloader_->setEncodingPreset(encoding_preset);
}

void VideoHandle::setMaximumCPUUsage(qreal factor) {
    maximum_cpu_usage_ = factor;

    if (downloader_)
        downloader_->setMaximumCPUUsage(maximum_cpu_usage_);
}

void VideoHandle::saveOutputOnExit(bool save_output) {
    if (downloader_)
        downloader_->saveOutputOnExit(save_output);
}

void VideoHandle::setDownloadPermission(bool download_allowed = true) {
    download_allowed_ = download_allowed;
}

void VideoHandle::download() {
    getflix_->download(this);
}

bool VideoHandle::isDownloading() const {
    return (downloader_
        && downloader_->isDownloading());
}

bool VideoHandle::isPaused() const {
    return (!downloader_
        || downloader_->isPaused());
}

void VideoHandle::pause() {
    getflix_->pause(this);
}

void VideoHandle::cancel() {
    getflix_->cancel(this);
}

void VideoHandle::handleTracksInfo(const TracksInfo& info) {
    if (downloader_
        && !downloader_->isCancelling()) {
        tracks_info_ = info;

        tracks_info_.fillAudioLanguages();
        if (!tracks_info_.audio_languages.isEmpty()) { //add fake tracks to front
            if (!tracks_info_.video_tracks.isEmpty()) {
                VideoTrackInfo video_track;
                video_track.is_fake_track = true;
                video_track.contains_audio = false;
                tracks_info_.video_tracks.push_back(video_track);
            }
            if (!tracks_info_.audio_tracks.isEmpty()) {
                AudioTrackInfo audio_track;
                audio_track.is_fake_track = true;
                audio_track.language_tag = "none";
                tracks_info_.audio_tracks.push_back(audio_track);
            }
            //if (!tracks_info_.image_tracks.isEmpty()) {
            //    ImageTrackInfo image_track;
            //    image_track.is_fake_track = true;
            //    tracks_info_.image_tracks.push_back(image_track);
            //}
        }

        if (!tracks_info_.subtitle_tracks.isEmpty()) {
            SubtitleTrackInfo subtitle_track;   //add fake track to front
            subtitle_track.is_fake_track = true;
            subtitle_track.language_tag = "none";
            tracks_info_.subtitle_tracks.push_back(subtitle_track);
        }

        tracks_info_.sortTracks();
        tracks_info_.all_audio_tracks = tracks_info_.audio_tracks;
        tracks_info_.all_image_tracks = tracks_info_.image_tracks;

        tracks_info_.fillAudioLanguages();
        tracks_info_.sortAudioLanguages();
        tracks_info_.all_audio_languages = tracks_info_.audio_languages;

        if (!tracks_info_.video_tracks.isEmpty())
            video_ids_ = { tracks_info_.getDefaultVideoTrack(tracks_info_.video_tracks, portal_) };

        if (!tracks_info_.audio_tracks.isEmpty())
            audio_ids_ = { tracks_info_.getDefaultAudioTrack(tracks_info_.audio_tracks, portal_) };

        if (!tracks_info_.audio_languages.isEmpty())
            audio_language_ids_ = { tracks_info_.getDefaultAudioLanguageTrack(tracks_info_.audio_languages) };

        if (!tracks_info_.image_tracks.isEmpty())
            image_ids_ = { tracks_info_.getDefaultImageTrack(tracks_info_.image_tracks, portal_) };

        if (!tracks_info_.subtitle_tracks.isEmpty()) {
            subtitle_ids_ = tracks_info_.getDefaultSubtitleTracks(tracks_info_.subtitle_tracks);
            saveas_subtitle_ids_ << kSaveSubtitlesAs_Original;
        }

        save_as_ids_ = { DEFAULTPRESET };

        setFlag(kVideoFlag_Tracks);

        downloader_->decryptRequired(tracks_info_.req_decrypt);
        qDebug() << "decryptRequired: " << tracks_info_.req_decrypt;

        if (!tracks_info_.req_decrypt) {
            qDebug() << "DecryptController ready req_decrypt = false";
            emit downloader_->ready();
        }
    }
}

void VideoHandle::handleVideoInfo(const VideoInfo& info) {
    video_info_ = info;
    if (video_info_.artwork_url.isEmpty())
        video_info_.artwork_url = portalArtwork(portal_);

#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
    video_info_.metadata_artwork_url.clear();
#endif

    setFlag(kVideoFlag_Info);
	if (info.playlist_kind != PlaylistKind::kPlaylistPending)
		setFlag(kVideoFlag_Playlist);
}

bool VideoHandle::downloadEnabled(QList<int>& saveas_ids, QList<const VideoTrackInfo*>& video_tracks,
    QList<const AudioTrackInfo*>& audio_tracks, QList<const ImageTrackInfo*>& image_tracks,
    QList<const SubtitleTrackInfo*>& subtitle_tracks) {
    if (!download_allowed_)
        return false;
    
    bool result = true;

    if (!saveas_ids.isEmpty()) {
        //if (saveas_ids[0] == kPreset_OriginalVideo) {
        //    if (video_tracks.isEmpty())
        //        result = false;
        //}
        //else if (saveas_ids[0] > kPreset_OriginalVideo) {
        //    if (audio_tracks.isEmpty())
        //        result = false;
        //}

        //if (!result
        //    && !image_tracks.isEmpty()) {
        //    result = true;
        //}

        if (video_tracks.isEmpty()
            && audio_tracks.isEmpty()
            && image_tracks.isEmpty()
            && subtitle_tracks.isEmpty())
            result = false;

        if (!video_tracks.isEmpty()
            && !audio_tracks.isEmpty()
            && !subtitle_tracks.isEmpty()
            && video_tracks[0]->is_fake_track
            && audio_tracks[0]->is_fake_track
            && subtitle_tracks[0]->is_fake_track)
            result = false;
    }

    return result;
}

void VideoHandle::addCompatibleAudioTrack(const VideoTrackInfo* video_track, QList<AudioTrackInfo*>& audio_tracks_transit,
    QList<const AudioTrackInfo*>& audio_tracks) {

    if (!video_track->contains_audio) {
        for (int id = 0; id < audio_tracks_transit.size(); id++) {
            if (!audio_tracks_transit[id]->is_fake_track
                && !audio_tracks_transit[id]->from_video_stream) {

                if ((video_track->profile == VideoProfile::AVC
                    || video_track->profile == VideoProfile::AV1
                    || video_track->profile == VideoProfile::HVC)
                    && audio_tracks_transit[id]->profile != AudioProfile::OGG) {
                    audio_tracks.push_back(audio_tracks_transit[id]);
                    break;
                }
                else if (video_track->profile == VideoProfile::WEBM
                    && audio_tracks_transit[id]->profile == AudioProfile::OGG) {
                    audio_tracks.push_back(audio_tracks_transit[id]);
                    break;
                }

            }
        }
    }
}

void VideoHandle::doDownload() {
    Q_ASSERT(downloader_ != nullptr);
    pending_ = false;
    if (isDownloadable()) {
               
        if (!downloader_->isDownloading()) {
            qreal maximum_cpu_usage = maximum_cpu_usage_;
            QString output_media_path = this->output_media_path_;

            downloader_->setSlowdown(slowdown_);
            downloader_->setMaximumCPUUsage(maximum_cpu_usage);
            downloader_->setMediaOutput(output_media_path);
            downloader_->setSubtitleOutput(output_media_path.left(output_media_path.lastIndexOf(".")));
            downloader_->setImageOutput(output_media_path.left(output_media_path.lastIndexOf(".")));
            downloader_->setPortal(portal_);

            QList<const VideoTrackInfo*>  video_tracks;
            QList<const AudioTrackInfo*>  audio_tracks;
            QList<const SubtitleTrackInfo*>  subtitle_tracks;
            QList<const ImageTrackInfo*>  image_tracks;
            QList<int> saveas_ids;
            QList<int> saveas_subtitle_ids = saveas_subtitle_ids_;

            for (const auto& id : save_as_ids_)
                saveas_ids.push_back(id);

            if (!saveas_ids.isEmpty()) {

                if (downloadVideoEnabled(portal_, getflix_->isActivated())) {
                    for (const auto& id : video_ids_) {
                        if (!tracks_info_.video_tracks[id].is_fake_track)
                            video_tracks.push_back(&tracks_info_.video_tracks[id]);
                    }

                    //if (saveas_ids[0] == kPreset_OriginalVideo  //do not convert webm to mp4
                    //    && !video_tracks.isEmpty()
                    //    && video_tracks[0]->profile == VideoProfile::WEBM) {
                    //    output_media_path_.replace(output_media_path_.mid(output_media_path_.lastIndexOf("."), output_media_path_.size()), ".webm");
                    //    downloader_->setMediaOutput(output_media_path_);
                    //}
                }

                if (!tracks_info_.audio_languages.isEmpty()
                    && !tracks_info_.all_audio_tracks.isEmpty()
                    && !tracks_info_.audio_tracks.isEmpty()) {
                    
                    int i = 0;
                    bool manual_choise = false;
                    for (const auto& lang : audio_language_ids_) {
                        QList<AudioTrackInfo*> audio_tracks_transit;
                        QList<AudioTrackInfo> traks;

                        for (int i = 0; i < tracks_info_.all_audio_tracks.size(); ++i) {
                            if (tracks_info_.all_audio_tracks[i].language_tag != kLanguagesAndCodes[0][1]
                                && tracks_info_.all_audio_tracks[i].language_tag == tracks_info_.audio_languages[lang]
                                && tracks_info_.all_audio_tracks[i].is_assistive == tracks_info_.audio_tracks[0].is_assistive) {

                                audio_tracks_transit << &tracks_info_.all_audio_tracks[i];
                                traks << tracks_info_.all_audio_tracks[i];
                            }
                        }

                        QList<int> ids;
                        if (!audio_tracks_transit.isEmpty()) {

                            ids = audio_ids_;
                            if (i > 0) {                            //get ids for next languages
                                ids.clear();

                                if (manual_choise                   //attempt to guess the user's manual choice
                                    && !audio_tracks.isEmpty()
                                    && audio_tracks[0]->bitrate > 0) {

                                    for (int i = 0; i < audio_tracks_transit.size(); i++) {
                                        if (audio_tracks_transit[i]->profile == audio_tracks[0]->profile
                                            && audio_tracks_transit[i]->channels == audio_tracks[0]->channels
                                            && audio_tracks_transit[i]->bitrate == audio_tracks[0]->bitrate) {
                                            ids = { i };
                                            break;
                                        }
                                    }

                                    if (ids.isEmpty()) {
                                        for (int i = 0; i < audio_tracks_transit.size(); i++) {
                                            if (audio_tracks_transit[i]->bitrate == audio_tracks[0]->bitrate
                                                && audio_tracks_transit[i]->channels == audio_tracks[0]->channels) {
                                                ids = { i };
                                                break;
                                            }
                                        }
                                    }

                                    if (ids.isEmpty()) {
                                        for (int i = 0; i < audio_tracks_transit.size(); i++) {
                                            if (audio_tracks_transit[i]->bitrate == audio_tracks[0]->bitrate) {
                                                ids = { i };
                                                break;
                                            }
                                        }
                                    }
                                }

                                if (ids.isEmpty()) {
                                    switch (audio_selector_) {
                                        case kAudioSelector_MinimumSize:
                                            ids = { (int)audio_tracks_transit.size() - 1 };
                                            break;
                                        case kAudioSelector_Default:
                                            ids = { tracks_info_.getDefaultAudioTrack(traks, portal_) };
                                            break;
                                        case kAudioSelector_HighQuality:
                                            ids = { 0 };
                                            break;
                                    }
                                }
                            }

                            QList<int>  added_ids;
                            for (auto& id : ids) {
                                if (id > audio_tracks_transit.size() - 1)
                                    id = audio_tracks_transit.size() - 1;

                                if ((audio_selector_ == kAudioSelector_MinimumSize
                                        && id < audio_tracks_transit.size() - 1)
                                    || (audio_selector_ == kAudioSelector_HighQuality
                                        && id > 0)
                                    || (audio_selector_ == kAudioSelector_Default
                                        && id != tracks_info_.getDefaultAudioTrack(traks, portal_)))
                                    manual_choise = true;

                                if (!added_ids.contains(id)) {
                                    if (!audio_tracks_transit[id]->is_fake_track) {
                                        if (portal_ == EPortal::kYoutube
                                            && saveas_ids[0] == kPreset_OriginalVideo
                                            && !video_tracks.isEmpty())
                                            addCompatibleAudioTrack(video_tracks.first(), audio_tracks_transit, audio_tracks);
                                        else
                                            audio_tracks.push_back(audio_tracks_transit[id]);
                                    }

                                    added_ids << id;
                                }
                            }

                            i++;
                        }
                    }
                }
                else {
                    if (saveas_ids[0] == kPreset_OriginalVideo
                        && !video_tracks.isEmpty()) {
                        QList<AudioTrackInfo*> audio_tracks_transit;
                        for (int i = 0; i < tracks_info_.audio_tracks.size(); ++i)
                            audio_tracks_transit << &tracks_info_.audio_tracks[i];
                        addCompatibleAudioTrack(video_tracks.first(), audio_tracks_transit, audio_tracks);
                    }
                    else {
                        for (const auto& id : audio_ids_) {
                            if (!tracks_info_.audio_tracks[id].is_fake_track)
                                audio_tracks.push_back(&tracks_info_.audio_tracks[id]);
                        }
                    }
                }

                if (!tracks_info_.image_tracks.isEmpty()) {
                    int image_numbers = 0;
                    for (const auto& it : tracks_info_.all_image_tracks) {
                        if (it.image_number > image_numbers)
                            image_numbers = it.image_number;
                    }

                    int i = 0;
                    bool manual_choise = false;
                    for (int j = 0; j <= image_numbers; j++) {
                        QList<ImageTrackInfo*> image_tracks_transit;
                        QList<ImageTrackInfo> traks;

                        for (int k = 0; k < tracks_info_.all_image_tracks.size(); ++k) {
                            if (tracks_info_.all_image_tracks[k].image_number == j) {
                                image_tracks_transit << &tracks_info_.all_image_tracks[k];
                                traks << tracks_info_.all_image_tracks[k];
                            }
                        }

                        QList<int> ids;
                        if (!image_tracks_transit.isEmpty()) {

                            ids = image_ids_;
                            if (i > 0) {                            //get ids for next image_number
                                ids.clear();

                                if (manual_choise                   //attempt to guess the user's manual choice
                                    && !image_tracks.isEmpty()) {

                                    for (int i = 0; i < image_tracks_transit.size(); i++) {
                                        if (image_tracks_transit[i]->height == image_tracks[0]->height
                                            && image_tracks_transit[i]->width == image_tracks[0]->width) {
                                            ids = { i };
                                            break;
                                        }
                                    }

                                    if (ids.isEmpty()) {
                                        for (int i = 0; i < image_tracks_transit.size(); i++) {
                                            if (image_tracks_transit[i]->height == image_tracks[0]->height) {
                                                ids = { i };
                                                break;
                                            }
                                        }
                                    }

                                    if (ids.isEmpty()) {
                                        for (int i = 0; i < image_tracks_transit.size(); i++) {
                                            if (image_tracks_transit[i]->width == image_tracks[0]->width) {
                                                ids = { i };
                                                break;
                                            }
                                        }
                                    }
                                }

                                if (ids.isEmpty()) {
                                    switch (image_selector_) {
                                        case kImageSelector_MinimumSize:
                                            ids = { int(image_tracks_transit.size()) - 1 };
                                            break;
                                        case kImageSelector_Default:
                                            ids = { tracks_info_.getDefaultImageTrack(traks, portal_) };
                                            break;
                                        case kImageSelector_HighQuality:
                                            ids = { 0 };
                                            break;
                                    }
                                }
                            }

                            QList<int>  added_ids;
                            for (auto& id : ids) {
                                if (id > image_tracks_transit.size() - 1)
                                    id = image_tracks_transit.size() - 1;

                                if ((image_selector_ == kImageSelector_MinimumSize
                                        && id < image_tracks_transit.size() - 1)
                                    || (image_selector_ == kImageSelector_HighQuality
                                        && id > 0)
                                    || (image_selector_ == kImageSelector_Default
                                        && id != tracks_info_.getDefaultImageTrack(traks, portal_)))
                                    manual_choise = true;

                                if (!added_ids.contains(id)) {
                                    if (!image_tracks_transit[id]->is_fake_track)
                                        image_tracks.push_back(image_tracks_transit[id]);

                                    added_ids << id;
                                }
                            }

                            i++;
                        }
                    }
                }

                for (const auto& id : subtitle_ids_) {
                    if (!tracks_info_.subtitle_tracks[id].is_fake_track)
                        subtitle_tracks.push_back(&tracks_info_.subtitle_tracks[id]);
                }

                if (downloadEnabled(saveas_ids, video_tracks, audio_tracks, image_tracks, subtitle_tracks)) {
                    if (downloader_->download(video_tracks, audio_tracks, subtitle_tracks, image_tracks, saveas_ids, saveas_subtitle_ids, video_info_))
                        emit starting();
                    else
                        setError(downloader_->status());
                }
                else {
                    getflix_->removeFromDownloads(this);
                //    emit ready();
                }
            }
            else {
                getflix_->removeFromDownloads(this);
            }
        }
        else if (downloader_->resume()) {
            emit starting();
        }
        else {
            setError(downloader_->status());
        }
    }
}

void VideoHandle::doPause() {
    Q_ASSERT(downloader_ != nullptr);
    pending_ = false;

    if (downloader_
        && downloader_->pause())
        emit pausing();
    else
        emit paused();
}

void VideoHandle::doPending() {
    Q_ASSERT(downloader_ != nullptr);
    pending_ = true;
//    if (!downloader_->isFinished() && downloader_->isDownloading() && !downloader_->isPaused()) {
    if (downloader_
        && downloader_->pause()) 
        emit pausing();
    else
        emit pending();
}

void VideoHandle::doCancel() {
    qDebug() << "Cancelling item: " << this->media_url_;
    //Q_ASSERT(!downloader_ || downloader_->isReady());
    //COMMENT: When analyzing is started the downloader_ is created;
    cancelling_ = true;

    if (downloader_
        && !downloader_->isFinished()) {

        bool wait_decrypt_service = false;

        if (!this->tracks_info_.video_tracks.isEmpty()
            && this->tracks_info_.video_tracks[0].is_encrypted)
            wait_decrypt_service = true;
        
        if (downloader_->cancel(wait_decrypt_service)) {
            cancelling_ = false; //wait service
            emit cancelling();
        }
        else {
            emit cancelled();
        }
    }
    else {
        emit cancelled();
    }
}

float VideoHandle::progress() const {
    if (downloader_)
        return downloader_->progress();

    return -1;
}

void VideoHandle::setFlag(EVideoFlags flag) {
    qDebug() << "setFlag " << flag << " => " << (video_flag_ | flag);

    const bool is_ready = checkFlag(video_flag_, kVideoFlag_Ready);
    const bool is_analyzed = checkFlag(video_flag_, kVideoFlag_Media);
    const bool is_playlist_ready= checkFlag(video_flag_, kVideoFlag_Playlist);

    if ((video_flag_
        & flag) == 0) {
                
        video_flag_ |= flag;
       
        if (checkFlag(video_flag_, kVideoFlag_Media)
            && !is_analyzed)
            emit described();

        if (checkFlag(video_flag_, kVideoFlag_Ready)
            && !is_ready) {

            emit ready();
            
            if (!downloader_
                || downloader_->isCancelling())
                getflix_->handleVideoCancelling(this);
            else
                getflix_->handleVideoReady(this);

            qApp->processEvents();
        }

        if (checkFlag(video_flag_, kVideoFlag_Playlist)
            && !is_playlist_ready)
            emit playlistReady();
    }
}

void VideoHandle::setError(const Status& status) {
    status_.update(status);
    output_lock_.reset();
    errorsetting_ = true;
    emit error();
    getflix_->handleVideoError(this);

    //if (downloader_) {
        //downloader_->deleteLater();
        //downloader_ = nullptr;
    //}
}

void VideoHandle::handleAnalyzing() {
    downloader_ = new DecryptController(this);

    cancelling_ = false;
    errorsetting_ = false;

    QObject::connect(downloader_, &DecryptController::paused, this, [this]() {
        if (pending_)
            emit pending(); //If we do pending state and pause returned;
        else
            emit paused();
    }, Qt::QueuedConnection);

    QObject::connect(downloader_, &DecryptController::resumed, this, &VideoHandle::resumed, Qt::QueuedConnection);
    QObject::connect(downloader_, &DecryptController::cancelled, this, &VideoHandle::cancelled, Qt::QueuedConnection);
    
    QObject::connect(downloader_, &DecryptController::finished, this, [this]() {
        setOutputPath(downloader_->mediaOutput());

        output_lock_.reset();

        video_flag_ = kVideoFlag_NotReady;

        emit finished();

        getflix_->handleFinished(this); //Notify engine about;
    } , Qt::QueuedConnection);

    QObject::connect(downloader_, &DecryptController::error, this, [this]() {
        setError(downloader_->status());
    }, Qt::QueuedConnection);

    QObject::connect(downloader_, &DecryptController::ready, this, [this]() {
        qDebug() << "setFlag kVideoFlag_Decryptor";
        setFlag(kVideoFlag_Decryptor);
    }, Qt::QueuedConnection);

    QObject::connect(downloader_, &DecryptController::progressChanged, this, &VideoHandle::progressChanged, Qt::QueuedConnection);

    emit analyzing();
}
