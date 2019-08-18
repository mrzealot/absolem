#ifndef CACHE_MODULE_H
#define CACHE_MODULE_H

#include "Module.h"

namespace absolem {

    class CacheModule : public Module {
        public:
        CacheModule() : Module("cache") {}

        virtual bool onDirectSearch(Key k, List<Rule>& candidates) override;
        virtual Byte onDirectSearchPriority() override;

        void set(Key k, Rule r);

        private:
        Map<Key, Rule> rules;
    };

} // namespace absolem

#endif