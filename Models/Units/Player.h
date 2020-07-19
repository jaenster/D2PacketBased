//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_PLAYER_H
#define D2PacketBased_PLAYER_H

#include "Unit.h"
#include "Intermediate/Living.h"
#include "Intermediate/UnitEmitter.h"
#include "../Collections/IdCollection.h"
#include "../Events/SkillEventEmitter.h"


namespace Models {
    class Player :
            public Living,
            public Unit<Player>,
            public UnitEmitter<Player>,
            public SkillEventEmitter<Player> {
    private:
        typedef IdCollection<Player, Skill, SkillEvents::SkillEvents> SkillCollection;

    public:
        std::string name;

        // Living Objects got skills
        SkillCollection *skills;

        AlterEgo *alterEgo;

        Player(Game *game) : UnitEmitter(this), Unit(game) {
            this->skills = new SkillCollection();

            this->skills->local = this;

            this->SkillEventEmitter::base = this;
            this->Unit<Player>::base = this;

            this->selectedSkill[0] = new hand_t();
            this->selectedSkill[1] = new hand_t();
        }

        ~Player() {
            delete this->skills;

            delete this->selectedSkill[0];
            delete this->selectedSkill[1];
        }

        struct hand_t {
            word skill;
            Item *item;
        } *selectedSkill[2];
    };
}


#endif //D2PacketBased_PLAYER_H
