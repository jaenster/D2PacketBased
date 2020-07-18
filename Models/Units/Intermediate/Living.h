//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_LIVING_H
#define D2PacketBased_LIVING_H

#include "Equipable.h"
#include "Locatable.h"

namespace Models {

    // A living object, like a player or a monster
    class Living :
            public Equipable,
            public Locatable {


    public:


        Living() {


        };


    };
}


#endif //D2PacketBased_LIVING_H
