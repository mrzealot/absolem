#include "decode/Decoder.h"

using namespace std;

namespace absolem {

    Decoder::Decoder(Wiring* w, Debounce* d) : wiring(w), debounce(d) {}

    List<Event> Decoder::getEvents() {
        return debounce->update(wiring->getState());
    }
    
} // namespace absolem
