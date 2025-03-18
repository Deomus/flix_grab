#include "messenger.h"

Messenger::Messenger()
{
    handlers_.resize(kMid_Max);
}

Messenger::~Messenger()
{
    StopServer();
}

bool Messenger::SendServiceMessage(MessageId id, const std::string& service_name, const std::string& message)
{
    if (!pipe_.IsValid() && !pipe_.Open(PIPE_SERVICE_MESSAGE_NAME))
        return false;

    bool result = true;
    PipeWriter      writer(pipe_);

    result &= writer.Pod(id);
    result &= writer.String(service_name);
    result &= writer.String(message);

    return result;
}

void Messenger::AddHandler(MessageId id, const MessageHandler_fn& handler)
{
    std::lock_guard<std::mutex> guard(handler_mtx_);

    handlers_[id] = handler;
}

bool Messenger::StartServer()
{
    bool result = pipe_.Create(PIPE_SERVICE_MESSAGE_NAME, 1024);

    if ( result && !message_thread_.joinable())
        message_thread_ = std::thread(std::bind(&Messenger::Run, this));

    //return true;
    return result;
}

void Messenger::StopServer()
{
    if (pipe_.IsValid() && message_thread_.joinable()) {
        if (SendServiceMessage(kMid_Finish, "", ""))
            message_thread_.join();
    }

    pipe_.Close();
}

void Messenger::Run()
{
    while (ReceiveServiceMessage() && ProcessServiceMessage());
    /*if (pipe_.Create(PIPE_SERVICE_MESSAGE_NAME, 1024)) {
        while (ReceiveServiceMessage() && ProcessServiceMessage());
        pipe_.Close();
        }*/
}

bool Messenger::ReceiveServiceMessage(void)
{
    bool result = true;
    PipeReader      reader(pipe_);

    result &= reader.Pod(last_id_);
    result &= reader.String(last_service_);
    result &= reader.String(last_message_);

    return result;
}

bool Messenger::ProcessServiceMessage()
{
    //Finish Processing;
    if (last_id_ == kMid_Finish)
        return false;
    std::lock_guard<std::mutex> guard(handler_mtx_);
    if (handlers_[last_id_])
        handlers_[last_id_](last_service_, last_message_);
    return true;
}
