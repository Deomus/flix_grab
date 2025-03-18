#pragma once 

#include <memory>
#include <mutex>
#include <vector>
#include <list>
#include "operation.h"

namespace recode {

    //Run Operation in parralell;
    class PartedOp : public Operation {
    public:
        explicit PartedOp(const std::vector<std::shared_ptr<Operation>>& ops) {
            for (auto o : ops) {
                ops_.push_back({ o, 1.0 });
            }
            total_weight_ = ops.size();
        }

        PartedOp(const std::vector<std::pair<std::shared_ptr<Operation>, double>>& weighted_ops) {
            for (auto o : weighted_ops) {
                ops_.push_back(o);
                total_weight_ += o.second;
            }
        }

        PartedOp() : total_weight_(0) {
        }

        //virtual ~PartedOp();


        /// This class listens to progress updates events.
        class PartedListener : public OperationListener {
        public:
            explicit PartedListener(OperationListener* listener)
                : listener_(listener) {}

            /// Called when there is a progress update.
            /// @param progress is the current progress metric, ranges from 0 to 1.
            virtual void OnProgress(double progress) {
                if(listener_)
                    listener_->OnProgress((progress_ + factor_ * progress) * total_factor_ );
            }

            void    set_total_factor(double factor) { total_factor_ = factor; }
            double  total_factor() const { return total_factor_; }

            void    set_factor(double factor) { factor_ = factor; }
            double  factor() const { return factor_; }

            void    set_progress(double progress) { progress_ = progress; }
            double  progress() const { return progress_; }


        private:
            PartedListener(const PartedListener&);
            void operator=(const PartedListener&);

        private:
            OperationListener*          listener_;
            double                      progress_ = 0;
            double                      factor_ = 0;
            double                      total_factor_ = 0;

        };


        Status              Do(OperationListener* listener) override {
            
            Status      status;
                

            /*double total_weight = 0;
            for (auto op : ops_)
                total_weight += op.second;*/

            auto proxy_listener = std::make_unique<PartedListener>(listener);

            while (!is_cancelled() && status.ok()) {
                {
                    //Lock ops list;
                    std::lock_guard<std::mutex>  locker(mtx_);
                    if (!ops_.empty()) {
                        current_ = ops_.front().first;
                        proxy_listener->set_total_factor(1.0 / total_weight_);
                        proxy_listener->set_factor(ops_.front().second);
                        ops_.pop_front();
                    }
                    else
                        break;
                }
                              
                status = current_->Do(proxy_listener.get());
                proxy_listener->set_progress(proxy_listener->progress() + proxy_listener->factor());
            }
         
            return status;
        }
        void		        Cancel() override {
            Operation::Cancel();
            std::lock_guard<std::mutex>  locker(mtx_);
            if (current_)
                current_->Cancel();
        }

    public:
        void                Add(std::shared_ptr<Operation> op, double weight) {
            std::lock_guard<std::mutex>     locker(mtx_);

            ops_.push_back({ op, weight });
            total_weight_ += weight;
        }

        void                Remove(std::shared_ptr<Operation> op) {
            std::lock_guard<std::mutex>     locker(mtx_);
            for (auto it = ops_.begin(); ops_.end() != it; ++it) {
                if (it->first == op) {
                    total_weight_ -= it->second;
                    ops_.erase(it);
                    break;
                }
            }
        }

  
    private:
        std::list<std::pair<std::shared_ptr<Operation>, double>>        ops_;

        std::shared_ptr<Operation>                                      current_;
        std::mutex                                                      mtx_;

        double                                                          total_weight_ = 0;
    };
}

