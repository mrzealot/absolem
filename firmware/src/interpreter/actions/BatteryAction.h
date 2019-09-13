#ifndef BATTERY_ACTION_H
#define BATTERY_ACTION_H

#include "Action.h"
#include "../Interpreter.h"
#include "../../common/String2CodeMap.h"

namespace absolem {

    class BatteryAction : public Action {
        public:
        BatteryAction(const String2CodeMap& m) : map(m) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        const String2CodeMap& map;
    };

} // namespace absolem

#endif