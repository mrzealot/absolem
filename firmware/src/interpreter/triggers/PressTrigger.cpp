#include "PressTrigger.h"

#if defined(DEBUG) && 1
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    TriggerState PressTrigger::state(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("PressTrigger::state: State is checked against %s...", press ? "press" : "release");)
        if (std::get<1>(interpreter.getEvent(0)) == press) {
            return TriggerState::YES;
        } else {
            return TriggerState::NO;
        }
    }
    
} // namespace absolem
