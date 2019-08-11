#ifndef KEYCODE_ACTION_H
#define KEYCODE_ACTION_H

#include "../../common/keys.h"
#include "Action.h"
#include "../Interpreter.h"

namespace absolem {

    class KeyCodeAction : public Action {
        public:
        KeyCodeAction(KeyCode k) : key(k) {}
        virtual void operator()(Interpreter& interpreter);

        private:
        KeyCode key;
    };

} // namespace absolem

#endif