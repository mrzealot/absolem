#ifndef TRIGGER_H
#define TRIGGER_H

namespace absolem {

    class Interpreter;

    enum class TriggerState {
        YES,
        NO,
        UNDECIDED
    };

    class Trigger {
        public:
        virtual TriggerState state(Interpreter& interpreter) = 0;
    };

} // namespace absolem

#endif