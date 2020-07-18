//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_IDCOLLECTION_H
#define D2PacketBased_IDCOLLECTION_H

#include "Collection.h"
#include "../Concepts/IDAble.h"

namespace Models {
    template<class Base, typename T,typename EventType, /* T extends Unit<T>*/ typename std::enable_if<std::is_base_of<IDAble, T>::value>::type * = nullptr>
    class IdCollection : public Collection<Base, T> {
    public:
        void add(T *unit) {
            if (this->internal.count(unit->id)) {
                delete this->internal[unit->id];
            }
            this->internal[unit->id] = unit;

            local->emitSkill(EventType::added, (T *) unit);
        }
    };
}

#endif //D2PacketBased_IDCOLLECTION_H
