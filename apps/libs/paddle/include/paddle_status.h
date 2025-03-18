#pragma once

#include <QtCore>

namespace paddle {

    enum {
        Code_Success = 0
    };

    struct Status {

        Status(int c = Code_Success, const QString& m = QString()) : code_(c), message_(m) {}

        bool            isOk() const { return code_ == Code_Success; }

        const QString&  message() const { return message_; }

        int             code() const { return code_; }

    private:
        int         code_;
        QString     message_;

    };
}


