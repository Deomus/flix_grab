#pragma once

#include <vector>
#include "interproc_status.h"
#include "invoker.h"
#include "pipe.h"

class InterprocServer
{
    struct Procedure
    {
        Invoke_fn           invoke;

        Procedure(Invoke_fn _invoke)
            :invoke(_invoke)
        {}

        Procedure() {}
        
        bool        is_valid() const { return (bool)invoke; }
    };

public:
    InterprocServer();
    ~InterprocServer();


    //Register new invoke function;
    void            RegisterFunction(uint32_t proc_num, Invoke_fn invoke);

    //Clear All List;
    void            ClearFunctions();

    //Start
    bool            Start(const std::string& name, size_t inbound_size, size_t outbound_size);

    bool            Cancel();

    void            Stop();

    //Process one Invoke Request;
    InterprocStatus ProcessInvoke();

private:
    std::vector<Procedure>          invoker_map_;
    Pipe                            in_pipe_;
    Pipe                            out_pipe_;
    std::string                     name_;
};


