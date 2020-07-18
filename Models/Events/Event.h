//
// Created by jaenster on 15/07/2020.
//

#ifndef D2PacketBased_EVENT_H
#define D2PacketBased_EVENT_H

#include <map>
#include <vector>
#include <functional>

namespace Models {

    namespace TypicalEventTypes {
        enum TypicalEventTypes {
            added,
            moved,
            removed

        };
    };


    template<class Model, class EventEnums, class Base>
    class Event {

    protected:
//        typedef void (*voidCB)(Base*, Model*);
        typedef std::function<void(Base *, Model *)> voidCB;

        typedef std::vector<voidCB> hookVector;
        std::map<EventEnums, hookVector *> hooks;

    protected:
        void on(EventEnums eventType, voidCB cb);

        void emit(EventEnums eventType, Base *b, Model *m);
    };

    template<class Model, class EventEnums, class Base>
    void Event<Model, EventEnums, Base>::on(EventEnums eventType, Event::voidCB cb) {
        // get hook vector
        hookVector *hv = nullptr;
        if (this->hooks.count(eventType)) {
            hv = this->hooks[eventType];
        } else {
            this->hooks[eventType] = (hv = new hookVector());
        }

        hv->push_back(cb);
    }

    template<class Model, class EventEnums, class Base>
    void Event<Model, EventEnums, Base>::emit(EventEnums eventType, Base *b, Model *m) {
        if (this->hooks.count(eventType)) {

            hookVector *hooks = this->hooks[eventType];
            for (int i = 0; i < hooks->size(); i++) {
                auto hook = hooks[i];

                for (int z = 0; z < hook.size(); z++) {
                    hook[z](b, m);
                }
            }
        }
    }
}

#endif //D2PacketBased_EVENT_H
