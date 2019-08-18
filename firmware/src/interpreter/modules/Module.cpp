#include "Module.h"

namespace absolem {

    const Byte Module::defaultPriority = 100;

    void Module::onBeforeEnqueue(List<Event> e) {}
    Byte Module::onBeforeEnqueuePriority() { return 0; }

    void Module::onAfterEnqueue(List<Event> e) {}
    Byte Module::onAfterEnqueuePriority() { return 0; }

    void Module::onBeforeTick() {}
    Byte Module::onBeforeTickPriority() { return 0; }

    VirtualKey Module::onMapKey(VirtualKey k) {}
    Byte Module::onMapKeyPriority() { return 0; }

    void Module::onAfterTick() {}
    Byte Module::onAfterTickPriority() { return 0; }

} // namespace absolem
