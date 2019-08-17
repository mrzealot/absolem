#include "ResetAction.h"

namespace absolem {

    void ResetAction::operator()(Interpreter& interpreter) {
        interpreter.getController()->reset();
    }
    
} // namespace absolem
