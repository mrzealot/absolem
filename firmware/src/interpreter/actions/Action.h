#ifndef ACTION_H
#define ACTION_H

#include "../Rule.h"

namespace absolem {

    class Interpreter;

    class Action {
        public:
        Action() {}
        Action(Rule c) : counterpart(c) {}
        virtual void operator()(Interpreter& interpreter);

        protected:
        Rule counterpart;
    };

} // namespace absolem

#endif