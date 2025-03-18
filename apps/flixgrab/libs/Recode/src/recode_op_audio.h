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

#include "operation.h"

/*
namespace shaka {
    namespace media {
        //class DecryptDemuxer;
        class MediaSample;
        class StreamInfo;
    }
}


namespace recode {
    class FFMuxer;
    //class DecryptDemuxer;

    /// Audio decoder
    class RecodeOpAudio : public Operation {

    public:
#ifdef DECRYPTSERVICE
        RecodeOpAudio(const std::string& url, const std::string& output_file, AbstractDecryptService* service);
#else
        RecodeOpAudio(const std::string& url, const std::string& output_file);
#endif
        virtual ~RecodeOpAudio();

        Status              Do(OperationListener* listener) override;


    public:
        void                set_output_file(const std::string& output_file) { output_file_ = output_file; }
        const std::string&  output_file() const { return output_file_; }

        void                set_average_bitrate(int64_t bitrate) { avg_bitrate_ = bitrate; }
        void                set_duration(long duration) { duration_ = duration; }

    private:
        void                InitDemuxer(const shaka::media::DemuxerAnnex* demuxer);
        bool                NewSample(const std::shared_ptr<shaka::media::MediaSample>& sample);
        void                FinishDemuxer(const shaka::media::DemuxerAnnex* demuxer);
//        bool                EncodeFrame(VideoFrameData* frame_header, bool stop_on_empty = false);

    private:
        Status                                          status_;
#ifdef DECRYPTSERVICE
        AbstractDecryptService*                         service_;
#endif

        bool                                            initialized_;
                
        std::unique_ptr<FFMuxer>                        muxer_;
        //std::map<int64_t, uint64_t>                     sample_duration_map_;
        std::shared_ptr<shaka::media::StreamInfo>       stream_info_;

        std::string                                     output_file_;
        std::string                                     url_;

        //Unmanaged listener;
        OperationListener*                              progress_listener_;
        int64_t                                         progress_value_;

        int dataSize = 0;
    private:
        int64_t                                         avg_bitrate_ = 0;
        uint64_t                                        duration_ = 0;
    };

}
*/