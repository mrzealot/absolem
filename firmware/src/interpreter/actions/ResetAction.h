#ifndef RESET_ACTION_H
#define RESET_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class ResetAction : public Action {
        public:
        ResetAction() {}
        virtual void operator()(Interpreter& interpreter);
    };

} // namespace absolem

#endif