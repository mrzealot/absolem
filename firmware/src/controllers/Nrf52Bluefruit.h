#ifndef ABSOLEM_NRF52BLUEFRUIT_H
#define ABSOLEM_NRF52BLUEFRUIT_H

#include <bluefruit.h>
#include "Controller.h"

#define ABSOLEM_SETUP_BEGIN void setup() {
#define ABSOLEM_SETUP_END }
#define ABSOLEM_LOOP_BEGIN void loop {
#define ABSOLEM_LOOP_END }

namespace absolem {

    class Nrf52Bluefruit : public Controller {
        public:

        Nrf52Bluefruit() {}

        #if defined(DEBUG) || defined(PROFILING)
        virtual void debug(char* message, ...);
        #endif

        virtual Time time();
        virtual void delay(Time time);

        virtual void setup();

        virtual float charge();
        virtual void sleep();
        virtual void hibernate();

        virtual void tick();

        virtual void input(Pin pin);
        virtual void output(Pin pin);
        virtual void disable(Pin pin);
        virtual void on(Pin pin);
        virtual void off(Pin pin);
        virtual bool read(Pin pin);

        virtual bool report(Modifiers mods, KeyCode keys[6]);
        virtual bool report(UsageCode usage);

        virtual void reset();

        private:
        BLEDis bledis;
        BLEHidAdafruit blehid;
        BLEBas blebas;
        Time blebasTimer;
    };
    
} // namespace absolem


#endif