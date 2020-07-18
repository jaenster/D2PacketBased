//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_MONSTEREVENTS_H
#define D2PacketBased_MONSTEREVENTS_H

#include "../Units/Monster.h"
#include "Event.h"

namespace Models {
    namespace MonsterEvents {
        enum MonsterEvents {
            added,
            moved,
        };
    }

    template<class Base>
    class MonsterEventEmitter : public Event<Monster, MonsterEvents::MonsterEvents, Base> {
    public:
        Base *base = nullptr;

        void onMonster(MonsterEvents::MonsterEvents eventType, std::function<void(Base *, Monster *)> cb) {
            this->Event::on(eventType, cb);
        }

        void emitMonster(MonsterEvents::MonsterEvents eventType, Monster *item) {

            this->Event::emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_MONSTEREVENTS_H
