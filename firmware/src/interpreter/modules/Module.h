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

        virtual void onBeforeEnqueue(List<Event> e);
        virtual Byte onBeforeEnqueuePriority();

        virtual void onAfterEnqueue(List<Event> e);
        virtual Byte onAfterEnqueuePriority();

        virtual void onBeforeTick();
        virtual Byte onBeforeTickPriority();

        virtual VirtualKey onMapKey(VirtualKey k);
        virtual Byte onMapKeyPriority();

        virtual void onAfterTick();
        virtual Byte onAfterTickPriority();

        protected:
        Interpreter* interpreter;

        friend class Interpreter;
    };

} // namespace absolem

#endif