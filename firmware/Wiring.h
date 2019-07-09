#ifndef WIRING_H
#define WIRING_H

#include "common.h"
#include "Controller.h"

namespace absolem {

    class Wiring {
        public:
        Wiring(Controller controller);
        virtual State getState() = 0;

        private:
        Controller controller;
    };

} // namespace absolem


#endif