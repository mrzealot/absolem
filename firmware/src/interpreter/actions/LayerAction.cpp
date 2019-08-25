#include "LayerAction.h"
#include "../modules/LayerModule.h"
#include "../modules/CacheModule.h"

#if defined(DEBUG) && 1
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    void LayerAction::operator()(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("LayerAction::operator(): %s [%d]...", press ? "pressing" : "releasing", layer);)
        auto l = (LayerModule*) interpreter.getModule("layer");
        if (press) {
            l->push(layer);
        } else {
            l->pop();
        }
        interpreter.complete(1);
        if (counterpart.first) {
            auto c = (CacheModule*) interpreter.getModule("cache");
            c->set(interpreter.getPhysicalKey(), counterpart);
        }
    }
    
} // namespace absolem
