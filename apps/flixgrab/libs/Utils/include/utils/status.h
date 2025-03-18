#pragma once

#include <iostream>
#include <string>

namespace utils {
    
class Status {
public:
    /// Error codes for the packager APIs.
    enum Code {
        // Not an error; returned on success
        Success = 0,
        // Unknown error.  An example of where this error may be returned is
        // errors raised by APIs that do not return enough error information
        // may be converted to this error.
        Unknown,
        // The operation was cancelled (typically by the caller).
        Cancelled,
        // Internal errors. Some invariants have been broken.
        InternalError,
        // Client specified an invalid argument. INVALID_ARGUMENT indicates
        // arguments that are problematic regardless of the state of the system
        // (e.g. a malformed file name).
        InvalidArgument,
        // Specified Invalid url.
        InvalidUrl,
        //Http response error
        HttpError,
        //Http response error
        HttpError403Forbidden,
        //Server connection error;
        HttpError416WrongRange,
        //server cannot serve the requested ranges;
        ConnectionError,
        //Internal buffer overflow.
        BufferOverflow,
        // The operation timed out.
        Timeout,
        NoMemory,
        // Cannot open/read or write to file.
        FileFailure,
        EndOfFile,
    };

public:
    /// Creates a "successful" status.
    Status() : error_code_(Success) {}

    /// Create a status with the specified code, and error message.
    /// If "error_code == error::OK", error_message is ignored and a Status
    /// object identical to Status::OK is constructed.
    Status(Code error_code, const std::string& error_message = "")
        : error_code_(error_code) {
        if (!ok())
            error_message_ = error_message;
    }

    Status(Status&& other) 
        : error_message_(std::move(other.error_message_))
        , error_code_(std::move(other.error_code_)) {
    }

    Status(const Status& other)
        : error_message_(other.error_message_)
        , error_code_(other.error_code_) {
    }

    ~Status() {}

    /// @name Some pre-defined Status objects.
    /// @{
    static const Status SUCCESS;  // Identical to 0-arg constructor.
    static const Status UNKNOWN;
    /// @}

    /// Store the specified error in this Status object.
    /// If "error_code == error::OK", error_message is ignored and a Status
    /// object identical to Status::OK is constructed.
    void SetError(Code error_code, const std::string& error_message) {
        if (error_code == Success) {
            Clear();
        }
        else {
            error_code_ = error_code;
            error_message_ = error_message;
        }
    }

    /// If "ok()", stores "new_status" into *this.  If "!ok()", preserves
    /// the current "error_code()/error_message()",
    ///
    /// Convenient way of keeping track of the first error encountered.
    /// Instead of:
    ///   if (overall_status.ok()) overall_status = new_status
    /// Use:
    ///   overall_status.Update(new_status);
    Status& Update(const Status& new_status) {
        if (ok())
            *this = new_status;

        return *this;
    }

    Status& Update(Status&& new_status) {
        if (ok())
            *this = std::move(new_status);

        return *this;
    }

    /// Clear this status object to contain the OK code and no error message.
    void Clear() {
        error_code_ = Success;
        error_message_ = "";
    }

    bool ok() const { return error_code_ == Success; }
    Code error_code() const { return error_code_; }
    const std::string& error_message() const { return error_message_; }

    bool operator==(const Status& x) const {
        return error_code_ == x.error_code();
    }
    bool operator!=(const Status& x) const { return !(*this == x); }

    operator bool() const {
        return ok();
    }

    Status& operator=(const Status& other) {
        error_message_ = other.error_message_;
        error_code_ = other.error_code_;
        return *this;
    }
    Status& operator=(Status&& other) {
        error_message_ = std::move(other.error_message_);
        error_code_ = std::move(other.error_code_);
        return *this;
    }

    /// @return true iff this has the same error_code as "x", i.e., the two
    ///         Status objects are identical except possibly for the error
    ///         message.
    bool Matches(const Status& x) const { return error_code_ == x.error_code(); }

    /// @return A combination of the error code name and message.
    std::string ToString() const;

    void Swap(Status* other) {
        Code error_code = error_code_;
        error_code_ = other->error_code_;
        other->error_code_ = error_code;
        error_message_.swap(other->error_message_);
    }

private:
    Code                error_code_;
    std::string         error_message_;

    // Not using DISALLOW_COPY_AND_ASSIGN here intentionally to allow the compiler
    // generated copy constructor and assignment operator.
};

}  // namespace utils

