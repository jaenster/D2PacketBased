//
// Created by jaenster on 19/07/2020.
//

#ifndef D2PACKETBASE_ORB_H
#define D2PACKETBASE_ORB_H

#include "../types.h"

namespace Models {
    struct Orb {
        // Current
        word current:15;

        // needs to be calculated
        word max:15;

        word base:15;
        word bo:15;
        word oak:15;
        word wolf:15;
        word bear:15;

        word regen:7;

        void update() {
            max = base + bo + oak + wolf + bear;
        }

        Orb() {
            current = 0;
            max = 0;
            base = 0;
            bo = 0;
            oak = 0;
            wolf = 0;
            bear = 0;
            regen = 0;
        }
    };
}
#endif //D2PACKETBASE_ORB_H
