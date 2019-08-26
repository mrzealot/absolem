#include "CacheModule.h"
#include "../Interpreter.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_CACHEMODULE))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    bool CacheModule::onDirectSearch(Key k, List<Rule>& candidates) {
        DD(interpreter->getController()->debug("CacheModule::onDirectSearch runs");)
        auto it = rules.find(k);
        if (it != rules.end()) {
            candidates.push_back(it->second);
            DD(interpreter->getController()->debug("CacheModule::onDirectSearch: found a match!");)
            rules.erase(k);
        }
        return true;
    }

    Byte CacheModule::onDirectSearchPriority() {
        return 10; // should come earlier than the default
    }

    void CacheModule::set(Key k, Rule r) {
        rules[k] = r;
    }
    
} // namespace absolem
