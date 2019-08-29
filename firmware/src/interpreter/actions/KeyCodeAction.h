#ifndef KEYCODE_ACTION_H
#define KEYCODE_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class KeyCodeAction : public Action {
        public:
        KeyCodeAction(bool p, Modifiers m, bool o, KeyCode k) : press(p), mods(m), oneshot(o), key(k) {}
        KeyCodeAction(bool p, Modifiers m, bool o, KeyCode k, Rule c) : Action(c), press(p), mods(m), oneshot(o), key(k) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        bool press;
        Modifiers mods;
        bool oneshot;
        KeyCode key;
    };

} // namespace absolem

#endif