#pragma once

#include <string>

namespace ipc {

    class Status
    {
    public:
        enum Value
        {
            kSuccess = 0,
            kInternalError,
            kCommandError,
            kCommandFinish,
            kCommandUnknown,
        };
    public:
        Status(Value status = kSuccess) : status_(status) {}
        Status(Value status, const std::string& message) : status_(status), message_(message) {}


        operator bool() const { return status_ == kSuccess; }

        const std::string&  message() const { return message_; }
        Status              status() const { return status_; }

        Status&    operator= (Value status) { status_ = status; return *this; }

        bool       operator== (const Value& status) const { return (status_ == status); }
        bool       operator!= (const Value& status) const { return (status_ != status); }

        bool        Ok() const { return status_ == kSuccess; }

    private:
        Value               status_;
        std::string         message_;

    };

}

