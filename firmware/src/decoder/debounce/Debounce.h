#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "../../common/types.h"
#include "../../common/State.h"
#include "../Event.h"
#include "../../controllers/Controller.h"

namespace absolem {

    class Debounce {
        public:
        Debounce(Controller* c) : controller(c) {}
        virtual List<Event> update(const State& state) = 0;

        protected:
        Controller* controller;
    };

} // namespace absolem


#endif