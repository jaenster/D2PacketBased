//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_MISSILEEVENTS_H
#define D2PacketBased_MISSILEEVENTS_H

#include "../Units/Missile.h"
#include "Event.h"

namespace Models {
    namespace MissileEvents {
        enum MissileEvents {
            added = TypicalEventTypes::added,
            moved = TypicalEventTypes::moved,
removed = TypicalEventTypes::removed,
        };
    }

    template<class Base>
    class MissileEventEmitter : public Event<Missile, MissileEvents::MissileEvents, Base> {
    public:
        Base *base = nullptr;

        void onMissile(MissileEvents::MissileEvents eventType, std::function<void(Base *, Missile *)> cb) {
            this->Event::__on(eventType, cb);
        }

        void emitMissile(MissileEvents::MissileEvents eventType, Missile *item) {
            this->Event::__emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_MISSILEEVENTS_H
