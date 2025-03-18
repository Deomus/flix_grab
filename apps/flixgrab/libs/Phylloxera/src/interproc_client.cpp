#include "interproc_client.h"
#include <iostream>

InterprocClient::InterprocClient() : is_finished_(false)
{
    std::cout << "InterprocClient::Constructor" << std::endl;
}

InterprocClient::~InterprocClient()
{
    std::cout << "InterprocClient::Destructor " << name_ << std::endl;
    Stop();
}

bool InterprocClient::Start(const std::string& name)
{
    bool result = true;

    name_ = name;
    result &= in_pipe_.Open(name + "_in");
    result &= out_pipe_.Open(name + "_out");

    return result;
}

void InterprocClient::Stop()
{
    //Cancel();
    in_pipe_.Close();
    out_pipe_.Close();
}

bool InterprocClient::Invoke(uint32_t proc_num, serialize_fn args_fn, deserialize_fn result_fn)
{
    bool            result = true;

    if (is_finished_) return false;
    //Write Scope;
    {
        PipeWriter      writer(in_pipe_);

        result &= writer.Pod(kMessage_Invoke);
        result &= writer.Pod(proc_num);
        result &= args_fn(writer);

        if (!result) {
            writer.Reset();
            result = writer.Pod(kMessage_Error);
            result &= writer.String("Can't write invoker arguments.");
            std::cout << "Interproc client " << name_ << ": Can't write invoker arguments." << std::endl;
        }
    }

//Read Scope;
    {
        PipeReader      reader(out_pipe_);
        uint32_t        msg = kMessage_Unknown;

        result = reader.Pod(msg);

        switch (msg)
        {
        case kMessage_Result:
            result &= result_fn(reader);
            break;
        case kMessage_Error:
        {
            std::string error_msg;
            result &= reader.String(error_msg);
            std::cout << "Interproc client " << name_ << ": Error message received - " << error_msg << std::endl;
        }
        case kMessage_Finish:
            std::cout << "Interproc client " << name_ << ": Finish message received!" << std::endl;
            is_finished_ = true;
        default:
            std::cout << "Interproc client " << name_ << ": Unknown message received " << msg << std::endl;
            result = false;
            break;
        }
    }

    return result;
}

bool InterprocClient::Cancel()
{
    bool            result = true;

    if (is_finished_) return true;
    //Write Scope;
    {
        PipeWriter      writer(in_pipe_);
        result &= writer.Pod(kMessage_Finish);
    }

    //Read Scope;
    {
        PipeReader      reader(out_pipe_);
        uint32_t        msg = kMessage_Unknown;

        result &= reader.Pod(msg);

        switch (msg)
        {
        case kMessage_Finish:
            std::cout << "Interproc client " << name_ << ": Finish message received!" << std::endl;
            is_finished_ = true;
            break;
        case kMessage_Result:
        case kMessage_Error:
        {
            std::string error_msg;
            result &= reader.String(error_msg);
            std::cout << "Interproc client " << name_ << ": Error message received - " << error_msg << std::endl;
        }

        default:
            result = false;
            break;
        }
    }

    return result;
}
