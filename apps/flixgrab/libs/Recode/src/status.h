#pragma once


#include <iostream>
#include <string>

namespace recode {
    namespace error {

        /// Error codes for the packager APIs.
        enum Code {
            // Not an error; returned on success
            OK,

            // Unknown error.  An example of where this error may be returned is
            // errors raised by APIs that do not return enough error information
            // may be converted to this error.
            UNKNOWN,
                    
            // Client specified an invalid argument. INVALID_ARGUMENT indicates
            // arguments that are problematic regardless of the state of the system
            // (e.g. a malformed file name).
            INVALID_ARGUMENT,

            // Operation is not implemented or not supported/enabled.
            UNIMPLEMENTED,
            
            SHAKA_ERROR,
            
            INJECTION_ERROR,

            ENCODER_ERROR,
            CDM_ERROR,
            HTTP_ERROR,
            NO_MEMORY,
            FILE_ERROR,
            MUXER_WRITE_ERROR,

            CONV_ERROR,
            CONV_SUBTITLES_ERROR,
            RECODE_SUBTITLES_ERROR,
            DECOMPRESS_SUBTITLES_ERROR,
            WRITE_METADATA_ERROR,
        };

    }  // namespace error

    class Status {
    public:
        /// Creates a "successful" status.
        Status() : error_code_(error::OK), subsystem_code_(0) {}

        /// Create a status with the specified code, and error message.
        /// If "error_code == error::OK", error_message is ignored and a Status
        /// object identical to Status::OK is constructed.
        Status(error::Code error_code, const std::string& error_message = "", int subsystem_code = 0)
            : error_code_(error_code) {
            if (!ok()) {
                error_message_ = error_message;
                subsystem_code_ = subsystem_code;
            }
        }

        
        ~Status() {}

        /// @name Some pre-defined Status objects.
        /// @{
        static const Status OK;  // Identical to 0-arg constructor.
        static const Status UNKNOWN;
        /// @}

        /// Store the specified error in this Status object.
        /// If "error_code == error::OK", error_message is ignored and a Status
        /// object identical to Status::OK is constructed.
        void SetError(error::Code error_code, const std::string& error_message, int subsystem_code = 0) {
            if (error_code == error::OK) {
                Clear();
            }
            else {
                error_code_ = error_code;
                error_message_ = error_message;
                subsystem_code_ = subsystem_code;
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
        Status Update(const Status& new_status) {
            if (ok())
                *this = new_status;

            return *this;
        }

        /// Clear this status object to contain the OK code and no error message.
        void Clear() {
            error_code_ = error::OK;
            error_message_ = "";
            subsystem_code_ = 0;
        }

        bool ok() const { return error_code_ == error::OK; }
        error::Code     error_code() const { return error_code_; }
        int             subsystem_code() const { return subsystem_code_; }

        const std::string& error_message() const { return error_message_; }

        bool operator==(const Status& x) const {
            return error_code_ == x.error_code() && error_message_ == x.error_message();
        }
        bool operator!=(const Status& x) const { return !(*this == x); }

        operator bool() const { return ok(); }

        /// @return true if this has the same error_code as "x", i.e., the two
        ///         Status objects are identical except possibly for the error
        ///         message.
        bool Matches(const Status& x) const { return error_code_ == x.error_code(); }
           
        void Swap(Status* other) {
            error::Code error_code = error_code_;
            error_code_ = other->error_code_;
            other->error_code_ = error_code;
            error_message_.swap(other->error_message_);
        }

    private:
        error::Code error_code_;
        //system code of submodule;
        int         subsystem_code_;
        std::string error_message_;

        // Not using DISALLOW_COPY_AND_ASSIGN here intentionally to allow the compiler
        // generated copy constructor and assignment operator.
    };
      
  
}  // namespace recode

