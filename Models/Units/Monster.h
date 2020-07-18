//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_MONSTER_H
#define D2PacketBased_MONSTER_H

#include "../Models.h"
#include "Unit.h"

namespace Models {

    class Monster : public Unit<Monster>,
                    public Living {

    };
}

#endif //D2PacketBased_MONSTER_H
