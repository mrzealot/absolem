#include "LayerAction.h"
#include "../modules/LayerModule.h"
#include "../modules/CacheModule.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_LAYERACTION))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    void LayerAction::operator()(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("LayerAction::operator(): %s%s [%d]...", press ? "pressing" : "releasing", oneshot ? "(oneshot)" : "", layer);)
        auto l = (LayerModule*) interpreter.getModule("layer");
        if (press) {
            if (oneshot) {
                l->oneshot(layer);
            } else {
                l->push(layer);
            }
        } else {
            l->pop();
        }
        Action::operator()(interpreter);
    }
    
} // namespace absolem
