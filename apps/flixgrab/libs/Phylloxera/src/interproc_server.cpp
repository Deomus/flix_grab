#include "interproc_server.h"
#include <iostream>


InterprocServer::InterprocServer()
{
    std::cout << "InterprocServer::Constructor" << std::endl;
}

InterprocServer::~InterprocServer()
{
    std::cout << "InterprocServer::Destructor " << name_ << std::endl;
}

void InterprocServer::RegisterFunction(uint32_t proc_num, Invoke_fn invoke)
{
    if (invoker_map_.size() <= proc_num)
        invoker_map_.resize(proc_num + 1);
    invoker_map_[proc_num] = Procedure(invoke);
}

void InterprocServer::ClearFunctions()
{
    invoker_map_.clear();
}

bool InterprocServer::Start(const std::string& name, size_t inbound_size, size_t outbound_size)
{
    std::cout << "Starting server '" << name << "'..." << std::endl;

    name_ = name;

    bool result = true;

    result &= in_pipe_.Create(name + "_in", inbound_size);
    result &= out_pipe_.Create(name + "_out", outbound_size);

    return result;
}

bool InterprocServer::Cancel()
{
    std::cout << "Cancel server " << name_ << std::endl;
    PipeWriter      writer(in_pipe_);
    return writer.Pod(kMessage_Finish);
}

void InterprocServer::Stop()
{
    std::cout << "Stopping server '" << name_ << "'." << std::endl;
    in_pipe_.Close();
    out_pipe_.Close();
}

InterprocStatus InterprocServer::ProcessInvoke()
{
    bool            result = true;
    InterprocStatus status;

    Procedure       proc;
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
            proc = invoker_map_[proc_num];

            assert(proc.is_valid());
 
            if (proc.is_valid()) {
                result &= writer.Pod(kMessage_Result);
                result &= proc.invoke(reader, writer);
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
                status = InterprocStatus::kInternalError;
                std::cout << "Interproc server: Result writer error!" << std::endl;
            }

            break;
        case kMessage_Error:
            {
                std::string message;
                result &= reader.String(message);
                std::cout << "Interproc server: Error message received: " << message << std::endl;
                result &= writer.Pod(kMessage_Error);
                result &= writer.String("Error message received.");
                                
                status = InterprocStatus(InterprocStatus::kCommandError, message);
            }
            
            break;
        case kMessage_Finish:
            std::cout << "Interproc server: Finish message receiving." << std::endl;
            result &= writer.Pod(kMessage_Finish);
            status = InterprocStatus::kCommandFinish;
            break;
        default:
            std::cout << "Interproc server: Unknown Error. Message Id: " << msg << std::endl;
            result &= writer.Pod(kMessage_Error);
            result &= writer.String("Unknown message received.");
            status = status = InterprocStatus(InterprocStatus::kCommandError, "Unknown message received.");
            break;
        }
    }
    
    if (!result)
        status = InterprocStatus(InterprocStatus::kInternalError, "Can't serialize data.");
    
    return status;
}
