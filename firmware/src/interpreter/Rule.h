#ifndef RULE_H
#define RULE_H

#include "../common/types.h"

namespace absolem {

    class Trigger;
    class Action;
    using Rule = Pair<Trigger*, Action*>;

} // namespace absolem

#endif