#ifndef MULTI_TRIGGER_H
#define MULTI_TRIGGER_H

#include "Trigger.h"
#include "../Interpreter.h"

namespace absolem {

    class MultiTrigger : public Trigger {
        public:
        MultiTrigger(Time term, Byte taps, bool hold)
        : term(term), taps(taps), hold(hold) {}
        virtual TriggerState state(Interpreter& interpreter);

        private:
        Time term;
        Byte taps;
        bool hold;
    };

} // namespace absolem

#endif