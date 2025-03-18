/*
#include "recode_op_audio.h"

#if NF_DLD == 1 || FG_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || HULU_DLD == 1
#include "decrypsis.h"
#endif

//#include "decrypt_demuxer.h"
//#include "video_stream_info.h"
//#include "decrypt_config.h"
#include "media/base/decrypt_config.h"
#include "media/base/audio_decoder_config.h"

//#include "x264_encoder.h"
#include "ff_muxer.h"
#include <demuxer_annex.h>

//#include "media_stream.h"

using namespace recode;

#if NF_DLD == 1 || FG_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || HULU_DLD == 1
RecodeOpAudio::RecodeOpAudio(const std::string& url, const std::string& output_file, AbstractDecryptService* service)
#else
RecodeOpAudio::RecodeOpAudio(const std::string& url, const std::string& output_file)
#endif
    : initialized_(false)
    , url_(url)
    , output_file_(output_file)
    , muxer_(std::make_unique<FFMuxer>(output_file))
#if NF_DLD == 1 || FG_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || HULU_DLD == 1
    , service_(service) 

{
    std::cout << "RecodeOpAudio() " << output_file << std::endl;

    PxStatus status = InstallDecrypsis();

    if (status != kPx_Success)
        status_.Update({ error::INJECTION_ERROR, "Can't initialize decrypt module", status });
#else
    {
#endif

    //#ifdef NDEBUG
    //LoggerSettings::SetMinLogLevel(LOG_INFO);
    //#endif
}

RecodeOpAudio::~RecodeOpAudio() {
#if NF_DLD == 1 || FG_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || HULU_DLD == 1
    UninstallDecrypsis();
#endif
}

Status RecodeOpAudio::Do(OperationListener* listener) {
    if (initialized_) return error::Code::INVALID_ARGUMENT;

    auto start_clock = std::chrono::steady_clock::now();

    progress_listener_ = listener;
    shaka::media::DemuxerAnnex demuxer(url_,
        std::bind(&RecodeOpAudio::InitDemuxer, this, std::placeholders::_1),
        std::bind(&RecodeOpAudio::NewSample, this, std::placeholders::_1),
        std::bind(&RecodeOpAudio::FinishDemuxer, this, std::placeholders::_1));


    do {
        shaka::media::Status status = demuxer.Initialize();

        if (!status.ok()) {
            status_.Update({ error::SHAKA_ERROR, status.error_message(), status.error_code() });
            break;
        }

        while (status.ok() && !is_cancelled()) {
            status = demuxer.Parse();
        }
        if (!status.ok() && status.error_code() != shaka::media::error::END_OF_STREAM)
            status_.Update({ error::SHAKA_ERROR, status.error_message(), status.error_code() });

    } while (false);

    if (initialized_) {
        initialized_ = false;
    }

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_clock).count();
    std::cout << "Converting duration (secs): " << seconds << std::endl;
    return status_;
}



void RecodeOpAudio::InitDemuxer(const shaka::media::DemuxerAnnex* demuxer) {
    std::cout << " Init DecryptDemuxer " << std::endl;
    
    //std::shared_ptr<shaka::media::StreamInfo> info;

    for (auto stream : demuxer->streams()) {
        if (stream->stream_type() == shaka::media::kStreamAudio) {
            stream_info_ = stream;
        }
    }

    if (!stream_info_)
        return;

    if (status_.Update(muxer_->AddStream(stream_info_)) &&
        status_.Update(muxer_->Open())) 
        initialized_ = status_.ok();    
}

bool RecodeOpAudio::NewSample(const std::shared_ptr<shaka::media::MediaSample>& sample) {
    if (!initialized_
#if NF_DLD == 1 || FG_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || HULU_DLD == 1
        || service_ == nullptr
#endif
        ) 
        return false;

    bool result = true;

    int32_t status = cdm::kSuccess;
    
    //std::cout << " progress: " << sample->dts() << "; " << stream_info_->duration() << std::endl;

    if (status == cdm::kSuccess || status == cdm::kNeedMoreData) {
        if (progress_listener_) {
            int progress = stream_info_->duration() != 0 ? (int)((1000 * sample->dts()) / stream_info_->duration()) :
                duration_ != 0 ? (1000 * sample->dts() / sample->timescale()) / (duration_)
                : 0;
            if (progress != progress_value_) {
                progress_listener_->OnProgress(progress / 1000.0);
                progress_value_ = progress;
            }
        }
        result = status_.Update(muxer_->Write(0, sample)).ok();
        if(!result)
            std::cout << "muxer_->Write error: " << status_.error_message() << std::endl;
    } else {
        std::cout << "Error Invoke DecodeVideo " << status << std::endl;
        result = false;
        status_.Update({ error::CDM_ERROR, "Error Invoke DecodeVideo", status });
    }
    return result;
}

void RecodeOpAudio::FinishDemuxer(const DecryptDemuxer*) {
    std::cout << " Finish Audio Demuxer ..." << std::endl;
    if (!stream_info_)
        return;
    try {
        muxer_->Close();
        std::cout << " Audio Muxer closed." << std::endl;
    }
    catch (...) {
        std::cout << " Failed to close Audio Muxer." << std::endl;
    }
}



*/