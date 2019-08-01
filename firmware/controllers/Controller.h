#ifndef ABSOLEM_CONTROLLER_H
#define ABSOLEM_CONTROLLER_H

#include "common/keys.h"
#include "common/Pin.h"
#include "common/Time.h"

namespace absolem {

    class Controller {
        public:

        virtual Time time() = 0;
        virtual void delay(Time time) = 0;

        virtual void setup() = 0;
        virtual void sleep() = 0;
        virtual void hibernate() = 0;

        virtual void input(Pin pin) = 0;
        virtual void output(Pin pin) = 0;
        virtual void disable(Pin pin) = 0;
        virtual void on(Pin pin) = 0;
        virtual void off(Pin pin) = 0;
        virtual bool read(Pin pin) = 0;

        virtual void report(Modifiers mods, KeyCode keys[6]) = 0;
        virtual void report(UsageCode usage) = 0;
    };
    
} // namespace absolem


#endif