#pragma once

#include <QtCore>

namespace vending {
    
    struct Status {
        friend class LicensePrivate;
        enum Code{
            Success = 0,
            GenericError,
            OutOfMemory,
            InvalidLicenseKey,
            UnsupportedVersion,
            NetError,
            InvalidHardwareId,
            LicenseExpired,
            InvalidActivationKey,
            IOError

        };

        Status(Code c = Success, const QString& m = QString()) : code_(c), message_(m) {}

        bool                isOk() const { return code_ == Success; }

        const QString&      message() const { return message_; }

        Code                code() const { return code_; }

        void                update(Code c, const QString& m = QString());

        void                update(const Status& s);
        
        bool                operator != (const Status& s) const {
            return s.code_ != code_;
        }

        bool                operator == (const Status& s) const {
            return s.code_ == code_;
        }

    private:
        void                updateInternal(int c, const QString& m = QString());

    private:
        Code            code_;
        QString         message_;

    };
}

