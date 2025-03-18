#include "recode_op.h"

#ifdef DECRYPTSERVICE
    #include "decrypsis.h"
#endif

#include "demuxer_annex.h"
//#include "video_stream_info.h"
#include "media/base/decrypt_config.h"
#include "media/base/video_decoder_config.h"
#include "media/base/audio_decoder_config.h"

#include "x264_encoder.h"
#include "ff_muxer.h"

#include "avimporter.h"

#include <windows.h>
#include <processthreadsapi.h>

using namespace recode;

#ifdef DECRYPTSERVICE
RecodeOp::RecodeOp(const std::string& video_url, const std::string& output_file, AbstractDecryptService* service, const std::string& encoding_preset)
#else
RecodeOp::RecodeOp(const std::string& video_url, const std::string& output_file, const std::string& encoding_preset)
#endif
    : initialized_(false)
    , video_url_(video_url)
    , output_file_(output_file)
    , encoding_preset_(encoding_preset)
    , encoder_(std::make_unique<X264Encoder>())
    , muxer_(std::make_unique<FFMuxer>(output_file, "mp4"))
#ifdef DECRYPTSERVICE
    , service_(service) 

{
    std::cout << "RecodeOp() " << output_file << std::endl;

    PxStatus status = InstallDecrypsis();

    if (status != kPx_Success)
        status_.Update({ error::INJECTION_ERROR, "Can't initialize decrypt module", status });
#else
    {
#endif

    //#ifdef NDEBUG
    LoggerSettings::SetMinLogLevel(LOG_WARNING);
    //#endif
}

RecodeOp::~RecodeOp() {
#ifdef DECRYPTSERVICE
    UninstallDecrypsis();
#endif
    if (pFormatCtx_)
        avformat_close_input(&pFormatCtx_);
}

std::vector<uint8_t> RecodeOp::GetInitData() {
    shaka::media::DemuxerAnnex demuxer(video_url_, nullptr, nullptr, nullptr);
    auto status = demuxer.Initialize();
    return demuxer.GetInitData();
}

Status RecodeOp::Do(OperationListener* listener) {
    if (initialized_) return error::Code::INVALID_ARGUMENT;
    
    // just for debug
    //auto hr = SetThreadDescription(GetCurrentThread(), L"RecodeOp_Do");

    auto start_clock = std::chrono::steady_clock::now();

    progress_listener_ = listener;
    shaka::media::DemuxerAnnex demuxer(video_url_,
                                       std::bind(&RecodeOp::InitDemuxer, this, std::placeholders::_1),
                                       std::bind(&RecodeOp::NewSample, this, std::placeholders::_1, std::placeholders::_2),
                                       std::bind(&RecodeOp::FinishDemuxer, this, std::placeholders::_1));


    do {
        shaka::media::Status status = demuxer.Initialize();

        if (!status.ok()) {
            status_.Update({ error::SHAKA_ERROR, status.error_message(), status.error_code() });
            break;
        }

        while (status.ok() && !is_cancelled()) {
            status = demuxer.Parse();
            if (0 < limit_duration_ && limit_duration_ < curr_ts_) {
                std::cout << "no freebies after " << curr_ts_ << std::endl;
                status.SetError(shaka::media::error::END_OF_STREAM, "");
                FinishDemuxer(&demuxer); 
                break;
            }
#ifdef _DEBUG
            std::cout << " curr_ts: " << curr_ts_ << ", limit: " << limit_duration_ << std::endl;
#endif // DEBUG
        }
        if (!status.ok() && status.error_code() != shaka::media::error::END_OF_STREAM)
            status_.Update({ error::SHAKA_ERROR, status.error_message(), status.error_code() });

    } while (false);

    if (initialized_) {
        encoder_->Close();
        initialized_ = false;
    }

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_clock).count();
    std::cout << "Converting duration (secs): " << seconds << std::endl;
    return status_.error_code() == error::MUXER_WRITE_ERROR ? Status(): status_;
}

void RecodeOp::InitDemuxer(shaka::media::DemuxerAnnex* demuxer) {
    //std::cout << " Init Demuxer " << std::endl;
    using namespace media;
    
    const auto& tracks = demuxer->tracks();

    for (const auto& t : tracks.tracks()) {
        const auto& track_id = t->bytestream_track_id();
        const auto& lang = t->language().value();
        
        if (t->type() == MediaTrack::Video) {
            const auto& track_info = tracks.getVideoConfig(track_id);
            
            const auto new_size = gfx::Size(width_, height_);    
            const auto new_rect = gfx::Rect(width_, height_);
            const media::VideoDecoderConfig& video_info = 
                width_ > 0 && height_ > 0 ? // video will be scaled 
                media::VideoDecoderConfig(track_info.codec(), track_info.profile(), track_info.alpha_mode(), track_info.color_space_info(), 
                    track_info.video_transformation(),
                    new_size, new_rect, new_size, // new video dimensions 
                    track_info.extra_data(), track_info.encryption_scheme())
                : track_info;
            
            input_sample_width_ = demuxer->GetSampleWidth(0);

            if (video_info.is_encrypted()) {
                VideoConfig     config;
                config.codec = cdm::VideoCodec::kCodecH264;
                config.width = video_info.natural_size().width();
                config.height = video_info.natural_size().height();
                std::cout << "decryptor video width: " << config.width << ", height: " << config.height << std::endl;

                config.format = cdm::kYv12;
                config.profile = cdm::VideoCodecProfile::kH264ProfileMain;
                //todo: config.frame_rate = video_info->framerate_numerator() / (float)video_info->framerate_denumerator();

                if (status_.Update(encoder_->Init(
                    video_info, 
                    demuxer->GetSampleWidth(0),
                    demuxer->GetSampleHeight(0),
                    encoding_preset_))
                    && status_.Update(muxer_->AddVideoStream(encoder_->stream().get(), lang))
                    && status_.Update(muxer_->Open())) {

                    initialized_ = status_.ok();
#ifdef DECRYPTSERVICE
                    int32_t service_status = service_->InitializeVideoDecoder(config);

                    if (service_status == cdm::kSuccess) {
                        progress_value_ = 0;
                    }
                    else {
                        status_.Update({ error::CDM_ERROR, "InitializeVideoDecoder error", service_status });
                        std::cout << "Error Invoke InitializeVideoDecoder " << service_status << std::endl;
                    }
#else
                    progress_value_ = 0;
#endif
                }
            }
        }
        else if (t->type() == MediaTrack::Audio) {
            const auto& audio_info = tracks.getAudioConfig(track_id);

            if (status_.Update(muxer_->AddAudioStream(&audio_info, lang))
                && status_.Update(muxer_->Open())) {

                //int32_t service_status = service_->InitializeAudioDecoder((int)audio_info.codec(), audio_info.channels(), audio_info.bits_per_channel(), audio_info.samples_per_second(), audio_info.extra_data());
                //if (service_status == cdm::kSuccess) {
                progress_value_ = 0;
                initialized_ = status_.ok();
                //}
                //else {
                //    status_.Update({ error::CDM_ERROR, "InitializeAudioDecoder error", service_status });
                //    std::cout << "Error Invoke InitializeAudioDecoder " << service_status << std::endl;
                //}
            }
            else {
                std::cout << "Error Initialize muxer " << status_ << std::endl;
            }
        }
    }


}

bool RecodeOp::NewSample(shaka::media::DemuxerAnnex* demuxer, const std::shared_ptr<shaka::media::MediaSample>& sample) {
    if (!initialized_
#ifdef DECRYPTSERVICE
        || service_ == nullptr
#endif
        ) {
        return false;
    }

    //detect tracktype. Indeed only one track expected here
    media::MediaTrack::Type ttype = media::MediaTrack::Type::Video;
    for (const auto& t : demuxer->tracks().tracks())
        if (sample->track_id() == t->bytestream_track_id())
            ttype = t->type();

    bool result = true;

    auto  decrypt_config = sample->decrypt_config();

    if (!sample->is_encrypted() || decrypt_config == NULL) {
        
        // I think no need to scale if frame size is very close to input parameter
        if (abs(input_sample_width_ - encoder_->VideoWidth()) > 2)
        {
            return DecodeAndScaleSample(sample);
        }
        
        status_ = muxer_->Write(0, sample);
        return status_.ok();
    }

    EncryptedData       encrypted;
    VideoFrameData      frame;

    encrypted.buffer.assign(sample->data(), sample->data() + sample->data_size());
    encrypted.iv = std::vector<uint8_t>(decrypt_config->iv().begin(), decrypt_config->iv().end());

    if (encrypted.iv.size() < 16)
        encrypted.iv.resize(16, 0);

    encrypted.key_id = std::vector<uint8_t>(decrypt_config->key_id().begin(), decrypt_config->key_id().end());

    for (auto sub : decrypt_config->subsamples()) {
		cdm::SubsampleEntry s = { sub.clear_bytes, sub.cypher_bytes };
        encrypted.subsamples.push_back(s);
    }
    encrypted.timestamp = sample->pts();

    FrameSource_fn frame_source = std::bind(&RecodeOp::EncodeFrame, this, std::placeholders::_1, false);
#ifdef DECRYPTSERVICE
    int32_t status;
    if (ttype == media::MediaTrack::Type::Video) {
        //Update duration map;
        sample_duration_map_.insert(std::make_pair(sample->pts(), sample->duration()));
        status = service_->DecodeVideo(encrypted, frame_source);
    }
    else { // audio
        if (sample->is_encrypted()) {
            DecryptedData       decrypted;
            status = service_->Decrypt(encrypted, decrypted);
            //std::cout << " Decrypted " << status << ", pts:" << sample->pts() << ", size: " << decrypted.buffer.size() << ", timescale: " << sample->timescale() << std::endl;
            if (status == cdm::kSuccess) {
                sample->set_data(decrypted.buffer.data(), decrypted.buffer.size());
                sample->set_is_encrypted(false);
                result = status_.Update(muxer_->Write(0, sample)).ok();
            }
        }
        else {
            status = cdm::kSuccess;
        }
    }
#else
    int32_t status = cdm::kSuccess;
#endif

    if (status == cdm::kSuccess || status == cdm::kNeedMoreData) {
        if (progress_listener_) {
            int progress = 0;
            int curr_ts = (int)(1000 * sample->dts() / sample->timescale());
            if(duration_ != 0) 
                progress = curr_ts / duration_;
            else if(demuxer->GetDuration() != 0)
                progress = curr_ts / demuxer->GetDuration();

            if (progress != progress_value_) {
                progress_listener_->OnProgress(progress / 1000.0);
                progress_value_ = progress;
            }
        }
        result = true;
        curr_ts_ = sample->dts() / sample->timescale();
    } else {
        std::cout << "RecodeOp::Error Invoke DecodeVideo " << status << ". Status: " << status_.error_message() << std::endl;
        result = status_.error_code() == error::MUXER_WRITE_ERROR? true: false;
        if(status_.ok())
            status_.Update({ error::CDM_ERROR, "Error Invoke DecodeVideo", status });
    }
    return result;
}

void RecodeOp::FinishDemuxer(shaka::media::DemuxerAnnex* demuxer) {
    std::cout << " Finish Demuxer " << std::endl;
    EncryptedData       encrypted;
    VideoFrameData      frame;

    encrypted.timestamp = 0;

    for (const auto& t : demuxer->tracks().tracks()) {
        if (t->type() == ::media::MediaTrack::Type::Video) {
            FrameSource_fn frame_source = std::bind(&RecodeOp::EncodeFrame, this, std::placeholders::_1, false);
    int32_t status = cdm::kSuccess;
#ifdef DECRYPTSERVICE
    //Decode all samples in decoder;
            do {
                status = service_->DecodeVideo(encrypted, frame_source);
            } while (status == cdm::kSuccess);
#endif
        //Get all other samples;
        while (EncodeFrame(nullptr, true));
        }
    }
    
    std::cout << " Close Encoder " << std::endl;
    encoder_->Close();
    muxer_->Close();
}

bool RecodeOp::DecodeAndScaleSample(const std::shared_ptr<shaka::media::MediaSample> &sample) {
    recode::Status st;

#pragma warning(disable : 4996)
    if (pFormatCtx_ == nullptr) {
        // init context
        auto res = avformat_open_input(&pFormatCtx_, video_url_.c_str(), NULL, NULL);
        if (res >= 0) {

            res = avformat_find_stream_info(pFormatCtx_, NULL);
            auto stream = av_find_best_stream(pFormatCtx_, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
#if _DEBUG                
            av_dump_format(pFormatCtx_, 0, video_url_.c_str(), false);
#endif

            auto video_dec = avcodec_find_decoder(pFormatCtx_->streams[0]->codec->codec_id);
            res = avcodec_open2(pFormatCtx_->streams[0]->codec, video_dec, NULL);
        }
    }

    if (pFormatCtx_ != nullptr) {
        std::shared_ptr<shaka::media::MediaSample> outSample;
        st.Update(encoder_->Recode(sample, pFormatCtx_, outSample));
        if (outSample && st.ok())
            st = muxer_->Write(0, outSample);
    }
    else // something goes wrong, write original sample
        st = muxer_->Write(0, sample);

    return true;
}

bool RecodeOp::EncodeFrame(VideoFrameData* frame, bool stop_on_empty) {
    
    //     std::cout << "Frame timestamp: " << frame_header->timestamp << std::endl;
    std::shared_ptr<shaka::media::MediaSample> sample;
    status_.Update( encoder_->Encode(frame, sample/*, is_key_frame*/));
    
    if (sample) {
        uint64_t     duration = 0;
        auto it = sample_duration_map_.find(sample->pts());
        if (it != sample_duration_map_.end()) {
            duration = it->second;
            sample_duration_map_.erase(it);
        }
        sample->set_duration(duration);
		auto size = sample->data_size();
		dataSize += size;
		//std::cout << " Encode size: +" << size << " = " << dataSize << std::endl;
        status_ = muxer_->Write(0, sample);
    }
        
    return status_.ok() && (!stop_on_empty || sample);
}



