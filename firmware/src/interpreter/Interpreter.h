#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <functional>

#include "../common/keys.h"
#include "../common/types.h"
#include "../controllers/Controller.h"
#include "../decoder/Event.h"
#include "Rule.h"
#include "modules/Module.h"

namespace absolem {

    using Callback = std::function<bool (Module*)>;

    class Interpreter {
        public:
        Interpreter(Controller* c) : controller(c) {}

        void enqueue(List<Event> events);
        void tick();

        void addRule(VirtualKey key, List<Rule> rule);
        void addModule(Module* module);

        Controller* getController();
        Event getEvent(Size num);
        void complete(Size num);

        Module* getModule(String name);

        Key getPhysicalKey();
        VirtualKey getVirtualKey();

        private:
        Controller* controller;
        List<Event> queue;
        Time lastUpdate;
        Key physicalKey;
        VirtualKey virtualKey;
        Map<VirtualKey, List<Rule>> rules;
        Map<String, Module*> modules;
        Map<String, List<Pair<Byte, Module*>>> priorities;

        void notify(String event, Callback callback);
    };

} // namespace absolem

#endif