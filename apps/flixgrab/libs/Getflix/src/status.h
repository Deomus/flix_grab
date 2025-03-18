#pragma once

#include <QtCore>

#include <error_codes.h>


class Status  {
public:
    Status()
        : code_(EStatusCode::Success)
        , subcode_(0)  {}

    Status(EStatusCode code, const QString& message, int subcode = 0)
        : code_(code)
        , message_(message)
        , subcode_(subcode) {}
    
    Status(EStatusCode code) :
        code_(code),
        subcode_(0) {
    }

    bool                ok() const { return code_ == EStatusCode::Success; }
    EStatusCode         code() const { return code_; }
    int                 subcode() const { return subcode_; }
    
    bool operator==(const Status& x) const {
        return code_ == x.code() && message_ == x.message();
    }

    bool operator!=(const Status& x) const { return !(*this == x); }
      
    operator bool() const {
        return ok();
    }

    /// @return true if this has the same error_code as "x", i.e., the two
    ///         Status objects are identical except possibly for the error
    ///         message.
    bool                matches(const Status& x) const { return code_ == x.code(); }

    const Status&       update(const Status& new_status) {
        if (ok()) {
            *this = new_status;
        }
        return *this;
    }

    const Status&       update(EStatusCode code, const QString& message = QString(), int subcode = 0) {
        if (ok()) {
            code_ = code;
            message_ = message;
            subcode_ = subcode;
        }

        return *this;
    }

    /// Clear this status object to contain the OK code and no error message.
    void                clear() {
        code_ = EStatusCode::Success;
        message_ = "";
        subcode_ = 0;
    }

    const QString&      message() const { return message_; }

private:
    EStatusCode             code_;
    int                     subcode_;
    QString                 message_;
};

Q_DECLARE_METATYPE(Status)
