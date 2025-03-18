#pragma once

#include "demuxer.h"
#include "abstract_service.h"
#include "x264_encoder.h"
#include "status.h"

#include <map>

namespace shaka {
	namespace media {
		class VideoStreamInfo;
		class AVCDecoderConfigurationRecord;

	}
}

namespace recode {



	class X264Demuxer : public shaka::media::Demuxer {
	public:
		explicit X264Demuxer(const std::string& file_name);
		~X264Demuxer();

	private:
		virtual void            InitStreams(const std::vector<std::shared_ptr<shaka::media::StreamInfo> >& streams) override;
		virtual bool            NewSample(const std::shared_ptr<shaka::media::MediaSample>& sample) override;
		virtual void            FinishStreams() override;

        bool                    FrameEncoder(VideoFrameData* frame_header, const uint8_t* frame_data, size_t frame_size);
        void                    FixSample(const std::shared_ptr<shaka::media::MediaSample>& sample);

	private:
        recode::Status                                              status_;
		//Stream index by track_id
		std::vector<int>                                            stream_index_;
		std::vector<AbstractDecryptService*>                        decryptors_;
		std::vector<std::shared_ptr<shaka::media::StreamInfo> >     streams_;

        X264Encoder                                                 encoder_;
        shaka::media::VideoStreamInfo*                              video_info_ = nullptr;
        AbstractDecryptService*                                     service_ = nullptr;

        size_t                                                      samples_ = 0;
        std::map<int64_t, std::pair<uint64_t, bool>>                sample_map_;
        //std::vector<std::shared_ptr<X264Encoder>>                   encoders_;

	};

}

