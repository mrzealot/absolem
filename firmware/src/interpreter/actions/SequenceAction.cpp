#include "SequenceAction.h"
#include "../modules/CacheModule.h"

namespace absolem {

    void SequenceAction::operator()(Interpreter& interpreter) {
        for (auto* elem : list) {
            (*elem)(interpreter);
        }
        Action::operator()(interpreter);
    }
    
} // namespace absolem
