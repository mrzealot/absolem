#ifndef ABSOLEM_EVENT_H
#define ABSOLEM_EVENT_H

#include <tuple>
#include "../common/keys.h"
#include "../common/Time.h"

namespace absolem {

    using Event = std::tuple<Key, bool, Time>;
    
} // namespace absolem


#endif