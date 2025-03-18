#include "x264_encoder.h"
#include <x264.h>

#include <product_config.h>

//#include <codecs/avc_decoder_configuration_record.h>
#include <media_sample.h>
//#include <video_stream_info.h>
//#include <buffer_writer.h>
#include <media/base/video_decoder_config.h>
#include <media/formats/mp4/box_definitions.h>
#include <media/video/h264_parser.h>

#ifdef DECRYPTSERVICE
    #include "decrypt_protocol.h"

#else
namespace cdm {
    namespace VideoFrame {
        enum VideoPlane {
            kYPlane = 0,
            kUPlane = 1,
            kVPlane = 2,
            kMaxPlanes = 3,
        };
    }
    enum VideoFormat {
        kUnknownVideoFormat = 0,  // Unknown format value. Used for error reporting.
        kYv12,  // 12bpp YVU planar 1x1 Y, 2x2 VU samples.
        kI420  // 12bpp YVU planar 1x1 Y, 2x2 UV samples.
    };
}

struct VideoFrameData
{
    //InvokeStatus                                    status;
    // Presentation timestamp in microseconds.
    int64_t                                         timestamp;

    cdm::VideoFormat                                format;

    // Width and height of video frame immediately post-decode. Not all pixels
    // in this region are valid.
    uint32_t                                        width;
    uint32_t                                        height;

    uint32_t                                        plane_offset[cdm::VideoFrame::kMaxPlanes];
    uint32_t                                        plane_stride[cdm::VideoFrame::kMaxPlanes];

    uint8_t*                                        data;
    size_t                                          data_size;
};
#endif

// ffmpeg lib:
extern "C" {
#include <libavutil/frame.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")

//DLL Module
#pragma comment(lib, "libx264.lib")

using namespace recode;
using namespace shaka::media;

#define EXTRA_BUFFER_PADDING_SIZE   32

X264Encoder::X264Encoder() {
#if _DEBUG
    LoggerSettings::SetMinLogLevel(LOG_DEBUG);
#endif
}


X264Encoder::~X264Encoder() {

}

static std::string GetX264Profile(int profile_indicator) {
    std::string profile = "main";

    switch (profile_indicator) {
    case media::H264SPS::kProfileIDCBaseline:
        profile = "baseline";
        break;
    case media::H264SPS::kProfileIDCMain:
        profile = "main";
        break;
    case media::H264SPS::kProfileIDCHigh:
    case media::H264SPS::kProfileIDScalableHigh:
        profile = "high";
        break;
    default:
        break;
    }
    return profile;
}
void logger(void* x, int i_level, const char* psz, va_list args)
{
    char logger_buf_[4000] = {0};
    //if (logger_buf_ == NULL)
    //    logger_buf_ = new char[4000];
    _vsnprintf_s(logger_buf_, 4000, _TRUNCATE, psz, args);
    std::cout << "X264Encoder: " << i_level << " " << logger_buf_ << std::endl;
}

recode::Status X264Encoder::Init(const VideoDecoderConfig& config, int sample_width, int sample_height, const std::string& preset_name) {
    Status status;
    params_ = std::make_unique<x264_param_t>();
    int ret;
    x264_param_default(&*params_);
    //{ "ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow", "placebo", 0 }
    if ((ret = x264_param_default_preset(&*params_, preset_name.c_str(), "film")) < 0) {
        status.Update({ error::ENCODER_ERROR, "Encoder preset error", -12 });
        return status;
    }
    //params_->rc.i_rc_method = X264_RC_CRF;
    params_->i_csp = X264_CSP_I420;
    params_->i_bitdepth = 8;

    params_->b_annexb = 1;

    params_->i_width = std::min(sample_width, config.natural_size().width());
    params_->i_height = std::min(sample_height, config.natural_size().height());
    if (params_->i_height % 2) // odd
        params_->i_height--; // to avoid "height not divisible by 2"

    params_->vui.i_sar_width = 0; // config.pixel_width();
    params_->vui.i_sar_height = 0; // config.pixel_height();

    params_->i_timebase_num = 1;
    params_->i_timebase_den = 1000000; // microseconds
    //uncomment to intercept messages:
    params_->pf_log = logger;

    if (config.codec() == VideoCodec::kCodecH264 && config.extra_data().size() > 0) {
        media::mp4::AVCDecoderConfigurationRecord avc_config;
        if (avc_config.Parse(config.extra_data().data(), config.extra_data().size())) {
            //Update params;
            UpdateConfig(&avc_config);
        }
    }
   /* else if (x264_param_apply_profile(&*params_, "main") < 0) {
        return video_stream_;
    }*/

    std::cout << "Init encoder width:" << params_->i_width << ", natural_width=" << config.natural_size().width() << ", coded_width=" << config.coded_size().width() << ", sample_width: " << sample_width << ", is_encrypted=" << config.is_encrypted() << std::endl;
    
    enc_ = x264_encoder_open(&*params_);
    if (enc_) {
        InitExtradata();

        video_stream_ = std::make_shared<VideoDecoderConfig>(
            config.codec(), config.profile(), config.alpha_mode(), config.color_space_info(), config.video_transformation(),
            config.coded_size(), config.visible_rect(), config.natural_size(),
            config.extra_data(), config.encryption_scheme());
    } else
        status.Update({error::ENCODER_ERROR, "Can't create encoder", -11});

    return status;
}

void recode::X264Encoder::UpdateConfig(const media::mp4::AVCDecoderConfigurationRecord* config) {

    //TODO: check deblocking_filter_control_present_flag
    params_->b_deblocking_filter = 1;

    //MY PArams
    params_->i_frame_reference = 3;
    params_->i_bframe_pyramid = 0;
    params_->b_open_gop = 0;
    //params_->i_bframe = 3;
    //params_->i_bframe_adaptive = 1;

    //Adaptive bframes;
    //params_->i_bframe = 16;
    //params_->i_bframe_adaptive = 1;

    params_->i_level_idc = config->avc_level;

    //TODO: need vbv buffer;
    //params_->rc.i_vbv_max_bitrate = (config->bitrate() *1.3)/ 1000;
    //params_->rc.i_vbv_buffer_size = (config->bitrate() * 0.75) / 1000;


    //params_->rc.i_vbv_max_bitrate = config->bitrate() / 1000;
    //params_->rc.i_vbv_buffer_size = (config->bitrate()*0.75) / 1000;

    ////////ABR
    /*
    params_->rc.i_rc_method = X264_RC_ABR;

    params_->rc.i_bitrate = int((config->bitrate()*0.6675) / 1000.0);
    //params_->rc.i_bitrate = int((config->bitrate()*0.0075) / 1000.0);
    params_->rc.i_vbv_max_bitrate = int(config->bitrate() / 1000.0);
    params_->rc.i_vbv_buffer_size = int((config->bitrate()*0.6675) / 1000.0);
    */
    ////////CQP
    /*params_->rc.i_rc_method = X264_RC_CQP;
    params_->rc.i_qp_constant = 28;
    params_->rc.i_vbv_max_bitrate = (config->bitrate()) / 1000;
    params_->rc.i_vbv_buffer_size = (config->bitrate() * 0.75) / 1000;*/
    
    ////////CRF
    params_->rc.i_rc_method = X264_RC_CRF;
    params_->rc.f_rf_constant = 21.5;
    params_->rc.f_rf_constant_max = 28;
    params_->rc.i_vbv_max_bitrate = 0; // config->bitrate() / 1000;
    //params_->rc.i_vbv_buffer_size = (config->bitrate()*0.1) / 1000;
    params_->rc.i_vbv_buffer_size = 0; // (config->bitrate()*0.1) / 1000;
    
    //x264_param_apply_fastfirstpass(&params);

    //TODO: check entropy_coding_mode_flag
    //params_->b_cabac = 1;

    auto profile = GetX264Profile(config->profile_indication);

    if (x264_param_apply_profile(&*params_, profile.c_str()) < 0) {
        return;
    }
    /*todo:
    params_->i_width = config->coded_width;
    params_->i_height = config->coded_height();


    params_->vui.i_sar_width = config->pixel_width();
    params_->vui.i_sar_height = config->pixel_height();

    //params.i_timebase_den = avc_config->timescale();
    //params_->i_timebase_num = 1;
    params_->i_fps_num = config->framerate_num();
    params_->i_fps_den = config->framerate_den();

    //params_->i_timebase_num = config->framerate_den();
    //params_->i_timebase_den = config->framerate_num();
    */
}


//void recode::X264Encoder::UpdateConfig(const shaka::media::AVCDecoderConfigurationRecord* config) {
//
//    //TODO: check deblocking_filter_control_present_flag
//    params_->b_deblocking_filter = 1;
//
//    //MY PArams
//    params_->i_frame_reference = 1;
//    params_->i_bframe_pyramid = 0;
//    params_->b_open_gop = 0;
//    params_->i_bframe = 2;
//    params_->i_bframe_adaptive = 0;
//
//    //Adaptive bframes;
//    //params_->i_bframe = 16;
//    //params_->i_bframe_adaptive = 1;
//
//
//
//    params_->i_level_idc = config->avc_level();
//
//    //TODO: need vbv buffer;
//    //params_->rc.i_vbv_max_bitrate = (config->bitrate() *1.3)/ 1000;
//    //params_->rc.i_vbv_buffer_size = (config->bitrate() * 0.75) / 1000;
//    
//    
//    params_->rc.i_vbv_max_bitrate = config->bitrate() / 1000;
//    params_->rc.i_vbv_buffer_size = (config->bitrate()*0.75) / 1000;
//
//    params_->rc.i_rc_method = X264_RC_CRF;
//    params_->rc.f_rf_constant = 21;
//    params_->rc.f_rf_constant_max = 23;
//
//    //x264_param_apply_fastfirstpass(&params);
//
//    //TODO: check entropy_coding_mode_flag
//    params_->b_cabac = 1;
//
//    auto profile = GetX264Profile(config->profile_indication());
//
//    if (x264_param_apply_profile(&*params_, profile.c_str()) < 0) {
//        return;
//    }
//
//
//    params_->i_width = config->coded_width();
//    params_->i_height = config->coded_height();
//
//
//    params_->vui.i_sar_width = config->pixel_width();
//    params_->vui.i_sar_height = config->pixel_height();
//
//    //params.i_timebase_den = avc_config->timescale();
//    //params_->i_timebase_num = 1;
//    params_->i_fps_num = config->framerate_num();
//    params_->i_fps_den = config->framerate_den();
//
//    //params_->i_timebase_num = config->framerate_den();
//    //params_->i_timebase_den = config->framerate_num();
//
//}


//
//bool X264Encoder::InitEncoder(shaka::media::AVCDecoderConfigurationRecord* avc_config) {
//
//    params_ = std::make_unique<x264_param_t>();
//
//    x264_param_default(&*params_);
//
//    //TODO: check deblocking_filter_control_present_flag
//    params_->b_deblocking_filter = 1;
//
//    if (x264_param_default_preset(&*params_, "slow", "film") < 0) {
//        return false;
//    }
//
//    params_->i_level_idc = avc_config->avc_level();
//
//    params_->rc.i_vbv_max_bitrate = avc_config->bitrate() / 1000;
//    params_->rc.i_rc_method = X264_RC_CRF;
//    //TODO: change
//    //params.rc.f_rf_constant = 23;
//    //X264_CSP_I420;
//    params_->i_csp = X264_CSP_YV12;
//    //params.i_bitdepth = 8;
//
//    //x264_param_apply_fastfirstpass(&params);
//    
//    //TODO: check entropy_coding_mode_flag
//    params_->b_cabac = 1;
//
//    auto profile = GetX264Profile(avc_config->profile_indication());
//
//    if (x264_param_apply_profile(&*params_, profile.c_str()) < 0) {
//        return false;
//    }
//
//
//    params_->i_width = avc_config->coded_width();
//    params_->i_height = avc_config->coded_height();
//
//
//    params_->vui.i_sar_width = avc_config->pixel_width();
//    params_->vui.i_sar_height = avc_config->pixel_height();
//
//    //params.i_timebase_den = avc_config->timescale();
//    params_->i_timebase_num = 1;
//    params_->i_fps_num = avc_config->framerate_num();
//    params_->i_fps_den = avc_config->framerate_den();
//
//    enc_ = x264_encoder_open(&*params_);
//    if (!enc_)
//        return false;
//
//    InitExtradata();
//
//    return true;
//}
//
//void X264Encoder::InitExtradata() {
//    x264_nal_t *nal = nullptr;
//    int nnal;
//    int size = x264_encoder_headers(enc_, &nal, &nnal);
//    extradata_.reserve(size + EXTRA_BUFFER_PADDING_SIZE);
//
//    //TODO: Write valid avcc (ff_isom_write_avcc);
//    x264_nal_t     *sps = nullptr, *pps = nullptr;
//
//    for (int i = 0; i < nnal; i++) {
//        /* Don't put the SEI in extradata. */
//        if (nal[i].i_type == NAL_SEI) {
//            sei_.reserve(nal[i].i_payload);
//
//            std::copy(nal[i].p_payload,
//                nal[i].p_payload + nal[i].i_payload,
//                std::back_inserter(sei_));
//            continue;
//        }
//
//        if (nal[i].i_type == NAL_SPS)
//            sps = &nal[i];
//
//        if (nal[i].i_type == NAL_PPS)
//            pps = &nal[i];
//             
//    }
//
//    assert(sps);
//    assert(pps);
//
//    shaka::media::BufferWriter        writer(128);
//
//    int sps_start = sps->b_long_startcode ? 4 : 3;
//    int pps_start = pps->b_long_startcode ? 4 : 3;
//                         
//    writer.AppendNBytes(1, 1); /* version = 1*/
//    writer.AppendNBytes(sps->p_payload[sps_start + 1], 1); /* profile */
//    writer.AppendNBytes(sps->p_payload[sps_start + 2], 1); /* profile compat*/
//    writer.AppendNBytes(sps->p_payload[sps_start + 3], 1); /* level*/
//    writer.AppendNBytes(0xff, 1); /* 6 bits reserved (111111) + 2 bits nal size length - 1 (11) */
//    writer.AppendNBytes(0xe1, 1); /* 3 bits reserved (111) + 5 bits number of sps (00001) */
//
//    writer.AppendNBytes(sps->i_payload - sps_start, 2);
//    writer.AppendArray(sps->p_payload + sps_start, sps->i_payload - sps_start);
//
//    writer.AppendNBytes(1, 1); /* number of pps */
//    writer.AppendNBytes(pps->i_payload - pps_start, 2);
//    writer.AppendArray(pps->p_payload + pps_start, pps->i_payload - pps_start);
//
//    std::copy(writer.Buffer(),
//              writer.Buffer() + writer.Size(),
//              std::back_inserter(extradata_));
//
//    AVCDecoderConfigurationRecord avc_config;
//    if (avc_config.Parse(extradata_)) {
//        LOG(INFO) << "Extradata parsed";
//
//    }
//}



void X264Encoder::InitExtradata() {
    x264_nal_t *nal = nullptr;
    int nnal;
    int size = x264_encoder_headers(enc_, &nal, &nnal);
    extradata_.reserve(size + EXTRA_BUFFER_PADDING_SIZE);

    for (int i = 0; i < nnal; i++) {
        /* Don't put the SEI in extradata. */
        if (nal[i].i_type == NAL_SEI) {
            sei_.reserve(nal[i].i_payload);

            std::copy(nal[i].p_payload,
                      nal[i].p_payload + nal[i].i_payload,
                      std::back_inserter(sei_));
            continue;
        }

        std::copy(nal[i].p_payload,
                  nal[i].p_payload + nal[i].i_payload,
                  std::back_inserter(extradata_));

    }


}



recode::Status X264Encoder::Encode(const VideoFrameData* frame, std::shared_ptr<shaka::media::MediaSample>& sample, bool force_keyframe /*= false*/) {

    Status status;
    x264_picture_t pic_in = { 0 };

    x264_picture_init(&pic_in);

    pic_in.img.i_csp = params_->i_csp;
    pic_in.img.i_plane = cdm::VideoPlane::kMaxPlanes;
    if (frame ) {

        for (int i = 0; i < cdm::VideoPlane::kMaxPlanes; i++) {
            pic_in.img.plane[i] = const_cast<uint8_t*>(frame->data) + frame->plane_offset[i];
            pic_in.img.i_stride[i] = frame->plane_stride[i];
        }

        pic_in.i_pts = frame->timestamp;

        pic_in.i_type = X264_TYPE_AUTO;

        if (force_keyframe)
            pic_in.i_type = X264_TYPE_IDR;

        //reconfig_encoder(ctx, frame);
        // it doesn't work:
        //if (params_->i_width != frame->width
        //    || params_->i_height != frame->height) {
        //    params_->i_width = frame->width;
        //    params_->i_height = frame->height;
        //    auto rc = x264_encoder_reconfig(enc_, &*params_);
        //    LOG(INFO) << "x264_encoder_reconfig: " << rc << ". new width: " << frame->width << ", new height: " << frame->height;
        //}
        //if (pic_in.img.i_stride[0] < params_->i_width) 

        // I think no need to scale if frame size is very close to input parameter
        if(abs(frame->width - params_->i_width) > 2)
        {
            ScalePicture(&pic_in, frame->width, frame->height, params_->i_width, params_->i_height);
        }
    }

    status = EncodePicture(frame ? &pic_in : nullptr, sample);
    return status;
}

void X264Encoder::ScalePicture(x264_picture_t* pic_in, int srcW, int srcH, int dstW, int dstH) 
{
    // fix: Input picture width (640) is greater than stride (576) 
    AVPixelFormat format = AVPixelFormat::AV_PIX_FMT_YUV420P; // frame->format == cdm::VideoFormat::kI420 ?

    if (resize_ctx_ == nullptr
        || sws_frame_width_ != srcW) {
        // init scaler:
#if _DEBUG
        //std::cout << "  skip frame, strides: " << pic_in.img.i_stride[0] << "," << pic_in.img.i_stride[1] << "," << pic_in.img.i_stride[2] << ", width:" << frame->width << std::endl;
        std::cout << "  scale frame : " << srcW << "x" << srcH << " >> " << dstW << "x" << dstH << std::endl;
#endif
        if (resize_ctx_)
            sws_freeContext(resize_ctx_);
        // get context and put ot cache:
        sws_frame_width_ = srcW;
        resize_ctx_ = sws_getContext(srcW, srcH, format, dstW, dstH, format, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

        if (sws_frame) {
            av_freep(&sws_frame->data[0]);
            av_frame_free(&sws_frame);
        }
        // allocate mem for output buf:
        const int align = 1; //? = 32
        sws_frame = av_frame_alloc();
        auto buf_size = av_image_alloc(sws_frame->data, sws_frame->linesize, params_->i_width, params_->i_height, AV_PIX_FMT_YUV420P, align);
    }
    auto scale_ret = sws_scale(resize_ctx_, pic_in->img.plane, pic_in->img.i_stride, 0, srcH, sws_frame->data, sws_frame->linesize);
    // return: the height of the output slice
    //std::cout << "   scale_ret: " << scale_ret << ", " << (scale_ret>=0? "ok": "fail") << std::endl;

    for (int i = 0; i < cdm::VideoPlane::kMaxPlanes; i++) {
        pic_in->img.plane[i] = const_cast<uint8_t*>(sws_frame->data[i]);
        pic_in->img.i_stride[i] = sws_frame->linesize[i];
    }
}

recode::Status X264Encoder::EncodePicture(x264_picture_t* pic_in, std::shared_ptr<shaka::media::MediaSample>& sample) 
{
    recode::Status status;
    x264_picture_t pic_out = { 0 };

    x264_nal_t* nal = nullptr;
    int nnal;
    auto delayed = x264_encoder_delayed_frames(enc_);
    if (pic_in || delayed) {
        int encode_result = x264_encoder_encode(enc_, &nal, &nnal, pic_in, &pic_out);
        if (encode_result > 0) {

            PackNals(nal, nnal);

            if (!frame_nals_.empty()) {
                auto stream_sample = shaka::media::MediaSample::CopyFrom(
                    frame_nals_.data(), frame_nals_.size(), pic_out.b_keyframe > 0);

                stream_sample->set_track_id(0); // only one track expected in the stream
                stream_sample->set_dts(pic_out.i_dts);
                stream_sample->set_pts(pic_out.i_pts);

                //stream_sample->set_duration(runs_->duration());
                //stream_sample->set_timescale(frame->timescale());
                stream_sample->set_timescale(1000000); // microseconds
                sample = stream_sample;

            }
            else {
                LOG(ERROR) << " Non nals: " << encode_result;
                //status.Update({ error::ENCODER_ERROR, "Encode nals error " + std::to_string(encode_result) });
            }
        }
        else if (encode_result < 0) {
            LOG(ERROR) << " Encoder error: " << encode_result;
            status.Update({ error::ENCODER_ERROR, "Encode error " + std::to_string(encode_result), -13 });
        }
    }
    return status;
}

void    X264Encoder::PackNals(const x264_nal_t *nals, int nnal) {

    int size = 0;

    for (int i = 0; i < nnal; i++)
        size += nals[i].i_payload;
#ifdef _DEBUG
    size += sei_.size();
#endif

    frame_nals_.clear();
    frame_nals_.reserve(size);
    
    /* Write the SEI as part of the first frame. */
#ifdef _DEBUG
    if (!sei_.empty()) {
        std::copy(sei_.begin(), sei_.end(),
            std::back_inserter(frame_nals_));
        sei_.clear();
    }
#endif

    for (int i = 0; i < nnal; i++) {
#ifndef _DEBUG
        if (nals[i].i_type != NAL_SEI)
#endif
            std::copy(nals[i].p_payload,
                nals[i].p_payload + nals[i].i_payload,
                std::back_inserter(frame_nals_));
    }
}

Status X264Encoder::Recode(const std::shared_ptr<shaka::media::MediaSample>& orig_sample, AVFormatContext* ctx, std::shared_ptr<shaka::media::MediaSample>& out_sample) {
#pragma warning(disable : 4996)
    AVFrame* frame;
    frame = av_frame_alloc();
    int gotFrame = false;
    auto st = Decode(orig_sample, frame, ctx, &gotFrame);

    if (gotFrame && st.ok()) {
        x264_picture_t pic_in = { 0 };
        x264_picture_t pic_out = { 0 };

        x264_picture_init(&pic_in);

        pic_in.img.i_csp = params_->i_csp;
        pic_in.img.i_plane = cdm::VideoPlane::kMaxPlanes;
        if (frame) {
            for (int i = 0; i < cdm::VideoPlane::kMaxPlanes; i++) {
                pic_in.img.plane[i] = const_cast<uint8_t*>(frame->data[i]);
                pic_in.img.i_stride[i] = frame->linesize[i];
            }

            pic_in.i_pts = frame->pts;
            pic_in.i_type = X264_TYPE_AUTO;

            if (abs(frame->width - params_->i_width) > 2)
            {
                ScalePicture(&pic_in, frame->width, frame->height, params_->i_width, params_->i_height);
            }
        }

        st.Update(EncodePicture(&pic_in, out_sample));
        if (out_sample && st.ok()) {
            out_sample->set_pts(orig_sample->pts());
            out_sample->set_dts(orig_sample->dts());
        }
    }
    av_frame_free(&frame);
    return st;
}

Status X264Encoder::Decode(const std::shared_ptr<shaka::media::MediaSample>& sample, AVFrame* decodedFrame, AVFormatContext* ctx, int* gotFrame)
{
#pragma warning(disable : 4996)
    AVPacket pkt = { 0 };
    //Free sample afterall;
    auto free_buf = [](void* s, uint8_t*) {
        delete static_cast<std::shared_ptr<shaka::media::MediaSample>*>(s);
    };

    pkt.data = const_cast<uint8_t*>(sample->data());
    pkt.size = sample->data_size();

    pkt.buf = av_buffer_create(pkt.data, pkt.size + AV_INPUT_BUFFER_PADDING_SIZE,
        free_buf, new std::shared_ptr<shaka::media::MediaSample>(sample), 0);

    if (!pkt.buf)
        return Status(recode::error::NO_MEMORY, "Could not allocate packet");

    pkt.stream_index = 0; // st->index;
    pkt.flags |= sample->is_key_frame() ? AV_PKT_FLAG_KEY : 0;

    memcpy(pkt.data, sample->data(), sample->data_size());

    pkt.pts = sample->pts();
    pkt.dts = sample->dts();
    pkt.duration = sample->duration();
    AVCodecContext* codcon = ctx->streams[0]->codec; // streams_[0]->codec;
    auto res = avcodec_decode_video2(codcon, decodedFrame, gotFrame, &pkt);
    if (res < 0) {
        std::cout << "error avcodec_decode_video2: " << res << std::endl;
        return Status(recode::error::CONV_ERROR, "Could not decode video");
    }
    return Status();
}

void X264Encoder::Close() {
    if (sws_frame) {
        std::cout << "  sws free buff ... " << std::endl;
        av_freep(&sws_frame->data[0]); 
        av_frame_free(&sws_frame);
    }
    if (resize_ctx_) {
        std::cout << "  sws_freeContext ... " << std::endl;
        //sws_freeContext(resize_ctx_);
    }
    if (enc_) {
        x264_encoder_close(enc_);
        enc_ = nullptr;
    }
}

int X264Encoder::VideoWidth() const
{ 
    return params_ != nullptr ? params_->i_width : 0; 
}
