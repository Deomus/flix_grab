#pragma once 

#include <memory>
#include <vector>
#include "operation.h"

namespace recode {

    //Run Operation in parralell;
    class ConcurrentOp : public Operation {
    public:
        ConcurrentOp( const std::vector<std::shared_ptr<Operation>>& ops );
        virtual ~ConcurrentOp();

        Status              Do(OperationListener* listener) override;
        void		        Cancel() override;

    public:
        std::vector<std::shared_ptr<Operation>>     ops() const { return ops_; }

    private:
        std::vector<std::shared_ptr<Operation>>             ops_;
    };
}
