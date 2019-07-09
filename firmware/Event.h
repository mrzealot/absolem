#ifndef EVENT_H
#define EVENT_H

namespace absolem {

    struct Event {
        public:
        Event(Key key, bool active);
        Key key;
        bool active;
    };
    
} // namespace absolem


#endif