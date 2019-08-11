#ifndef DECODER_H
#define DECODER_H

#include "../common/containers.h"
#include "wiring/Wiring.h"
#include "debounce/Debounce.h"

namespace absolem {

    class Decoder {
        public:
        Decoder(Wiring* w, Debounce* d);
        List<Event> getEvents();

        private:
        Wiring* wiring;
        Debounce* debounce;
    };

} // namespace absolem

#endif