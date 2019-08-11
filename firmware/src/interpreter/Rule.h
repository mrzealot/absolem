#ifndef RULE_H
#define RULE_H

#include "../common/containers.h"
#include "triggers/Trigger.h"
#include "actions/Action.h"

namespace absolem {

    using Rule = Pair<Trigger*, Action*>;

} // namespace absolem

#endif