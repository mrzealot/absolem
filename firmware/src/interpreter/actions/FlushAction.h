#ifndef FLUSH_ACTION_H
#define FLUSH_ACTION_H

#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class FlushAction : public Action {
        public:
        // no counterpart handling, as it should never be called directly
        FlushAction() {}
        virtual void operator()(Interpreter& interpreter);
    };

} // namespace absolem

#endif