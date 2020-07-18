//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_COLLECTION_H
#define D2PacketBased_COLLECTION_H

#include <map>
#include "../../Types.h"

namespace Models {
    template<class T, class U>
    class Collection {
    private:
        std::map<T, U> map;

        CRITICAL_SECTION *cs = nullptr;
    protected:
        void setCS(CRITICAL_SECTION *section) { if (this->cs != nullptr) {delete cs;} /* clean up*/ this->cs = section; }

    public:
        U *&operator[](T);

        void set(T id, U val);

    };

    template<class T, class U>
    U *&Collection<T, U>::operator[](T) {
        if (!this->map.count(_byVal)) {
            return nullptr;
        }
        return this->map[_byVal];
    }

    template<class T, class U>
    void Collection<T, U>::set(T id, U val) {

        // Figure out if we need to delete the _old_ value
        std::map<T,U>::iterator it = this->map.find(_byVal);

        EnterCriticalSection(&cs);
        if (it != this->map->end()) {
            this->map.erase(it);
        }

        this->operator[](id) = val;
        LeaveCriticalSection(&cs);
    }
}


#endif //D2PacketBased_COLLECTION_H
