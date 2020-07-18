//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_PLAYER_H
#define D2PacketBased_PLAYER_H

#include "Unit.h"
#include "Intermediate/Living.h"
#include "Intermediate/UnitEmitter.h"
#include "../Collections/IdCollection.h"


namespace Models {
    class Player :
            public Living<Player>,
            public Unit<Player>,
            public UnitEmitter<Player>,
            public SkillEventEmitter<Player> {
    private:
        typedef IdCollection<Player, Skill, SkillEvents::SkillEvents> SkillCollection;

    public:
        std::string name;
        // Living Objects got skills
        SkillCollection *skills;

        Player(Game *game) : UnitEmitter(this), Unit(game) {
            this->skills = new SkillCollection();

            this->skills->local = this;

            this->SkillEventEmitter::base = this;
            this->Unit<Player>::base = this;
        }
    };
}


#endif //D2PacketBased_PLAYER_H
