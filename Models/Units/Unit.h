//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_UNIT_H
#define D2PacketBased_UNIT_H

#include "../Enums.h"
#include "../Concepts/Storable.h"
#include "../Events/SelfEventEmitter.h"

namespace Models {
    namespace UnitEvents {
        enum UnitEvents {
            attacked,
            attacking,
        };
    }

    class BaseUnit : public SelfEventEmitter<BaseUnit, UnitEvents::UnitEvents> {
    public:
        Game *game;
        dword uid = 0;
        UnitType::UnitType type = UnitType::Invalid;
        word classid;
        dword mode;
    };

    template<class Base>
    class Unit :
            public Storable<Base, dword>,
            public BaseUnit {
    protected:
        Base *base = nullptr;
    public:
        Unit() {

        }

        Unit(Game *game) {
            this->game = game;
        }
    };
}

#endif //D2PacketBased_UNIT_H
