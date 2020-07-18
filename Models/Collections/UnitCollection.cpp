//
// Created by jaenster on 17/07/2020.
//

#include "../Models.h"
#include "UnitCollection.h"

#define T_EXTENDS_X(x) typename std::enable_if<std::is_base_of<x, T> ::value> ::type * = nullptr

//template<class Base, typename T>
//template<T_EXTENDS_X(Item)>
//void Models::UnitCollection<Base,T>::add(T *unit) {
//        if (this->internal.count(unit->uid)) {
//            delete this->internal[unit->uid];
//        }
//        this->internal[unit->uid] = unit;
//        local->emitItem(ItemEvents::added, (Item *) unit);
//        break;
//};
