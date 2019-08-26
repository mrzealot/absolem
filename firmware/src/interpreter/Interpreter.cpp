#include "Interpreter.h"
#include "../common/keys.h"
#include "../profiling/profiling.h"

#include <algorithm>

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_INTERPRETER))
#define DD(x) x
#else
#define DD(x)
#endif

#define MODULE_HOOK(name) \
    auto name##prio = module->name##Priority(); \
    DD(controller->debug("Interpreter::addModule: Priority for %s event is %d", #name, name##prio);) \
    if (name##prio > 0) { \
        DD(controller->debug("Interpreter::addModule: %s hook registered...", #name);) \
        auto& list = priorities[#name]; \
        list.push_back(std::make_pair(name##prio, module)); \
        std::sort(list.begin(), list.end()); \
    }

namespace absolem {

    void Interpreter::enqueue(List<Event> events) {

        PF(6);

        #ifndef DISABLE_HOOK_ON_BEFORE_ENQUEUE
        notify("onBeforeEnqueue", [&](Module* m) {
            return m->onBeforeEnqueue(events);
        });
        #endif

        queue.insert(queue.end(), events.begin(), events.end());

        #ifndef DISABLE_HOOK_ON_AFTER_ENQUEUE
        notify("onAfterEnqueue", [&](Module* m) {
            return m->onAfterEnqueue(events);
        });
        #endif
    }

    void Interpreter::tick() {

        PF(10);

        controller->tick();
        currentTime = controller->time();

        if (!queue.size()) {
            DD(controller->debug("Interpreter::tick: Queue empty at T%lu...", currentTime);)
            return;
        }

        #ifndef DISABLE_HOOK_ON_BEFORE_TICK
        notify("onBeforeTick", [&](Module* m) {
            return m->onBeforeTick();
        });
        #endif

        PF(11);

        Time currentTimeBatch = 0;
        while (queue.size()) {

            // get the Key that's affected by the front of the queue
            auto& event = queue.front();
            physicalKey = std::get<0>(event);
            Time eventTime = std::get<2>(event);
            DD(controller->debug("Interpreter::tick: Physical key is %d at %lu", physicalKey, eventTime);)

            // if the current event belongs to another batch, we force a flush
            if (currentTimeBatch == 0) {
                currentTimeBatch = eventTime;
            }
            if (currentTimeBatch != eventTime) {
                break;
            }

            // the matching rules will be searched 3 ways:
            List<Rule> candidates;
            bool foundRules = false;

            // 1. a direct hook for modules
            #ifndef DISABLE_HOOK_ON_DIRECT_SEARCH
            notify("onDirectSearch", [&](Module* m) {
                return m->onDirectSearch(physicalKey, candidates);
            });
            foundRules = candidates.size() > 0;
            DD(controller->debug("Interpreter::tick: Direct search ended, %s", foundRules ? "rules found!" : "no rules found...");)
            #endif

            PF(12);

            // 2. a virtual hook for the mapped key
            virtualKey = physicalKey;
            if (!foundRules) {
                #ifndef DISABLE_HOOK_ON_MAP_KEY
                notify("onMapKey", [&](Module* m) {
                    return m->onMapKey(virtualKey);
                });
                DD(controller->debug("Interpreter::tick: Mapping ended, virtual key is %d", virtualKey);)
                #endif

                #ifndef DISABLE_HOOK_VIRTUAL_SEARCH
                notify("onVirtualSearch", [&](Module* m) {
                    return m->onVirtualSearch(virtualKey, candidates);
                });
                DD(controller->debug("Interpreter::tick: Virtual search ended, %s", foundRules ? "rules found!" : "no rules found...");)
                #endif
            }

            PF(13);

            // 3. using the built-in mapping
            if (!foundRules) {
                auto ruleIt = rules.find(virtualKey);
                if (ruleIt == rules.end()) {
                    DD(controller->debug("Interpreter::tick: No rule, continuing...");)
                    complete(1);
                    continue;
                }
                candidates = ruleIt->second;
            }

            PF(14);

            Action* match = nullptr;
            bool undecided = false;
            for (auto rule : candidates) {
                Trigger* trigger = rule.first;
                Action* action = rule.second;
                auto state = trigger->state(*this);
                if (state == TriggerState::YES && match == nullptr) {
                    match = action;
                    break;
                } else if (state == TriggerState::UNDECIDED) {
                    DD(controller->debug("Interpreter::tick: Undecided rule, will check back later...");)
                    undecided = true;
                    break;
                }
            }

            // need to break the outer loop, too
            if (undecided) {
                break;
            }

            PF(15);

            if (match) {
                DD(controller->debug("Interpreter::tick: There's a match...");)
                (*match)(*this);
            }

        }

        PF(16);

        #ifndef DISABLE_HOOK_ON_AFTER_TICK
        notify("onAfterTick", [&](Module* m) {
            return m->onAfterTick();
        });
        #endif

        // emergency fix for potentially stuck elements
        // 3000 is just a magic number here, TODO
        //if (currentTime - lastUpdate > 3000) {
        //    complete(1);
        //}
    }

    void Interpreter::addRule(VirtualKey key, List<Rule> rule) {
        rules[key] = rule;
    }

    void Interpreter::addModule(Module* module) {
        modules[module->getName()] = module;
        module->interpreter = this;

        MODULE_HOOK(onBeforeEnqueue);
        MODULE_HOOK(onAfterEnqueue);

        MODULE_HOOK(onBeforeTick);
        MODULE_HOOK(onDirectSearch);
        MODULE_HOOK(onMapKey);
        MODULE_HOOK(onVirtualSearch);
        MODULE_HOOK(onAfterTick);
    }

    Controller* Interpreter::getController() {
        return controller;
    }

    List<Event>& Interpreter::getQueue() {
        return queue;
    }

    void Interpreter::complete(Size num) {
        queue.erase(queue.begin(), queue.begin() + num);
        DD(controller->debug("Interpreter::complete: %d elem is completed, new queue size is %d...", num, queue.size());)
        lastUpdate = currentTime;
    }

    Module* Interpreter::getModule(String name) {
        return modules.at(name);
    }

    Key Interpreter::getPhysicalKey() {
        return physicalKey;
    }
    
    VirtualKey Interpreter::getVirtualKey() {
        return virtualKey;
    }

    Time Interpreter::getCurrentTime() {
        return currentTime;
    }

    Time Interpreter::getLastUpdate() {
        return lastUpdate;
    }

    void Interpreter::notify(String event, Callback callback) {
        auto list = priorities[event];
        DD(controller->debug("Interpreter::notify: %s starts (%d)", event.c_str(), list.size());)
        for (auto pair : list) {
            Module* m = pair.second;
            bool shouldContinue = callback(m);
            if (!shouldContinue) {
                DD(controller->debug("Interpreter::notify: %s breaks the chain", m->getName().c_str());)
                break;
            }
        }
        DD(controller->debug("Interpreter::notify: %s ends", event.c_str());)
    }
    
} // namespace absolem
