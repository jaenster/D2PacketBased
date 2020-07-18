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
            removed,
            updated

        };
    };


    template<class Model, class EventEnums, class Base>
    class Event {

    protected:
        typedef std::function<void(Base *)> voidCBSingle;
        typedef std::function<void(Base *, Model *)> voidCBDuo;

        typedef std::vector<voidCBSingle> hookVectorSingle;
        typedef std::vector<voidCBDuo> hookVectorDuo;

        std::map<EventEnums, hookVectorSingle *> hooksSingle;
        std::map<EventEnums, hookVectorDuo *> hooksDuo;
    protected:
        void __on(EventEnums eventType, voidCBSingle cb);

        void __on(EventEnums eventType, voidCBDuo cb);

        void __emit(EventEnums eventType, Base *b);

        void __emit(EventEnums eventType, Base *b, Model *m);
    };


    template<class Model, class EventEnums, class Base>
    void Event<Model, EventEnums, Base>::__on(EventEnums eventType, Event::voidCBSingle cb) {
        // get hook vector
        hookVectorSingle *hv = nullptr;
        if (this->hooksSingle.contains(eventType)) {
            hv = this->hooksSingle[eventType];
        } else {
            this->hooksSingle[eventType] = (hv = new hookVectorSingle());
        }

        hv->push_back(cb);
    }

    template<class Model, class EventEnums, class Base>
    void Event<Model, EventEnums, Base>::__on(EventEnums eventType, voidCBDuo cb) {
        // get hook vector
        hookVectorDuo *hv = nullptr;
        if (this->hooksDuo.contains(eventType)) {
            hv = this->hooksDuo[eventType];
        } else {
            this->hooksDuo[eventType] = (hv = new hookVectorDuo());
        }

        hv->push_back(cb);
    }

    template<class Model, class EventEnums, class Base>
    void Event<Model, EventEnums, Base>::__emit(EventEnums eventType, Base *b) {
        if (this->hooksSingle.contains(eventType)) {

            auto *hooks = this->hooksSingle[eventType];
            for (int i = 0; i < hooks->size(); i++) {
                auto hook = hooks[i];

                for (int z = 0; z < hook.size(); z++) {
                    hook[z](b);
                }
            }
        }
    }

    template<class Model, class EventEnums, class Base>
    void Event<Model, EventEnums, Base>::__emit(EventEnums eventType, Base *b, Model *m) {
        if (this->hooksDuo.contains(eventType)) {

            auto *hooks = this->hooksDuo[eventType];
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
