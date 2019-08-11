#ifndef ABSOLEM_KEYS_H
#define ABSOLEM_KEYS_H

#include <cstdint>

namespace absolem {

    // physical keys are identified by numbers
    // the wiring can give them simple names with constants later on
    using Key = uint8_t;

    // virtual keys are numbers as well, but there can be more than
    // 2^8 of them
    using VirtualKey = uint16_t;

    // keycodes are also simple numbers according to the HID specs
    using KeyCode = uint8_t;
    // ...as are modifiers
    using Modifiers = uint8_t;

    // there are also special, so called "consumer usage codes"
    // for things like volume/brightness/media keys
    using UsageCode = uint16_t;

} // namespace absolem

#endif