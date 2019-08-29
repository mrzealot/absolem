#ifndef LAYER_ACTION_H
#define LAYER_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class LayerAction : public Action {
        public:
        LayerAction(bool p, Byte l, bool o) : press(p), layer(l), oneshot(o) {}
        LayerAction(bool p, Byte l, bool o, Rule c) : Action(c), press(p), layer(l), oneshot(o) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        bool press;
        Byte layer;
        bool oneshot;
    };

} // namespace absolem

#endif