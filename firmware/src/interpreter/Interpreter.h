#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../common/keys.h"
#include "../common/containers.h"
#include "../controllers/Controller.h"
#include "../decoder/Event.h"
#include "Rule.h"
#include "Module.h"

namespace absolem {

    class Interpreter {
        public:
        Interpreter(Controller* c) : controller(c) {}

        void enqueue(List<Event> events);
        void tick();

        void addRule(VirtualKey key, List<Rule> rule);
        // void addModule(Module module);

        Controller* getController();
        Event getEvent(size_t num);
        void complete(size_t num);

        private:
        Controller* controller;
        List<Event> queue;
        Map<VirtualKey, List<Rule>> rules;
        List<Module> modules;
    };

} // namespace absolem

#endif