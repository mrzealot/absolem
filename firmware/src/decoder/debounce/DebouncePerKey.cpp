#include "DebouncePerKey.h"
#include <algorithm>

#if defined(DEBUG) && 0
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    List<Event> DebouncePerKey::update(const State& state) {
        Time new_time = state.first;
        const Set<Key>& keys = state.second;
        auto events = List<Event>();
        auto not_found = keys.end();
        for (auto key : keys) {
            DD(controller->debug("DebouncePerKey::update: %d is checked in history at %lu", key, new_time);)
            // this ensures that the key has some state in the history
            // if it has already been active, then that state remains the same
            // if not, then the state is initialized to a default value
            auto match = history[key];
        }
        for (auto i = history.begin(); i != history.end(); ++i) {
            const Key& key = i->first;
            bool& old_active = i->second.first;
            Time& old_time = i->second.second;
            DD(controller->debug("DebouncePerKey::update: %d is being debounced, active = %s, time = %lu", key, old_active ? "on" : "off", old_time);)
            bool new_active = keys.find(key) != not_found;
            if (new_active == old_active) continue;
            if (new_time - old_time < timeout) continue;
            old_active = new_active;
            old_time = new_time;
            DD(controller->debug("DebouncePerKey::update: New event --> %d is now %s", key, new_active ? "on" : "off");)
            events.push_back(Event(key, new_active, new_time));
        }
        return events;
    }
    
} // namespace absolem
