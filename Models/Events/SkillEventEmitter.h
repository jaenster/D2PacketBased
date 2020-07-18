//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_SKILLEVENTS_H
#define D2PacketBased_SKILLEVENTS_H


#include <utility>
#include "Event.h"
#include "../Concepts/Skill.h"

namespace Models {
    namespace SkillEvents {
        enum SkillEvents {
            added,
            moved,
        };
    }

    template<class Base>
    class SkillEventEmitter : Event<Skill, SkillEvents::SkillEvents, Base> {
    public:
        Base *base = nullptr;
        void onSkill(SkillEvents::SkillEvents eventType, std::function<void(Base *, Skill *)> cb) {
            this->Event::on(eventType, cb);
        }
        void emitSkill(SkillEvents::SkillEvents eventType, Skill* item) {
            this->Event::emit(eventType, base, item);
        }
    };
}


#endif //D2PacketBased_SKILLEVENTS_H
