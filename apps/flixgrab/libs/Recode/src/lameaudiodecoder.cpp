#include "lameaudiodecoder.h"
#include <assert.h>
#include <limits>



#define NOMINMAX

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#include "avimporter.h"
#else
extern "C"
{
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}
#endif



// TODO: migrate to new functions
#pragma warning(disable : 4996)

#if defined(WIN32) || defined(WIN64)
#define AVCODEC_CALL2(x) AV_CALL( x, m_pImpCodec )
#define AVFORMAT_CALL2(x) AV_CALL( x, m_pImpFormat )
#define AVUTIL_CALL2(x) AV_CALL( x, m_pImpUtil )
#define AVSWRESAMPLE_CALL2(x) AV_CALL( x, m_pImpSwresample )
#else
#define AVCODEC_CALL2(x) x
#define AVFORMAT_CALL2(x) x
#define AVUTIL_CALL2(x) x
#define AVSWRESAMPLE_CALL2(x) x
#define AVFORMAT_CALL(x) x
#endif

#define AUDIO_FRAME_SIZE (192000 * 2 )
#define MOD_NAME    "AudDec"
#define MAX_DECODE_ERRORS           (64)

#include <iostream>
#define TIER0_DBG_A(p, m) std::cout << p << ";" << m
#define TIER0_ERR_A(p,m)  std::cerr << p << ";" << m


CAudioDecoder::CAudioDecoder():
m_pFormatCtx(NULL),
m_pCodecCtx(NULL),
m_pCodec(NULL),
m_offset(0),
m_pReformatCtx(NULL),
m_fmtAudio(AV_SAMPLE_FMT_S16),
m_audioStream( -1 ),
m_pSwrCtx( NULL ),
m_dBufferFactor( 1 )
{
    //CEnvConstructor::GetEnv();
#if defined(WIN32) || defined(WIN64)
    m_pImpCodec = AVCodecImporter::getHandle();
    m_pImpFormat = AVFormatImporter::getHandle();
    m_pImpUtil = AVUtilImporter::getHandle();
    m_pImpSwresample = AVSwresampleImporter::getHandle();
#endif
    AVFORMAT_CALL(av_register_all());
   /* AVUTIL_CALL(av_log_set_level( AV_LOG_DEBUG ));
    AVUTIL_CALL(av_log_set_flags( AV_LOG_SKIP_REPEATED ));
    AVUTIL_CALL(av_log_set_callback( FFDebugCallback ));
*/
    m_pPacket = (AVPacket*)AVUTIL_CALL2(av_malloc( sizeof( AVPacket ) ));

    AVCODEC_CALL2(av_init_packet( m_pPacket ));
    m_pPacket->size = 0;
    m_pPacket->data = NULL;

    m_pFrameBuffer = (uint8_t*)AVUTIL_CALL2( av_malloc(AUDIO_FRAME_SIZE) );
    m_pReformatBuffer = (uint8_t*)AVUTIL_CALL2(av_malloc(AUDIO_FRAME_SIZE) );


}

CAudioDecoder::~CAudioDecoder()
{    
    AVSWRESAMPLE_CALL2( swr_free( &m_pSwrCtx ) );
    AVCODEC_CALL2( av_free_packet(m_pPacket) );
    AVUTIL_CALL2( av_free( m_pPacket ) );
    AVUTIL_CALL2( av_free( m_pFrameBuffer ) );
    AVUTIL_CALL2( av_free( m_pReformatBuffer ) );
}


EDecodeResult CAudioDecoder::decodeFrame( double& timestamp, CAudioFrame* pFrame )
{
 
    //int sizeFrame = 0;
    int sizeDecoded = 0;
   
    EDecodeResult   eResult = kDecodeResult_Success;

    pFrame->setLength(0);
    int gotFrame = 0;

    AVFrame*     pAvFrame = AVUTIL_CALL2(av_frame_alloc());

    //AVCODEC_CALL2(avcodec_get_frame_defaults( &frame ));

    //Sometimes (in MP3 Container) First Frame Cant Be Decode Properly,
    //So We Skip All First Packets;
    bool        bFirstFrame = false;
    size_t      nDecodeErrors = 0;
    
     /* NOTE: the audio packet can contain several frames */
    while( eResult == kDecodeResult_Success )
    {
        if ( m_offset >= (size_t)m_pPacket->size )
        {
            if ( pFrame->getLength() > 0 ) break;
            m_offset = 0;
            do
            {
                // Free old packet
                AVCODEC_CALL2( av_free_packet(m_pPacket));
                AVCODEC_CALL2( av_init_packet( m_pPacket ));

                // Read new packet
                int ret = AVFORMAT_CALL2( av_read_frame(m_pFormatCtx, m_pPacket));

                if (ret == AVERROR(EAGAIN))
                {
                    //log_info( MOD_NAME, "Read packet again" );
                    m_pPacket->stream_index = -1;
                    continue;
                }

                if (ret < 0) 
                {
                    //log_info( MOD_NAME, "End Of File" );
                    eResult = kDecodeResult_Eof;
                    m_pPacket->size = 0;
                    break;
                    //return false;

                }


            } while(m_pPacket->stream_index!=m_audioStream);

        }

        AVPacket packet = *m_pPacket;
        if ( m_offset > 0 )
        {
            packet.size -= m_offset;
            packet.data += m_offset;
        }
             
        int len = AVCODEC_CALL2(avcodec_decode_audio4(m_pCodecCtx, pAvFrame, &gotFrame,  &packet));
        
        if ( len >= 0 ) m_offset += len;
        else 
        { 
            nDecodeErrors++;
            m_offset = packet.size; 
            if ( /*bFirstFrame || */nDecodeErrors > MAX_DECODE_ERRORS )
                eResult = kDecodeResult_Error;
                       
            continue; 
        }

        if ( !gotFrame ) continue;

        bFirstFrame = true;
      
        int planeSize = 0;
        int nOutBufferSamples = (int)(pAvFrame->nb_samples * m_dBufferFactor + 0.5 );
        int s16DataSize = AVUTIL_CALL2(av_samples_get_buffer_size(&planeSize, m_xOutParam.channels, nOutBufferSamples, AV_SAMPLE_FMT_S16, 1));

        pFrame->resize( sizeDecoded + s16DataSize );
        uint8_t* pDstBuffer =  pFrame->getBuffer() + sizeDecoded;

        int nOutSamples = AVSWRESAMPLE_CALL2(swr_convert(m_pSwrCtx, &pDstBuffer, nOutBufferSamples, (const uint8_t**)pAvFrame->extended_data, pAvFrame->nb_samples));
        if ( nOutSamples < 0 )
        {
            //Error While Conver;
            eResult = kDecodeResult_Error;
            break;
        }
        //memcpy( pDstBuffer, pSrcBuffer, s16DataSize );
        s16DataSize = AVUTIL_CALL2(av_samples_get_buffer_size(&planeSize, m_xOutParam.channels, nOutSamples, AV_SAMPLE_FMT_S16, 1));
        sizeDecoded += s16DataSize;
        pFrame->setLength( sizeDecoded );
        timestamp = packet.pts * av_q2d( m_pFormatCtx->streams[m_audioStream]->time_base );
    }
    
    AVUTIL_CALL2(av_frame_free( &pAvFrame ));

    return eResult;
}


bool CAudioDecoder::open( char const* file )
{
    // Open video file
    int ret;
    if ((ret=AVFORMAT_CALL2( avformat_open_input(&m_pFormatCtx, file, NULL, NULL)))!=0)
    {
        char errbuf[1000];
        //av_strerror(ret, errbuf, 1000);
       // log_error( MOD_NAME, "Could not open file " << file );
        return false; // Couldn't open file
    }

    if (m_pFormatCtx->nb_streams <= 0) {
        TIER0_ERR_A("AudDec", "m_pFormatCtx->nb_streams: " << m_pFormatCtx->nb_streams);
        return false;
    }
    //Support EXPERIMENTAL codecs;
    for (unsigned int i = 0; i < m_pFormatCtx->nb_streams; i++)
    {
        //?todo: m_pFormatCtx->streams[i]->codec->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    }

    // Retrieve stream information
    if (AVFORMAT_CALL2( avformat_find_stream_info(m_pFormatCtx, NULL))<0)
    {
       // log_error( MOD_NAME, "Couldn't find stream information " << file );
        return false; // Couldn't find stream information
    }

    // Dump information about file onto standard error
    AVFORMAT_CALL2( av_dump_format(m_pFormatCtx, 0, file, false));
        
    

    return true;

}

void CAudioDecoder::close( void )
{
    if ( m_pFormatCtx )
        // Close the video file
        AVFORMAT_CALL2( avformat_close_input(&m_pFormatCtx));
    m_pFormatCtx = NULL;


    AVCODEC_CALL2( av_free_packet( m_pPacket ));
}


bool CAudioDecoder::beginDecode( TAudioParams& outParams, int stream )
{
    if ( m_pFormatCtx == NULL )
        return false;

    if ( stream < 0 )
    {
        // Find the first stream
        m_audioStream = AVFORMAT_CALL2(av_find_best_stream( m_pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0 ));

        if ( m_audioStream == -1)
        {
            // LOG_ERR( "Didn't find a video stream " << file );
            return false; // Didn't find a video stream
        }
    }
    else m_audioStream = stream;
    
    m_xOutParam = outParams;
    // Get a pointer to the codec context for the video stream
    m_pCodecCtx = m_pFormatCtx->streams[m_audioStream]->codec;
    
    int channels = m_pCodecCtx->channels;
    int sample_rate = m_pCodecCtx->sample_rate;
    int frame_size = m_pCodecCtx->frame_size;
       
  
    m_pCodecCtx->request_channel_layout = AV_CH_LAYOUT_STEREO_DOWNMIX;
    // Find the decoder for the video stream
    m_pCodec = AVCODEC_CALL2( avcodec_find_decoder(m_pCodecCtx->codec_id));
    if (m_pCodec==NULL)
    {
        //log_error( MOD_NAME, "Codec not found: tag= " << m_pCodecCtx->codec_tag );
        return false; // Codec not found
    }

    // Inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
    if (m_pCodec->capabilities & AV_CODEC_CAP_TRUNCATED)
        m_pCodecCtx->flags|=AV_CODEC_FLAG_TRUNCATED;
    
    
    // Open codec
    if (AVCODEC_CALL2( avcodec_open2(m_pCodecCtx, m_pCodec, NULL))<0)
    {
        //log_error( MOD_NAME, "Could not open codec: tag= " << m_pCodecCtx->codec_tag );
        return false; // Could not open codec
    }

    if ( m_pCodecCtx->channels <= 0 )
        m_pCodecCtx->channels = channels;

    if ( m_pCodecCtx->frame_size <= 0 )
        m_pCodecCtx->frame_size = frame_size;

    if ( m_pCodecCtx->sample_rate <= 0 )
        m_pCodecCtx->sample_rate = sample_rate;


    int64_t     nChLayoutOut = AV_CH_LAYOUT_STEREO;
    int64_t     nChLayoutIn = m_pCodecCtx->channel_layout;

   /* if ( m_pCodecCtx->channels == 1 )
        nChLayoutOut = AV_CH_LAYOUT_MONO;
        */

    if ( nChLayoutIn == 0 )
        nChLayoutIn = AVUTIL_CALL2(av_get_default_channel_layout( m_pCodecCtx->channels ));

    nChLayoutOut = AVUTIL_CALL2(av_get_default_channel_layout( m_xOutParam.channels ));

    m_pSwrCtx = AVSWRESAMPLE_CALL2(swr_alloc_set_opts( m_pSwrCtx, nChLayoutOut, AV_SAMPLE_FMT_S16, m_xOutParam.sampleRate, nChLayoutIn, m_pCodecCtx->sample_fmt, m_pCodecCtx->sample_rate, 0, NULL ));

    //Factor of Growing/Reducing Number Of Samples Due Sample Rate Convertations;
    m_dBufferFactor = m_xOutParam.sampleRate / (double)m_pCodecCtx->sample_rate;

    if ( 0 != AVSWRESAMPLE_CALL2(swr_init( m_pSwrCtx )) )
    {
        //Error While Alloc Swr;
        return false;
    }

    m_offset = 0;
    AVCODEC_CALL2( av_init_packet( m_pPacket ));
    
    return true;
}


bool CAudioDecoder::endDecode()
{    
    if ( m_pCodecCtx )
        // Close the codec
        AVCODEC_CALL2( avcodec_close(m_pCodecCtx));

    m_pCodecCtx = NULL;
    m_pCodec = NULL;
    
    return true;
}


size_t CAudioDecoder::skipSilence( double silenceThreshold /* = 0.0001 */ )
{
    silenceThreshold *= static_cast<double>( std::numeric_limits<short>::max() );
    CAudioFrame frame;

    double ts = 0;
    size_t  bytesCount = 0;

    for (;;)
    {
        if ( !decodeFrame( ts, &frame ) )
            break;
                
        double sum = 0;
        short* pBuffer = (short*)frame.getBuffer();
        bytesCount += frame.getLength();
        size_t length = frame.getLength() >> 1;

        switch (m_pCodecCtx->channels)
        {
            //Mono channels
        case 1:
            for (size_t j = 0; j < length; ++j)
                sum += abs((int)pBuffer[j]);
            break;
            //Stereo channels;
        case 2:
            for (size_t j = 0; j < length; j+= 2)
                sum += abs((int)(pBuffer[j]) + (int)(pBuffer[j+1]) );
            break;
        }

        if ( (sum >= silenceThreshold * length) )
            break;
    }
    return bytesCount;
}



bool CAudioDecoder::getAudioParams( TAudioParams& params, int stream )
{
    if ( m_pFormatCtx == NULL )
        return false;
    

    if ( stream < 0 )
    {
        // Find the first stream
        stream = AVFORMAT_CALL2(av_find_best_stream( m_pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0 ));

        if (stream < 0)
        {
            if (stream == AVERROR_STREAM_NOT_FOUND)
                TIER0_ERR_A("AudDec", "Audio stream not found\n");
            else if (stream == AVERROR_DECODER_NOT_FOUND)
                TIER0_ERR_A("AudDec", "Failed to find decoder\n");
            else
                TIER0_ERR_A("AudDec", "Failed to av_find_best_stream\n");
            // LOG_ERR( "Didn't find a video stream " << file );
            return false; // Didn't find a stream
        }
    }
    
    AVStream* pStream = m_pFormatCtx->streams[stream];

    auto pContext = m_pFormatCtx->streams[stream]->codecpar;
    
    params.sampleRate = pContext->sample_rate;
    params.channels = pContext->channels;
    params.frameSize = pContext->frame_size;
    params.duration = (int64_t)(pStream->duration * TIME_RESCALE * av_q2d( pStream->time_base ));
    params.bitrate = pContext->bit_rate;

    if ( pStream->duration == AV_NOPTS_VALUE )
    {
        params.duration = (int64_t)(m_pFormatCtx->duration * TIME_RESCALE / ((double)AV_TIME_BASE) + 0.5);
    }

    /*params.startTime = pStream->start_time;

    if ( params.startTime == 0 )
    {
        params.startTime = (int64_t)(m_pFormatCtx->start_time / (av_q2d( pStream->time_base )*(double)AV_TIME_BASE) + 0.5);
    }*/
    return true;
}

void CAudioDecoder::getMetadata( TMetadata& metadata )
{
    metadata.clear();
    if ( m_pFormatCtx )
    {      

        AVDictionaryEntry *pEntry = NULL;
     /*   while ((pEntry = av_dict_get(m_pFormatCtx->metadata, "", pEntry, AV_DICT_IGNORE_SUFFIX)))
        {
            metadata[pEntry->key] = pEntry->value;
        }*/
    }
}

bool CAudioDecoder::seekTo( double timestamp /*ms*/ )
{
    TIER0_DBG_A( "AudDec", "Seeking Decoder: " << timestamp );
    if ( 0 == AVFORMAT_CALL2(av_seek_frame( m_pFormatCtx, m_audioStream, int64_t(timestamp / av_q2d( m_pFormatCtx->streams[m_audioStream]->time_base )), 0 )) )
        return true;

    TIER0_ERR_A( "AudDec", "Seeking Error: " << timestamp );
    return false;
}

/*
          album        -- name of the set this work belongs to
00083  * album_artist -- main creator of the set/album, if different from artist.
00084  *                 e.g. "Various Artists" for compilation albums.
00085  * artist       -- main creator of the work
00086  * comment      -- any additional description of the file.
00087  * composer     -- who composed the work, if different from artist.
00088  * copyright    -- name of copyright holder.
00089  * creation_time-- date when the file was created, preferably in ISO 8601.
00090  * date         -- date when the work was created, preferably in ISO 8601.
00091  * disc         -- number of a subset, e.g. disc in a multi-disc collection.
00092  * encoder      -- name/settings of the software/hardware that produced the file.
00093  * encoded_by   -- person/group who created the file.
00094  * filename     -- original name of the file.
00095  * genre        -- <self-evident>.
00096  * language     -- main language in which the work is performed, preferably
00097  *                 in ISO 639-2 format. Multiple languages can be specified by
00098  *                 separating them with commas.
00099  * performer    -- artist who performed the work, if different from artist.
00100  *                 E.g for "Also sprach Zarathustra", artist would be "Richard
00101  *                 Strauss" and performer "London Philharmonic Orchestra".
00102  * publisher    -- name of the label/publisher.
00103  * service_name     -- name of the service in broadcasting (channel name).
00104  * service_provider -- name of the service provider in broadcasting.
00105  * title        -- name of the work.
00106  * track        -- number of this work in the set, can be in form current/total.
*/

#undef LOG_MODULE
