#include "Interpreter.h"
#include "../common/keys.h"

#include <algorithm>

#define MODULE_REG(name) \
    auto name##prio = module->name##Priority(); \
    if (name##prio > 0) { \
        auto list = priorities[#name]; \
        list.push_back(std::make_pair(name##prio, module)); \
        std::sort(list.begin(), list.end()); \
    }

namespace absolem {

    void Interpreter::enqueue(List<Event> events) {
        notify("onBeforeEnqueue", [&](Module* m) {
            m->onBeforeEnqueue(*this, events);
        });
        queue.insert(queue.end(), events.begin(), events.end());
        notify("onAfterEnqueue", [&](Module* m) {
            m->onAfterEnqueue(*this, events);
        });
    }

    void Interpreter::tick() {

        notify("onBeforeTick", [&](Module* m) {
            m->onBeforeTick(*this);
        });

        if (!queue.size()) {
            controller->debug("Queue empty at %lu...", controller->time());
            return;
        }

        // get the Key that's affected by the front of the queue
        Key physicalKey = std::get<0>(queue.front());

        VirtualKey virtualKey = physicalKey;
        notify("onMapKey", [&](Module* m) {
            virtualKey = m->onMapKey(*this, virtualKey);
        });

        // again, module insert point, TODO
        auto ruleIt = rules.find(virtualKey);
        if (ruleIt == rules.end()) {
            return;
        }

        List<Rule> ruleList = ruleIt->second;
        Action* match = nullptr;
        for (auto rule : ruleList) {
            Trigger* trigger = rule.first;
            Action* action = rule.second;
            auto state = trigger->state(*this);
            if (state == TriggerState::UNDECIDED) {
                return;
            } else if (state == TriggerState::YES) {
                if (match == nullptr) {
                    match = action;
                } else {
                    // multiple rules matched, TODO
                }
            }
        }

        if (match) {
            controller->debug("There's a match...");
            (*match)(*this);
        }

        notify("onAfterTick", [&](Module* m) {
            m->onAfterTick(*this);
        });
    }

    void Interpreter::addRule(VirtualKey key, List<Rule> rule) {
        rules[key] = rule;
    }

    void Interpreter::addModule(String name, Module* module) {
        modules[name] = module;

        MODULE_REG(onBeforeEnqueue);
        MODULE_REG(onAfterEnqueue);

        MODULE_REG(onBeforeTick);
        MODULE_REG(onAfterTick);

        MODULE_REG(onMapKey);
    }

    Controller* Interpreter::getController() {
        return controller;
    }

    Event Interpreter::getEvent(size_t num) {
        return queue.at(num);
    }

    void Interpreter::complete(size_t num) {
        controller->debug("%d elem is completed...", num);
        queue.erase(queue.begin(), queue.begin() + num);
    }

    Module* Interpreter::getModule(String name) {
        return modules.at(name);
    }

    void Interpreter::notify(String event, Callback callback) {
        for (auto pair : priorities[event]) {
            Module* m = pair.second;
            callback(m);
        }
    }
    
} // namespace absolem
