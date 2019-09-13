#include "BatteryAction.h"
#include "../modules/ReporterModule.h"

namespace absolem {

    void BatteryAction::operator()(Interpreter& interpreter) {
        auto* c = interpreter.getController();
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        r->type(std::to_string((Byte)c->charge()) + "%", map);
    }
    
} // namespace absolem
