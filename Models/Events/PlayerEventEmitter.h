//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_PLAYEREVENTS_H
#define D2PacketBased_PLAYEREVENTS_H

#include "../Units/Player.h"
#include "Event.h"

namespace Models {
    namespace PlayerEvents {
        enum PlayerEvents {
            added = TypicalEventTypes::added,
            moved = TypicalEventTypes::moved,
            removed = TypicalEventTypes::removed
        };
    }

    template<class Base, class T=Player>
    class PlayerEventEmitter : public Event<T, PlayerEvents::PlayerEvents, Base> {
    public:
        Base *base = nullptr;

        void onPlayer(PlayerEvents::PlayerEvents eventType, std::function<void(Base *, T *)> cb) {
            this->Event::__on(eventType, cb);
        }

        void emitPlayer(PlayerEvents::PlayerEvents eventType, T *item) {

            this->Event::__emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_PLAYEREVENTS_H
