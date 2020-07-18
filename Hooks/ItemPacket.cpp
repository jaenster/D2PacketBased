#include "Hook.h"


/*

 Add some post item packet parsing info on the item

 */

namespace Hooks::ItemPacket {

    void itemAdded(Game *game, Item *item) {
        // Store the game @ the item
        item->game = game;

        // Fetch the variable locally
        Ego *ego = game->ego;

        // If new item is a cube
        if (item->destination != ItemDestination::Ground) {
            if (item->baseItem && item->baseItem->code == "box") {
                ego->cube = item;
            }
        }

        // Allocate cursor
        auto *cursor = ego->cursor;
        if (item->destination == ItemDestination::Cursor) {
            if (item->action != ItemActionType::RemoveFromShop) {
                cursor->item = item;
            }
        } else if (cursor->item && cursor->item->uid == item->uid) {
            cursor->item = nullptr;
        }

        //
        switch (item->action) {
            case ItemActionType::AddToGround:
            case ItemActionType::GroundToCursor:
            case ItemActionType::DropToGround:
            case ItemActionType::OnGround:
                game->world->ground->add(item);
                break;

            case ItemActionType::UNKNOWNx16:
            case ItemActionType::UNKNOWNx14:
                break;

            case ItemActionType::PutInContainer: {
                switch (item->container) {
                    case ItemContainer::Unspecified: // Shouldnt happen, but he
                        std::cout << "Investigate packet!" << std::endl;
                        break;
                    case ItemContainer::Inventory:
                    case ItemContainer::TraderOffer:
                    case ItemContainer::ForTrade:
                    case ItemContainer::Cube:
                    case ItemContainer::Stash:
                    case ItemContainer::Belt:
                         ego->addItem(item);
                        break;

                        // Equipment (special, as we can see other players)
//                    case ItemContainer::Item:
//                        // ToDo; Other players can have equipment too, that isnt ours
//                        personal = ego->container->equipment;
//                        break;
//
//                        // Shops from here
//
//                    case ItemContainer::ArmorTab:
//                        shop = game->shop->armorTab;
//                        break;
//                    case ItemContainer::WeaponTab1:
//                        shop = game->shop->weaponTab1;
//                        break;
//                    case ItemContainer::WeaponTab2:
//                        shop = game->shop->weaponTab2;
//                        break;
//                    case ItemContainer::MiscTab:
//                        shop = game->shop->miscTab;
//                        break;
                }
                break;
            }
            case ItemActionType::RemoveFromContainer:
                break;
            case ItemActionType::Equip:
                break;
            case ItemActionType::IndirectlySwapBodyItem:
                break;
            case ItemActionType::Unequip:
                break;
            case ItemActionType::SwapBodyItem:
                break;
            case ItemActionType::AddQuantity:
                break;
            case ItemActionType::AddToShop:
                break;
            case ItemActionType::RemoveFromShop:
                break;
            case ItemActionType::SwapInContainer:
                break;
            case ItemActionType::PutInBelt:
                // Belt in this isnt an item but a map, as you can have items in a belt without actually wearing a belt

                break;
            case ItemActionType::RemoveFromBelt:

                break;

            case ItemActionType::SwapInBelt:

                break;
            case ItemActionType::AutoUnequip:
                break;
            case ItemActionType::ToCursor:
                break;
            case ItemActionType::ItemInSocket:
                break;
            case ItemActionType::UpdateStats:
                break;
            case ItemActionType::WeaponSwitch:
                break;
        }


    }

    void playerAdded(Game *game, Player *player) {
        player->game = game;
        std::cout << "new player joined game - " << player->name << std::endl;
    }

    struct myData {

    };
    class : Hook<myData> {
    public:
        void game(Models::Game *game) {

            // Hook on new item event
            game->onItem(ItemEvents::packet_0x9C, itemAdded);
            game->onItem(ItemEvents::packet_0x9D, itemAdded);
        }

    } hook; // NOLINT(cert-err58-cpp)mspa
};