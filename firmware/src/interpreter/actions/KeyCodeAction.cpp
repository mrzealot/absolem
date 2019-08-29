#include "KeyCodeAction.h"
#include "../modules/ReporterModule.h"
#include "../modules/CacheModule.h"
#include "../../common/binary.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_KEYCODEACTION))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    void KeyCodeAction::operator()(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("KeyCodeAction::operator(): %s ["BINARY_PATTERN"][%d]...", press ? "pressing" : "releasing", BINARY(mods), key);)
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        if (press) {
            if (key) {
                DD(interpreter.getController()->debug("KeyCodeAction::operator(): press");)
                r->press(key);
            }
            if (oneshot) {
                DD(interpreter.getController()->debug("KeyCodeAction::operator(): oneshot");)
                r->oneshot(mods);
            } else {
                DD(interpreter.getController()->debug("KeyCodeAction::operator(): modify");)
                r->modify(mods);
            }
        } else {
            if (key) {
                DD(interpreter.getController()->debug("KeyCodeAction::operator(): release");)
                r->release(key);
            }
            if (!oneshot) {
                DD(interpreter.getController()->debug("KeyCodeAction::operator(): unmodify");)
                r->unmodify(mods);
            }
        }
        Action::operator()(interpreter);
    }
    
} // namespace absolem
