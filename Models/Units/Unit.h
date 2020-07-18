//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_UNIT_H
#define D2PacketBased_UNIT_H

#include "../Enums.h"

namespace Models {
    template <class Base>
    class Unit {
    protected:
        Base *base = nullptr;
    public:
        Game *game;
        dword uid = 0;
        UnitType::UnitType type = UnitType::Invalid;
        word classid;

        Unit() {

        }
        Unit(Game* game) {
            this->game = game;
        }
    };
}

#endif //D2PacketBased_UNIT_H
