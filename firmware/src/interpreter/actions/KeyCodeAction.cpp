#include "KeyCodeAction.h"
#include "../modules/ReporterModule.h"

namespace absolem {

    void KeyCodeAction::operator()(Interpreter& interpreter) {
        // dummy stuff
        interpreter.getController()->debug("Something should happen now...");
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        if (press) {
            r->press(key);
        } else {
            r->release(key);
        }
        interpreter.complete(1);
    }
    
} // namespace absolem
