#pragma once
#include <assert.h>
#include "invoker.h"
#include "pipe.h"


class InterprocClient : public Invoker
{
public:

    InterprocClient();
    ~InterprocClient();

    //Start client;
    bool                Start(const std::string& name);

    //Stop Client;
    void                Stop();

    //Invoke function by proc_num;
    virtual bool        Invoke(uint32_t proc_num, serialize_fn args_fn, deserialize_fn result_fn);
    //Send cansel signal to server;
    virtual bool        Cancel();

private:
    InterprocClient(const InterprocClient&);
    void operator=(const InterprocClient&);

private:
    Pipe                in_pipe_;
    Pipe                out_pipe_;
    bool                is_finished_;
    std::string         name_;
};