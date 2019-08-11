#include "KeyCodeAction.h"

namespace absolem {

    void KeyCodeAction::operator()(Interpreter& interpreter) {
        // dummy stuff
        KeyCode arr[6] = {key, 0, 0, 0, 0, 0};
        interpreter.getController()->report(0, arr);
    }
    
} // namespace absolem
