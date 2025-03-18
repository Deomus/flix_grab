#pragma once
#pragma once

#include <QtCore>

//namespace inapppurchase {


    struct InAppStatus {

        enum Code{
            Succeeded = 0,

            AlreadyPurchased = 1,
            NotPurchased = 2,
            NetworkError = 3,
            ServerError = 4,
            OtherError,
            Aborted,
            InternalError,

        };


        InAppStatus(Code c = Succeeded, const QString& m = QString()) : code_(c), message_(m) {}

        //InAppStatus(HRESULT result) : code_(result), message_() {}

        bool            isOk() const { return code_ == Succeeded; }

        const QString&  message() const { return message_; }

        int             code() const { return code_; }

        void            update(Code c, const QString& m) {
            if (isOk()) {
                code_ = c;
                message_ = m;
            }
        }

        void            update(const InAppStatus& s) {
            if (isOk() && !s.isOk()) {
                code_ = s.code_;
                message_ = s.message_;
            }
        }

        QString         toString() const { return "TODO:"; }

    private:
        Code        code_;
        QString     message_;

    };

    Q_DECLARE_METATYPE(InAppStatus);
//}
