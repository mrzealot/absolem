#include "DebouncePerKey.h"
#include <algorithm>

namespace absolem {

    DebouncePerKey::DebouncePerKey(Time timeout) : timeout(timeout) {}

    List<Event> DebouncePerKey::update(const State& state) {
        Time new_time = state.first;
        const List<Key>& keys = state.second;
        auto events = List<Event>();
        auto front = keys.begin();
        auto not_found = keys.end();
        for (auto i = history.begin(); i != history.end(); ++i) {
            const Key& key = i->first;
            bool& old_active = i->second.first;
            Time& old_time = i->second.second;
            bool new_active = std::find(front, not_found, key) != not_found;
            if (new_active == old_active) continue;
            if (new_time - old_time < timeout) continue;
            old_active = new_active;
            old_time = new_time;
            events.push_back(Event(key, new_active, new_time));
        }
        return events;
    }
    
} // namespace absolem
