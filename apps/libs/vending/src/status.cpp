#include "vending/status.h"

#include <licensing.h>

namespace vending {
    void Status::update(Code c, const QString& m /*= QString()*/) {
        if (isOk()) {
            code_ = c;
            message_ = m;
        }
    }

    void Status::update(const Status& s) {
        if (isOk()) {
            code_ = s.code_;
            message_ = s.message_;
        }
    }

    void Status::updateInternal(int code, const QString& message /*= QString()*/) {
        switch (code) {
        case STATUS_SUCCESS:
            break;
        case STATUS_GENERIC_ERROR:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Generic error is occurred.");
            break;
        case STATUS_OUT_OF_MEMORY:
            update(Status::Code::OutOfMemory,
                   !message.isEmpty() ? message : "Memory is out.");
            break;
        case STATUS_FIELD_NOT_FOUND:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "License field not found.");
            break;
        case STATUS_BUFFER_TOO_SMALL:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Buffer too small.");
            break;
        case STATUS_INVALID_XML:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Invalid template xml.");
            break;
        case STATUS_INVALID_LICENSE_KEY:
            update(Status::Code::InvalidLicenseKey,
                   !message.isEmpty() ? message : "License key is invalid.");
            break;
        case STATUS_INVALID_KEY_ENCODING:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Wrong key encoding.");
            break;
        case STATUS_INVALID_PARAM:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Invalid param.");
            break;
        case STATUS_INVALID_SIGNATURE_SIZE:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Wrong signature size.");
            break;
        case STATUS_UNSUPPORTED_VERSION:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Unsupported version.");
            break;
        case STATUS_NET_ERROR:
            update(Status::Code::NetError,
                   !message.isEmpty() ? message : "Network error.");
            break;
        case STATUS_INVALID_HARDWARE_ID:
            update(Status::Code::InvalidHardwareId,
                   !message.isEmpty() ? message : "Hardware id does not match this computer.");
            break;
        case STATUS_LICENSE_EXPIRED:
            update(Status::Code::LicenseExpired,
                   !message.isEmpty() ? message : "License is expired.");
            break;
        case STATUS_INVALID_ACTIVATION_KEY:
            update(Status::Code::InvalidActivationKey,
                   !message.isEmpty() ? message : "Activation key is invalid.");
            break;
        default:
            update(Status::Code::GenericError,
                   !message.isEmpty() ? message : "Unknown error.");
            break;
        }
    }

}

