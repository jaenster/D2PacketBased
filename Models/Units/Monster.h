//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_MONSTER_H
#define D2PacketBased_MONSTER_H

#include "Unit.h"
#include "Intermediate/Living.h"

namespace Models {

    class Monster: Living<Monster>, public Unit<Monster> {

    };
}

#endif //D2PacketBased_MONSTER_H
