#pragma once

#include <vector>
#include <thread>

#include "status.h"
#include "pipe.h"

namespace ipc {

    typedef bool(*ServerFunc)(BufferReader& reader, BufferWriter& writer);

    class Server
    {
      
    public:
        Server(const std::string& name, const std::vector<ServerFunc>& table, size_t inbound_size = 16 * 1024, size_t outbound_size = 16 * 1024);
        ~Server();

    public:
        bool                Cancel();

    public:
        const Status&       status() const { return status_; }

        const std::string&  name() const { return name_; }

    protected:
        virtual void        Start() {};
        virtual void        Stop() {};

    private:
        //Process one Invoke Request;
        Status              ProcessInvoke();
        void                Run();


    private:
        std::vector<ServerFunc>         table_;
        Pipe                            in_pipe_;
        Pipe                            out_pipe_;
        std::string                     name_;

        std::thread                     thread_;

        Status                          status_;
    };


}