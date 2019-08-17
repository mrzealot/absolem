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

    using Callback = std::function<void (Module*)>;

    class Interpreter {
        public:
        Interpreter(Controller* c) : controller(c) {}

        void enqueue(List<Event> events);
        void tick();

        void addRule(VirtualKey key, List<Rule> rule);
        void addModule(String name, Module* module);

        Controller* getController();
        Event getEvent(size_t num);
        void complete(size_t num);

        Module* getModule(String name);

        private:
        Controller* controller;
        List<Event> queue;
        Map<VirtualKey, List<Rule>> rules;
        Map<String, Module*> modules;
        Map<String, List<Pair<Byte, Module*>>> priorities;

        void notify(String event, Callback callback);
    };

} // namespace absolem

#endif