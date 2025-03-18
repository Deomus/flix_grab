#pragma once
#include <assert.h>

#include "pipe.h"
#include "status.h"

namespace ipc {
      

    class Storage {
    public:
        virtual bool    Serialize(BufferWriter&) = 0;
        virtual bool    Deserialize(BufferReader&) = 0;
    };

    class Client
    {
    public:

        Client(const std::string& name);
        ~Client();
              

        //Invoke function by proc_num;
        //bool                Invoke(uint32_t proc_num, serialize_fn args_fn, deserialize_fn result_fn);
        bool                Invoke(uint32_t proc_num, Storage* storage);
        //Send cansel signal to server;
        bool                Cancel();

        const Status&       status() const { return status_; }

        const std::string&  name() const { return name_; }

    private:
        Client(const Client&);
        void operator=(const Client&);

    private:
        Pipe                in_pipe_;
        Pipe                out_pipe_;
        bool                is_finished_;
        std::string         name_;
        Status              status_;
    };

}

