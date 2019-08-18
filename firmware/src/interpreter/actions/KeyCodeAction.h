#ifndef KEYCODE_ACTION_H
#define KEYCODE_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class KeyCodeAction : public Action {
        public:
        KeyCodeAction(bool p, Modifiers m, KeyCode k) : press(p), mods(m), key(k) {}
        KeyCodeAction(bool p, Modifiers m, KeyCode k, Rule c) : press(p), mods(m), key(k), counterpart(c) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        bool press;
        Modifiers mods;
        KeyCode key;
        Rule counterpart;
    };

} // namespace absolem

#endif