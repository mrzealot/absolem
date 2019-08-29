#ifndef REPORTER_MODULE_H
#define REPORTER_MODULE_H

#include "Module.h"

namespace absolem {

    class ReporterModule : public Module {
        public:
        ReporterModule() : Module("reporter") {}

        virtual bool onAfterTick() override;
        virtual Byte onAfterTickPriority();

        void press(KeyCode key);
        void release(KeyCode key);

        void pressConsumer(UsageCode usage);
        void releaseConsumer();

        void modify(Modifiers mods);
        void unmodify(Modifiers mods);
        void oneshot(Modifiers mods);

        void flush();

        private:
        bool dirty;
        Modifiers currentMods;
        Modifiers oneshotMods;
        Set<KeyCode> keys;
    };

} // namespace absolem

#endif