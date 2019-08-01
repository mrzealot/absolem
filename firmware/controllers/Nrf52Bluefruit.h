#ifndef ABSOLEM_NRF52BLUEFRUIT_H
#define ABSOLEM_NRF52BLUEFRUIT_H

#include "controllers/Controller.h"

namespace absolem {

    class Nrf52Bluefruit : public Controller {
        public:

        virtual Time time();
        virtual void delay(Time time);

        virtual void setup();
        virtual void sleep();
        virtual void hibernate();

        virtual void input(Pin pin);
        virtual void output(Pin pin);
        virtual void disable(Pin pin);
        virtual void on(Pin pin);
        virtual void off(Pin pin);
        virtual bool read(Pin pin);

        virtual void report(Modifiers mods, KeyCode keys[6]);
        virtual void report(UsageCode usage);
    };
    
} // namespace absolem


#endif