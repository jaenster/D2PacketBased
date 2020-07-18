//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_TILE_H
#define D2PacketBased_TILE_H

#include "Unit.h"
#include "Intermediate/Locatable.h"

namespace Models {
    class Tile : public Unit<Tile>, public Locatable {


        Tile() {
            this->base = this;
            this->type = UnitType::Tile;
        }
    };
}

#endif //D2PacketBased_TILE_H
