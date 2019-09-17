#include "Decoder.h"

namespace absolem {

    Decoder::Decoder(Wiring* w, Debounce* d) : wiring(w), debounce(d) {}

    List<Event> Decoder::getEvents() {
        return debounce->update(wiring->getState());
    }

    void Decoder::sleep() {
        wiring->sleep();
    }
    
} // namespace absolem
