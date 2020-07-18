//
// Created by jaenster on 17/07/2020.
//

#ifndef D2PacketBased_STORABLE_H
#define D2PacketBased_STORABLE_H

#include "../Game.h"

namespace Models {
    template <class T, typename U=dword>
    class Storable {

    protected:
        virtual U __storable__GetStoreIdentifer() {
            return 0;
        };
    public:
        U getStoreIdentifier() {
            return this->__storable__GetStoreIdentifer();
        }

    };

}

#endif //D2PacketBased_STORABLE_H
