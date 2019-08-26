#include "MultiTrigger.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_MULTITRIGGER))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    TriggerState MultiTrigger::state(Interpreter& interpreter) {
        DD(interpreter.getController()->debug("MultiTrigger::state: State is checked against holding with %d preceding taps %s hold...", taps, hold ? "with" : "WITHOUT");)
        auto& queue = interpreter.getQueue();
        auto& front = queue.front();
        Time currentTime = interpreter.getCurrentTime();

        // initialize the "previous" key and time from the front of the queue
        Key key = std::get<0>(front);
        Time time = std::get<2>(front);

        Byte index = 0;
        Byte maxIndex = queue.size() - 1;

        // iterate over the press/release pairs needed
        for (Byte i = 1; /* infinite */; ++i) {
            for (bool press : {true, false}) {

                auto& event = queue.at(index);
                Key eventKey = std::get<0>(event);
                bool eventPress = std::get<1>(event);
                Time eventTime = std::get<2>(event);
                
                // if it's not what we expect here
                // then we already know that it's definitely not a match
                if (
                    key != eventKey ||
                    press != eventPress ||
                    (eventTime - time) > term
                ) {
                    return TriggerState::NO;
                }

                // check if we're done
                // when holding...
                if (hold) {

                    // there's an extra press needed,
                    if (i > taps && press) {
                    
                        // which should either be active past term
                        if ((currentTime - eventTime) > term) {
                            interpreter.complete(index);
                            return TriggerState::YES;
                        }

                        // or there should be a different release after it
                        // (however, if there is a same key release, then we
                        // again know that it's not a hold because it was cut
                        // short before term)
                        bool differentRelease = false;
                        for (Byte j = index + 1; j < maxIndex; ++j) {
                            auto& ej = queue.at(j);
                            if (!std::get<1>(ej)) { // it's a release
                                if (std::get<0>(ej) != key) { // and it's different as well
                                    differentRelease = true;
                                } else {
                                    // cut short...
                                    return TriggerState::NO;
                                }
                            }
                        }

                        if (differentRelease) {
                            interpreter.complete(index);
                            return TriggerState::YES;
                        }

                        // if we make it here, then we haven't found anything "bad" yet
                        // so the patter looks fine so far, it's just not complete...
                        return TriggerState::UNDECIDED;
                    }

                // when not holding, simply getting to the end is enough
                } else {
                    if (i == taps && press) {
                        interpreter.complete(index);
                        return TriggerState::YES;
                    }
                }

                // if not done, then we need further info within term
                // that's only possible within term...
                if ((currentTime - eventTime) > term) {
                    return TriggerState::NO;
                }

                // okay, we're inside the term still...
                // but if we don't have more info yet, then it's undecided
                if (index == maxIndex) {
                    return TriggerState::UNDECIDED;
                }

                // otherwise we should advance the index
                // and continue with the next event
                time = eventTime;
                ++index;
            }
        }
    }
    
} // namespace absolem
