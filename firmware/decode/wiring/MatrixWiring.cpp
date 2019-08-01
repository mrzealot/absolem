#include "decode/wiring/MatrixWiring.h"

namespace absolem {

    State MatrixWiring::getState() {

        // This is usually where diode direction comes into play
        // with defines and conditionals for how to handle it.
        // In this case we handle it in a static col -> row manner.
        // If your wiring is reversed for some reason, simply swap
        // your pins when defining rows and columns...

        State state;
        state.first = controller->time();
        Set<Key> actives;
        size_t row_count = rows.size();

        short col_index = 0;
        for (auto& col : cols) {            // loop through the cols
            controller->output(col);        // set the current one as an output
            controller->on(col);            // and activate it
            for (auto& row : rows) {        // now mark all rows as inputs
                controller->input(row);
            }
            controller->delay(1);           // wait for the signals to settle
            short row_index = 0;
            for (auto& row : rows) {        // now check activity on the rows
                if (controller->read(row)) {
                    // physical key ids are automatically calculated
                    Key id = col_index * row_count + row_index;
                    actives.insert(id);     // and add active ones to the result
                }
            }
            col_index++;
        }

        state.second = actives;
        return state;
    }
    
} // namespace absolem
