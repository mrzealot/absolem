#include "Interpreter.h"
#include "../common/keys.h"

#include <algorithm>

#if defined(DEBUG) && 1
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
        notify("onBeforeEnqueue", [&](Module* m) {
            m->onBeforeEnqueue(events);
        });
        queue.insert(queue.end(), events.begin(), events.end());
        notify("onAfterEnqueue", [&](Module* m) {
            m->onAfterEnqueue(events);
        });
    }

    void Interpreter::tick() {

        notify("onBeforeTick", [&](Module* m) {
            m->onBeforeTick();
        });

        if (!queue.size()) {
            DD(controller->debug("Interpreter::tick: Queue empty at T%lu...", controller->time());)
            return;
        }

        // get the Key that's affected by the front of the queue
        Key physicalKey = std::get<0>(queue.front());
        DD(controller->debug("Interpreter::tick: Physical key is %d, mapping...", physicalKey);)

        VirtualKey virtualKey = physicalKey;
        notify("onMapKey", [&](Module* m) {
            virtualKey = m->onMapKey(virtualKey);
        });

        DD(controller->debug("Interpreter::tick: Mapping ended, virtual key is %d", virtualKey);)

        auto ruleIt = rules.find(virtualKey);
        if (ruleIt == rules.end()) {
            DD(controller->debug("Interpreter::tick: No rule, aborting...");)
            return;
        }

        List<Rule> ruleList = ruleIt->second;
        Action* match = nullptr;
        for (auto rule : ruleList) {
            Trigger* trigger = rule.first;
            Action* action = rule.second;
            auto state = trigger->state(*this);
            if (state == TriggerState::UNDECIDED) {
                DD(controller->debug("Interpreter::tick: Undecided rule, will check back later...");)
                return;
            } else if (state == TriggerState::YES) {
                if (match == nullptr) {
                    match = action;
                } else {
                    // multiple rules matched, what happens here?
                    DD(controller->debug("Interpreter::tick: Multiple rules matched at once, FYI");)
                }
            }
        }

        if (match) {
            DD(controller->debug("Interpreter::tick: There's a match...");)
            (*match)(*this);
        }

        notify("onAfterTick", [&](Module* m) {
            m->onAfterTick();
        });
    }

    void Interpreter::addRule(VirtualKey key, List<Rule> rule) {
        rules[key] = rule;
    }

    void Interpreter::addModule(String name, Module* module) {
        modules[name] = module;
        module->interpreter = this;

        MODULE_HOOK(onBeforeEnqueue);
        MODULE_HOOK(onAfterEnqueue);

        MODULE_HOOK(onBeforeTick);
        MODULE_HOOK(onAfterTick);

        MODULE_HOOK(onMapKey);
    }

    Controller* Interpreter::getController() {
        return controller;
    }

    Event Interpreter::getEvent(size_t num) {
        return queue.at(num);
    }

    void Interpreter::complete(size_t num) {
        DD(controller->debug("Interpreter::complete: %d elem is completed...", num);)
        queue.erase(queue.begin(), queue.begin() + num);
    }

    Module* Interpreter::getModule(String name) {
        return modules.at(name);
    }

    void Interpreter::notify(String event, Callback callback) {
        auto list = priorities[event];
        DD(controller->debug("Interpreter::notify: %s starts (%d)", event.c_str(), list.size());)
        for (auto pair : list) {
            Module* m = pair.second;
            callback(m);
        }
        DD(controller->debug("Interpreter::notify: %s ends", event.c_str());)
    }
    
} // namespace absolem
