#include "KeyCodeAction.h"
#include "../modules/ReporterModule.h"

#if defined(DEBUG) && 1
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    void KeyCodeAction::operator()(Interpreter& interpreter) {
        // dummy stuff
        DD(interpreter.getController()->debug("Something should happen now...");)
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        if (press) {
            r->press(key);
        } else {
            r->release(key);
        }
        interpreter.complete(1);
    }
    
} // namespace absolem
