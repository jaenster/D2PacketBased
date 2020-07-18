//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_LIVING_H
#define D2PacketBased_LIVING_H

#include "Equipable.h"
#include "Locatable.h"
#include "../../Collections/IdCollection.h"
#include "../../Concepts/Skill.h"
#include "../../Events/SkillEventEmitter.h"

namespace Models {

    // A living object, like a player or a monster
    template<class Base>
    class Living :
            public Equipable,
            public Locatable
            {



    public:


        Living() {


        };


    };
}


#endif //D2PacketBased_LIVING_H
