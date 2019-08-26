#include "MultiTrigger.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_MULTITRIGGER))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    TriggerState MultiTrigger::state(Interpreter& interpreter) {
        auto* controller = interpreter.getController();
        auto& queue = interpreter.getQueue();
        auto& front = queue.front();
        Time currentTime = interpreter.getCurrentTime();

        // initialize the "previous" key and time from the front of the queue
        Key key = std::get<0>(front);
        Time time = std::get<2>(front);

        Byte index = 0;
        Byte maxIndex = queue.size() - 1;
        DD(controller->debug("");)
        DD(controller->debug("MultiTrigger::state: START, taps = %d, hold = %s, key = %d, currentTime = %lu, queue = %d", taps, hold ? "true" : "false", key, currentTime, maxIndex);)

        // iterate over the press/release pairs needed
        for (Byte i = 1; /* infinite */; ++i) {
            for (bool press : {true, false}) {

                DD(controller->debug("MultiTrigger::state: iteration at %d/%s, time = %lu", i, press ? "true" : "false", time);)

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
                    DD(controller->debug("MultiTrigger::state: basic failure with key = %d, press = %s, time = %lu", key, eventPress ? "true" : "false", eventTime);)
                    return TriggerState::NO;
                }

                // check if we're done
                // when holding...
                if (hold) {

                    // there's an extra press needed,
                    if (i > taps && press) {
                    
                        DD(controller->debug("MultiTrigger::state: hold check");)

                        // which should either have a different keypress after it within term
                        // (both press and release, while the original key is held)
                        // (however, if there is a same key release in the meantime, then we
                        // know that it's not a hold because it was cut short before term)
                        Set<Key> presses;
                        bool differentRelease = false;
                        for (Byte j = index + 1; j <= maxIndex; ++j) {
                            auto& ej = queue.at(j);
                            Key ejKey = std::get<0>(ej);
                            bool ejPress = std::get<1>(ej);
                            Time ejTime = std::get<2>(ej);
                            DD(controller->debug("MultiTrigger::state: Event[%d] = {key = %d, press = %s, time = %lu}", j, ejKey, ejPress ? "true" : "false", ejTime);)
                            // we said "within" term, so if we're already past that, it's a hold
                            if (ejTime - eventTime > term) {
                                DD(controller->debug("MultiTrigger::state: hold matches because of time (while examining events)");)
                                interpreter.complete(index);
                                return TriggerState::YES;
                            }
                            // okay, we're within the term still, so look for the different press
                            if (ejPress) {
                                presses.insert(std::get<0>(ej));
                            } else {
                                if (ejKey == key) {
                                    // oops, same key release within term
                                    // that's what we called "cut short" above...
                                    DD(controller->debug("MultiTrigger::state: hold cut short");)
                                    return TriggerState::NO;
                                } else if (presses.find(ejKey) != presses.end()) {
                                    // got our proof
                                    DD(controller->debug("MultiTrigger::state: hold matches because of a different release");)
                                    interpreter.complete(index);
                                    return TriggerState::YES;
                                }
                            }
                        }

                        // ...or it should be active past term on its own
                        if ((currentTime - eventTime) > term) {
                            interpreter.complete(index);
                            DD(controller->debug("MultiTrigger::state: hold matches because of time");)
                            return TriggerState::YES;
                        }

                        // if we make it here, then we haven't found anything "bad" yet
                        // so the patter looks fine so far, it's just not complete...
                        DD(controller->debug("MultiTrigger::state: hold is undecided");)
                        return TriggerState::UNDECIDED;
                    }

                // when not holding, simply getting to the end is enough
                } else {
                    if (i == taps && press) {
                        DD(controller->debug("MultiTrigger::state: press got to the end");)
                        interpreter.complete(index);
                        return TriggerState::YES;
                    }
                }

                // if not done, then we need further info within term
                // so if there's no more info in the queue
                if (index == maxIndex) {

                    // then we're either still within term and should wait for more
                    if ((currentTime - eventTime) < term) {
                        DD(controller->debug("MultiTrigger::state: need more info");)
                        return TriggerState::UNDECIDED;

                    // or we're not, in which case it's not a match
                    } else {
                        DD(controller->debug("MultiTrigger::state: broken chain, fail");)
                        return TriggerState::NO;
                    }
                }

                // if we DO have more info in the queue, then we should
                // advance the index and continue with the next event
                DD(controller->debug("MultiTrigger::state: iteration over, continue...");)
                time = eventTime;
                ++index;
            }
        }
    }
    
} // namespace absolem
