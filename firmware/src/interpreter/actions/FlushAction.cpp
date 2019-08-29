#include "FlushAction.h"
#include "../modules/ReporterModule.h"

namespace absolem {

    void FlushAction::operator()(Interpreter& interpreter) {
        auto r = (ReporterModule*) interpreter.getModule("reporter");
        r->flush();
    }
    
} // namespace absolem
