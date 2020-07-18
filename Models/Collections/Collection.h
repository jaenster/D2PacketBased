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
    protected:
        void _add(U u, T* unit) {
            uchar count = this->internal.contains(u);
            if (count) this->internal.erase(u);

            this->internal[u] = unit;
        }
        void _remove(U u )
        {
            uchar count = this->internal.contains(u);
            if (count) this->internal.erase(u);
        }
    public:
        Base *local;

        virtual void remove(U u) {
            this->_remove(u);
        }
        virtual void add(U u,T *unit) {
            this->_add(u,unit);
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
