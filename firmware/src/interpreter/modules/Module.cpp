#include "Module.h"

namespace absolem {

    const Byte Module::defaultPriority = 100;

    String Module::getName() { return name; }

    bool Module::onBeforeEnqueue(List<Event>& e) { return true; }
    Byte Module::onBeforeEnqueuePriority() { return 0; }

    bool Module::onAfterEnqueue(List<Event>& e) { return true; }
    Byte Module::onAfterEnqueuePriority() { return 0; }

    bool Module::onBeforeTick() { return true; }
    Byte Module::onBeforeTickPriority() { return 0; }

    bool Module::onMapKey(VirtualKey& k) { return true; }
    Byte Module::onMapKeyPriority() { return 0; }

    bool Module::onAfterTick() { return true; }
    Byte Module::onAfterTickPriority() { return 0; }

} // namespace absolem
