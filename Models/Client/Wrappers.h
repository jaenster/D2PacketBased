//
// Created by jaenster on 16/07/2020.
//
#ifndef D2PacketBased_WRAPPERS_H
#define D2PacketBased_WRAPPERS_H

#include "../Models.h"
#include "../../D2/D2Ptrs.h"

namespace Models {
    class Native {
    public:
        inline static Act *LoadAct(uchar actNumber, dword mapid, uchar dificulty) {
            if (actNumber > 4) return nullptr;

            static int actTownLevel[] = {1, 40, 75, 103, 109, 137};
            return D2COMMON_LoadAct(actNumber, mapid, true, false, dificulty, NULL, actTownLevel[actNumber],
                                    D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2);
        }

        inline static void UnloadAct(Act *act) {
            return D2COMMON_UnloadAct(act);
        };

        inline static void AddRoomData(Room2 *room) {
            D2COMMON_AddRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY,
                                 room->pRoom1);
        };

        inline static void RemoveRoomData(Room2 *room) {
            D2COMMON_RemoveRoomData(room->pLevel->pMisc->pAct, room->pLevel->dwLevelNo, room->dwPosX, room->dwPosY,
                                    room->pRoom1);
        }
    };
};

#endif //D2PacketBased_WRAPPERS_H
