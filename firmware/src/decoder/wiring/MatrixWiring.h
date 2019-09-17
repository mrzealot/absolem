#ifndef MATRIX_WIRING_H
#define MATRIX_WIRING_H

#include "Wiring.h"

namespace absolem {

    class MatrixWiring : public Wiring {
        public:
        MatrixWiring(Controller* controller, List<Pin> rows, List<Pin> cols)
        : Wiring(controller), rows(rows), cols(cols) {};
        virtual State getState();
        virtual void sleep();

        private:
        List<Pin> rows;
        List<Pin> cols;
    };

} // namespace absolem


#endif