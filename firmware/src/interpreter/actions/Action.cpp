#include "Action.h"
#include "../Interpreter.h"
#include "../modules/CacheModule.h"

namespace absolem {

    void Action::operator()(Interpreter& interpreter) {
        // basically noop
        // but if it has a counterpart, it can handle that
        if (counterpart.first) {
            auto c = (CacheModule*) interpreter.getModule("cache");
            c->set(interpreter.getPhysicalKey(), counterpart);
        }
    }
    
} // namespace absolem