#include "PressTrigger.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_PRESSTRIGGER))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    TriggerState PressTrigger::state(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("PressTrigger::state: State is checked against %s...", press ? "press" : "release");)
        if (std::get<1>(interpreter.getQueue().front()) == press) {
            return TriggerState::YES;
        } else {
            return TriggerState::NO;
        }
    }
    
} // namespace absolem
