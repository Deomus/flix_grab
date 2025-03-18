#pragma once

#include <functional>
#include <cstdint>

#include "buffer_writer.h"
#include "buffer_reader.h"

enum Message
{
    kMessage_Unknown = 0,
    kMessage_Invoke,        //Payload as Audio Configuration;
    kMessage_Result,             //int32_t as operation Result;
    kMessage_Error,             //int32_t as operation Result;
    kMessage_Finish,             //Cancel Process Messages;

};

typedef std::function<bool(BufferWriter& buffer)>       serialize_fn;
typedef std::function<bool(BufferReader& buffer)>       deserialize_fn;

typedef std::function<void()>                           invoke_fn;

typedef std::function<bool(const BufferReader& reader, BufferWriter& writer)>       Invoke_fn;



class Invoker
{
public:
    //Invoke function by proc_num;
    virtual bool    Invoke(uint32_t proc_num, serialize_fn args_fn, deserialize_fn result_fn) = 0;
    virtual bool    Cancel() = 0;

protected:
    virtual ~Invoker() {}
};
