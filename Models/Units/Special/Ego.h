//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_EGO_H
#define D2PacketBased_EGO_H

#include "../../Collections/UnitCollection.h"
#include "../Item.h"
#include "../../Concepts/Skill.h"
#include "../../Concepts/Orb.h"
#include "../../D2Data/Quests.h"

namespace Models {
    class Ego : public Player {
    private:
        typedef UnitCollection<Ego, Item> ItemContainer;
    public:


        // Special items
        Item *cube{};

        struct cursor_t {
            Item *item{};
        } *cursor = new cursor_t();

        struct Container {
            // _All_ items on you (contains duplicates of these collections below)
            ItemContainer *items;

            ItemContainer *inventory;
            ItemContainer *traderOffer;
            ItemContainer *forTrade;
            ItemContainer *cube;
            ItemContainer *stash;
            ItemContainer *belt;
            ItemContainer *equipment; // Not tagged as a buffer type

            Container(Ego *player) {
                this->items = new ItemContainer();
                this->items->local = player;

                this->inventory = new ItemContainer();
                this->traderOffer = new ItemContainer();
                this->forTrade = new ItemContainer();
                this->cube = new ItemContainer();
                this->stash = new ItemContainer();
                this->belt = new ItemContainer();
                this->equipment = new ItemContainer();

                this->inventory->local = player;
                this->traderOffer->local = player;
                this->forTrade->local = player;
                this->cube->local = player;
                this->stash->local = player;
                this->belt->local = player;
                this->equipment->local = player;
            }

            ~Container() {
                delete this->items;
                delete this->inventory;
                delete this->traderOffer;
                delete this->forTrade;
                delete this->cube;
                delete this->stash;
                delete this->belt;
                delete this->equipment;
            }

            void process(Item *item) {
                ItemContainer *curContainer = nullptr;

                if (item->container == ::ItemContainer::Inventory) curContainer = inventory;
                else if (item->container == ::ItemContainer::TraderOffer) curContainer = traderOffer;
                else if (item->container == ::ItemContainer::ForTrade) curContainer = forTrade;
                else if (item->container == ::ItemContainer::Cube) curContainer = cube;
                else if (item->container == ::ItemContainer::Stash) curContainer = stash;
                else if (item->container == ::ItemContainer::Belt) curContainer = belt;

                if (curContainer) {
                    if (item->action == ItemActionType::RemoveFromContainer) {
                        curContainer->remove(item, ItemEvents::containerRemove);
                        items->remove(item);
                    } else {
                        // add in specific container
                        curContainer->add(item, ItemEvents::containerAdd);

                        // Just trigger the event for a
                        items->add(item);
                    }
                }

            }
        } *container;

        Ego(Game *game) : Player(game) {
            this->container = new Container(this);
            this->life = new Orb();
            this->mana = new Orb();
            this->stamina = new Orb();

            for (int i = 0; i < 41; i++) this->quests[i] = new QuestData();
        }

        ~Ego() {
            delete this->life;
            delete this->mana;
            delete this->stamina;

            for (int i = 0; i < 41; i++) delete this->quests[i];
        }

        Orb *life;
        Orb *mana;
        Orb *stamina;

        dword experience;

        dword gold;
        dword goldBank;

        struct QuestData {
            QuestType::QuestType type;
            QuestState::QuestState state;
            QuestStanding::QuestStanding standing;
            bool availability;
            uchar log;
        } *quests[41];

        struct icon_t {
            uchar type;
            Item item;
            short skillId;
        } icon;
    };
}

#endif //D2PacketBased_EGO_H
