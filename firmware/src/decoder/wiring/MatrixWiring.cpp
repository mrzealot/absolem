#include "MatrixWiring.h"
#include "../../profiling/profiling.h"

#if defined(DEBUG) && 0
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    State MatrixWiring::getState() {

        PF(4);

        // This is usually where diode direction comes into play
        // with defines and conditionals for how to handle it.
        // In this case we handle it in a static col -> row manner.
        // If your wiring is reversed for some reason, simply swap
        // your pins when defining rows and columns...

        State state;
        state.first = controller->time();
        Set<Key> actives;
        Size col_count = cols.size();

        short row_index = 0;
        for (auto& row : rows) {            // loop through the rows
            DD(controller->debug("MatrixWiring::getState: scanning %d. row", row);)
            controller->output(row);        // set the current one as an output
            controller->on(row);            // and activate it
            for (auto& col : cols) {        // now mark all cols as inputs
                DD(controller->debug("MatrixWiring::getState: setting up col %d", col);)
                controller->input(col);
            }
            controller->delay(1);           // wait for the signals to settle
            short col_index = 0;
            for (auto& col : cols) {        // now check activity on the cols
                bool result = controller->read(col);
                DD(controller->debug("MatrixWiring::getState: reading col %d = %d", col, result);)
                if (result) {
                    // physical key ids are automatically calculated
                    Key id = row_index * col_count + col_index;
                    actives.insert(id);     // and add active ones to the result

                    DD(controller->debug("MatrixWiring::getState: %d (= %d * %d + %d) is active.", id, row_index, col_count, col_index);)
                }
                controller->disable(col);
                col_index++;
            }
            controller->disable(row);
            row_index++;
        }

        DD(controller->debug("scan ends");)

        state.second = actives;
        return state;
    }
    
} // namespace absolem
