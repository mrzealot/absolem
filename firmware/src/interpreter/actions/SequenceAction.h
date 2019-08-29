#ifndef SEQUENCE_ACTION_H
#define SEQUENCE_ACTION_H

#include "../../common/types.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class SequenceAction : public Action {
        public:
        SequenceAction(List<Action*> l) : list(l) {}
        SequenceAction(List<Action*> l, Rule c) : Action(c), list(l) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        List<Action*> list;
    };

} // namespace absolem

#endif