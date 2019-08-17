#ifndef MODULE_H
#define MODULE_H

#include "../../common/keys.h"
#include "../../common/types.h"
#include "../../decoder/Event.h"

namespace absolem {

    class Interpreter;

    class Module {
        public:
        static const Byte defaultPriority;

        virtual void onBeforeEnqueue(Interpreter& i, List<Event> e);
        virtual Byte onBeforeEnqueuePriority();

        virtual void onAfterEnqueue(Interpreter& i, List<Event> e);
        virtual Byte onAfterEnqueuePriority();

        virtual void onBeforeTick(Interpreter& i);
        virtual Byte onBeforeTickPriority();

        virtual VirtualKey onMapKey(Interpreter& i, VirtualKey k);
        virtual Byte onMapKeyPriority();

        virtual void onAfterTick(Interpreter& i);
        virtual Byte onAfterTickPriority();
    };

} // namespace absolem

#endif