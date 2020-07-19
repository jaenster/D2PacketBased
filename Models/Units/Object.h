//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_OBJECT_H
#define D2PacketBased_OBJECT_H

#include "Unit.h"
#include "Intermediate/Locatable.h"

namespace Models {
    class Object : public Unit<Object>, public Locatable {


    public:

        uchar portalFlags;
        bool targetable;


        Object() {
            this->base = this;
            this->type = UnitType::Object;
        }
    };
}

#endif //D2PacketBased_OBJECT_H
