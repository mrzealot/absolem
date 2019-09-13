#ifndef ABSOLEM_STRING2CODEMAP_H
#define ABSOLEM_STRING2CODEMAP_H

#include "types.h"
#include "keys.h"

namespace absolem {

    using String2CodeMap = Map<char, Pair<Modifiers, KeyCode>>;

} // namespace absolem

#endif