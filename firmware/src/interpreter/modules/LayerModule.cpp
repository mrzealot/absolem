#include "LayerModule.h"
#include "../Interpreter.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_LAYERMODULE))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    // TODO: layers don't "see" below them
    // and there's no notion of transparency yet
    bool LayerModule::onMapKey(VirtualKey& k) {
        Byte multiplier = stack.top();
        k = multiplier * offset + k;
        if (oneshotActive) {
            pop();
            oneshotActive = false;
        }
        return true;
    }

    Byte LayerModule::onMapKeyPriority() {
        return defaultPriority;
    }

    void LayerModule::push(Byte layer) {
        if (locked) return;
        stack.push(layer);
    }

    void LayerModule::pop() {
        if (locked) return;
        if (stack.size() > 1) {
            stack.pop();
        }
    }

    void LayerModule::oneshot(Byte layer) {
        if (locked) return;
        push(layer);
        oneshotActive = true;
    }

    void LayerModule::toggleLock() {
        // upon unlocking, we reset the layer initiating the lock
        // we also clear oneshot state in case it was a oneshot
        if (locked) {
            pop();
            oneshotActive = false;
        }
        locked = !locked;
    }
    
} // namespace absolem
