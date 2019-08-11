#ifndef ABSOLEM_STATE_H
#define ABSOLEM_STATE_H

#include <utility>
#include "containers.h"
#include "keys.h"
#include "Time.h"

namespace absolem {

    // a type to represent the current state of the physical switches
    // when did the scan happen, and which keys were active at that time?
    using State = std::pair<Time, List<Key>>;

} // namespace absolem

#endif