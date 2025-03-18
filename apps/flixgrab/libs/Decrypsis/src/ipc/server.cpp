#include "server.h"
#include <iostream>

#include "message.h"

using namespace ipc;

Server::Server(const std::string& name, const std::vector<ServerFunc>& table, size_t inbound_size, size_t outbound_size)
    : name_(name)
    , table_(table)
{
    std::cout << "InterprocServer::Constructor '" << name << "'" << std::endl;

    bool result = true;

    result &= in_pipe_.Create(name + "_in", inbound_size);
    result &= out_pipe_.Create(name + "_out", outbound_size);

    if (result) {
        thread_ = std::thread(&Server::Run, this);
    }
    else {
        std::cout << "Cant start server '" << name << "'" << std::endl;
        status_ = Status::kInternalError;
    }
}

Server::~Server()
{
    std::cout << "Stopping server '" << name_ << "'." << std::endl;
    if (status_ != Status::kCommandFinish)
        Cancel();

    if (thread_.joinable())
        thread_.join();

    in_pipe_.Close();
    out_pipe_.Close();

    std::cout << "InterprocServer::Destructor '" << name_ << "'" << std::endl;
}


bool Server::Cancel()
{
    std::cout << "Cancel server '" << name_ << "'" << std::endl;
    PipeWriter      writer(in_pipe_);
    return writer.Pod(kMessage_Finish);
}

void Server::Run() {
    Start();
    while ( (status_ = ProcessInvoke()) != Status::kCommandFinish);
    Stop();
}


Status Server::ProcessInvoke()
{
    bool            result = true;
    Status status;

    ServerFunc      proc;
    uint32_t        msg = kMessage_Unknown;
    uint32_t        proc_num = 0;

    std::string     message;

    PipeReader      reader(in_pipe_);
    PipeWriter      writer(out_pipe_);

    result &= reader.Pod(msg);

    if (result) {
        
        switch (msg)  {
        case kMessage_Invoke:
            result &= reader.Pod(proc_num);
            proc = table_[proc_num];

            assert(proc != nullptr);
 
            if (proc) {
                result &= writer.Pod(kMessage_Result);
                result &= proc(reader, writer);
            }
            else {
                std::cout << "Interproc server: No invoke procedure registered: " << proc_num << std::endl;
                message = "Invoke procedure " + std::to_string(proc_num) + " not registered.";
                result = false;
            }

            if (!result) {
                writer.Reset();
                result = writer.Pod(kMessage_Error);
                result &= writer.String(message);
                status = Status::kInternalError;
                std::cout << "Interproc server '" << name_ << "': Result writer error!" << std::endl;
            }

            break;
        case kMessage_Error:
            {
                std::string message;
                result &= reader.String(message);
                std::cout << "Interproc server: Error message received: " << message << std::endl;
                result &= writer.Pod(kMessage_Error);
                result &= writer.String("Error message received.");
                                
                status = Status(Status::kCommandError, message);
            }
            
            break;
        case kMessage_Finish:
            std::cout << "Interproc server: Finish message receiving." << std::endl;
            result &= writer.Pod(kMessage_Finish);
            status = Status::kCommandFinish;
            break;
        default:
            std::cout << "Interproc server: Unknown Error. Message Id: " << msg << std::endl;
            result &= writer.Pod(kMessage_Error);
            result &= writer.String("Unknown message received.");
            status = status = Status(Status::kCommandError, "Unknown message received.");
            break;
        }
    }
    
    if (!result)
        status = Status(Status::kInternalError, "Can't serialize data.");
    
    return status;
}
