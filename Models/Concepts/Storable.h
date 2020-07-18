//
// Created by jaenster on 17/07/2020.
//

#ifndef D2PacketBased_STORABLE_H
#define D2PacketBased_STORABLE_H

namespace Models {
    template<class T, typename U=dword>
    class Storable {

    protected:
        dword __storeId = 0;
    public:
        U getStoreIdentifier() { // Its used @ Store.h
            static dword __storeId = 0;
            if (this->__storeId == 0) this->__storeId = ++__storeId;
            return this->__storeId;
        }

    };

}

#endif //D2PacketBased_STORABLE_H
