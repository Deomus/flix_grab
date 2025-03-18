#pragma once

#include "status.h"
#include <vector>
#include <memory>

struct x264_t;
struct x264_param_t;
struct x264_nal_t;
struct x264_picture_t;
struct SwsContext;
struct AVFrame;

namespace shaka {
    namespace media {
        class MediaSample;
    }
}
namespace media {
    class VideoDecoderConfig;
    namespace mp4 {
        struct AVCDecoderConfigurationRecord;
    }
}

struct VideoFrameData;
struct AVFormatContext;

namespace recode {
    
    class X264Encoder {
    public:
        explicit X264Encoder();
        ~X264Encoder();

    public:
        Status      Init(const media::VideoDecoderConfig& video_stream, int sample_width, int sample_height, const std::string& preset_name = "superfast");

        Status      Encode(const VideoFrameData* frame, std::shared_ptr<shaka::media::MediaSample>& sample, bool force_keyframe = false);
        
        Status      Recode(const std::shared_ptr<shaka::media::MediaSample>& orig_sample, AVFormatContext* ctx, std::shared_ptr<shaka::media::MediaSample>& out_sample);

        int         VideoWidth() const;

        std::shared_ptr<media::VideoDecoderConfig> stream() { return video_stream_; };
        
        void        Close();

    private:
        void        UpdateConfig(const media::mp4::AVCDecoderConfigurationRecord* config);
        void        InitExtradata();
        void        PackNals(const x264_nal_t *nals, int nnal);
        Status      EncodePicture(x264_picture_t* pic_in, std::shared_ptr<shaka::media::MediaSample>& sample);
        void        ScalePicture(x264_picture_t* pic_in, int srcW, int srcH, int dstW, int dstH);
        Status      Decode(const std::shared_ptr<shaka::media::MediaSample>& sample, AVFrame* pOutFrame, AVFormatContext* ctx, int* gotFrame);

    private:
        //Stream index by track_id
        std::shared_ptr<media::VideoDecoderConfig>      video_stream_;

    private:
        //x264 data;
        x264_t *                                            enc_ = nullptr;

        std::unique_ptr<x264_param_t>                       params_;
        
        SwsContext*                                     resize_ctx_ = nullptr;  // cached context
        int                                             sws_frame_width_ = 0;   // cached width value
        AVFrame*                                        sws_frame = nullptr;    // cached memory buffer for frame


        std::vector<uint8_t>                            sei_;
        std::vector<uint8_t>                            extradata_;

        std::vector<uint8_t>                            frame_nals_;

    };

}


