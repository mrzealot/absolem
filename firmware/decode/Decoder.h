#ifndef DECODER_H
#define DECODER_H

#include <memory>

#include "common/containers.h"
#include "decode/wiring/Wiring.h"
#include "decode/debounce/Debounce.h"

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