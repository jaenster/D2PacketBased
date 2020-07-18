//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_STATCOLLECTION_H
#define D2PacketBased_STATCOLLECTION_H

#include "../Models.h"
#include "Collection.h"
#include "../Concepts/IDAble.h"

namespace Models {
    template<class Base>
    class StatCollection : public Collection<Base, StatBase> {
    public:
        void add(StatBase *unit) {
            auto type = unit->Type;
            if (this->internal.count(type)) {
                delete this->internal[type];
            }
            this->internal[type] = unit;
        }
    };
}

#endif //D2PacketBased_STATCOLLECTION_H
