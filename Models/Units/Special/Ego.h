//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_EGO_H
#define D2PacketBased_EGO_H

#include "../../Collections/UnitCollection.h"
#include "../Item.h"
#include "../../Concepts/Skill.h"
#include "../../Events/SkillEventEmitter.h"

namespace Models {
    class Ego : public Player {
    public:


        // Special items
        Item *cube{};

        struct cursor_t {
            Item *item{};
        } *cursor = new cursor_t();

    private:
        typedef UnitCollection<Ego, Item> ItemContainer;
    public:

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
                this->inventory = new ItemContainer();
                this->traderOffer = new ItemContainer();
                this->forTrade = new ItemContainer();
                this->cube = new ItemContainer();
                this->stash = new ItemContainer();
                this->belt = new ItemContainer();
                this->equipment = new ItemContainer();

                this->items->local = player;
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
        } *container;

        Ego(Game *game) : Player(game) {
            this->container = new Container(this);
        }


        void addItem(Item *item) {
            ItemContainer *curContainer = nullptr;

            if (item->container == ::ItemContainer::Inventory) curContainer = this->container->inventory;
            else if (item->container == ::ItemContainer::TraderOffer) curContainer = this->container->traderOffer;
            else if (item->container == ::ItemContainer::ForTrade) curContainer = this->container->forTrade;
            else if (item->container == ::ItemContainer::Cube) curContainer = this->container->cube;
            else if (item->container == ::ItemContainer::Stash) curContainer = this->container->stash;
            else if (item->container == ::ItemContainer::Belt) curContainer = this->container->belt;

            if (curContainer) {
                // add in specific container
                curContainer->add(item, ItemEvents::putInContainer);

                // Just trigger the event for a
                this->container->items->add(item);

            }

        }
    };
}

#endif //D2PacketBased_EGO_H
