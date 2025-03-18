#pragma once

#include <stdint.h>


namespace recode {
    /// This class listens to progress updates events.
    class OperationListener {
    public:
        virtual ~OperationListener() {}

        /// Called when there is a progress update.
        /// @param progress is the current progress metric, ranges from 0 to 1.
        virtual void OnProgress(double progress) = 0;

    protected:
        OperationListener() {}

    private:
        OperationListener(const OperationListener&);
        void operator=(const OperationListener&);
    };

}

