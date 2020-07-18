//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_GAME_H
#define D2PacketBased_GAME_H


#include <algorithm>
#include "Collections/UnitCollection.h"
#include "Concepts/Storable.h"

#include "Units/Unit.h"
#include "Units/Monster.h"

#include "Events/Event.h"
#include "Events/PlayerEventEmitter.h"
#include "Events/MonsterEventEmitter.h"
#include "Events/MissileEventEmitter.h"
#include "Events/ObjectEventEmitter.h"
#include "Events/TileEventEmitter.h"
#include "../D2/D2Structs.h"
#include "Client/Wrappers.h"


namespace Models {

    class Game :

            // EVENTS
            public PlayerEventEmitter<Game>,
            public MonsterEventEmitter<Game>,
            public MissileEventEmitter<Game>,
            public ObjectEventEmitter<Game>,
            public ItemEventEmitter<Game>,
            public TileEventEmitter<Game>,
            public Storable<Game, dword> {

    public:
        UnitCollection<Game, Player> *players; // 0
        UnitCollection<Game, Monster> *monsters; // 1
        UnitCollection<Game, Object> *objects; // 2
        UnitCollection<Game, Missile> *missiles; // 3
        UnitCollection<Game, Item> *items; // 4, note these contain _all_ items in the game
        UnitCollection<Game, Tile> *tiles; // 5

        // Special units
        Ego *ego;

        struct World {
        private:
            dword mapid;
        public:
            bool compressed = false;
            Difficulty difficulty;
            bool unknown;
            bool hardcore;
            bool expansion;
            bool ladder;

            void setMapSeed(dword seed) {
                this->mapid = seed;
                for (int i = 0; i < 5; i++) this->act[i] = Native::LoadAct(i, this->mapid, this->difficulty);
            }

            dword getMapSeed() {
                return mapid;
            }

            Act *act[5];

            UnitCollection<Game, Item> *ground; // items in the world, aka on the floor

            World(Game *game) {
                for (int i = 0; i < 5; i++) this->act[i] = nullptr;
                this->ground = new UnitCollection<Game, Item>;
                this->ground->local = game;
            }

            ~World() {
                for (int i = 0; i < 5; i++) if (this->act[i] != nullptr) Native::UnloadAct(this->act[i]);
                delete this->ground;
            }
        } *world;

    private:
        typedef UnitCollection<Game, Item> ItemContainer;
    public:

        struct Shop {
            // _All_ items on you (contains duplicates of these collections below)
            ItemContainer *items;

            ItemContainer *armorTab;
            ItemContainer *weaponTab1;
            ItemContainer *weaponTab2;
            ItemContainer *miscTab;

            void process(Item *item) {
                ItemContainer *curContainer = nullptr;

                if (item->container == ::ItemContainer::ArmorTab) curContainer = armorTab;
                else if (item->container == ::ItemContainer::WeaponTab1) curContainer = weaponTab1;
                else if (item->container == ::ItemContainer::WeaponTab2) curContainer = weaponTab2;
                else if (item->container == ::ItemContainer::MiscTab) curContainer = miscTab;

                if (curContainer) {
                    if (item->action == ItemActionType::RemoveFromShop) {
                        curContainer->remove(item, ItemEvents::containerRemove);
                        items->remove(item);
                    } else {
                        curContainer->add(item, ItemEvents::containerAdd);
                        items->add(item);
                    }
                }
            }

            Shop(Game *game) {
                this->items = new ItemContainer();
                this->items->local = game;


                this->armorTab = new ItemContainer();
                this->weaponTab1 = new ItemContainer();
                this->weaponTab2 = new ItemContainer();
                this->miscTab = new ItemContainer();

                this->armorTab->local = game;
                this->weaponTab1->local = game;
                this->weaponTab2->local = game;
                this->miscTab->local = game;
            }

            ~Shop() {
                delete this->items;
                delete this->armorTab;
                delete this->weaponTab1;
                delete this->weaponTab2;
                delete this->miscTab;
            }
        } *shop;


        Game();

        ~Game();

        void newItem(Item *item) {
            this->items->add(item);
        }


        // Just a unique number generator for the store

    protected:
        dword __storable__GetStoreIdentifer() {
            static dword __storeId = 0;
            if (this->__storeId ==  0) this->__storeId = ++__storeId;
            return this->__storeId;
        };
        dword __storeId = 0;
    };
}

#endif //D2PacketBased_GAME_H
