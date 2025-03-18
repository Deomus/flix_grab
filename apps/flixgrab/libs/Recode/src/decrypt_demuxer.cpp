/*
#include "decrypt_demuxer.h"

#include <common_config.h>
#include <event/progress_listener.h>

#include "video_stream_info.h"
#include "decrypt_config.h"
#include "media_sample.h"


#if NF_DLD == 1 || FG_DLD == 1 || DP_DLD == 1 || AP_DLD == 1 || HULU_DLD == 1
#include "decrypsis.h"
#endif

#define         DEFAULT_TIMEOUT         (20000)

using namespace shaka::media;
using namespace recode;

DecryptDemuxer::DecryptDemuxer(const std::string& file_name, demuxer_init_fn init_callback, demuxer_sample_fn new_sample_callback, demuxer_finish_fn finish_callback) :
    Demuxer(file_name),
    new_sample_callback_(new_sample_callback), init_callback_(init_callback), finish_callback_(finish_callback)
{

}

DecryptDemuxer::~DecryptDemuxer()
{

}

void DecryptDemuxer::InitStreams(const std::vector<std::shared_ptr<shaka::media::StreamInfo> >& streams)
{
    //std::cout << " Init Demuxer " << std::endl;
        
    StreamInfo* stream_info = NULL;

    decryptors_.resize(streams.size(), NULL);
    for (uint32_t i = 0; i < streams.size(); ++i) {

        if (stream_index_.size() <= streams[i]->track_id())
            stream_index_.resize(streams[i]->track_id()+1, -1);

        stream_index_[streams[i]->track_id()] = i;

        if (streams[i]->is_encrypted()) {

            std::vector<uint8_t> key_id = streams[i]->decryption_key_id();
            if (kPx_Success == GetDecryptService(key_id.data(), key_id.size(), &decryptors_[i], DEFAULT_TIMEOUT)) {
                //TODO: Check Type Of Decryption Key;
                std::cout << "Got Decrypt Service for demuxer track_id=" << streams[i]->track_id() << " " << streams[i]->codec_string() << " key: " << std::string((const char*)key_id.data(), key_id.size()) << std::endl;
            }
            else
            {
                std::cout << "Error While Getting Decrypt Service for demuxer track_id=" << streams[i]->track_id() << " " << streams[i]->codec_string() << " key: " << std::string((const char*)key_id.data(), key_id.size()) << std::endl;
            }
        }
    }

    streams_ = streams;
    if (init_callback_)
        init_callback_(this);
}

bool DecryptDemuxer::NewSample(const std::shared_ptr<shaka::media::MediaSample>& sample)
{
    //  std::cout << " NewSample Demuxer " << std::endl;

    //Sleep(1000);
   
    bool result = true;

    if (sample->is_encrypted()) {

        shaka::media::DecryptConfig*  decrypt_config = sample->decrypt_config();

        EncryptedData       encrypted;
        DecryptedData       decrypted;
        VideoFrameData      frame;

        int stream_index = stream_index_[sample->track_id()];
        StreamInfo* stream_info = streams_[stream_index].get();
        
        encrypted.buffer.assign(sample->data(), sample->data() + sample->data_size());
        encrypted.iv = decrypt_config->iv();

        if (encrypted.iv.size() < 16)
            encrypted.iv.resize(16, 0);

        encrypted.key_id = decrypt_config->key_id();

        for (auto sub : decrypt_config->subsamples()) {
            cdm::SubsampleEntry s = { sub.clear_bytes, sub.cipher_bytes};
            encrypted.subsamples.push_back(s);
        }

        encrypted.timestamp = sample->pts();

        if ((decryptors_.size() > stream_index) && decryptors_[stream_index]) {
            int32_t status = decryptors_[stream_index]->Decrypt(encrypted, decrypted);
            if (status == cdm::kSuccess) {
                sample->set_data(decrypted.buffer.data(), decrypted.buffer.size());
                sample->set_is_encrypted(false);
            }
            else{
                std::cout << "Error Invoke Decrypt " << status << std::endl;
                result = false;
            }
        }
        else {
            result = false;
        }
    }
       
    if (new_sample_callback_)
        new_sample_callback_(sample);

    //return false;
    return result;
}

void DecryptDemuxer::FinishStreams()
{
    if (finish_callback_)
        finish_callback_(this);
}
//
//std::vector<std::vector<uint8_t>> ParsePSSHKeys(const std::vector<uint8_t>& data) {
//    shaka::media::KeyIdList key_list;
//    GetKeyIdsForCommonSystemId(data, &key_list);
//    return key_list;
//}
*/