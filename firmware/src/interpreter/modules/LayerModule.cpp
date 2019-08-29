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
        VirtualKey oldK = k;
        k = multiplier * offset + k;
        DD(interpreter->getController()->debug("LayerModule::onMapKey: %d --> %d (physical = %d)", oldK, k, interpreter->getPhysicalKey());)
        if (oneshotActive && oneshotActive != interpreter->getPhysicalKey()) {
            DD(interpreter->getController()->debug("LayerModule::onMapKey: oneshot popped");)
            pop();
            oneshotActive = 0;
        }
        return true;
    }

    Byte LayerModule::onMapKeyPriority() {
        return defaultPriority;
    }

    void LayerModule::push(Byte layer) {
        if (locked) return;
        DD(interpreter->getController()->debug("LayerModule::push: %d");)
        stack.push(layer);
    }

    void LayerModule::pop() {
        if (locked) return;
        if (stack.size() > 1) {
            DD(interpreter->getController()->debug("LayerModule::pop: %d", stack.top());)
            stack.pop();
        }
    }

    // oneshot layers need the key that activated them
    // because the release of the same key should NOT
    // consume the oneshot, only the next event
    void LayerModule::oneshot(Byte layer) {
        if (locked) return;
        push(layer);
        oneshotActive = interpreter->getPhysicalKey();
        DD(interpreter->getController()->debug("LayerModule::oneshot: after push with physical = %d", oneshotActive);)
    }

    void LayerModule::toggleLock() {
        // upon unlocking, we reset the layer initiating the lock
        // we also clear oneshot state in case it was a oneshot
        if (locked) {
            pop();
            oneshotActive = 0;
        }
        locked = !locked;
    }
    
} // namespace absolem
