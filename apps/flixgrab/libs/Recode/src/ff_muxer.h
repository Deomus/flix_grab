#pragma once

#include "status.h"

#include <vector>
#include <string>

//#include <codecs/avc_decoder_configuration_record.h>
#include <media_sample.h>
//#include <video_stream_info.h>
//#include <audio_stream_info.h>

namespace shaka {
    namespace media {
        class StreamInfo;
        //class VideoStreamInfo;
        class MediaSample;
        
    }
}
namespace media {
    class VideoDecoderConfig;
    class AudioDecoderConfig;
    enum VideoCodec;
    enum AudioCodec;
}

//extern "C" {
//    struct AVStream;
//    struct AVFormat;
//}

struct AVStream;
struct AVFormatContext;
struct AVPacket;

namespace recode {

 /*   struct AVStream;
    struct AVFormat;*/

    //http://ffmpeg.org/doxygen/4.0/muxing_8c-example.html
    class FFMuxer {
    public:
        FFMuxer(const std::string& file_name, const std::string& format = std::string());

        Status          AddVideoStream(const media::VideoDecoderConfig* stream, const std::string& lang);
        Status          AddAudioStream(const media::AudioDecoderConfig* stream, const std::string& lang);

        Status          Open();
                
        Status          Write(int stream, const std::shared_ptr<shaka::media::MediaSample>& sample);

        int             GetNextStreamId() { return 0; }

        void            Close();

    private:
        void            Initialize(const std::string& file_name, const std::string& format = std::string());
        void            GuessTimestamp(AVPacket* pkt, AVStream* st);
        void            GuessContainer(media::VideoCodec);
        void            GuessContainer(media::AudioCodec);

    private:
        std::vector<AVStream*>              streams_;
        AVFormatContext*                    format_;
        int64_t                             max_pts_ = 0;
        int64_t                             last_mux_dts = 0;
        int64_t                             last_mux_pts = 0;
        int64_t                             last_mux_dur = 0;
        std::string                         file_name_;
    };
}
