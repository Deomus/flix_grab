#include "decrypt_server.h"
#include "messenger.h"
#include <iostream>
#include <windows.h>

DecryptServer::DecryptServer(AbstractDecryptService* service) 
    : service_(service)
    , cancel_(false)
{
    std::cout << "DecryptServer::Constructor" << std::endl;
    Register();
}

DecryptServer::~DecryptServer()
{
    std::cout << "DecryptServer::Destructor " << service_name_ << std::endl;
    /*if (service_thread_.joinable())
            service_thread_.join();*/
    //Stop();
}

void DecryptServer::Start(const std::string& session_id)
{
    if (!service_thread_.joinable()) {
        service_name_ = DECRYPT_SERVICE_PREFIX + session_id;
        //MessageBoxA(NULL, "CreateThread", "DecryptServer", 0);
        service_thread_ = std::thread(std::bind(&DecryptServer::Run, this));
    }
    
}


void DecryptServer::Stop()
{
    if (service_thread_.joinable()) {

        cancel_ = true;
        if (service_thread_.get_id() != std::this_thread::get_id()) {

            server_.Cancel();
            std::cout << "Server Cancelled" << std::endl;
            //service_thread_.join();
            std::cout << "Server Join" << std::endl;
            service_name_.clear();
        }


    }
}

void DecryptServer::Run()
{
    std::cout << "DecryptService::Run" << std::endl;
    auto locker = this->shared_from_this();

    bool result = server_.Start(service_name_, MAX_IN_BUFFER, MAX_OUT_BUFFER);

    if (result) {
        //Send Message;
        messanger_.SendServiceMessage(kMid_NewServer, service_name_, "");

        std::cout << "Start Decrypting..." << std::endl;
        while (!cancel_ && server_.ProcessInvoke().status() != InterprocStatus::kCommandFinish);

        std::cout << "Finish Decrypting." << std::endl;
        messanger_.SendServiceMessage(kMid_DeleteServer, service_name_, "");
        server_.Stop();
    }
    else
        messanger_.SendServiceMessage(kMid_Error, service_name_, "Can't start server");

    service_thread_.detach();
}

//void DecryptServer::Run()
//{
//    //Send Message;
//    messanger_.SendServiceMessage(kMid_NewServer, service_name_, "");
//
//    std::cout << "Start Decrypting..." << std::endl;
//
//    while (server_.ProcessInvoke());
//
//    std::cout << "Finish Decrypting." << std::endl;
//
//    messanger_.SendServiceMessage(kMid_DeleteServer, service_name_, "");
//}

void DecryptServer::Register()
{
    server_.RegisterFunction(kInvoke_InitVideoDecoder,
        std::bind(&DecryptServer::rpc_InitVideoDecoder, this, std::placeholders::_1, std::placeholders::_2));
    
    server_.RegisterFunction(kInvoke_DecryptVideo,
        std::bind(&DecryptServer::rpc_DecryptVideo, this, std::placeholders::_1, std::placeholders::_2));

    server_.RegisterFunction(kInvoke_Decrypt,
        std::bind(&DecryptServer::rpc_Decrypt, this, std::placeholders::_1, std::placeholders::_2));

    server_.RegisterFunction(kInvoke_GetKeyIds,
        std::bind(&DecryptServer::rpc_GetKeyIds, this, std::placeholders::_1, std::placeholders::_2));

    server_.RegisterFunction(kInvoke_Finish,
        std::bind(&DecryptServer::rpc_Finish, this, std::placeholders::_1, std::placeholders::_2));
}

bool DecryptServer::rpc_InitVideoDecoder(const BufferReader& reader, BufferWriter& writer)
{
    std::cout << "rpc_InitVideoDecoder Called" << std::endl;

    bool            result = true;
    VideoConfig     config;
    InvokeStatus    status;

    result = config.Serializer(reader);
    if (result) {
        status.value = service_->InitializeVideoDecoder(config);
        result = status.Serializer(writer);
    }
    return result;
}

bool DecryptServer::rpc_DecryptVideo(const BufferReader& reader, BufferWriter& writer)
{
    //std::cout << "rpc_DecryptVideo Called" << std::endl;

    bool            result = true;
    EncryptedData   encrypted;
    InvokeStatus    status;
    FrameSource_fn  frame_store = std::bind(&DecryptServer::rpc_FrameSink, this, writer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    result = encrypted.Serializer(reader);
    if (result) {
        status.value = service_->DecodeVideo(encrypted, frame_store);
        if (status.value != cdm::kSuccess) {
            result = status.Serializer(writer);
        }

    }
    return result;
}

bool DecryptServer::rpc_GetKeyIds(const BufferReader& , BufferWriter& writer)
{
    bool            result = true;
    Keys            key_ids;

    std::cout << "rpc_GetKeyIds Called" << std::endl;

    service_->GetKeyIds(key_ids);
    result = key_ids.Serializer(writer);

    return result;
}

bool DecryptServer::rpc_Finish(const BufferReader& , BufferWriter& )
{
    bool            result = true;

    std::cout << "rpc_Finish Called" << std::endl;

    service_->Finish();

    return result;
}

bool DecryptServer::rpc_FrameSink(BufferWriter& writer, VideoFrameData* frame_header, const uint8_t* frame_data, size_t frame_size)
{
    bool result = true;

    InvokeStatus    status;
    status.value = cdm::kSuccess;
    // std::cout << "rpc_FrameSink Called" << std::endl;
    result &= status.Serializer(writer);
    result &= frame_header->Serializer(writer);
    result &= writer.Data(frame_data, frame_size);
    return result;
}

bool DecryptServer::rpc_Decrypt(const BufferReader& reader, BufferWriter& writer)
{
    bool            result = true;
    EncryptedData   encrypted;
    DecryptedData   decrypted;

    result = encrypted.Serializer(reader);
    if (result) {
        service_->Decrypt(encrypted, decrypted);
        result = decrypted.Serializer(writer);
    }
    return result;
}

/* void       Start(const std::string& session_id)
{
if (!service_thread_.joinable())  {
service_name_ = DECRYPT_SERVICE_PREFIX + session_id;
service_thread_ = std::thread(std::bind(&DecryptServer::Run, this));
}

}

void        Stop() {

if (service_thread_.joinable()) {
rpc_server_.Cancel();
service_thread_.join();
service_name_.clear();
}

}*/

//void        Run() {


//    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

//    if (rpc_server_.Start(service_name_, MAX_IN_BUFFER, MAX_OUT_BUFFER)) {

//        //Send Message;
//        messanger_.SendServiceMessage(kMid_NewServer, service_name_, "");

//        std::cout << "Start Decrypting..." << std::endl;

//        while (rpc_server_.ProcessInvoke());

//        std::cout << "Finish Decrypting." << std::endl;

//        messanger_.SendServiceMessage(kMid_DeleteServer, service_name_, "");

//        rpc_server_.Stop();


//    }
//    else {
//        messanger_.SendServiceMessage(kMid_Error, service_name_, "Can't start server");
//    }

//    service_thread_.detach();
//}

