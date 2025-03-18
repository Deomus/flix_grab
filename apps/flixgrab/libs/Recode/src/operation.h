#pragma once
#include "operation_listener.h"
#include "status.h"
//Time consuming Shaka operation, supported pause (waiting)

namespace recode {
    class Operation {

    public:
        Operation() : cancelled_(false) {}
        virtual ~Operation() {}

    public:

        //If false returned then error is occured;
        virtual Status      Do(OperationListener* listener) = 0;

        virtual void        Cancel() { cancelled_ = true; }

    public:
        bool                is_cancelled() const { return cancelled_; }
        
    private:
             
        bool                    cancelled_;
    };
}

