//
// Created by jaenster on 18/07/2020.
//

#ifndef D2PacketBased_STORE_H
#define D2PacketBased_STORE_H

#include "Storable.h"

namespace Models {
    template<class Base, typename T, class U=dword, /* T extends Unit<T>*/ typename std::enable_if<std::is_base_of<Models::Storable<T, U>, T>::value>::type * = nullptr>
    class Store : public Collection<T, Base, U> {
    public:
        Base *get(T *t) {
            return this->operator[](t);
        }

        Base *operator[](T *t) {
            U key = t->getStoreIdentifier();

            Base *base = nullptr;
            if (!this->internal.count(key)) {
                this->internal[key] = (base = new Base());
            } else {
                base = this->internal[key];
            };


            return base;
        }
    };
}
#endif //D2PacketBased_STORE_H
