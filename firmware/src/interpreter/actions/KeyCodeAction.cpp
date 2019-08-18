#include "KeyCodeAction.h"
#include "../modules/ReporterModule.h"
#include "../modules/CacheModule.h"
#include "../../common/binary.h"

#if defined(DEBUG) && 1
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    void KeyCodeAction::operator()(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("KeyCodeAction::operator(): %s ["BINARY_PATTERN"][%d]...", press ? "pressing" : "releasing", BINARY(mods), key);)
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        if (press) {
            r->modify(mods);
            r->press(key);
        } else {
            r->unmodify(mods);
            r->release(key);
        }
        interpreter.complete(1);
        if (counterpart.first) {
            DD(interpreter.getController()->debug("KeyCodeAction::operator(): Setting counterpart");)
            auto c = (CacheModule*) interpreter.getModule("cache");
            c->set(interpreter.getPhysicalKey(), counterpart);
        }
    }
    
} // namespace absolem
