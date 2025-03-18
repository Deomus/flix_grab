#include "x264_demuxer.h"
#include "decrypsis.h"

#include <codecs/avc_decoder_configuration_record.h>
#include <media_sample.h>

#include <stream_info.h>
#include <video_stream_info.h>
#include <sys_byteorder.h>

#include "status.h"


using namespace recode;
using namespace shaka;
using namespace media;

X264Demuxer::X264Demuxer(const std::string& file_name) 
    : Demuxer(file_name) {
    PxStatus status = InstallDecrypsis();

    if (status != kPx_Success)
        status_.Update({ error::INJECTION_ERROR, "Can't initialize decrypt module", status });


    //#ifdef NDEBUG
    LoggerSettings::SetMinLogLevel(LOG_INFO);
    //#endif
}

recode::X264Demuxer::~X264Demuxer() {
    UninstallDecrypsis();
}

void X264Demuxer::InitStreams(const std::vector<std::shared_ptr<shaka::media::StreamInfo> >& streams) {

    std::vector<std::shared_ptr<shaka::media::StreamInfo> > new_streams;
    streams_ = streams;
    std::cout << "X264Demuxer::InitStreams size:" << streams_.size() << std::endl;
    //TODO: make all streams
    for (auto& s : streams) {
        if (s->is_encrypted() && s->stream_type() == kStreamVideo) {
            video_info_ = static_cast<VideoStreamInfo*>(s.get());
            auto encoder_stream = encoder_.Init(video_info_);
            new_streams.push_back(encoder_stream);
        } else
            new_streams.push_back(s);
    }

    if (video_info_) {

        std::vector<uint8_t> key_id = video_info_->decryption_key_id();

        if (service_ == nullptr) {
            //TODO: remove
            PxStatus px_status = GetDecryptService(key_id.data(), key_id.size(), &service_, 30000);
            if (px_status != kPx_Success) {

                std::cout << "Error While Getting Decrypt Service for key: " << std::string((const char*)key_id.data(), key_id.size()) << std::endl;
                status_.Update({ recode::error::INJECTION_ERROR, "Not found service for '" + std::string((const char*)key_id.data(), key_id.size()) + "'", px_status });
                return;
            }
        }


        VideoConfig     config;

        config.codec = cdm::VideoDecoderConfig::kCodecH264;

        config.width = video_info_->width();
        config.height = video_info_->height();

        config.format = cdm::kYv12;
        config.profile = cdm::VideoDecoderConfig::kH264ProfileMain;
        config.frame_rate = video_info_->framerate_numerator() / (float)video_info_->framerate_denumerator();

        int32_t status = service_->InitializeVideoDecoder(config);

        if (status != cdm::kSuccess) {
            status_.Update({ error::CDM_ERROR, "InitializeVideoDecoder error", status });
            std::cout << "Error Invoke InitializeVideoDecoder " << status << std::endl;
        }
    }

    Demuxer::InitStreams(new_streams);
}

bool X264Demuxer::NewSample(const std::shared_ptr<shaka::media::MediaSample>& sample) {
    if (service_ == nullptr) return false;

    bool result = true;

    FixSample(sample);

    shaka::media::DecryptConfig*  decrypt_config = sample->decrypt_config();

    EncryptedData       encrypted;
    VideoFrameData      frame;
       
    encrypted.buffer.assign(sample->data(), sample->data() + sample->data_size());
    encrypted.iv = decrypt_config->iv();

    if (encrypted.iv.size() < 16)
        encrypted.iv.resize(16, 0);

    encrypted.key_id = decrypt_config->key_id();

    for (auto sub : decrypt_config->subsamples()) {
        encrypted.subsamples.push_back(cdm::SubsampleEntry(sub.clear_bytes, sub.cipher_bytes));
    }

    encrypted.timestamp = sample->pts();

    sample_map_.insert(std::make_pair(sample->pts(), std::make_pair(sample->duration(), sample->is_key_frame())));

    FrameSource_fn frame_source = std::bind(&X264Demuxer::FrameEncoder, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    int32_t status = service_->DecodeVideo(encrypted, frame_source);

    result = status == cdm::kSuccess || status == cdm::kNeedMoreData;

    if (!result ) {
        std::cout << "X264::Error Invoke DecodeVideo " << status << std::endl;
        status_.Update({ error::CDM_ERROR, "Error Invoke DecodeVideo", status });
    }

    //if (result) {
    //    if (progress_listener_) {
    //        int progress = (int)((1000 * sample->dts()) / video_info_->duration());
    //        if (progress != progress_value_) {
    //            progress_listener_->OnProgress(progress / 1000.0);
    //            progress_value_ = progress;
    //        }
    //    }
    //} else {
    //    std::cout << "Error Invoke DecodeVideo " << status << std::endl;

    //    status_.Update({ error::CDM_ERROR, "Error Invoke DecodeVideo", status });
    //}

    return result;
}

void X264Demuxer::FinishStreams() {

    while (auto sample = encoder_.Encode(nullptr, nullptr, 0)) {
        std::cout << "Delayed Sample " << std::endl;
        if (!Demuxer::NewSample(sample))
            break;
    }

    encoder_.Close();

    std::cout << "Encoder closed! " << std::endl;

    Demuxer::FinishStreams();
}

bool X264Demuxer::FrameEncoder(VideoFrameData* frame_header, const uint8_t* frame_data, size_t frame_size) {
    
    bool     is_key_frame = false;
    auto it = sample_map_.find(frame_header->timestamp);
    assert(it != sample_map_.end());
    if (it != sample_map_.end()) {
        is_key_frame = it->second.second;
        
    }
    //     std::cout << "Frame timestamp: " << frame_header->timestamp << std::endl;
    auto sample = encoder_.Encode(frame_header, frame_data, frame_size/*, is_key_frame*/);
       

    if (sample) {
        uint64_t     duration = 0;
        auto it = sample_map_.find(sample->pts());

        assert(it != sample_map_.end());

        if (it != sample_map_.end()) {
            duration = it->second.first;
            sample_map_.erase(it);
        }
        
        //TODO: remove
        sample->set_duration(duration);
        return Demuxer::NewSample(sample);
    }
    return true;
    
}

void X264Demuxer::FixSample(const std::shared_ptr<shaka::media::MediaSample>& sample) {
    //check sample
    
    if (video_info_ && video_info_->track_id() == sample->track_id()) {

        if (video_info_->codec() == kCodecH264 ||
            video_info_->codec() == kCodecHEV1 ||
            video_info_->codec() == kCodecHVC1) {
            //fix start_code;
            uint8_t* sample_data = sample->writable_data();
            for (auto& entry : sample->decrypt_config()->subsamples()) {
                uint32_t nalu_start_code = 1;
                nalu_start_code = base::ByteSwap(nalu_start_code);

                if (entry.clear_bytes > sizeof(nalu_start_code)) {
                    uint32_t* start_data = (uint32_t*)sample_data;
                    //check start_code of nalu;
                    uint32_t start_code = base::ByteSwap(*start_data);

                    if (start_code != 1) {
                        //Start Code overwritten by NAL length (with header)
                        DCHECK_EQ(entry.clear_bytes - sizeof(nalu_start_code) + entry.cipher_bytes, start_code);
                        *start_data = nalu_start_code;
                    }
                } else {
                    LOG(ERROR) << " Clear Bytes < NALU start_code: " << entry.clear_bytes;
                }
                sample_data += entry.clear_bytes + entry.cipher_bytes;
            }

            if (samples_ == 0) {
                //Add PPS&SPS
                sample->append_extra_data(video_info_->sample_extra_data());
            }
            samples_++;

        }

    }



}
