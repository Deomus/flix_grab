#ifndef AUDIO_DECODER
#define AUDIO_DECODER


#include <stdint.h>

#include <map>
#include <string>
#include <algorithm>
//#include <libavcodec/avcodec.h>

struct TAudioParams
{
    int        sampleRate;
    int        channels;
    int        frameSize;
    int64_t    duration;
    int64_t     bitrate;
};

class CAudioFrame
{
public:
    CAudioFrame():m_size(0),m_pBuffer(NULL), m_length(0){}
    ~CAudioFrame()
    {
        if ( m_pBuffer ) delete [] m_pBuffer;
    }
    void        resize( size_t size ) 
    {  
        if ( m_size < size )
        {
            uint8_t* pOldBuffer = m_pBuffer;
            
            m_pBuffer = new uint8_t [ size ];
            memcpy( m_pBuffer, pOldBuffer, m_length );
            m_size = size;
            delete [] pOldBuffer;
        }
    }

    size_t        copyFrom( CAudioFrame & frame, size_t from = 0, size_t length = -1 )
    {
        size_t count = std::min( frame.m_length - from, length );
        this->resize( m_length + count );
        memcpy( m_pBuffer + m_length, frame.m_pBuffer + from, count );
        m_length += count;
        return count;
    }

    void        setLength( size_t length ) { m_length = length; }
    size_t        getLength( void ) const { return m_length; }
    uint8_t*    getBuffer( void ) { return m_pBuffer; }
    size_t        getSize( void ) const { return m_size; }

private:
    uint8_t        *m_pBuffer;
    size_t        m_size;
    size_t        m_length;

};
    

class AVCodecImporter;
class AVFormatImporter;
class AVUtilImporter;
class AVSwresampleImporter;
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVPacket;
struct AVAudioConvert;
struct SwrContext;

typedef std::map<std::string, std::string> TMetadata;

#define TIME_RESCALE        1000   //(time in ms)

enum EDecodeResult
{
    kDecodeResult_Success = 0,
    kDecodeResult_Eof,
    kDecodeResult_Error,
};

class CAudioDecoder
{

public:
    CAudioDecoder( );
    ~CAudioDecoder();
    
    bool            open(char const* file);
    void            close( void );
            
    bool            beginDecode( TAudioParams& outParams, int stream = -1 );
    bool            seekTo( double timestamp /*ms*/ );
    size_t          skipSilence( double silenceThreshold = 0.0001 );
    EDecodeResult    decodeFrame( double& timestamp /*ms*/, CAudioFrame* pFrame  );

    void            getMetadata( TMetadata& );

    bool            endDecode( void );
    bool            getAudioParams( TAudioParams& inParams, int stream = -1 );
    
private:
    AVFormatContext *m_pFormatCtx;
    int             m_audioStream;
    AVCodecContext  *m_pCodecCtx;
    AVPacket        *m_pPacket;
    size_t          m_offset;

    AVCodec         *m_pCodec;

    //////////////////////////////////////////////////////////////////////////
    int                m_fmtAudio;
    uint8_t            *m_pFrameBuffer;
    uint8_t            *m_pReformatBuffer;

    AVAudioConvert        *m_pReformatCtx;

    SwrContext*         m_pSwrCtx;
    TAudioParams        m_xOutParam;

    double              m_dBufferFactor;
    

private:
    AVCodecImporter*        m_pImpCodec;
    AVFormatImporter*       m_pImpFormat;
    AVUtilImporter*         m_pImpUtil;
    AVSwresampleImporter*   m_pImpSwresample;

};

#undef NOMINMAX

#endif