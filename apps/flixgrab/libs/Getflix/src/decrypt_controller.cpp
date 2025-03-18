#include "decrypt_controller.h"

#include <filesystem>

#include <languages.h>
#include <error_codes.h>

#include <recode_op.h>
#include <recode_op_audio.h>
#include <download_op.h>
#include <addtags_op.h>
#include <remux_mp3_op.h>
#include <transcode_op.h>
#include <concurrent_op.h>
#include <parted_op.h>
#include <decompress_op.h>
#include <recode_op_subtitles.h>
#include <converttostr_op.h>
#include <utils/http_file.h>
#include <utils/string.h>

#include "video_handle.h"
#include "utils/qs_utils.h"
#include "utils/str_parse.h"

#include "get_utils.h"
#include "lang_code.h"
#include <recode_op_keys.h>

#define PROGRESS_TIMEOUT                    200     //ms


DecryptController::DecryptController(QObject* parent) 
    : QObject(parent)
    , progress_(0)
#ifdef DECRYPTSERVICE
    , service_(nullptr)
#endif
    , downloading_(false)
    , initialized_(false)
    , finished_(false)
    , paused_(true)
    , cancelling_(false)
    , mutex_(){

    QMutexLocker locker(&mutex_);
#ifdef DECRYPTSERVICE
    PxStatus status = InstallDecrypsis();
    if (status == kPx_Success) {
        initialized_ = (kPx_Success == RegisterDecrypsisCallback(this));
        elapsed_.start();
    }
    else {
        qDebug() << "Error in intercept module status: " << status;
        status_ = Status(EStatusCode::PxError, "Install module error", status);
    }
#else
    elapsed_.start();
#endif
}

DecryptController::~DecryptController() {
    qDebug() << "~DecryptController";
    exec_.reset();
#ifdef DECRYPTSERVICE
    //Hang on if before exec reset;
    QMutexLocker locker(&mutex_);
    PxStatus status = UnregisterDecrypsisCallback(this);

    if(status != kPx_Success)
        qDebug() << "Error in intercept module status: " << status;

    if (service_)
        service_->Finish();

    UninstallDecrypsis();
#endif
    if (save_output_)
        clearTemp({ temps_recode_ });
    else
        clearTemp({ temps_download_, temps_recode_ });
}

bool DecryptController::download(const QList<const VideoTrackInfo*>& video_tracks, const QList<const AudioTrackInfo*>& audio_tracks,
    const QList <const SubtitleTrackInfo*>& subtitle_tracks, const QList<const ImageTrackInfo*>& image_tracks,
    const QList<int> save_as_ids, const QList<int> saveas_subtitle_ids, const VideoInfo& video_info) {
    QMutexLocker locker(&mutex_);

    if (downloading_)
        return false;
#ifdef DECRYPTSERVICE
    if (!initialized_)
        return false;

    if (req_decrypt_
        && service_ == nullptr) {
        status_.update(Status(EStatusCode::ParseError, "Decryptor not ready yet"));
        return false;
    }
#endif
    video_info_ = video_info;
    audio_file_names_.clear();
    subtitle_file_names_.clear();
    temps_download_.clear();
    temps_recode_.clear();
    thumbnail_file_.clear();
    collection_file_names_.clear();
    post_download_event_ = false;

    std::vector<std::shared_ptr<recode::Operation>> concurrent_ops;

    auto transcode_op = std::make_shared<recode::TranscodeOp>(media_file_.toUtf8().constData(), false, false);
    transcode_op->SetLimitDuration(limit_duration_);

    auto recodemp3_op = std::make_shared<recode::RemuxMP3Op>();
    auto add_tags_op = std::make_shared<recode::AddTagsOp>();

//Video section
    preset_ = save_as_ids[0];

    if (preset_ == kPreset_OriginalVideo) {
        if (!video_tracks.isEmpty()) {

            auto video_track = video_tracks[0];

            if (!video_track->urls.isEmpty()) {
                transcode_op->set_artwork_url(video_info.metadata_artwork_url.toUtf8().constData());
                transcode_op->set_metadata_title(video_info.metadata_title.toUtf8().constData());
                transcode_op->set_metadata_artist(video_info.metadata_artist.toUtf8().constData());
                transcode_op->set_metadata_album(video_info.metadata_album.toUtf8().constData());
                transcode_op->set_metadata_album_artist(video_info.metadata_album_artist.toUtf8().constData());
                int track = video_info.metadata_track.toInt();
                if (track > 0)
                    transcode_op->set_metadata_track(track);
                int year = video_info.metadata_year.toInt();
                if (year > 0)
                    transcode_op->set_metadata_year(year);
                transcode_op->set_metadata_comment(video_info.metadata_comment.toUtf8().constData());
                transcode_op->set_metadata_genre(video_info.metadata_genre.toUtf8().constData());

                auto video_op = std::make_shared<recode::PartedOp>();
                auto download_file = makeTemp(kDownload);
                auto download_file_recoded = makeTemp(kRecode);

                if ((video_track->profile == VideoProfile::AVC
                    || video_track->profile == VideoProfile::AV1
                    || video_track->profile == VideoProfile::HVC)
                    && video_track->contains_audio
                    && !video_track->urls[0].contains(".m3u8")
                    && !video_track->is_encrypted
                    && portal_ != EPortal::kFacebook
                    && limit_duration_ == 0) {

                    download_file = media_file_;
                }
                else {
                    if (video_track->is_encrypted) {
                        if (portal_ == EPortal::kNetflix
                            && encoding_preset_ == "2pass")
                            transcode_op->UseH264Codec(true);
                        transcode_op->AddStream(download_file_recoded.toStdString());
                    }
                    else {
                        if (video_track->urls[0].contains(".m3u8"))
                            transcode_op->AddStream(QS2S(video_track->urls[0]));
                        else
                            transcode_op->AddStream(download_file.toStdString());
                    }

                    if ((video_track->profile == VideoProfile::AVC
                        || video_track->profile == VideoProfile::AV1
                        || video_track->profile == VideoProfile::HVC
                        /*|| video_track->profile == VideoProfile::WEBM*/) //do not convert webm to mp4
                        && !video_track->urls[0].contains(".m3u8")) {
                        transcode_op->SetCopyCodec(true);
                    }
                }

                utils::HttpFile::Mode mode = utils::HttpFile::Mode::Continue;
                if (video_track->is_chunked
                    && portal_ != EPortal::kInstagram)
                    mode = utils::HttpFile::Mode::Append;

                if (!video_track->urls[0].contains(".m3u8")) {
                    if (!video_track->is_chunked) {
                            auto download_op = std::make_shared<recode::DownloadOp>(video_track->urls[0].toUtf8().constData(), download_file.toStdString(), mode);
                            download_op->set_chunk_size(chunk_size_);
                            if (limit_duration_ > 0
                                && video_info.duration > limit_duration_)
                                download_op->set_limit_size((double)limit_duration_ / video_info.duration + 0.06); // +6 % just in case

                            if (video_track->is_encrypted)
                                video_op->Add(download_op, 0.25); // download part
                            else
                                video_op->Add(download_op, 1);
                    }
                    else {
                        long total_received_ = 0;
                        long i = calcTotalReceived(download_file, video_track->size, video_track->urls, total_received_);
                        double chunk = i > 0 ? i : 1; // for the purpose of progress calc: we assume that the weight of an incomplete chunk is equal to the sum of the weights of all completed chunks. 
                                                        
                        double limit_size = video_track->urls.size();
                        if (limit_duration_ > 0
                            && video_info.duration > limit_duration_) {
                            double limit_parts = (double)limit_duration_ / video_info.duration + 0.06; // +6 % just in case
                            if (limit_parts > 0) {
                                limit_size = video_track->urls.size() * limit_parts;
                                if (limit_size > 0)
                                    limit_size = ceil(limit_size);
                            }
                        }

                        for (int i = 0; i < limit_size; i++) {
                            if (portal_ == EPortal::kInstagram) {
                                download_file = getInstagramOutName(i).c_str();
                                collection_file_names_.push_back(download_file);
                            }

                            auto download_op = std::make_shared<recode::DownloadOp>(video_track->urls.at(i).toUtf8().constData(), download_file.toStdString(), mode, total_received_);
                            total_received_ = 0;
                            download_op->set_chunk_size(chunk_size_);
                            if (video_track->is_encrypted)
                                video_op->Add(download_op, chunk / 4 / video_track->urls.size());
                            else
                                video_op->Add(download_op, chunk / video_track->urls.size());

                            chunk = 1; // consider all chunk weights are equal
                        }
                    }

                    if (video_track->is_encrypted)
                        addDecryptOperation(video_op, download_file, download_file_recoded, video_info.duration, limit_duration_, video_track->keys);

                    concurrent_ops.push_back(video_op);
                }
            }
        }

        //Audio section - if audio separated
        if ((!video_tracks.isEmpty() && !video_tracks[0]->contains_audio)
            || (video_tracks.isEmpty() && !audio_tracks.isEmpty())) {                   // none video case

            auto audio_op = std::make_shared<recode::PartedOp>();

            for (const auto& audio_track : audio_tracks) {
                if (!audio_track->urls.isEmpty()) {
                    auto download_file = makeTemp(kDownload);
                    auto download_file_recoded = makeTemp(kRecode);

                    if (video_tracks.isEmpty()) {
                        if (!audio_track->is_encrypted) {
                            download_file = makeTemp(kSave, audio_track->language_tag.toUpper());
                            audio_file_names_.push_back(download_file);
                            if (audio_track == audio_tracks.last())
                                setMediaOutput(download_file);
                        }
                        else {
                            download_file_recoded = makeTemp(kSave, audio_track->language_tag.toUpper());
                            audio_file_names_.push_back(download_file_recoded);
                            if (audio_track == audio_tracks.last())
                                setMediaOutput(download_file_recoded);
                        }
                    }
                    else {
                        if (audio_track->is_encrypted) {
                            transcode_op->AddStream(download_file_recoded.toStdString(), 
                                std::pair("language", recode::lang_code_3(audio_track->language_tag.split("-").first().toUtf8().constData()))
                            );
                        }
                        else {
                            if (audio_track->urls[0].contains(".m3u8")) {
                                transcode_op->AddStream(QS2S(audio_track->urls[0]),
                                    std::pair("language", recode::lang_code_3(audio_track->language_tag.split("-").first().toUtf8().constData()))
                                );
                            }
                            else {
                                transcode_op->AddStream(download_file.toStdString(),
                                    std::pair("language", recode::lang_code_3(audio_track->language_tag.split("-").first().toUtf8().constData()))
                                );
                            }
                        }

                        if ((!video_tracks.isEmpty()
                            && !video_tracks[0]->urls.isEmpty()
                            && !video_tracks[0]->urls[0].contains(".m3u8")
                            && (video_tracks[0]->profile == VideoProfile::AVC
                                || video_tracks[0]->profile == VideoProfile::AV1
                                /*|| video_tracks[0]->profile == VideoProfile::WEBM*/       //do not convert webm to mp4
                                || video_tracks[0]->profile == VideoProfile::HVC))
                            || (video_tracks.isEmpty() && !audio_tracks.isEmpty()))        // none video case
                            transcode_op->SetCopyCodec(true);
                    }

                    utils::HttpFile::Mode mode = utils::HttpFile::Mode::Continue;
                    if (audio_track->is_chunked)
                        mode = utils::HttpFile::Mode::Append;

                    if (!audio_track->urls[0].contains(".m3u8")) {
                        if (!audio_track->is_chunked) {
                            auto download_op = std::make_shared<recode::DownloadOp>(audio_track->urls[0].toUtf8().constData(), download_file.toStdString(), mode);
                            download_op->set_chunk_size(chunk_size_);

                            if (limit_duration_ > 0
                                && video_info.duration > limit_duration_)
                                download_op->set_limit_size((double)limit_duration_ / video_info.duration + 0.02); // +2 % just in case


                            if (audio_track->is_encrypted
                                || video_tracks.isEmpty()) {
                                audio_op->Add(download_op, 0.25 / audio_tracks.size());
                            }
                            else {
                                audio_op->Add(download_op, 1 / audio_tracks.size());
                            }
                        }
                        else {
                            long total_received_ = 0;
                            long i = calcTotalReceived(download_file, audio_track->size, audio_track->urls, total_received_);
                            double chunk = i > 0 ? i : 1; // for the purpose of progress calc: we assume that the weight of an incomplete chunk is equal to the sum of the weights of all completed chunks. 

                            double limit_size = audio_track->urls.size();
                            if (limit_duration_ > 0
                                && video_info.duration > limit_duration_) {
                                double limit_parts = (double)limit_duration_ / video_info.duration + 0.02; // +2 % just in case
                                if (limit_parts > 0) {
                                    limit_size = audio_track->urls.size() * limit_parts;
                                    if (limit_size > 0)
                                        limit_size = ceil(limit_size);
                                }
                            }

                            for (int i = 0; i < limit_size; i++) {
                                auto download_op = std::make_shared<recode::DownloadOp>(audio_track->urls.at(i).toUtf8().constData(), download_file.toStdString(), mode, total_received_);
                                total_received_ = 0;
                                download_op->set_chunk_size(chunk_size_);
                                if (audio_track->is_encrypted)
                                    audio_op->Add(download_op, chunk / 4 / audio_tracks.size() / audio_track->urls.size());
                                else
                                    audio_op->Add(download_op, chunk / audio_tracks.size() / audio_track->urls.size());
                            }

                            chunk = 1; // consider all chunk weights are equal
                        }

                        if (audio_track->is_encrypted)    //amazon, hulu cases
                            addDecryptOperation(audio_op, download_file, download_file_recoded, video_info.duration, limit_duration_, audio_track->keys);
                    }
                }
            }

            concurrent_ops.push_back(audio_op);
        }
    }
//Audio section 
    if (preset_ != kPreset_OriginalVideo
        && !audio_tracks.isEmpty()) {

        auto audio_op = std::make_shared<recode::PartedOp>();
        auto thumbnail_op = std::make_shared<recode::PartedOp>();

        //audio thumbnail
        if (!video_info.metadata_artwork_url.isEmpty()) {
            thumbnail_file_ = makeTemp(kThumbnail).toStdString();
            auto artwork_url = video_info.metadata_artwork_url.toStdString();
            auto download_thumbnail_op = std::make_shared<recode::DownloadOp>(artwork_url, thumbnail_file_, utils::HttpFile::Mode::Continue);
            download_thumbnail_op->set_chunk_size(image_chunk_size_);

            thumbnail_op->Add(download_thumbnail_op, 1);
        }

        //for (const auto& audio_track : audio_tracks) { //TODO fix converting some items to mp3
        auto audio_track = audio_tracks[0];

            if (!audio_track->urls.isEmpty()) {
                const auto download_file_recoded = makeTemp(kRecode);
                auto download_file = makeTemp(kDownload);
                if (audio_track->urls[0].contains(".m3u8")) {
                    download_file = media_file_;
                    download_file.replace(download_file.mid(download_file.lastIndexOf("."), download_file.size()), ".m4a");
                    transcode_op->SetOutputFile(QS2S(download_file));
                    if (preset_ != kPreset_OriginalAudio)
                        temps_download_.push_back(download_file);
                }

                double size = audio_track->size;
                if (audio_track->from_video_stream) {
                    if (preset_ == kPreset_MP3Original)
                        preset_ = kPreset_MP3FromVideoStream;            //use bitrate from input
                    if (!video_tracks.isEmpty()
                        && !video_tracks[0]->urls.isEmpty())
                        size = video_tracks[0]->urls[0].size();
                }

                utils::HttpFile::Mode mode = utils::HttpFile::Mode::Continue;
                if (audio_track->is_chunked
                    && portal_ != EPortal::kInstagram)
                    mode = utils::HttpFile::Mode::Append;

                if (preset_ == kPreset_OriginalAudio
                    || audio_track->urls[0].contains(".m3u8")) {
                    transcode_op->SetAudioOnly(true);
                    if ((thumbnail_file_.empty()
                        || media_file_.mid(media_file_.lastIndexOf("."), media_file_.size()) != ".ogg")
                        && !audio_track->urls[0].contains(".m3u8"))
                        transcode_op->SetCopyCodec(true);

                    if (audio_track->is_encrypted) {
                        transcode_op->AddStream(download_file_recoded.toStdString(),
                            std::pair("language", recode::lang_code_3(audio_track->language_tag.split("-").first().toUtf8().constData()))
                        );
                    }
                    else {
                        if (audio_track->urls[0].contains(".m3u8"))
                            transcode_op->AddStream(QS2S(audio_track->urls[0]),
                                std::pair("language", recode::lang_code_3(audio_track->language_tag.split("-").first().toUtf8().constData()))
                            );
                        else
                            transcode_op->AddStream(download_file.toStdString(),
                                std::pair("language", recode::lang_code_3(audio_track->language_tag.split("-").first().toUtf8().constData()))
                            );

                        //download_thumbnail_file.clear(); // TODO image to m4a -> Specified pixel format yuvj420p is invalid or not supported

                        //if (!download_thumbnail_file.empty() && media_file_.mid(media_file_.lastIndexOf("."), media_file_.size()) == ".m4a") //only m4a container supported
                        //    transcode_op->AddStream(download_thumbnail_file);
                    }
                }

                if (preset_ != kPreset_OriginalAudio) {                                                                          //mp3 audio
                    if (audio_track->is_encrypted) {
                        recodemp3_op->AddStream(download_file_recoded.toStdString(), std::make_pair(QS2W(media_file_), preset_));
                    }
                    else {
                        if (portal_ != EPortal::kInstagram) {
                            recodemp3_op->AddStream(download_file.toStdString(), std::make_pair(QS2W(media_file_), preset_));
                        }
                        else {
                            for (int i = 0; i < audio_track->urls.size(); i++) {
                                QString mp3_name = makeTemp(kDownload); // media_file_
                                recodemp3_op->AddStream(getInstagramOutName(i), std::make_pair(QS2W(mp3_name), preset_));
                            }
                        }
                    }
                }

                if (!audio_track->urls[0].contains(".m3u8")) {
                    if (!audio_track->is_chunked) {
                        auto download_op = std::make_shared<recode::DownloadOp>(audio_track->urls[0].toStdString(), download_file.toStdString(), mode);
                        download_op->set_chunk_size(chunk_size_);
                        if (limit_duration_ > 0
                            && video_info.duration > limit_duration_)
                            download_op->set_limit_size((double)limit_duration_ / video_info.duration + 0.02); // +2 % just in case

                        if (audio_track->is_encrypted)  //amazon case
                            audio_op->Add(download_op, 0.25);
                        else
                            audio_op->Add(download_op, 1);
                    }
                    else {
                        long total_received_ = 0;
                        long i = calcTotalReceived(download_file, audio_track->size, audio_track->urls, total_received_);
                        double chunk = i > 0 ? i : 1; // for the purpose of progress calc: we assume that the weight of an incomplete chunk is equal to the sum of the weights of all completed chunks.

                        double limit_size = audio_track->urls.size();
                        if (limit_duration_ > 0
                            && video_info.duration > limit_duration_) {
                            double limit_parts = (double)limit_duration_ / video_info.duration + 0.02; // +2 % just in case
                            if (limit_parts > 0) {
                                limit_size = audio_track->urls.size() * limit_parts;
                                if (limit_size > 0)
                                    limit_size = ceil(limit_size);
                            }
                        }

                        for (int i = 0; i < limit_size; i++) {
                            if (portal_ == EPortal::kInstagram)
                                std::string download_file = getInstagramOutName(i);

                            auto download_op = std::make_shared<recode::DownloadOp>(audio_track->urls.at(i).toUtf8().constData(), download_file.toStdString(), mode, total_received_);
                            total_received_ = 0;
                            download_op->set_chunk_size(chunk_size_);
                            if (audio_track->is_encrypted)
                                audio_op->Add(download_op, chunk / 4 / audio_tracks.size() / audio_track->urls.size());
                            else
                                audio_op->Add(download_op, chunk / audio_tracks.size() / audio_track->urls.size());
                        }

                        chunk = 1; // consider all chunk weights are equal
                    }

                    if (audio_track->is_encrypted) {
                        addDecryptOperation(audio_op, download_file, download_file_recoded, video_info.duration, limit_duration_, audio_track->keys);
                    }
                }
            }
        //}
        concurrent_ops.push_back(audio_op);
        concurrent_ops.push_back(thumbnail_op);
    }

//Subtitles section
    QString configDataDir = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
    std::string tess_data_folder = QS2S(configDataDir + "/" + TESSERACTFOLDER + "/");
    auto conv_sub_op = std::make_shared<recode::ConvertToStrOp>(tess_data_folder);
    auto decompress_op = std::make_shared<recode::DecompressOp>(QS2S(configDataDir));

    if (!subtitle_tracks.isEmpty()) {
        QDir dir(configDataDir);
        if (!dir.exists())
            dir.mkpath(configDataDir);

        decompress_op->SaveOriginalZipTrack(saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Original)), QS2W(subtitle_file_.left(subtitle_file_.lastIndexOf("/") + 1)));

        auto sub_op = std::make_shared<recode::PartedOp>();

        for (const auto& subtitle_track : subtitle_tracks) {
            if (!subtitle_track->urls.isEmpty()) {
                utils::HttpFile::Mode mode = utils::HttpFile::Mode::Truncate;
                if (subtitle_track->is_chunked)
                    mode = utils::HttpFile::Mode::Append;

                //Make file path;
                QString lang = SS2Q(getLangNameByLangTag(QS2S(subtitle_track->language_tag)));
                if (lang.isEmpty()) {
                    lang = subtitle_track->language;
                    if (lang.isEmpty())
                        lang = subtitle_track->language_tag.toUpper();
                }

                QString cc_info;
                if (subtitle_track->is_closed_captions) {
                    cc_info = subtitlesCCInfo(portal_);
                    if (!cc_info.isEmpty())
                        cc_info = " " + cc_info;
                }

                QString addit_info;
                if (subtitle_track->is_additional) {
                    addit_info = subtitlesAdditionalInfo(portal_);
                    if (!addit_info.isEmpty())
                        addit_info = " - " + addit_info;
                }
                
                QString base_name = QString("%1.%2%3%4.").arg(subtitle_file_).arg(lang).arg(cc_info).arg(addit_info);
                QString srt_file_name = base_name + "srt";
                
                if (subtitle_track->profile == SubtitleTrackInfo::SubtitleProfile::SRT) {
                    auto download_op = std::make_shared<recode::DownloadOp>(subtitle_track->urls[0].toUtf8().constData(), QS2S(srt_file_name), mode);
                    download_op->set_chunk_size(image_chunk_size_);
                    sub_op->Add(download_op, subtitle_track->size);
                    subtitle_file_names_.push_back(srt_file_name);
                }
                else {
                    QString download_file = configDataDir + "/" + base_name.section('/', -1) + getSubtitleTrackExt(subtitle_track->profile);
                    if ((!saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_SRT))
                        && !saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Embedded)))
                        || !conv_sub_op->SupportedFormat(QS2W(download_file)))
                        download_file = base_name + getSubtitleTrackExt(subtitle_track->profile);

                    if (download_file.contains(".zip")) {
                        QString original_zip_file = subtitle_file_.left(subtitle_file_.lastIndexOf("/") + 1)
                            + download_file.mid(download_file.lastIndexOf("/") + 1, download_file.size());
                        if (saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Original)))
                            subtitle_file_names_.push_back(original_zip_file);
                    }

                    subtitle_file_names_.push_back(download_file);
                    auto recode_subtitles_op = std::make_shared<recode::RecodeOpSubtitles>(QS2S(download_file));

                    if (!subtitle_track->is_chunked) {
                        auto download_op = std::make_shared<recode::DownloadOp>(subtitle_track->urls[0].toUtf8().constData(), QS2S(download_file), mode);
                        download_op->set_chunk_size(image_chunk_size_);
                        sub_op->Add(download_op, subtitle_track->size);
                        sub_op->Add(recode_subtitles_op, subtitle_track->size);
                    }
                    else {
                        long total_received_ = 0;
                        long i = calcTotalReceived(download_file, subtitle_track->size, subtitle_track->urls, total_received_);
                        double chunk = i > 0 ? i : 1; // for the purpose of progress calc: we assume that the weight of an incomplete chunk is equal to the sum of the weights of all completed chunks.

                        for (int i = 0; i < subtitle_track->urls.size(); i++) {
                            auto download_op = std::make_shared<recode::DownloadOp>(subtitle_track->urls.at(i).toUtf8().constData(), QS2S(download_file), mode, total_received_);
                            total_received_ = 0;
                            download_op->set_chunk_size(image_chunk_size_);
                            sub_op->Add(download_op, chunk / subtitle_tracks.size() / subtitle_track->urls.size());
                        }

                        chunk = 1; // consider all chunk weights are equal
                        sub_op->Add(recode_subtitles_op, subtitle_track->size);
                    }

                    if (saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_SRT))
                        || saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Embedded))) {

                        if (conv_sub_op->SupportedFormat(QS2W(download_file))) {
                            if (video_tracks.isEmpty()
                                && audio_tracks.isEmpty()
                                && image_tracks.isEmpty()) {
                                setMediaOutput(srt_file_name);
                            }

                            subtitle_file_names_.push_back(srt_file_name);

                            std::pair<std::string, int64_t> tesseract_track = tesseractDataUrl(QS2S(subtitle_track->language_tag));
                            if (subtitle_track->profile == SubtitleTrackInfo::SubtitleProfile::NflxCmisc
                                && tesseract_track.second > 0) {

                                bool tesseract_file_exists = false;
                                std::string  tesseract_lang = tesseractLang(QS2S(subtitle_track->language_tag));
                                if (std::filesystem::exists(tess_data_folder)) {
                                    for (const auto& entry : std::filesystem::directory_iterator(tess_data_folder)) {
                                        std::string file_name = entry.path().filename().string();
                                        if (file_name == (tesseract_lang + ".traineddata")) {
                                            tesseract_file_exists = true;
                                            break;
                                        }
                                    }
                                }

                                if (!tesseract_file_exists) {
                                    if (!std::filesystem::exists(tess_data_folder))
                                        std::filesystem::create_directory(tess_data_folder);
                                    //std::string download_tess_file = QS2S(QDir::currentPath()) + tesseract_track.first.substr(tesseract_track.first.find_last_of("/"), tesseract_track.first.size());
                                    std::string download_tess_file = tess_data_folder + tesseract_track.first.substr(tesseract_track.first.find_last_of("/") + 1, tesseract_track.first.size());
                                    auto download_op = std::make_shared<recode::DownloadOp>(tesseract_track.first, download_tess_file, mode);
                                    download_op->set_chunk_size(image_chunk_size_);
                                    sub_op->Add(download_op, tesseract_track.second);
                                    //decompress_op->AddTarGz(download_tess_file, QS2S(tess_data_folder));
                                }

                                QFileInfo fi(download_file);
                                QString dir_name = fi.fileName().left(fi.fileName().lastIndexOf("."));

                                QString output_path = fi.path() + "/" + dir_name + "/";
                                decompress_op->AddZip(QS2S(download_file), QS2S(output_path));

                                std::string unziped_file = QS2S(output_path + "manifest_ttml2.xml");

                                conv_sub_op->Add(unziped_file, QS2W(srt_file_name));

                                if (saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Embedded))) {
                                    transcode_op->AddStream(srt_file_name.toStdString(),
                                        std::pair("language", recode::lang_code_3(subtitle_track->language_tag.split("-").first().toUtf8().constData()))
                                    );

                                    if (!saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_SRT)))
                                        temps_download_.push_back(srt_file_name);
                                }
                            }
                            else {
                                conv_sub_op->Add(QS2S(download_file), QS2W(srt_file_name));
                                conv_sub_op->SaveOriginalTrack(saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Original)));

                                if (saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Embedded))
                                    && !video_tracks.isEmpty()) {

                                    transcode_op->AddStream(srt_file_name.toStdString(),
                                        std::pair("language", recode::lang_code_3(subtitle_track->language_tag.split("-").first().toUtf8().constData()))
                                    );

                                    if (!saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_SRT)))
                                        temps_download_.push_back(srt_file_name);
                                }
                            }
                        }
                        else {
                            if (!saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_Original))
                                && !saveas_subtitle_ids.contains(int(kSaveSubtitlesAs_SRT)))
                                temps_download_.push_back(download_file);
                        }
                    }
                    else {
                        if (video_tracks.isEmpty()
                            && audio_tracks.isEmpty()
                            && image_tracks.isEmpty()) {
                            setMediaOutput(download_file);
                        }
                    }
                }
            }
        }

        concurrent_ops.push_back(sub_op);
    }

//Images section
    if (!image_tracks.isEmpty()) {
        auto img_op = std::make_shared<recode::PartedOp>();
        
        utils::HttpFile::Mode mode = utils::HttpFile::Mode::Continue;
        
        for (const auto& image_track : image_tracks) {
            if (!image_track->urls.isEmpty()) {

                QString file_name = QString("%1.%2").arg(image_file_).arg(image_track->extension);
                if (image_track->image_number > 0)
                    file_name = QString("%1%2.%3").arg(image_file_).arg(image_track->image_number).arg(image_track->extension);

                collection_file_names_.push_back(file_name);

                auto download_op = std::make_shared<recode::DownloadOp>(image_track->urls[0].toUtf8().constData(), QS2S(file_name), mode);
                download_op->set_chunk_size(image_chunk_size_);
                img_op->Add(download_op, image_track->size);
            }
        }

        concurrent_ops.push_back(img_op);
    }

    std::vector<std::pair<std::shared_ptr<recode::Operation>, float>> ops;
    auto streams_op = std::make_shared<recode::ConcurrentOp>(concurrent_ops);

//Progress section
    if (transcode_op->StreamCount() > 0) {
        if (concurrent_ops.empty()) {
            ops.push_back({ transcode_op , 1.0f });
        }
        else {
            if (transcode_op->CopyCodec()) {
                if (decompress_op->StreamTarGzCount() > 0
                    || decompress_op->StreamZipCount() > 0) {
                    ops.push_back({ streams_op, 0.35f });
                    ops.push_back({ conv_sub_op, 0.6f });
                    ops.push_back({ transcode_op , 0.01f });
                    ops.push_back({ decompress_op, 0.04f });
                }
                else if (conv_sub_op->StreamCount() > 0) {
                    ops.push_back({ streams_op, 0.9f });
                    ops.push_back({ conv_sub_op, 0.05f });
                    ops.push_back({ transcode_op , 0.05f });
                }
                else {
                    ops.push_back({ streams_op , 0.9f });
                    ops.push_back({ transcode_op , 0.1f });
                }
            }
            else {
                if (decompress_op->StreamTarGzCount() > 0
                    || decompress_op->StreamZipCount() > 0) {
                    ops.push_back({ streams_op, 0.3f });
                    ops.push_back({ conv_sub_op, 0.08f });
                    ops.push_back({ transcode_op , 0.6f });
                    ops.push_back({ decompress_op, 0.02f });
                }
                else if (conv_sub_op->StreamCount() > 0) {
                    ops.push_back({ streams_op, 0.23f });
                    ops.push_back({ conv_sub_op, 0.07f });
                    ops.push_back({ transcode_op , 0.7f });
                }
                else {
                    ops.push_back({ streams_op , 0.3f });
                    ops.push_back({ transcode_op , 0.7f });
                }
            }
        }
    }
    else if (!concurrent_ops.empty()) {
        if (decompress_op->StreamTarGzCount() > 0
            || decompress_op->StreamZipCount() > 0) {
            ops.push_back({ streams_op, 0.35f });
            ops.push_back({ conv_sub_op, 0.6f });
            ops.push_back({ decompress_op, 0.05f });
        }
        else if (conv_sub_op->StreamCount() > 0) {
            ops.push_back({ streams_op, 0.9f });
            ops.push_back({ conv_sub_op, 0.1f });
        }
        else {
            if (recodemp3_op->StreamCount() > 0)
                ops.push_back({ streams_op, 0.05f });
            else
                ops.push_back({ streams_op, 1.0f });
        }
    }

    if (recodemp3_op->StreamCount() > 0) {
        ops.push_back({ recodemp3_op , 0.95f });
    }

    if (preset_ != kPreset_OriginalVideo) {
        bool add_metainfo = false;
        if (video_info.enable_metadata) {
            add_metainfo = true;
            add_tags_op->set_metadata_title(QS2W(video_info.metadata_title));
            add_tags_op->set_metadata_artist(QS2W(video_info.metadata_artist));
            add_tags_op->set_metadata_album(QS2W(video_info.metadata_album));
            add_tags_op->set_metadata_album_artist(QS2W(video_info.metadata_album_artist));
            add_tags_op->set_metadata_comment(QS2W(video_info.metadata_comment));
            add_tags_op->set_metadata_genre(QS2W(video_info.metadata_genre));
            add_tags_op->set_metadata_year(video_info.metadata_year.toInt());
            add_tags_op->set_metadata_track(video_info.metadata_track.toInt());   //can be in form current
        }

        if (!thumbnail_file_.empty()) {
            add_metainfo = true;
            add_tags_op->set_metadata_image_path(thumbnail_file_);
        }

        if (add_metainfo) {
            add_tags_op->Add(QS2W(media_file_));
            ops.push_back({ add_tags_op , 0.01f });
        }
    }

    if (ops.empty())
        return false;

    post_download_event_ = true;

    exec_ = std::make_unique<recode::Exec>(ops, this);
    exec_->set_slow_factor(slowdown_);
    exec_->set_maximum_cpu_usage(maximum_cpu_usage_);

    downloading_ = true;
    paused_ = false;
    return true;
}

void DecryptController::addDecryptOperation(const std::shared_ptr<recode::PartedOp>& op, const QString download_file, const QString download_file_recoded, int duration, int limit_duration,
    const QList<QString>& keys) {
    if (keys.size() > 0) {
        // Decrypt with private keys 

        std::vector<std::string> stdKeys;
        for (const auto& k : keys)
            stdKeys.push_back(k.toStdString()); // convert to std:

        auto recode_op = std::make_shared<recode::RecodeOpKeys>(download_file.toStdString(), download_file_recoded.toStdString(), stdKeys);
        //recode_op->set_duration(duration);
        op->Add(recode_op, 0.15);
        qDebug() << "prepeare decrypt with privateKeys";
    }
    else {
        // CDM decryption
#ifdef DECRYPTSERVICE
        if (encoding_preset_ == "2pass")
            encoding_preset_ = "ultrafast";
        auto recode_op = std::make_shared<recode::RecodeOp>(download_file.toStdString(), download_file_recoded.toStdString(), service_, QS2S(encoding_preset_));
        recode_op->set_duration(duration);
        recode_op->set_limit_duration(limit_duration);

        qDebug() << "prepeare decrypt with CDM";
#if _DEBUG
        //recode_op->set_video_width(400);
        //recode_op->set_video_height(300);
#endif
        op->Add(recode_op, 0.75);
#endif
    }
}

std::string DecryptController::getInstagramOutName(int i) {
    std::string part_name;
    if (i > 0)
        part_name = std::to_string(i);
    std::vector<std::string> vL;
    str::tokenize<char>(media_file_.toUtf8().constData(), ".", vL);
    std::string out_name;
    
    for (size_t i = 0; i < vL.size(); ++i) {
        std::string part = vL[i];
        if (i != vL.size() - 1 && i != vL.size() - 2)
            part.append(".");
        else if (i != vL.size() - 2)
            part = part_name + "." + part;

        out_name.append(part);
    }

    if (out_name.empty())
        out_name = media_file_.toUtf8().constData();

    return out_name;
}

bool DecryptController::cancel(bool wait_decrypt_service) {
    QMutexLocker locker(&mutex_);
    qDebug() << "DecryptController::cancel";
    cancelling_ = true;

    bool res = false;

    if (exec_ 
        && downloading_) {
        exec_->Cancel();
        res = true;
    }
    else if (wait_decrypt_service) {
#ifdef DECRYPTSERVICE
        if (service_ == nullptr) //wait until service is ready;
            res = true;
#endif
    }

    return res;
}

bool DecryptController::pause() {
    QMutexLocker locker(&mutex_);
      
    if (exec_ && downloading_ && !paused_) 
        exec_->Pause(true);
    else
        return false;
    paused_ = true;

    return true;
}

bool DecryptController::resume() {
    QMutexLocker locker(&mutex_);
    qDebug() << "Start Resume";
   
    if (exec_ && downloading_ && paused_)
        exec_->Pause(false);
    else
        return false;
    paused_ = false;

    return true;
}


void DecryptController::setSlowdown(const float factor) {
    QMutexLocker locker(&mutex_);
    slowdown_ = factor;

    if (exec_)
        exec_->set_slow_factor(slowdown_);
}

void DecryptController::setLimitDuration(const int limit_duration) {
    limit_duration_ = limit_duration * 60; //min
}

void DecryptController::setMaximumCPUUsage(float factor) {
    QMutexLocker locker(&mutex_);
    maximum_cpu_usage_ = factor;

    if (exec_)
        exec_->set_maximum_cpu_usage(maximum_cpu_usage_);
}

void DecryptController::setMediaOutput(const QString& media_file) {
    media_file_ = media_file;
}

void DecryptController::setSubtitleOutput(const QString& file_path) {
    subtitle_file_ = file_path;
}

void DecryptController::setImageOutput(const QString& file_path) {
    image_file_ = file_path;
}

void DecryptController::setPortal(const EPortal& portal) {
    portal_ = portal;
}

void DecryptController::OnProgress(recode::Operation* , float value) {
    QMutexLocker locker(&mutex_);
    progress_ = value*100.f;
    //TODO: remove progress factors from recode::Exec;
    if (progress_ > 0
        && elapsed_.hasExpired(progress_timeout_)) {
        elapsed_.start();
        progress_timeout_ = PROGRESS_TIMEOUT;
        emit progressChanged(progress_);
    }
    
}

void DecryptController::OnPaused(recode::Operation* , bool is_paused) {
    QMutexLocker locker(&mutex_);
    if (is_paused) {
        qDebug() << "Paused";
        emit paused();
    }
    else {
        qDebug() << "Resumed";
        emit resumed();
    }
}

void DecryptController::OnCancelled(recode::Operation* ) {
    QMutexLocker locker(&mutex_);
    qDebug() << "Operation Cancelled";
    downloading_ = false;
    cancelling_ = false;

#ifdef DECRYPTSERVICE
    if (service_)
        service_->Finish();
#endif
    if (save_output_)
        clearTemp({ temps_recode_ });
    else
        clearTemp({ temps_download_, temps_recode_, subtitle_file_names_ });

    emit cancelled();
}

void DecryptController::OnFinished(void) {
    if (post_download_event_) {
        post_download_event_ = false;

        if (preset_ != kPreset_OriginalVideo
            && video_info_.enable_metadata)
            addTags();
    }

    QMutexLocker locker(&mutex_);
    qDebug() << "Download Finished";
    downloading_ = false;
    finished_ = true;
#ifdef DECRYPTSERVICE
    if (service_)
        service_->Finish();
#endif
    clearTemp({ temps_download_, temps_recode_ });
    emit finished();
}

void DecryptController::OnError(recode::Operation* , const recode::Status& op_status) {
    QMutexLocker locker(&mutex_);

    qDebug() << "Operation Error: " << op_status.error_message().c_str();
    EStatusCode code = EStatusCode::Success;

    switch (op_status.error_code()) {
        case recode::error::UNKNOWN:
        case recode::error::INVALID_ARGUMENT:
        case recode::error::UNIMPLEMENTED:
            code = EStatusCode::UnknownError;
            break;
        case recode::error::SHAKA_ERROR:
            code = EStatusCode::StreamError;
            break;
        case recode::error::INJECTION_ERROR:
            code = EStatusCode::PxError;
            break;
        case recode::error::ENCODER_ERROR:
            code = EStatusCode::EncoderError;
            break;
        case recode::error::CDM_ERROR:
            code = EStatusCode::CdmError;
            break;
        case recode::error::HTTP_ERROR:
            code = EStatusCode::HttpError;
            break;
        case recode::error::NO_MEMORY:
            code = EStatusCode::NoMemory;
            break;
        case recode::error::FILE_ERROR:
            code = EStatusCode::FileError;
            break;
        case recode::error::CONV_ERROR:
            code = EStatusCode::ConvError;
            break;
        case recode::error::CONV_SUBTITLES_ERROR:
            code = EStatusCode::ConvSubtitlesError;
            break;
        case recode::error::RECODE_SUBTITLES_ERROR:
            code = EStatusCode::RecodeSubtitlesError;
            break;
        case recode::error::DECOMPRESS_SUBTITLES_ERROR:
            code = EStatusCode::DecompressSubtitlesError;
            break;
        case recode::error::WRITE_METADATA_ERROR:
            code = EStatusCode::WriteMetadataError;
            break;

        default:
            code = EStatusCode::UnknownError;
            break;
    }

    status_ = Status( code, 
        op_status.error_message().c_str(), 
        op_status.subsystem_code());
    
    downloading_ = false;


#ifdef DECRYPTSERVICE
    if (service_)
        service_->Finish();
#endif
    
    if (code != EStatusCode::ConvSubtitlesError
        && code != EStatusCode::DecompressSubtitlesError
        && code != EStatusCode::RecodeSubtitlesError
        && code != EStatusCode::WriteMetadataError) {
        clearMediaFile();
        clearTemp({ temps_download_, temps_recode_ });
    }
    else {
        clearTemp({ temps_download_, temps_recode_, subtitle_file_names_ });
    }

    emit error();
}

#ifdef DECRYPTSERVICE
//First activated service will be used for decrypt;
void DecryptController::OnNewService(AbstractDecryptService* service) {
    QMutexLocker locker(&mutex_);
    if (req_decrypt_) {
        if (service_ == nullptr) {
            service_ = service;
            qDebug() << "DecryptController::OnNewService : " << service;
            emit ready();

            if (cancelling_)
                service->Finish();
        }
        else {
            qDebug() << "DecryptController::OnNewService : service already acquired " << service;
        }
    }
}

void DecryptController::OnReleaseService(AbstractDecryptService* service) {
    QMutexLocker locker(&mutex_);
    if (service_ == service) {
        qDebug() << "DecryptController::OnReleaseService: " << service << " cancelling_: " << cancelling_;
        emit offline();
        if (cancelling_)
            emit cancelled();
        service_ = nullptr;
    }
}
#endif

void DecryptController::clearMediaFile() {
    if (QFileInfo::exists(media_file_)) {
        QFile file(media_file_);
        file.remove();
    }
}

void DecryptController::clearTemp(QList<QStringList> tmp_lists) {
    for (const auto& list : tmp_lists) {
        for (const auto& t : list) {
            if (QFileInfo::exists(t)) {
                QFile file(t);
                file.remove();
            }

            qApp->processEvents();
        }
    }
}

QString DecryptController::makeTemp(EOperationType type, const QString& lang) {
    QFileInfo media_fi(media_file_);
    QDir temp_dir = media_fi.dir();
    if (type == EOperationType::kThumbnail)
        temp_dir = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);

    QString temp_file = QString("%1.tmp%2").arg(media_fi.completeBaseName()).arg(QString::number(temps_download_.size() + temps_recode_.size() + 1));

    if (!lang.isEmpty())
        temp_file = QString("%1_%2.aac").arg(media_fi.completeBaseName()).arg(lang);

    if (type == EOperationType::kThumbnail)
        temp_file = QString("%1.tmp").arg("thumb_" + QString::number(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()));

    QFileInfo temp(temp_dir, temp_file);
    QString temp_name = temp.filePath();

    qDebug() << "Temp File " << temp_name;

    switch (type) {
        case kSave:
            break;
        case kThumbnail:
        case kDownload:
            temps_download_.push_back(temp_name);
            break;
        case kRecode:
            temps_recode_.push_back(temp_name);
            break;
        default:
            temps_download_.push_back(temp_name);
            break;
    }

    qApp->processEvents();

    return temp_name;
}

//static 
int DecryptController::calcTotalReceived(const QString& fileName, const long& expected_file_size, const QList<QString>& urls, long& from) {
    long total_received_ = 0;

    if (QFileInfo::exists(fileName)) {
        QFile   file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            total_received_ = file.size();
            //file.resize(0); // truncate to avoid appending
            file.close();
        }
    }

    // calculate which files have already been downloaded and the position of the partially downloaded file
    int i = 0;
    if (total_received_ > 0) {
        for (i = 0; i < urls.size(); i++) {
            auto trackSize = expected_file_size / urls.size(); //auto trackSize = getTrackSize(urls.at(i));
            if (total_received_ - trackSize < 0 
                || trackSize == 0)
                break;
            total_received_ -= trackSize;
        }
    }

    from = total_received_;

    qApp->processEvents();

    return i;
}

bool DecryptController::enableMetadata(const QString& str) {
    bool enable = true;

    for (const auto& chr : str) {
        if (chr.unicode() > 127) {
            enable = false;
            break;
        }
    }

    return enable;
}

void DecryptController::addTags() {
    VideoInfo vi;
    if (tags_info_.GetTagsInfo(media_file_, vi)) {
        std::vector<std::pair<std::shared_ptr<recode::Operation>, float>> ops;

        std::string thumbnail_file;
        if (isVideoHosting(portal_)) {
            if (!vi.metadata_artwork_url.isEmpty()) {
                thumbnail_file = makeTemp(kThumbnail).toStdString();
                auto download_thumbnail_op = std::make_shared<recode::DownloadOp>(vi.metadata_artwork_url.toStdString(), thumbnail_file, utils::HttpFile::Mode::Truncate);
                download_thumbnail_op->set_chunk_size(image_chunk_size_);
                ops.push_back({ download_thumbnail_op , 0.99f });

                auto add_tags_op = std::make_shared<recode::AddTagsOp>();
                add_tags_op->Add(QS2W(media_file_));

                add_tags_op->set_metadata_title(QS2W(vi.metadata_title));
                add_tags_op->set_metadata_artist(QS2W(vi.metadata_artist));
                add_tags_op->set_metadata_album(QS2W(vi.metadata_album));
                add_tags_op->set_metadata_album_artist(QS2W(vi.metadata_album_artist));
                add_tags_op->set_metadata_comment(QS2W(vi.metadata_comment));
                add_tags_op->set_metadata_genre(QS2W(vi.metadata_genre));
                add_tags_op->set_metadata_year(vi.metadata_year.toInt());
                add_tags_op->set_metadata_track(vi.metadata_track.toInt());
                add_tags_op->set_metadata_image_path(thumbnail_file);
                ops.push_back({ add_tags_op , 0.99f });
            }
        }
        else {
            auto add_tags_op = std::make_shared<recode::AddTagsOp>();
            add_tags_op->Add(QS2W(media_file_));

            if (thumbnail_file_.empty()
                && !vi.metadata_artwork_url.isEmpty()) {
                thumbnail_file = makeTemp(kThumbnail).toStdString();
                auto download_thumbnail_op = std::make_shared<recode::DownloadOp>(vi.metadata_artwork_url.toStdString(), thumbnail_file, utils::HttpFile::Mode::Truncate);
                download_thumbnail_op->set_chunk_size(image_chunk_size_);
                ops.push_back({ download_thumbnail_op , 0.99f });

                add_tags_op->set_metadata_image_path(thumbnail_file);
            }
            else if (!video_info_.metadata_artwork_url.isEmpty()) {
                add_tags_op->set_metadata_image_path(thumbnail_file_);
            }

            if (video_info_.metadata_title.isEmpty())
                add_tags_op->set_metadata_title(QS2W(vi.metadata_title));

            if (video_info_.metadata_artist.isEmpty())
                add_tags_op->set_metadata_artist(QS2W(vi.metadata_artist));

            if (video_info_.metadata_album.isEmpty())
                add_tags_op->set_metadata_album(QS2W(vi.metadata_album));

            if (video_info_.metadata_album_artist.isEmpty())
                add_tags_op->set_metadata_album_artist(QS2W(vi.metadata_album_artist));

            if (video_info_.metadata_comment.isEmpty())
                add_tags_op->set_metadata_comment(QS2W(vi.metadata_comment));

            if (video_info_.metadata_genre.isEmpty())
                add_tags_op->set_metadata_genre(QS2W(vi.metadata_genre));

            if (video_info_.metadata_year.isEmpty())
                add_tags_op->set_metadata_year(vi.metadata_year.toInt());

            if (video_info_.metadata_track.isEmpty())
                add_tags_op->set_metadata_track(vi.metadata_track.toInt());

            ops.push_back({ add_tags_op , 0.99f });
        }

        if (ops.empty())
            return;

        exec_ = std::make_unique<recode::Exec>(ops, this);
    }
    else {
        qDebug() << "Failed to get video info";
    }
}
