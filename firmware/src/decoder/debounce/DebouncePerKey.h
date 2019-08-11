#ifndef DEBOUNCE_PER_KEY_H
#define DEBOUNCE_PER_KEY_H

#include "../../common/History.h"
#include "Debounce.h"

namespace absolem {

    class DebouncePerKey : public Debounce {
        public:
        DebouncePerKey(Time timeout);
        List<Event> update(const State& state) override;

        private:
        Time timeout;
        History history;
    };

} // namespace absolem


#endif