#pragma once

#include <thread>
#include <condition_variable>
#include <memory>
#include <future>
#include <common_config.h>

#include "operation.h"
#include "exec_listener.h"
#include "cpu_usage.h"


namespace recode {
    class Exec : public OperationListener
    {
    public:
        using shared_op = std::shared_ptr<Operation>;

    public:
        Exec(shared_op op, ExecListener* listener);

        Exec(const std::vector<shared_op>& operations, ExecListener* listener);
        Exec(const std::vector<std::pair<shared_op, float>>& operations
            , ExecListener* listener);

        virtual ~Exec();

    public:
        const Status&           status() const { return status_; }

        void                    set_slow_factor(float factor) { slow_factor_ = factor; }
        float                   slow_factor() const { return slow_factor_; }

        void                    set_maximum_cpu_usage(float factor) { maximum_cpu_usage_ = factor; }
        float                   maximum_cpu_usage() const { return maximum_cpu_usage_; }

    public:

        void                    Pause(bool paused = true);
        void                    Cancel();

    protected:
        //progress = [0, 1.0]
        virtual void            OnProgress(double progress);
        void                    Run();


    private:
        ExecListener*                                   listener_;
        shared_op                                       current_op_;
        float                                           progress_;
        float                                           progress_factor_;

        int                                             progress_cache_;
        Status                                          status_;

    private:
        std::chrono::system_clock::time_point           timestamp_;
        float                                           slow_factor_ = 0;

    private:
        //threading;
        bool                                            paused_ = false;
        bool                                            cancelled_ = false;

        std::future<void>                               run_;

        //std::thread                                     thread_;
        std::condition_variable                         cv_;
        std::mutex                                      mutex_;
        std::mutex                                      mutex4Pause_;

        std::vector<std::pair<shared_op, float>>        operations_;

        CpuUsage                                        cpu_usage_;

        float                                           maximum_cpu_usage_ = DEFAULT_MAXIMUM_CPU_USAGE;
    };

}

