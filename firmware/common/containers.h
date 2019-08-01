#ifndef ABSOLEM_CONTAINERS_H
#define ABSOLEM_CONTAINERS_H

#include <vector>
#include <set>
#include <map>

namespace absolem {

    // more "thematically cohesive" names for containers
    template<class T>
    using List = std::vector<T>;

    template<class T>
    using Set = std::set<T>;

    template<class K, class V>
    using Map = std::map<K, V>;
    
} // namespace absolem

#endif