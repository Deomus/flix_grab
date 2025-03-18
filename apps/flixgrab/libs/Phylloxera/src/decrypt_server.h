#pragma once

#include <sstream>
#include <thread>
#include <string>
#include <memory>

#include "abstract_service.h"
#include "interproc_server.h"
#include "messenger.h"


#define DECRYPT_SERVICE_PREFIX           "decrypt_service_"
#define MESSAGE_PAD             (1024)
#define MAX_OUT_BUFFER             ((1920*1080*3)/2 + MESSAGE_PAD) 
#define MAX_IN_BUFFER             (1024*1024 + MESSAGE_PAD) 
//#define MAX_IN_BUFFER             (MESSAGE_PAD) 

class DecryptServer : public std::enable_shared_from_this<DecryptServer>
{
public:
    DecryptServer(AbstractDecryptService* service);

    ~DecryptServer();
     

  //  void            Cancel();

    void            Start(const std::string& session_id);

    void            Stop();

private:

    void            Run();
    void            Register();

private:
    //RPC Functions;
    bool            rpc_InitVideoDecoder(const BufferReader& reader, BufferWriter& writer);
    bool            rpc_DecryptVideo(const BufferReader& reader, BufferWriter& writer);
    bool            rpc_Decrypt(const BufferReader& reader, BufferWriter& writer);
    bool            rpc_GetKeyIds(const BufferReader& reader, BufferWriter& writer);
    bool            rpc_Finish(const BufferReader& reader, BufferWriter& writer);
    bool            rpc_FrameSink(BufferWriter& writer, VideoFrameData* frame_header, const uint8_t* frame_data, size_t frame_size);
    

private:
    bool                                cancel_;
    Messenger                           messanger_;
    InterprocServer                     server_;

    //TODO: check memory management;
    AbstractDecryptService*             service_;

    std::string                         service_name_;
    std::thread                         service_thread_;
};