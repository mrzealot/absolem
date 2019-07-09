#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

namespace absolem {

    class Controller {
        public:

        void setup();
        void sleep();

        void on(Pin pin);
        void off(Pin pin);
        bool read(Pin pin);

        // other possible controller-specific functionality
    };
    
} // namespace absolem


#endif