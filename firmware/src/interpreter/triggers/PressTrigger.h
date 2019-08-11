#ifndef PRESS_TRIGGER_H
#define PRESS_TRIGGER_H

#include "Trigger.h"
#include "../Interpreter.h"

namespace absolem {

    class PressTrigger : public Trigger {
        public:
        PressTrigger(bool p) : press(p) {}
        virtual TriggerState state(Interpreter& interpreter);

        private:
        bool press;
    };

} // namespace absolem

#endif