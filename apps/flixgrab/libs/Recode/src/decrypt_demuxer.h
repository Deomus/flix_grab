#pragma once

#include <common_config.h>
#include "demuxer.h"
#include "abstract_service.h"

namespace recode {
    class DecryptDemuxer : public shaka::media::Demuxer {
    public:
        typedef std::function<void(const DecryptDemuxer*)> demuxer_init_fn;
        typedef std::function<bool(const std::shared_ptr<shaka::media::MediaSample>& sample)> demuxer_sample_fn;
        typedef std::function<void(const DecryptDemuxer*)> demuxer_finish_fn;

        explicit DecryptDemuxer(const std::string& file_name, demuxer_init_fn init_callback, demuxer_sample_fn new_sample_callback, demuxer_finish_fn finish_callback);
        ~DecryptDemuxer();

        const std::vector<std::shared_ptr<shaka::media::StreamInfo> >& streams() const { return streams_; }

    private:
        virtual void            InitStreams(const std::vector<std::shared_ptr<shaka::media::StreamInfo> >& streams);
        virtual bool            NewSample(const std::shared_ptr<shaka::media::MediaSample>& sample);
        virtual void            FinishStreams();

    private:
        //Stream index by track_id
        std::vector<int>                                            stream_index_;
        std::vector<AbstractDecryptService*>                        decryptors_;
        std::vector<std::shared_ptr<shaka::media::StreamInfo> >     streams_;

        demuxer_init_fn                                             init_callback_;
        demuxer_sample_fn                                           new_sample_callback_;
        demuxer_finish_fn                                           finish_callback_;
    };


//    //TODO: remove
//#include "cenc_utils.h"
//
//    std::vector<std::vector<uint8_t>> ParsePSSHKeys(const std::vector<uint8_t>& data);
}
