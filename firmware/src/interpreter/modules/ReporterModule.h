#ifndef REPORTER_MODULE_H
#define REPORTER_MODULE_H

#include "Module.h"

namespace absolem {

    class ReporterModule : public Module {
        public:
        virtual void onAfterTick(Interpreter& interpreter) override;
        virtual Byte onAfterTickPriority();

        void press(KeyCode key);
        void release(KeyCode key);

        void modify(Modifiers mods);
        void unmodify(Modifiers mods);

        private:
        bool dirty;
        Modifiers currentMods;
        Modifiers oneshotMods;
        Set<KeyCode> keys;
    };

} // namespace absolem

#endif