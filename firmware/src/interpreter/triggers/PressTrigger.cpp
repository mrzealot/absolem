#include "PressTrigger.h"

namespace absolem {

    TriggerState PressTrigger::state(Interpreter& interpreter) {
        if (std::get<1>(interpreter.getEvent(0)) == press) {
            return TriggerState::YES;
        } else {
            return TriggerState::NO;
        }
    }
    
} // namespace absolem
