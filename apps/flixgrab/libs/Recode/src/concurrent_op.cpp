#include "concurrent_op.h"
#include <future>
#include <assert.h>
#include <ppltasks.h>

using namespace recode;

//Concurrent Progress Dispatcher
class ConcurrentProgress {
public:
    ConcurrentProgress(OperationListener* listener) : listener_(listener) {}
    virtual ~ConcurrentProgress() {}

    /// Called when there is a progress update.
    /// @param progress is the current progress metric, ranges from 0 to 1.
    virtual void OnProgress(size_t index, double progress) {
        std::lock_guard<std::mutex>     locker(mtx_);

        if (progress_.size() <= index)
            progress_.resize(index + 1, 0);

        auto min_it = std::min_element(progress_.begin(), progress_.end());
        double prev_min = *min_it;
        size_t min_index = static_cast<size_t>(std::distance(progress_.begin(), min_it));
        progress_[index] = progress;
        if (min_index == index && listener_) {
            min_it = std::min_element(progress_.begin(), progress_.end());

            if (*min_it != prev_min)
                listener_->OnProgress(*min_it);
        }

    }
    
private:
    ConcurrentProgress(const ConcurrentProgress&);
    void operator=(const ConcurrentProgress&);

private:
    std::mutex                  mtx_;
    std::vector<double>         progress_;
    OperationListener*          listener_;
};

/// This class listens to progress updates events.
class ConcurrentListener : public OperationListener {
public:
    explicit ConcurrentListener(int op_index, ConcurrentProgress* progress) 
        : op_index_(op_index)
        , progress_(progress) {}

    /// Called when there is a progress update.
    /// @param progress is the current progress metric, ranges from 0 to 1.
    virtual void OnProgress(double progress) {
        if(progress_)
        progress_->OnProgress(op_index_, progress);
    }


private:
    ConcurrentListener(const ConcurrentListener&);
    void operator=(const ConcurrentListener&);

private:
    int                         op_index_;
    ConcurrentProgress*         progress_;
};


//////////////////////////////////////////////////////////////////////////////
ConcurrentOp::ConcurrentOp(const std::vector<std::shared_ptr<Operation>>& ops)
    :ops_(ops){
}

ConcurrentOp::~ConcurrentOp() {}

Status ConcurrentOp::Do(OperationListener * listener) {
    assert(!ops_.empty());

    ConcurrentProgress                  concurrent_progress(listener);

    std::vector<std::unique_ptr<ConcurrentListener>>     listeners;
    std::vector<concurrency::task<Status>>    tasks;

    //Start operations;
    for (size_t index = 0; index < ops_.size(); ++index) {
        listeners.emplace_back(
            std::make_unique<ConcurrentListener>(index, &concurrent_progress)
        );

        tasks.emplace_back(
            concurrency::create_task(std::bind(&Operation::Do, ops_[index], listeners[index].get()))
        );
    }

    Status status;

    auto ops_in_process = ops_;

    while (status.ok() && !tasks.empty()) {
        auto any_task = concurrency::when_any(tasks.begin(), tasks.end());

        status.Update(any_task.get().first);
        tasks.erase(tasks.begin()+any_task.get().second);
        ops_in_process.erase(ops_in_process.begin() + any_task.get().second);
    }

    if (!status.ok()) {
        for (auto op : ops_in_process)
            op->Cancel();

        auto all_task = concurrency::when_all(tasks.begin(), tasks.end());
        auto result = all_task.wait();
        assert(result == concurrency::completed);
    }

    return status;
}

//Status ConcurrentOp::Do(OperationListener * listener) {
//    assert(!ops_.empty());
//    
//    
//    ConcurrentProgress                  concurrent_progress(listener);
//
//    std::vector<std::unique_ptr<ConcurrentListener>>     listeners;
//    std::vector<std::future<Status>>    futures;
//
//    listeners.reserve(ops_.size());
//
//    for (size_t index = 0; index < ops_.size(); ++index ) {
//        listeners.emplace_back(std::make_unique<ConcurrentListener>(index, &concurrent_progress));
//    }
//    
//    //Start operations;
//    for (size_t index = 1; index < ops_.size(); ++index) {
//        futures.emplace_back(std::async(std::launch::async, 
//                                        &Operation::Do, ops_[index], listeners[index].get()));
//    }
//
//    Status status = ops_[0]->Do(listeners[0].get());
//
//    //Sync other operations;
//    for (auto& op_status : futures) {
//        status.Update(op_status.get());
//    }
//    
//    return status;
//}

void ConcurrentOp::Cancel() {
    Operation::Cancel();

    for (auto op : ops_) {
        op->Cancel();
    }
}
