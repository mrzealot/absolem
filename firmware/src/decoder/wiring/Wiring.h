#ifndef ABSOLEM_WIRING_H
#define ABSOLEM_WIRING_H

#include "../../common/State.h"
#include "../../controllers/Controller.h"

namespace absolem {

    class Wiring {
        public:
        Wiring(Controller* controller) : controller(controller) {};
        virtual State getState() = 0;

        protected:
        Controller* controller;
    };

} // namespace absolem


#endif