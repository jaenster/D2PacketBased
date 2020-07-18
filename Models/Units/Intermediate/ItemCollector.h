//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_ITEMCOLLECTOR_H
#define D2PacketBased_ITEMCOLLECTOR_H

#include "../Item.h"
#include "../../Events/ItemEventEmitter.h"
#include "../../Collections/UnitCollection.h"

namespace Models {
    template<class Base>
    class ItemCollector : public ItemEventEmitter<Base> {
    protected:
        Base *base;

    public:
        UnitCollection<Base, Item> *inventory;
        UnitCollection<Base, Item> *equipment;
        UnitCollection<Base, Item> *shop;
        UnitCollection<Base, Item> *stash;
        UnitCollection<Base, Item> *cube;



        ItemCollector() {

            this->inventory = new UnitCollection<Base,Item>;
            this->equipment = new UnitCollection<Base,Item>;
            this->shop = new UnitCollection<Base,Item>;
            this->stash = new UnitCollection<Base,Item>;
            this->cube = new UnitCollection<Base,Item>;

//            this->inventory->local = this;
        }


    };
}


#endif //D2PacketBased_ITEMCOLLECTOR_H
