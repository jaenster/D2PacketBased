//
// Created by jaenster on 17/07/2020.
//

#ifndef D2PacketBased_HOOK_H
#define D2PacketBased_HOOK_H

#include "../Models/Models.h"
#include "../Models/Collections/Collection.h"
#include "../Models/Concepts/Store.h"


namespace Hooks {
    template<class Self, class T, /* T extends Unit<T>*/ typename std::enable_if<std::is_base_of<Models::Storable<T, dword>, T>::value>::type * = nullptr>
    class HookData {
    public:
        HookData() {};
    };


    class BaseHook {
    public:
        virtual void init() {

        }

        virtual void init(int argc, char *argv[]) {

        }

        virtual void game(Models::Game *game) {

        };

        virtual void player(Models::Player *player) {

        };

        virtual void item(Models::Item *item) {

        };

        BaseHook() {
            this->previous = last;
            last = this;
        };

        BaseHook *previous;
        static inline BaseHook *last = nullptr;
    };

    inline void addGame(Game *game) {
        for (auto *f = BaseHook::last; f; f = f->previous) f->game(game);
    }

    inline void init(int argc, char *argv[]) {
        for (auto *f = BaseHook::last; f; f = f->previous) {
            f->init(argc, argv);
            f->init();
        }
    }

    // Dont want a template in the base hook due to the inability to use static variables
    template<typename T>
    class Hook : public BaseHook {
    protected:
        typedef Models::Store<T, Game> myStore;
    };
}

#endif //D2PacketBased_HOOK_H
