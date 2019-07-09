#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <list>
#include <set>
#include <map>

namespace absolem {

    // a more thematically cohesive name for containers
    template<class T>
    using List = std::list<T>;

    template<class T>
    using Set = std::set<T>;
    
    // pins are identified by a number
    using Pin = unsigned;
    // time in millisecs
    using Time = unsigned long;
    // keys are identified by a simple name, like "LIM" for left index middle
    using Key = std::string;
    // when did the scan happen, and which keys were active at that time?
    using State = std::pair<Time, Set<Key>>;
    // the last known valid state for every key
    using History = std::map<Key, std::pair<bool, Time>>;

} // namespace absolem

#endif