#ifndef ACTION_H
#define ACTION_H

namespace absolem {

    class Interpreter;

    class Action {
        public:
        virtual void operator()(Interpreter& interpreter) = 0;
    };

} // namespace absolem

#endif