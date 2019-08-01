#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "common/containers.h"
#include "common/State.h"
#include "decode/Event.h"

namespace absolem {

    class Debounce {
        public:
        virtual List<Event> update(const State& state) = 0;
    };

} // namespace absolem


#endif