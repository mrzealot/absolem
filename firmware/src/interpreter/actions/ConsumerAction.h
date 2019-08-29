#ifndef CONSUMER_ACTION_H
#define CONSUMER_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class ConsumerAction : public Action {
        public:
        ConsumerAction(bool p, UsageCode u) : press(p), usage(u) {}
        ConsumerAction(bool p, UsageCode u, Rule c) : Action(c), press(p), usage(u) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        bool press;
        UsageCode usage;
    };

} // namespace absolem

#endif