//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_UNITCOLLECTION_H
#define D2PacketBased_UNITCOLLECTION_H
#include "Collection.h"
#include "../Models.h"

namespace Models {
    template<class Base, typename T, class U=dword, /* T extends Unit<T>*/ typename std::enable_if<std::is_base_of < Unit < T>, T> ::value> ::type * = nullptr>

    class UnitCollection : public Collection<Base, T> {
    public:
        void add(T *unit) {
            if (this->internal.count(unit->uid)) {
                delete this->internal[unit->uid];
            }
            this->internal[unit->uid] = unit;

            switch ((UnitType::UnitType) unit->type) {
                case UnitType::Player:
                    local->emitPlayer(PlayerEvents::added, (Player *) unit);
                    break;
                case UnitType::Monster:
                    local->emitMonster(MonsterEvents::added, (Monster *) unit);
                    break;
                case UnitType::Missile:
                    local->emitMissile(MissileEvents::added, (Missile *) unit);
                    break;
                case UnitType::Object:
                    local->emitObject(ObjectEvents::added, (Object *) unit);
                    break;
                case UnitType::Item:
                    local->emitItem(ItemEvents::added, (Item *) unit);
                    break;
                case UnitType::Tile:
                    local->emitTile(TileEvents::added, (Tile *) unit);
                    break;
            }
        }
        void add(T *unit, unsigned int e) {
            if (this->internal.count(unit->uid)) {
                delete this->internal[unit->uid];
            }
            this->internal[unit->uid] = unit;

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
    };
}

#endif //D2PacketBased_UNITCOLLECTION_H
