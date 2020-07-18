#include "Hook.h"
#include "../Models/D2Data/UnitType.h"


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

            case ItemActionType::RemoveFromContainer:
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
                        ego->container->process(item);
                        break;

                        // Equipment (special, as we can see other players)
                    case ItemContainer::Item:
                        // ToDo; Other players can have equipment too, that isnt ours
                        //Equipable* epq = nullptr;
                        std::cout << "here? " << std::endl;
                        break;
                }
                break;
            }
            case ItemActionType::AddToShop:
            case ItemActionType::RemoveFromShop:
            {
                switch(item->container) {
                    case ItemContainer::ArmorTab:
                    case ItemContainer::WeaponTab1:
                    case ItemContainer::WeaponTab2:
                    case ItemContainer::MiscTab:
                        game->shop->process(item);
                        break;
                }
            }

            case ItemActionType::Unequip:
            case ItemActionType::WeaponSwitch:
            case ItemActionType::SwapBodyItem:
            case ItemActionType::IndirectlySwapBodyItem:
            case ItemActionType::Equip: {
                std::cout << "Lol " << std::endl;
                if (item->location < 0 || item->location > 12) break;

                Models::Equipable *equipable = nullptr;
                if (item->ownerType == Models::UnitType::Player) equipable = game->players->get(item->ownerUID);
                if (item->ownerType == Models::UnitType::Monster) equipable = game->monsters->get(item->ownerUID);

                if (equipable) {
                    if (item->action == ItemActionType::Unequip) {
                        equipable->equipment[item->location] = nullptr;
                    } else {
                        equipable->equipment[item->location] = item;
                    }

                }
                break;
            }

            case ItemActionType::UpdateStats:
            {
                if (!item->updated) {
                    std::cout << "Bug? Updated stats on a new item? wtf?" << std::endl;
                }
                break;
            }

            case ItemActionType::SwapInBelt:
            case ItemActionType::RemoveFromBelt:
            case ItemActionType::PutInBelt: {
                bool add = item->action == ItemActionType::PutInBelt || (item->action == ItemActionType::SwapInBelt && item->destination == ItemDestination::Belt);

                if (add) {
                    // Belt in this isnt an item but a map, as you can have items in a belt without actually wearing a belt
                    ego->container->belt->add(item);
                    ego->container->items->add(item);
                } else {
                    ego->container->items->remove(item);
                    ego->container->belt->remove(item);
                }
                break;
            }
            case ItemActionType::ItemInSocket:
                if (item->ownerType == Models::UnitType::Item) {
                    Item *parent = game->items->get(item->ownerUID);

                    item->parent = parent;
                    parent->sockets->push_back(item);
                }
                break;
            case ItemActionType::SwapInContainer: // To cursor is already handled above
                break;
            case ItemActionType::AddQuantity:
            case ItemActionType::AutoUnequip:
            case ItemActionType::ToCursor:
            case ItemActionType::UNKNOWNx16:
            case ItemActionType::UNKNOWNx14:
                std::cout << "Unhandled action type: " << item->action << std::endl;
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