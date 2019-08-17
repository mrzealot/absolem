#include "Module.h"
#include "../Interpreter.h"

namespace absolem {

    const Byte Module::defaultPriority = 100;

    void Module::onBeforeEnqueue(Interpreter& i, List<Event> e) {}
    Byte Module::onBeforeEnqueuePriority() { return 0; }

    void Module::onAfterEnqueue(Interpreter& i, List<Event> e) {}
    Byte Module::onAfterEnqueuePriority() { return 0; }

    void Module::onBeforeTick(Interpreter& i) {}
    Byte Module::onBeforeTickPriority() { return 0; }

    VirtualKey Module::onMapKey(Interpreter& i, VirtualKey k) {}
    Byte Module::onMapKeyPriority() { return 0; }

    void Module::onAfterTick(Interpreter& i) {}
    Byte Module::onAfterTickPriority() { return 0; }

} // namespace absolem
