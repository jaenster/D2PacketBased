//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_SKILL_H
#define D2PacketBased_SKILL_H
#include "IDAble.h"
namespace Models {
    class Skill: private IDAble {
    public:
        uchar baseLevel = 0;
        Skills::Type id;
    };
}

#endif //D2PacketBased_SKILL_H
