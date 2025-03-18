/********************************************************************
    created:    2011/08/30
    created:    30:8:2011   17:42
    filename:   AVImporter.h
    file base:  AVImporter
    file ext:   h
    author:     fga
    
    purpose:    AVLib functions importer class
*********************************************************************/

#pragma once
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#include <tchar.h>

#define EMULATE_INTTYPES
#define MSVCBUILD
#define __restrict__ __restrict

//#define __STDC_CONSTANT_MACROS
//#define __STDC_FORMAT_MACROS

#include <string>
#include "vamacro.h"

extern "C"
{
#include "./win32fix/bprint.h"
#include "./win32fix/snprintf.h"
}
#else
extern "C"
{
#include "./macfix/bprint.h"
}
#endif
extern "C"
{
#include "libavutil/attributes.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
#include "libavutil/fifo.h"
#include "libavutil/pixfmt.h"
#include "libavutil/pixdesc.h"
#include "libavutil/samplefmt.h"

#include "libavutil/buffer.h"
#include "libavutil/channel_layout.h"
#include "libavutil/mathematics.h"
#include "libavutil/frame.h"

    #include "libavutil/imgutils.h"
    #include "libavutil/audio_fifo.h"
    #include "libavfilter/avfilter.h"
    //#include "libavfilter/avfiltergraph.h"


#include "libswscale/swscale.h"
#include "libswresample/swresample.h"

//Filters
#include "libavfilter/avfilter.h"
//#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/bprint.h"
#include <inttypes.h>
}
#if defined(WIN32) || defined(WIN64)
#define AV_INLINE  __forceinline

#define AVLIBS_PATH             TEXT("")
#define AVCODEC_DLL_NAME        TEXT("avcodec-58.dll")
#define AVFORMAT_DLL_NAME       TEXT("avformat-58.dll")
#define AVUTIL_DLL_NAME         TEXT("avutil-56.dll")
#define AVSWSCALE_DLL_NAME      TEXT("swscale-5.dll")
#define AVSWRESAMPLE_DLL_NAME   TEXT("swresample-3.dll")

#define AV_CALL(x, h) h->x

#define AVCODEC_CALL(x) AVCodecImporter::getHandle()->x
#define AVFORMAT_CALL(x) AVFormatImporter::getHandle()->x
#define AVUTIL_CALL(x) AVUtilImporter::getHandle()->x
#define AVSWSCALE_CALL(x) AVSwscaleImporter::getHandle()->x

class AVException
{
public:
    AVException( HINSTANCE hDLL, const LPTSTR func )
    {
        _tcscpy_s( m_nameFunc, func );
        ::GetModuleFileName(hDLL, m_nameModule, _MAX_PATH);
    }

    //void  getDesc( LPTSTR ) { return std::wstring(L"Module ") + 
        //m_nameModule + std::wstring(L": Error while GetProcAddress of function ") + m_nameFunc; }
private:
    TCHAR               m_nameFunc[_MAX_PATH];
    TCHAR               m_nameModule[_MAX_PATH];

};


//Much more efficient to use templates but ins this case we need to use a ListType template from BOOST... heap of code.
#define IMPORT_FUNCTION( module, rettype, name, ... )\
private: class functor_##name {\
public:\
    typedef rettype (*func)(VA_PAIR(VA_ARG, __VA_ARGS__));\
    functor_##name( ):m_func( NULL ), m_hDLL( NULL ){/*::SetDllDirectory( AVLIBS_PATH );*/ m_hDLL = ::LoadLibrary(module);\
    if ( m_hDLL ) m_func = (func)GetProcAddress(m_hDLL, #name); \
    if ( !m_func ) { ::FreeLibrary( m_hDLL ); throw new AVException(m_hDLL, TEXT(#name));} }\
    ~functor_##name( ){::FreeLibrary(m_hDLL);}\
    AV_INLINE rettype operator()(VA_PAIR(VA_ARG, __VA_ARGS__)) { return m_func(VA_PAIR(VA_VAL, __VA_ARGS__)); }\
private: \
    func            m_func; \
    HINSTANCE       m_hDLL; };\
functor_##name      m_func##name;\
public: AV_INLINE rettype name(VA_PAIR(VA_ARG, __VA_ARGS__)){ return m_func##name( VA_PAIR(VA_VAL,__VA_ARGS__) ); }


#define AVCODEC_IMPORT(rettype, name, ...)      IMPORT_FUNCTION( AVCODEC_DLL_NAME, rettype, name, __VA_ARGS__ )
#define AVFORMAT_IMPORT(rettype, name, ...)     IMPORT_FUNCTION( AVFORMAT_DLL_NAME, rettype, name, __VA_ARGS__ )
#define AVUTIL_IMPORT(rettype, name, ...)       IMPORT_FUNCTION( AVUTIL_DLL_NAME, rettype, name, __VA_ARGS__ )
#define AVSWSCALE_IMPORT(rettype, name, ...)    IMPORT_FUNCTION( AVSWSCALE_DLL_NAME, rettype, name, __VA_ARGS__ )
#define AVSWRESMPL_IMPORT(rettype, name, ...)   IMPORT_FUNCTION( AVSWRESAMPLE_DLL_NAME, rettype, name, __VA_ARGS__ )

//////////////////////////////////////////////////////////////////////////
//SINGLETON CLASS: AVCodec
//////////////////////////////////////////////////////////////////////////
struct AVAudioConvert;
typedef struct AVAudioConvert AVAudioConvert;
typedef void * const TVp6[6];
typedef int TInt6 [6];

class AVCodecImporter
{

    //BEGIN IMPORT SECTION:
    //AVCodec 
    AVCODEC_IMPORT(int, avcodec_open2, AVCodecContext*, avctx, AVCodec*, codec, AVDictionary**, options);
    AVCODEC_IMPORT(AVCodec*, avcodec_find_encoder, enum AVCodecID, id);
    //AVCODEC_IMPORT( void, avcodec_get_frame_defaults, AVFrame*, pic);
    AVCODEC_IMPORT( int, avpicture_alloc, AVPicture*, picture, enum PixelFormat, pix_fmt, int, width, int, height);
//    AVCODEC_IMPORT( int, avcodec_encode_video, AVCodecContext*, avctx, uint8_t*, buf, int, buf_size, const AVFrame*, pict); 
    AVCODEC_IMPORT( AVCodec*, avcodec_find_decoder, enum AVCodecID, id);
    AVCODEC_IMPORT( AVCodec*, avcodec_find_encoder_by_name, const char*, name );
    AVCODEC_IMPORT( int, avcodec_decode_video2, AVCodecContext*, avctx, AVFrame*, picture, int *, got_picture_ptr, AVPacket*, avpkt );
    //AVCODEC_IMPORT( int, avcodec_decode_audio3, AVCodecContext*, avctx, int16_t*, samples, int*, frame_size_ptr, AVPacket* ,avpkt);
    AVCODEC_IMPORT(int, avcodec_decode_audio4, AVCodecContext *, avctx, AVFrame *, frame, int *, got_frame_ptr, const AVPacket *, avpkt);
    AVCODEC_IMPORT( void, avcodec_register_all );
    AVCODEC_IMPORT( const char*, avcodec_license );
    AVCODEC_IMPORT( const char*, avcodec_configuration );
    AVCODEC_IMPORT( void, av_init_packet, AVPacket*, pkt);
    AVCODEC_IMPORT( void, av_free_packet,AVPacket *, pkt);
       
    AVCODEC_IMPORT( int, avcodec_close, AVCodecContext *, avctx);
    //AVCODEC_IMPORT( void, avcodec_get_context_defaults, AVCodecContext *, s);
    AVCODEC_IMPORT( int, avpicture_get_size, enum PixelFormat, pix_fmt, int, width, int, height);
    AVCODEC_IMPORT( int, avpicture_layout, const AVPicture*, src, enum PixelFormat, pix_fmt, int, width, int, height,
        unsigned char *, dest, int, dest_size);
    AVCODEC_IMPORT( int, avpicture_fill, AVPicture *, picture, uint8_t *, ptr, enum PixelFormat, pix_fmt, int, width, int, height);
    AVCODEC_IMPORT( int, av_picture_pad, AVPicture *, dst, const AVPicture *, src, int, height, int, width, enum PixelFormat, pix_fmt,
        int, padtop, int, padbottom, int, padleft, int, padright, int *, color);
    AVCODEC_IMPORT( void, av_picture_copy, AVPicture *, dst, const AVPicture *, src,
        enum PixelFormat, pix_fmt, int, width, int, height);

    //Audio Converter
    //AVCODEC_IMPORT( AVAudioConvert*, av_audio_convert_alloc, enum AVSampleFormat, out_fmt, int, out_channels, enum AVSampleFormat, in_fmt, int, in_channels, const float *,matrix, int, flags);
    //AVCODEC_IMPORT( void, av_audio_convert_free, AVAudioConvert*, ctx );
    //AVCODEC_IMPORT( int, av_audio_convert, AVAudioConvert*, ctx, TVp6, out, const TInt6, out_stride, const TVp6,  in, const TInt6,  in_stride, int, len);

    
    //END IMPORT SECTION;

private: AVCodecImporter( ){}
public:
    static AVCodecImporter* getHandle( void )
    {
        static AVCodecImporter      s_handle;
        return &s_handle;
    }
};

//////////////////////////////////////////////////////////////////////////
//SINGLETON CLASS: AVFormat
//////////////////////////////////////////////////////////////////////////

class AVFormatImporter
{

    //BEGIN IMPORT SECTION:
    //AVFormat
    AVFORMAT_IMPORT( int, av_register_all);
    AVFORMAT_IMPORT( int, avformat_open_input, AVFormatContext**, ps, const char*, filename, AVInputFormat*, fmt, AVDictionary**, options);
    AVFORMAT_IMPORT( AVFormatContext*, avformat_alloc_context);
    AVFORMAT_IMPORT( AVOutputFormat*, av_guess_format, const char*, short_name,
        const char*, filename, const char*, mime_type);

    //AVFORMAT_IMPORT( AVStream*, av_new_stream, AVFormatContext*, s, int, id);
    AVFORMAT_IMPORT( int, avformat_write_header, AVFormatContext*, s, AVDictionary**, options);
    AVFORMAT_IMPORT( int, av_interleaved_write_frame, AVFormatContext*, s, AVPacket*, pkt);
    AVFORMAT_IMPORT( int, av_write_trailer, AVFormatContext*, s);
    //AVFORMAT_IMPORT( void, av_close_input_file, AVFormatContext*, s);
    AVFORMAT_IMPORT( int, av_read_frame, AVFormatContext*, s, AVPacket*, pkt);
    AVFORMAT_IMPORT( int, av_seek_frame, AVFormatContext*, s, int, stream_index, int64_t, timestamp, int, flags);
    AVFORMAT_IMPORT( int, avformat_find_stream_info, AVFormatContext*, ic, AVDictionary**, options);
    AVFORMAT_IMPORT( int, avio_open, AVIOContext**,s, const char*, url, int, flags );
    AVFORMAT_IMPORT( int, avio_close, AVIOContext*, s);
    AVFORMAT_IMPORT( int64_t, avio_size, AVIOContext*, s);

    AVFORMAT_IMPORT( void, av_dump_format, AVFormatContext*, ic, int, index, const char *, url, int, is_output);
    AVFORMAT_IMPORT( int, avformat_alloc_output_context2, AVFormatContext**, ctx, AVOutputFormat*, oformat, const char *, format_name, const char *, filename);
    AVFORMAT_IMPORT( unsigned int, av_codec_get_tag, const struct AVCodecTag * const *, tags, enum AVCodecID, id);
    AVFORMAT_IMPORT( void, avformat_free_context, AVFormatContext *, s);
    AVFORMAT_IMPORT( int, av_write_frame, AVFormatContext *, s, AVPacket *, pkt);

    AVFORMAT_IMPORT( int, av_find_best_stream, AVFormatContext*, ic, enum AVMediaType, type, int, wanted_stream_nb, int, related_stream, AVCodec **, decoder_ret,
        int, flags);
    
    AVFORMAT_IMPORT(void, avformat_close_input, AVFormatContext **, s);
      

    //END IMPORT SECTION;

private: AVFormatImporter( ){}
public:
    static AVFormatImporter* getHandle( void )
    {
        static AVFormatImporter     s_handle;
        return &s_handle;
    }
};

//////////////////////////////////////////////////////////////////////////
//SINGLETON CLASS: AVImporter
//////////////////////////////////////////////////////////////////////////

class AVUtilImporter
{

    //BEGIN IMPORT SECTION:
    //AVUtil
    AVUTIL_IMPORT( int64_t, av_rescale_q, int64_t, a, AVRational, bq, AVRational, cq);
    AVUTIL_IMPORT( int64_t, av_rescale, int64_t, a, int64_t, b, int64_t, c);
    AVUTIL_IMPORT( void, av_free, void *, ptr);
    AVUTIL_IMPORT( void, av_frame_free, AVFrame **, frame);
    AVUTIL_IMPORT( AVFrame *, av_frame_alloc);
    AVUTIL_IMPORT( void, av_log_set_level, int, a);
    typedef void (*avcb)(void*, int, const char*, va_list);
    AVUTIL_IMPORT( void, av_log_set_callback, avcb, f);
    AVUTIL_IMPORT( void, av_log_set_flags, int, arg);

    AVUTIL_IMPORT( void *, av_malloc, size_t, size);
    AVUTIL_IMPORT( int, av_get_bytes_per_sample, enum AVSampleFormat, sample_fmt);
    AVUTIL_IMPORT(int, av_samples_get_buffer_size, int *, linesize, int, nb_channels, int, nb_samples, enum AVSampleFormat, sample_fmt, int, align);
    
    AVUTIL_IMPORT(int64_t, av_get_default_channel_layout, int, nb_channels);
    
    //END IMPORT SECTION;

private: AVUtilImporter( ){}
public:
    static AVUtilImporter* getHandle( void )
    {
        static AVUtilImporter       s_handle;
        return &s_handle;
    }
};

//////////////////////////////////////////////////////////////////////////
//SINGLETON CLASS: AVImporter
//////////////////////////////////////////////////////////////////////////

class AVSwscaleImporter
{

    //BEGIN IMPORT SECTION:
    //SWS
    AVSWSCALE_IMPORT( struct SwsContext*, sws_alloc_context);
    AVSWSCALE_IMPORT( struct SwsContext*, sws_getCachedContext, struct SwsContext*, context,
        int, srcW, int, srcH, PixelFormat, srcFormat, int, dstW, int, dstH, PixelFormat, dstFormat,
        int, flags, SwsFilter*, srcFilter, SwsFilter*, dstFilter, const double*, param);

    AVSWSCALE_IMPORT( void, sws_freeContext, struct SwsContext *, swsContext);
    AVSWSCALE_IMPORT( int, sws_scale, struct SwsContext *, c, const uint8_t* const*, srcSlice,
        const int*, srcStride, int, srcSliceY, int, srcSliceH, uint8_t* const*, dst, const int*, dstStride);

    //END IMPORT SECTION;

private: AVSwscaleImporter( ){}
public:
    static AVSwscaleImporter* getHandle( void )
    {
        static AVSwscaleImporter        s_handle;
        return &s_handle;
    }
};

class AVSwresampleImporter {
    //
    AVSWRESMPL_IMPORT(void, swr_free, SwrContext**, ptr);

    AVSWRESMPL_IMPORT(int, swr_convert, struct SwrContext *, s, uint8_t **, out, int, out_count, const uint8_t **, in, int, in_count);
    AVSWRESMPL_IMPORT(struct SwrContext *, swr_alloc_set_opts, struct SwrContext *, s, int64_t, out_ch_layout, enum AVSampleFormat, out_sample_fmt, int, out_sample_rate,int64_t,  in_ch_layout, enum AVSampleFormat, in_sample_fmt, int, in_sample_rate, int, log_offset, void *, log_ctx);
    AVSWRESMPL_IMPORT(int, swr_init, struct SwrContext *, s);

private: AVSwresampleImporter(){}
public:
    static AVSwresampleImporter* getHandle(void)
    {
        static AVSwresampleImporter     s_handle;
        return &s_handle;
    }
};

#endif
