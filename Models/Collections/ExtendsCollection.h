//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_EXTENDSCOLLECTION_H
#define D2PacketBased_EXTENDSCOLLECTION_H

//#include <type_traits>
#include "Collection.h"
namespace Models {

    // T extends StatBase
    template<class Base, class T, class Parent,typename U, typename std::enable_if<std::is_base_of<Parent, T>::value>::type * = nullptr>
    struct ExtendsCollection : public Collection<Base,T,U> {

    };
};


#endif //D2PacketBased_EXTENDSCOLLECTION_H
