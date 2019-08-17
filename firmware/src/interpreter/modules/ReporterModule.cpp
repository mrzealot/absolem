#include "ReporterModule.h"
#include "../Interpreter.h"

namespace absolem {

    void ReporterModule::onAfterTick(Interpreter& interpreter) {
        if (dirty) {
            KeyCode arr[6];
            Byte i = 0;
            for (auto k : keys) {
                arr[i] = k;
                if (i >= 5) break;
            }
            Modifiers m = currentMods | oneshotMods;
            oneshotMods = 0;
            interpreter.getController()->report(m, arr);
        }
    }

    Byte ReporterModule::onAfterTickPriority() {
        return defaultPriority;
    }

    void ReporterModule::press(KeyCode key) {
        keys.insert(key);
    }

    void ReporterModule::release(KeyCode key) {
        keys.erase(key);
    }

    void ReporterModule::modify(Modifiers mods) {
        currentMods |= mods;
    }

    void ReporterModule::unmodify(Modifiers mods) {
        currentMods &= ~mods;
    }

    
} // namespace absolem
