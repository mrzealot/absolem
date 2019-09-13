#ifndef REPORTER_MODULE_H
#define REPORTER_MODULE_H

#include "../../common/String2CodeMap.h"
#include "Module.h"

namespace absolem {

    using Report = Pair<Modifiers, Set<KeyCode>>;

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

        void report();
        void flush();
        void type(String str, const String2CodeMap& map);

        private:
        bool dirty;
        Modifiers currentMods;
        Modifiers oneshotMods;
        Set<KeyCode> keys;
        List<Report> queue;
    };

} // namespace absolem

#endif