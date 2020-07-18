//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_ITEMEVENTS_H
#define D2PacketBased_ITEMEVENTS_H


#include <utility>

#include "../Units/Item.h"
#include "Event.h"

namespace Models {
    namespace ItemEvents {
        enum ItemEvents {
            added = TypicalEventTypes::added,
            moved = TypicalEventTypes::moved,
removed = TypicalEventTypes::removed,

            putInContainer,


            // Special packet stuff
            packet_0x9C,
            packet_0x9D,
        };
    }

    template<class Base>
    class ItemEventEmitter : public Event<Item, ItemEvents::ItemEvents, Base> {
    public:
        Base *base = nullptr;
        void onItem(ItemEvents::ItemEvents eventType, std::function<void(Base *, Item *)> cb) {
            this->Event::on(eventType, cb);
        }
        void emitItem(ItemEvents::ItemEvents eventType, Item* item) {

            this->Event::emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_ITEMEVENTS_H
