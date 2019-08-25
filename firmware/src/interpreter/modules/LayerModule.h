#ifndef LAYER_MODULE_H
#define LAYER_MODULE_H

#include "Module.h"

namespace absolem {

    class LayerModule : public Module {
        public:
        LayerModule(Byte o) : Module("layer"), offset(o) {
            stack.push(0);
        }

        virtual bool onMapKey(VirtualKey& k) override;
        virtual Byte onMapKeyPriority();

        void push(Byte layer);
        void pop();
        void oneshot(Byte layer);
        void toggleLock();

        private:
        Byte offset;
        Stack<Byte> stack;
        bool oneshotActive;
        bool locked;

    };

} // namespace absolem

#endif