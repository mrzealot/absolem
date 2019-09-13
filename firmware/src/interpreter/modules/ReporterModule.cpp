#include "ReporterModule.h"
#include "../Interpreter.h"
#include "../../common/binary.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_REPORTERMODULE))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    bool ReporterModule::onAfterTick() {
        flush();
        report();
        return true;
    }

    void ReporterModule::report() {
        auto* controller = interpreter->getController();
        DD(controller->debug("ReporterModule::report runs");)
        if (!queue.empty()) {
            Report& r = queue.front();
            KeyCode arr[6] = {0,0,0,0,0,0};
            Byte i = 0;
            for (auto k : r.second) {
                arr[i++] = k;
                if (i >= 5) break;
            }
            DD(controller->debug("ReporterModule::report: ["BINARY_PATTERN"] + [%d, %d, %d, %d, %d, %d]", BINARY(m), arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);)
            if (controller->report(r.first, arr)) {
                // only erase the front if the report is successful
                queue.erase(queue.begin());
            }
        }
    }

    void ReporterModule::flush() {
        auto* controller = interpreter->getController(); 
        DD(controller->debug("ReporterModule::flush runs");)
        if (dirty) {
            Modifiers m = currentMods | oneshotMods;
            queue.push_back(Report(m, keys));
            oneshotMods = 0;
            dirty = false;
        }
    }

    void ReporterModule::type(String str, const String2CodeMap& map) {
        auto end = map.end();
        auto empty = Report(0, Set<KeyCode>());
        char last = 0;
        for (char c : str) {
            auto it = map.find(c);
            if (it != end) {
                if (c == last) {
                    queue.push_back(empty);
                }
                Set<KeyCode> key;
                key.insert(it->second.second);
                queue.push_back(Report(it->second.first, key));
                last = c;
            }
            queue.push_back(empty);
        }
    }

    Byte ReporterModule::onAfterTickPriority() {
        return defaultPriority;
    }

    void ReporterModule::press(KeyCode key) {
        DD(interpreter->getController()->debug("ReporterModule::press: %d", key);)
        keys.insert(key);
        dirty = true;
    }

    void ReporterModule::release(KeyCode key) {
        DD(interpreter->getController()->debug("ReporterModule::release: %d", key);)
        keys.erase(key);
        dirty = true;
    }

    void ReporterModule::pressConsumer(UsageCode usage) {
        DD(interpreter->getController()->debug("ReporterModule::pressConsumer: %d", usage);)
        interpreter->getController()->report(usage);
    }

    void ReporterModule::releaseConsumer() {
        DD(interpreter->getController()->debug("ReporterModule::releaseConsumer...");)
        interpreter->getController()->report(0);
    }


    void ReporterModule::modify(Modifiers mods) {
        currentMods |= mods;
        dirty = true;
    }

    void ReporterModule::unmodify(Modifiers mods) {
        currentMods &= ~mods;
        dirty = true;
    }

    void ReporterModule::oneshot(Modifiers mods) {
        oneshotMods |= mods;
        // NOT dirty, as oneshots will only affect the next report
    }

    
} // namespace absolem
