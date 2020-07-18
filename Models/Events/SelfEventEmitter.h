//
// Created by jaenster on 18/07/2020.
//

#ifndef D2PACKETBASE_SELFEVENTEMITTER_H
#define D2PACKETBASE_SELFEVENTEMITTER_H

#include "Event.h"
#include "../types.h"

namespace Models {
    namespace Events {
        enum Events {
            added = TypicalEventTypes::added,
            moved = TypicalEventTypes::moved,
            removed = TypicalEventTypes::removed,
        };
    }

    template<class Base, class Enums>
    class SelfEventEmitter : Event<void, Enums, Base> {
    public:
        Base *base = nullptr;

        void on(Enums eventType, std::function<void(Base *)> cb) {
            this->Event::__on(eventType, cb);
        }

        void emit(Enums eventType) {
            this->Event::__emit(eventType, base);
        }
    };
}
#endif //D2PACKETBASE_SELFEVENTEMITTER_H
