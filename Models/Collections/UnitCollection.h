//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_UNITCOLLECTION_H
#define D2PacketBased_UNITCOLLECTION_H

#include "Collection.h"
#include "../Models.h"

namespace Models {
    template<class Base, typename T, class U=dword, /* T extends Unit<T>*/ typename std::enable_if<std::is_base_of<Unit<T>, T>::value>::type * = nullptr>

    class UnitCollection : public Collection<Base, T> {
    private:
        void emit(dword e, T *unit) {
            switch ((UnitType::UnitType) unit->type) {
                case UnitType::Player:
                    local->emitPlayer((PlayerEvents::PlayerEvents) e, (Player *) unit);
                    break;
                case UnitType::Monster:
                    local->emitMonster((MonsterEvents::MonsterEvents) e, (Monster *) unit);
                    break;
                case UnitType::Missile:
                    local->emitMissile((MissileEvents::MissileEvents) e, (Missile *) unit);
                    break;
                case UnitType::Object:
                    local->emitObject((ObjectEvents::ObjectEvents) e, (Object *) unit);
                    break;
                case UnitType::Item:
                    local->emitItem((ItemEvents::ItemEvents) e, (Item *) unit);
                    break;
                case UnitType::Tile:
                    local->emitTile((TileEvents::TileEvents) e, (Tile *) unit);
                    break;
            }
        }

    public:
        void remove(T *unit) {
            this->remove(unit, TypicalEventTypes::removed);
        }

        void remove(T *unit, dword e) {
            this->emit(e, unit);
            this->_remove(unit->uid);
        }

        void remove(U u) {
            if (this->internal.count(u)) this->remove(this->get(u));
        }

        void add(T *unit, dword e) {
            this->_add(unit->uid, unit);
            this->emit(e, unit);
        }

        void add(T *unit) {
            this->add(unit, TypicalEventTypes::added);
        }
    };
}

#endif //D2PacketBased_UNITCOLLECTION_H
