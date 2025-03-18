#pragma once
#include "operation.h"

namespace recode {

    class ExecListener {
    public:
        virtual ~ExecListener() {}

        /// Called when there is a progress update. [0:100]
        virtual void OnProgress(Operation* current, float overall_progress) = 0;

        virtual void OnCancelled(Operation* current) = 0;
        virtual void OnPaused(Operation* current, bool is_paused) = 0;

        virtual void OnFinished(void) = 0;

        virtual void OnError(Operation* current, const recode::Status&) = 0;


    protected:
        ExecListener() {}
    };
}
