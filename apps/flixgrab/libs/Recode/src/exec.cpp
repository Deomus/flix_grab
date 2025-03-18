#include "exec.h"
using namespace recode;

using shared_op = Exec::shared_op;

#define         MINIMUM_SLEEP_TIME              (5)

Exec::Exec(Exec::shared_op op, ExecListener* listener) 
    : listener_(listener) {
    operations_.push_back(std::make_pair(op, 1.f));

    run_ = std::async(std::launch::async, &Exec::Run, this);
    //thread_ = std::thread(&Exec::Run, this);
}

Exec::Exec(const std::vector<shared_op>& operations, ExecListener* listener) 
    : listener_(listener) {
    for (auto op : operations) {
        operations_.push_back(std::make_pair(op, 1.f / (float)operations_.size()));
    }
    run_ = std::async(std::launch::async, &Exec::Run, this);
    //thread_ = std::thread(&Exec::Run, this);
}

Exec::Exec(const std::vector<std::pair<shared_op, float>>& operations, ExecListener* listener) : listener_(listener) {
    operations_ = operations;
    run_ = std::async(std::launch::async, &Exec::Run, this);
    //thread_ = std::thread(&Exec::Run, this);
}

Exec::~Exec() {
    Cancel();
    run_.get();
    /*if (thread_.joinable())
        thread_.join();*/
}

void Exec::Pause(bool paused /*= true*/) {
    if (paused != paused_) {
        paused_ = paused;
        if (!paused_)
            cv_.notify_all();
    }
}

void Exec::Cancel() {
    cancelled_ = true;
    Pause(false);
}

void Exec::OnProgress(double progress) {
    std::unique_lock<std::mutex> locker(mutex4Pause_); // , std::try_to_lock);
    
    if (timestamp_ != std::chrono::system_clock::time_point()) {
        auto duration = std::chrono::system_clock::now() - timestamp_;
        //std::cout << "CPU Load: " << cpu_usage_.load() << " Sleep: " << cpu_usage_.sleepForBalance(maximum_cpu_usage_) << std::endl;
        auto sleep_balance = cpu_usage_.sleepForBalance(maximum_cpu_usage_);

        duration *= slow_factor_;

        if (sleep_balance > MINIMUM_SLEEP_TIME) 
            duration += std::chrono::milliseconds(sleep_balance);
        
        if (duration > std::chrono::milliseconds(0))
            std::this_thread::sleep_for(duration);
        cpu_usage_.reset();
    }
    
    
    
    if (paused_) {
        //Pause Signal;
        listener_->OnPaused(current_op_.get(), paused_);
        while(paused_) // Spurious wakeup protect
            cv_.wait(locker);
        
        //Resume signal
        listener_->OnPaused(current_op_.get(), paused_);
    }

    //Cancel All Ops;
    if (cancelled_) {
        for (auto op : operations_) {
            if (!op.first->is_cancelled())
                op.first->Cancel();
        }
    }

    //xx.x progress changes;
    int current = (int)((progress_ + progress * progress_factor_) * 1000);
    if (current != progress_cache_) {
        progress_cache_ = current;
        listener_->OnProgress(current_op_.get(), current / 1000.0f);
    }

    timestamp_ = std::chrono::system_clock::now();
}

void Exec::Run() {
    
    std::unique_lock<std::mutex>  locker(mutex_);
    progress_ = 0;
    //std::chrono::system_clock::time_point()
    //timestamp_ = std::chrono::system_clock::now();
    timestamp_ = std::chrono::system_clock::time_point();

    for (auto op : operations_) {
        progress_factor_ = op.second;
        current_op_ = op.first;
        cpu_usage_.reset();
        status_ = current_op_->Do(this);
        
        if (!status_.ok() || cancelled_)
            break;

        progress_ += op.second;
    }
    
    if (cancelled_)
        listener_->OnCancelled(current_op_.get());
    else if (status_.ok())
        listener_->OnFinished();
    else
        listener_->OnError(current_op_.get(), status_);

    current_op_.reset();
}
