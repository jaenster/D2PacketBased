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

    public:
        void getsHit() {

        }

        struct info_t {
            uchar show; // 1
            uchar unused; // 2
            word messageId; // 4
        } *info[8];

        Monster() {
            this->Unit<Monster>::base = this;
            for (int i = 0; i < 8; i++) info[i] = new info_t();
        }

        ~Monster() {
            for (int i = 0; i < 8; i++) delete info[i];
        }
    };
}

#endif //D2PacketBased_MONSTER_H
