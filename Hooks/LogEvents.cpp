
#include <iostream>
#include "Hook.h"

namespace Hooks::LogEvents {

    // Structure that is newly created for each game
    struct myData {
        int itemsInInventory = 0;
    };

    struct _ : Hook<myData> {
        static inline myStore *store;

        static void addedItem(Player *player, Item *item) {

            myData *gameData = store->get(player->game);

            int startWith = gameData->itemsInInventory;

            // If item added in inventory
            if (item->container == ItemContainer::Inventory) {
                if (item->action == ItemActionType::PutInContainer) gameData->itemsInInventory++;
                if (item->action == ItemActionType::RemoveFromContainer) gameData->itemsInInventory--;
            }

            if (startWith != gameData->itemsInInventory)
                std::cout << "Items in inventory " << gameData->itemsInInventory << std::endl;
        }

        static void LoadSuccessful(Game *game) {
            std::cout << "load scucesfully" << std::endl;
        }

        // When a new game is created
        void game(Models::Game *game) override {

            game->ego->onItem(ItemEvents::added, addedItem);

            game->on(Models::GameEvents::LoadSuccessful, LoadSuccessful);

        }

        _() {
            store = new myStore(); // Init the store
        }

    } self; // NOLINT(cert-err58-cpp) <-- stfu clion
}
