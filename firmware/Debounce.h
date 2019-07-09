#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "common.h"
#include "Event.h"

namespace absolem {

    class Debounce {
        public:
        virtual List<Event> update(const State& state) = 0;
    };

    class DebouncePerKey : public Debounce {
        public:
        DebouncePerKey(Time timeout);
        List<Event> update(const State& state) override;

        private:
        Time timeout;
        History history;
    }

} // namespace absolem


#endif