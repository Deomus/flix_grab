#pragma once

#include <product_config.h>

#ifdef DECRYPTSERVICE
    #include "decrypsis.h"
#endif

#include <map>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

//#include <key_source.h>

#include "operation.h"


namespace shaka {
    namespace media {
        class DemuxerAnnex;
        class MediaSample;
    }
}

struct VideoFrameData;
typedef struct AVFormatContext;

namespace recode {
    class X264Encoder;
    class FFMuxer;

    class RecodeOp : public Operation {

    public:
#ifdef DECRYPTSERVICE
        RecodeOp(const std::string& video_url, const std::string& output_file, AbstractDecryptService* service, const std::string& encoding_preset = "superfast");
#else
        RecodeOp(const std::string& video_url, const std::string& output_file, const std::string& encoding_preset = "superfast");
#endif
        virtual ~RecodeOp();

        Status              Do(OperationListener* listener) override;

        std::vector<uint8_t> GetInitData();

    public:
        void                set_output_file(const std::string& output_file) { output_file_ = output_file; }
        const std::string&  output_file() const { return output_file_; }

        void                set_average_bitrate(int64_t bitrate) { avg_bitrate_ = bitrate; }
        void                set_duration(long duration) { duration_ = duration; }
        void                set_video_width(int width) { width_ = width; }
        void                set_video_height(int height) { height_ = height; }
        void                set_limit_duration(uint64_t limit_duration) { limit_duration_ = limit_duration; }

    private:
        void                InitDemuxer(shaka::media::DemuxerAnnex* demuxer);
        bool                NewSample(shaka::media::DemuxerAnnex* demuxer, const std::shared_ptr<shaka::media::MediaSample>& sample);
        void                FinishDemuxer(shaka::media::DemuxerAnnex* demuxer);
        bool                EncodeFrame(VideoFrameData* frame_header, bool stop_on_empty = false);
        bool                DecodeAndScaleSample(const std::shared_ptr<shaka::media::MediaSample>& sample);

    private:
        Status                                          status_;
#ifdef DECRYPTSERVICE
        AbstractDecryptService*                         service_;
#endif

        bool                                            initialized_;

        std::unique_ptr<X264Encoder>                    encoder_;
        std::unique_ptr<FFMuxer>                        muxer_;
        std::map<int64_t, uint64_t>                     sample_duration_map_;

        std::string                                     output_file_;
        std::string                                     video_url_;
        std::string                                     encoding_preset_;

        //Unmanaged listener;
        OperationListener*                              progress_listener_;
        int64_t                                         progress_value_;

        int dataSize = 0;
    private:
        int64_t                                         avg_bitrate_ = 0;
        uint64_t                                        duration_ = 0;
        uint64_t                                        curr_ts_ = 0; // seconds. Current timestamp
        uint64_t                                        limit_duration_ = 0; // seconds. If set then cut video and audio
        int                                             height_ = 0;
        int                                             width_ = 0;

        int                                             input_sample_width_ = 0; // width of first sample

        // for video decoding:
        AVFormatContext*                                pFormatCtx_ = nullptr;
    };
}
