#include "Interpreter.h"
#include "../common/keys.h"

namespace absolem {

    void Interpreter::enqueue(List<Event> events) {
        queue.insert(queue.end(), events.begin(), events.end());
    }

    void Interpreter::tick() {

        // testing...
        //KeyCode arr[6] = {4, 0, 0, 0, 0, 0};
        //controller->report(0, arr);
        controller->debug("arstneio");

        // get the Key that's affected by the front of the queue
        Key physicalKey = std::get<0>(queue.front());

        // map this to a virtual key
        // TODO: module insert point
        VirtualKey virtualKey = physicalKey;

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
           (*match)(*this);
        }
    }

    void Interpreter::addRule(VirtualKey key, List<Rule> rule) {
        rules[key] = rule;
    }

    Controller* Interpreter::getController() {
        return controller;
    }

    Event Interpreter::getEvent(size_t num) {
        return queue.at(num);
    }

    void Interpreter::complete(size_t num) {
        queue.erase(queue.begin(), queue.begin() + num);
    }
    
} // namespace absolem
