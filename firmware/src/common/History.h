#ifndef ABSOLEM_HISTORY_H
#define ABSOLEM_HISTORY_H

#include <utility>
#include "containers.h"
#include "keys.h"
#include "Time.h"

namespace absolem {

    // the last known valid state for every key
    using History = Map<Key, std::pair<bool, Time>>;

} // namespace absolem

#endif