#pragma once

#include "status.h"

namespace shaka {
    namespace media {
        class StreamInfo;
        class MediaSample;
        
    }
}

namespace recode {

    //http://ffmpeg.org/doxygen/4.0/muxing_8c-example.html
    class FfMuxer {
    public:

        Status          Open(const char* file_path);

        Status          AddStream(std::shared_ptr<shaka::media::StreamInfo> stream);
        Status          Write(const std::shared_ptr<shaka::media::MediaSample>& sample);

        Status          Close();

    private:


    };
}
