//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_COLLECTION_H
#define D2PacketBased_COLLECTION_H

namespace Models {
    template<class Base, class T, typename U=unsigned long>
    struct Collection {
    protected:
        std::map<U, T *> internal;
    public:
        Base *local;

        virtual void add(U u,T *unit) {
            if (this->internal.count(u)) {
                delete this->internal[u];
            }
            this->internal[u] = unit;
        }
        virtual T* get(U u) {
            return this->operator[](u);
        }

        Collection() {

        }

        ~Collection() {
            // clean up map references
            std::for_each(internal.begin(), internal.end(), [](std::pair<U, T *> a) { delete a.second; });
        }



        T *operator[](U id) {
            if (this->internal.count(id)) {
                return this->internal[id];
            }
            return nullptr;
        }
    };
};


#endif //D2PacketBased_COLLECTION_H
