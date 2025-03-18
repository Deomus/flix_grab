#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <vector>

#include "pipe.h"

enum MessageId
{
    kMid_None = 0,
    kMid_NewServer,
    kMid_DeleteServer,
    kMid_Error,
    kMid_Finish,
    kMid_Max,
};

#define PIPE_SERVICE_MESSAGE_NAME       "decrypt_messenger"


typedef std::function<void(const std::string& service_name, const std::string& message)>     MessageHandler_fn;

//TODO: Divide in Server And Client;
class Messenger
{
public:
    Messenger();

    ~Messenger();

    bool            SendServiceMessage(MessageId id, const std::string& service_name, const std::string& message);

    void            AddHandler(MessageId id, const MessageHandler_fn& handler);

    bool            StartServer();
    void            StopServer();

private:
    void            Run();
    bool            ReceiveServiceMessage(void);
    bool            ProcessServiceMessage();

private:
    Messenger(const Messenger&);
    void operator=(const Messenger&);

private:
    std::vector<MessageHandler_fn>  handlers_;
    
    Pipe                            pipe_;

    std::thread                     message_thread_;
    std::mutex                      handler_mtx_;
private:
    MessageId                       last_id_;
    std::string                     last_service_;
    std::string                     last_message_;
    
};


