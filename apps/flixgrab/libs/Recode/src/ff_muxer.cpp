#include "ff_muxer.h"

#include <media/base/video_codecs.h>
#include <media/base/audio_codecs.h>
#include <media/base/video_decoder_config.h>
#include <media/base/audio_decoder_config.h>

using namespace recode;
using namespace shaka::media;

extern "C" {
    //#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>

}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")

inline AVCodecID GetCodecId(media::VideoCodec c) {
    switch (c) {
    case kCodecH264:
        return AV_CODEC_ID_H264;
        break;
    case kCodecHEVC:
        //case kCodecHVC:
        return AV_CODEC_ID_HEVC;
        break;
    case kCodecVC1:
        return AV_CODEC_ID_VC1;
        break;
    case kCodecMPEG2:
        return AV_CODEC_ID_MPEG2VIDEO;
        break;

    case kCodecMPEG4:
        return AV_CODEC_ID_MPEG4;
        break;
    case kCodecVP8:
        return AV_CODEC_ID_VP8;
    case kCodecVP9:
        return AV_CODEC_ID_VP9;
    default:
        return AV_CODEC_ID_NONE;
    }
}

inline AVCodecID GetCodecId(AudioCodec c) {
    switch (c) {
    case kCodecAAC:
        return AV_CODEC_ID_AAC;
        break;

    case kCodecAC3:
        return AV_CODEC_ID_AC3;
        break;
    case kCodecEAC3:
        return AV_CODEC_ID_EAC3;
        break;
    case kCodecOpus:
        return AV_CODEC_ID_OPUS;
        break;

    case kCodecVorbis:
        return AV_CODEC_ID_VORBIS;
        break;

    default:
        return AV_CODEC_ID_NONE;
    }
}

inline std::string av_error_str(int err) {
    char str[AV_ERROR_MAX_STRING_SIZE] = { 0 };
    av_strerror(err, str, AV_ERROR_MAX_STRING_SIZE);

    return str;
}

inline std::string hexStr(const unsigned char* data, int len) {
    static constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    std::string s(len * 2, ' ');

    for (int i = 0; i < len; ++i) {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }

    return s;
}

// The container for Muxer is detected by the extension of file_name. Or by the format provided in second parameter.
FFMuxer::FFMuxer(const std::string& file_name, const std::string& format) {
    auto r = avformat_alloc_output_context2(&format_, NULL, format.empty() ? NULL : format.c_str(), file_name.c_str());
    file_name_ = file_name;
}


Status FFMuxer::Open() {
    av_dump_format(format_, 0, format_->url, 1);
    int ret = 0;
    /* open the output file, if needed */
    if (!(format_->oformat->flags & AVFMT_NOFILE)) {
        std::cout << "ffmuxer::Open:" << std::string(format_->url) << std::endl;
        ret = avio_open(&format_->pb, format_->url, AVIO_FLAG_WRITE);
        if (ret < 0) {
            return Status(recode::error::FILE_ERROR, "Could not open '" + std::string(format_->url) + "' : " + av_error_str(ret));

        }
    }

    AVDictionary* opt = nullptr;
    /* Write the stream header, if any. */
    ret = avformat_write_header(format_, &opt);
    if (ret < 0) {
        return Status(recode::error::FILE_ERROR, "Error occurred when opening output file: " + av_error_str(ret));
    }

    return Status();
}

Status FFMuxer::Write(int stream_index, const std::shared_ptr<shaka::media::MediaSample>& sample) {

    AVPacket pkt = { 0 };
    AVStream* st = streams_[stream_index];
    AVRational time_base = { 1, (int)sample->timescale() };

    pkt.data = const_cast<uint8_t*>(sample->data());
    pkt.size = sample->data_size();

    //Free sample afterall;
    auto free_buf = [](void* opaque, uint8_t*) {
        delete static_cast<std::shared_ptr<shaka::media::MediaSample>*>(opaque);
    };

    pkt.buf = av_buffer_create(pkt.data, pkt.size + AV_INPUT_BUFFER_PADDING_SIZE,
        free_buf, new std::shared_ptr<shaka::media::MediaSample>(sample), 0);

    if (!pkt.buf)
        return Status(recode::error::NO_MEMORY, "Could not allocate packet");

    //std::cout << "Sample:" << sample->ToString() << std::endl;
    pkt.stream_index = st->index;
    pkt.flags |= sample->is_key_frame() ? AV_PKT_FLAG_KEY : 0;

    pkt.pts = av_rescale_q(sample->pts(), time_base, st->time_base);
    pkt.dts = av_rescale_q(sample->dts(), time_base, st->time_base);

    std::cout << " dts/pts/" << pkt.dts << "/" << pkt.pts;
    if (sample->duration())
        pkt.duration = av_rescale_q(sample->duration(), time_base, st->time_base);
    GuessTimestamp(&pkt, st);
    std::cout << " after dts/pts/dura: " << pkt.dts << "/" << pkt.pts << "/" << pkt.duration << std::endl;
    memcpy(pkt.data, sample->data(), sample->data_size());

    int ret = av_interleaved_write_frame(format_, &pkt);

    av_packet_unref(&pkt);

    if (ret < 0)
        return Status(recode::error::MUXER_WRITE_ERROR, "Could not write packet: " + av_error_str(ret));

    return Status();
}

void FFMuxer::GuessTimestamp(AVPacket* pkt, AVStream* st)
{
    bool stream_copy = false;
    int oformat_flags = format_->oformat->flags;
    if (!(oformat_flags & AVFMT_NOTIMESTAMPS))
    {
        if (pkt->dts != AV_NOPTS_VALUE &&
            pkt->pts != AV_NOPTS_VALUE &&
            pkt->dts > pkt->pts) {
            std::cout << " Invalid DTS:" << pkt->dts << " PTS:" << pkt->pts << " replacing by guess ";
            pkt->pts = last_mux_pts + pkt->duration;  //PS++
            std::cout << " new DTS:" << pkt->dts << " PTS:" << pkt->pts << std::endl;
        }
        if ((st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO || st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO || st->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) &&
            pkt->dts != AV_NOPTS_VALUE &&
            !(st->codecpar->codec_id == AV_CODEC_ID_VP9 && stream_copy) &&
            last_mux_dts != AV_NOPTS_VALUE) {
            int64_t max = last_mux_dts + !(oformat_flags & AVFMT_TS_NONSTRICT);
            if (pkt->dts < max) {
                int loglevel = max - pkt->dts > 2 || st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ? AV_LOG_WARNING : AV_LOG_DEBUG;
                std::cout << "Non-monotonous DTS in output stream " << last_mux_dts << " -> " << pkt->dts << std::endl;
                if (pkt->pts >= pkt->dts)
                    pkt->pts = FFMAX(pkt->pts, max);
                pkt->dts = max;
            }
        }
    }
    last_mux_pts = pkt->pts;
    last_mux_dts = pkt->dts;
}
// static
void FFMuxer::GuessContainer(media::VideoCodec codec)
{
    // Now trying to guess container by stream codec:
    std::string formatStr = "mp4";
    AVOutputFormat* outFmt = NULL;
    if (codec == media::VideoCodec::kCodecVP8 || codec == media::VideoCodec::kCodecVP9) //if (formatStr == "vp9" || formatStr == "vp8")
    {
        outFmt = av_guess_format(formatStr.c_str(), NULL, NULL);
        formatStr = "webm"; // vp9 -> webm container 
    }
    if (codec == media::VideoCodec::kCodecH264)
    {
        formatStr = "mp4"; // h264 -> mp4 container 
    }
    auto r = avformat_alloc_output_context2(&format_, outFmt, formatStr.c_str(), file_name_.c_str());
}
// static
void FFMuxer::GuessContainer(media::AudioCodec codec)
{
    // Now trying to guess container by stream codec:
    std::string formatStr = "mp4";
    AVOutputFormat* outFmt = NULL;

    if (codec == media::AudioCodec::kCodecAAC)
        formatStr = "adts"; // aac -> adts container 
    else if (codec == media::AudioCodec::kCodecVorbis)
        formatStr = "ogg";
    else if (codec == media::AudioCodec::kCodecAC3) //(formatStr == "ec-3")
    {
        outFmt = av_guess_format(formatStr.c_str(), NULL, NULL);
        formatStr = "mp4"; // dolby -> mp4 container 
    }
    auto r = avformat_alloc_output_context2(&format_, outFmt, formatStr.c_str(), file_name_.c_str());
}
// If you did not have set the correct extension for the output file,
//  then the first stream to be added should be a video stream. This will help us choose the right container.
Status FFMuxer::AddVideoStream(const media::VideoDecoderConfig* config, const std::string& lang) {

    if (format_ == NULL)
    {
        GuessContainer(config->codec());
        if (format_ == NULL)
            throw std::runtime_error("Unknown FFMuxer format for output file: " + file_name_ + ". Codec: " + std::to_string(config->codec()));
    }

    AVStream* s = avformat_new_stream(format_, NULL);

    if (!s)
        return Status(recode::error::NO_MEMORY, "Could not allocate stream");
    streams_.push_back(s);

    AVCodecParameters* par = s->codecpar;

    av_dict_set(&s->metadata, "language", lang.c_str(), 0);

    //todo: s->time_base = { 1, (int)stream->time_scale() };

    par->codec_id = GetCodecId(config->codec());
    par->codec_type = AVMEDIA_TYPE_VIDEO;

    par->width = config->natural_size().width();
    par->height = config->natural_size().height();

    par->sample_aspect_ratio.num = par->height * config->GetPixelAspectRatio(); // video->pixel_width();
    par->sample_aspect_ratio.den = par->height; // video->pixel_height();
    //avs->codecpar->profile = video->pro;
    //avs->codecpar->level = codec->level;

    //todo: s->avg_frame_rate = { video->framerate_numerator(), video->framerate_denumerator() };
    std::cout << "Muxer init video: " << par->width << "x" << par->height << std::endl;

    if (config->extra_data().size()) {
        par->extradata = (uint8_t*)av_mallocz(config->extra_data().size() + AV_INPUT_BUFFER_PADDING_SIZE);
        if (!s->codecpar->extradata)
            return Status(recode::error::NO_MEMORY, "Could not allocate stream");

        memcpy(par->extradata, config->extra_data().data(), config->extra_data().size());
        par->extradata_size = config->extra_data().size();
    }
    return Status();
}


// If you did not have set the correct extension for the output file,
//  then the first stream to be added should be a video stream. This will help us choose the right container.
Status FFMuxer::AddAudioStream(const media::AudioDecoderConfig* config, const std::string& lang) {

    if (format_ == NULL)
    {
        GuessContainer(config->codec());
        if (format_ == NULL)
            throw std::runtime_error("Unknown FFMuxer format for output file: " + file_name_ + ". Codec: " + std::to_string(config->codec()));
    }

    AVStream* s = avformat_new_stream(format_, NULL);

    if (!s)
        return Status(recode::error::NO_MEMORY, "Could not allocate stream");
    streams_.push_back(s);

    AVCodecParameters* par = s->codecpar;

    av_dict_set(&s->metadata, "language", lang.c_str(), 0);

    //todo: s->time_base = { 1, (int)stream->time_scale() };

    par->codec_id = GetCodecId(config->codec());
    par->codec_type = AVMEDIA_TYPE_AUDIO;

    par->sample_rate = config->samples_per_second(); // audio->sampling_frequency();
    //par->bits_per_coded_sample = 8 * config->bytes_per_frame(); //todo: audio->sample_bits();
    par->frame_size = 0; // ? variable count of samples per frame

    par->channels = config->channels();
    par->seek_preroll = config->seek_preroll().InNanoseconds() * config->samples_per_second() / 1000000000ull; // int(audio->seek_preroll_ns() * audio->sampling_frequency() / 1000000000ull);
    //par->bit_rate = config->samples_per_second() * config->bits_per_channel(); //audio->avg_bitrate();

    if (config->extra_data().size()) {
        par->extradata = (uint8_t*)av_mallocz(config->extra_data().size() + AV_INPUT_BUFFER_PADDING_SIZE);
        if (!s->codecpar->extradata)
            return Status(recode::error::NO_MEMORY, "Could not allocate config");

        memcpy(par->extradata, config->extra_data().data(), config->extra_data().size());
        par->extradata_size = config->extra_data().size();

    }

    return Status();
}

void FFMuxer::Close() {

    /* Write the trailer, if any. The trailer must be written before you
    * close the CodecContexts open when you wrote the header; otherwise
    * av_write_trailer() may try to use memory that was freed on
    * av_codec_close(). */
    std::cout << "ffmuxer::close" << std::endl;
    if (format_ == NULL)
        return;
    av_write_trailer(format_);

    if (!(format_->oformat->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_closep(&format_->pb);

    /* free the stream */
    avformat_free_context(format_);

}
