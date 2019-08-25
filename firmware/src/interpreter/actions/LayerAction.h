#ifndef LAYER_ACTION_H
#define LAYER_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class LayerAction : public Action {
        public:
        LayerAction(bool p, Byte l) : press(p), layer(l) {}
        LayerAction(bool p, Byte l, Rule c) : press(p), layer(l), counterpart(c) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        bool press;
        Byte layer;
        Rule counterpart;
    };

} // namespace absolem

#endif