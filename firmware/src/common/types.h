#ifndef ABSOLEM_TYPES_H
#define ABSOLEM_TYPES_H

#include <string>
#include <utility>
#include <vector>
#include <set>
#include <map>

namespace absolem {

    // more "thematically cohesive" names for built-in types
    using String = std::string;

    using Size = size_t;

    using Byte = uint8_t;

    template<class F, class S>
    using Pair = std::pair<F, S>;

    template<class T>
    using List = std::vector<T>;

    template<class T>
    using Set = std::set<T>;

    template<class K, class V>
    using Map = std::map<K, V>;
    
} // namespace absolem

#endif