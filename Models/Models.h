//
// Created by jaenster on 15/07/2020.
//


namespace Models {
    class Game;

    class Player;

    class Monster;

    class Object;

    class Missile;

    class Item;

    class Tile;

    class AlterEgo;

    class Ego;

}

#ifndef D2PacketBased_MODELS_H
#define D2PacketBased_MODELS_H


// Handy shit

//#include <windows.h>
#include "types.h"

#include "Enums.h"

// The units itself
#include "Units/Player.h" // 0
#include "Units/Monster.h" // 1
#include "Units/Object.h" // 2
#include "Units/Missile.h" // 3
#include "Units/Item.h" // 4
#include "Units/Tile.h" // 5

// Special units
#include "Units/Special/Ego.h"
#include "Units/Special/AlterEgo.h"

// Event emmiters
#include "Events/Event.h"
#include "Events/PlayerEventEmitter.h" // 0
#include "Events/MonsterEventEmitter.h" // 1
#include "Events/ObjectEventEmitter.h" // 2
#include "Events/MissileEventEmitter.h" // 3
#include "Events/ItemEventEmitter.h" // 4
#include "Events/TileEventEmitter.h" // 5

// Game class
#include "Game.h"

#include "Packets/GS/Receiving.h"

#include "UsingModels.h"

#endif //D2PacketBased_MODELS_H
