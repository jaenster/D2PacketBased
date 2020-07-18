//
// Created by jaenster on 17/07/2020.
//

#ifndef D2PacketBased_UNITEMITTER_H
#define D2PacketBased_UNITEMITTER_H

#include "../../Events/PlayerEventEmitter.h"
#include "../../Events/MonsterEventEmitter.h"
#include "../../Events/ObjectEventEmitter.h"
#include "../../Events/MissileEventEmitter.h"
#include "../../Events/ItemEventEmitter.h"
#include "../../Events/TileEventEmitter.h"

namespace Models {
    template<class Base>
    class UnitEmitter :
            public PlayerEventEmitter<Base>,
            public MonsterEventEmitter<Base>,
            public MissileEventEmitter<Base>,
            public ObjectEventEmitter<Base>,
            public ItemEventEmitter<Base>,
            public TileEventEmitter<Base> {
    protected:
    public:
        UnitEmitter(Base *local) {
            this->PlayerEventEmitter::base = local;
            this->MonsterEventEmitter::base = local;
            this->MissileEventEmitter::base = local;
            this->ObjectEventEmitter::base = local;
            this->ItemEventEmitter::base = local;
            this->TileEventEmitter::base = local;
        }
    };
}

#endif //D2PacketBased_UNITEMITTER_H
