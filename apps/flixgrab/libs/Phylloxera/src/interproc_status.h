#pragma once

#include <string>


class InterprocStatus
{
public:
    enum Status
    {
        kSuccess = 0,
        kInternalError,
        kCommandError,
        kCommandFinish,
        kCommandUnknown,
    };
public:
    InterprocStatus(Status status = kSuccess) : status_(status) {}
    InterprocStatus(Status status, const std::string& message) : status_(status), message_(message) {}


    operator bool() const { return status_ == kSuccess; }

    const std::string&  message() const { return message_; }
    Status              status() const { return status_; }

    InterprocStatus&    operator= (Status status) { status_ = status; return *this; }

private:
    Status              status_;
    std::string         message_;

};