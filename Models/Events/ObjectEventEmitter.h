//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_OBJECTEVENTS_H
#define D2PacketBased_OBJECTEVENTS_H

#include "../Units/Object.h"
#include "Event.h"

namespace Models {
    namespace ObjectEvents {
        enum ObjectEvents {
            added = TypicalEventTypes::added,
            moved = TypicalEventTypes::moved,
removed = TypicalEventTypes::removed,
        };
    }

    template<class Base>
    class ObjectEventEmitter : public Event<Object, ObjectEvents::ObjectEvents, Base> {
    public:
        Base *base = nullptr;

        void onObject(ObjectEvents::ObjectEvents eventType, std::function<void(Base *, Object *)> cb) {
            this->Event::on(eventType, cb);
        }

        void emitObject(ObjectEvents::ObjectEvents eventType, Object *item) {

            this->Event::emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_OBJECTEVENTS_H
