#ifndef ABSOLEM_HISTORY_H
#define ABSOLEM_HISTORY_H

#include <utility>
#include "common/containers.h"
#include "common/keys.h"
#include "common/Time.h"

namespace absolem {

    // the last known valid state for every key
    using History = Map<Key, std::pair<bool, Time>>;

} // namespace absolem

#endif