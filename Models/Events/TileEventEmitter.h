//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_TILEEVENTS_H
#define D2PacketBased_TILEEVENTS_H

#include "../Units/Tile.h"
#include "Event.h"

namespace Models {
    namespace TileEvents {
        enum TileEvents {
            added = TypicalEventTypes::added,
            moved = TypicalEventTypes::moved,
removed = TypicalEventTypes::removed,
        };
    }

    template<class Base>
    class TileEventEmitter : public Event<Tile, TileEvents::TileEvents, Base> {
    public:
        Base *base = nullptr;

        void onTile(TileEvents::TileEvents eventType, std::function<void(Base *, Tile *)> cb) {
            this->Event::__on(eventType, cb);
        }

        void emitTile(TileEvents::TileEvents eventType, Tile *item) {
            this->Event::__emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_TILEEVENTS_H
