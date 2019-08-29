#ifndef ABSOLEM_CONTROLLER_H
#define ABSOLEM_CONTROLLER_H

#include <cstdarg>

#include "../common/keys.h"
#include "../common/Pin.h"
#include "../common/Time.h"

#define ABSOLEM_SETUP_BEGIN
#define ABSOLEM_SETUP_END
#define ABSOLEM_LOOP_BEGIN
#define ABSOLEM_LOOP_END

#include "../profiling/profiling.h"
//#define DEBUG
//#define DEBUG_REPORTERMODULE
//#define DEBUG_LAYERMODULE
//#define DEBUG_KEYCODEACTION
//#define DEBUG_INTERPRETER
//#define DEBUG_CONSUMERACTION

namespace absolem {

    class Controller {
        public:

        #if defined(DEBUG) || defined(PROFILING)
        virtual void debug(char* message, ...) = 0;
        #endif

        virtual Time time() = 0;
        virtual void delay(Time time) = 0;

        virtual void setup() = 0;
        virtual void sleep() = 0;
        virtual void hibernate() = 0;

        virtual void tick() = 0;

        virtual void input(Pin pin) = 0;
        virtual void output(Pin pin) = 0;
        virtual void disable(Pin pin) = 0;
        virtual void on(Pin pin) = 0;
        virtual void off(Pin pin) = 0;
        virtual bool read(Pin pin) = 0;

        virtual void report(Modifiers mods, KeyCode keys[6]) = 0;
        virtual void report(UsageCode usage) = 0;

        virtual void reset() = 0;

        // freely accessible, so no setters needed
        const char* name;
        const char* manufacturer;
        const char* model;
    };
    
} // namespace absolem


#endif