#include "ConsumerAction.h"
#include "../modules/ReporterModule.h"
#include "../modules/CacheModule.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_CONSUMERACTION))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    void ConsumerAction::operator()(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("ConsumerAction::operator(): %s %d...", press ? "pressing" : "releasing", usage);)
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        if (press) {
            r->pressConsumer(usage);
        } else {
            r->releaseConsumer();
        }
        Action::operator()(interpreter);
    }
    
} // namespace absolem
