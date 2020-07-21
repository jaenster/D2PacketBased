//
// Created by jaenster __on 16/07/2020.
//

#ifndef D2PacketBased_SERVERCLIENT_H
#define D2PacketBased_SERVERCLIENT_H

#include "Identifiers.h"
#include "../../Models.h"
#include "../../UsingModels.h"
#include "../../AOBot/D2Data/AreaLevel.h"

namespace GameServer {

    inline void ServerClient(uchar *data, dword size, Models::Game *game) {
        // these variables are so often used i declare them globally

#define rtype(offset) (Models::UnitType::UnitType)(data[offset])
        Models::UnitType::UnitType type;
        dword uid;

        printf("GS: %-5d", size);
        for (int i = 0; i < size; i++) {
            printf("%02x ", (unsigned char) data[i]);
        }
        printf("\n");


        auto packetId = (PacketIdentifiers) data[0];
        switch (packetId) {
            case GameLoading:
                std::cout << "Game loading" << std::endl;
                game->emit(Models::GameEvents::GameLoading);
                break;
            case GameFlags:  //01 02 04 20 10 00 01 00
                game->world->difficulty = (Models::Difficulty) rbyte(1);
                game->world->hardcore = rword(4) == 0;
                game->world->expansion = rword(6) > 0;
                game->world->ladder = rbyte(7) > 0;
                break;
            case LoadSuccessful:
                std::cout << "Load success?" << std::endl;
                game->emit(Models::GameEvents::LoadSuccessful);
                break;
            case LoadAct:
                game->world->setMapSeed(rdword(2));
                game->emit(Models::GameEvents::LoadAct);
                break;
            case LoadComplete: // Loading act
                game->emit(Models::GameEvents::LoadActComplete);
                break;
            case UnloadComplete:
                game->emit(Models::GameEvents::UnloadActComplete);
                break;
            case GameExitSucessful:
                game->emit(Models::GameEvents::ExitGameComplete);
                break;
            case MapAdd: {
                word x = rword(1);
                word y = rword(3);
                auto areaId = (AreaLevel::AreaLevel) rbyte(5);

                std::cout << "Map add, x,y" << std::endl;
                break;
            }
            case MapHide:
                break;
            case AssignLevelWarp: {
                Models::Object *model = new Models::Object();

                model->uid = rdword(2);
                model->classid = rbyte(6);
                model->x = rdword(7);
                model->y = rdword(9);

                game->objects->add(model);
                break;
            }
            case RemoveObject: {
                type = rtype(1);
                uid = rdword(2);

                if (type == Models::UnitType::Player) game->players->remove(uid);
                else if (type == Models::UnitType::Monster) game->players->remove(uid);
                else if (type == Models::UnitType::Object) game->monsters->remove(uid);
                else if (type == Models::UnitType::Missile) game->missiles->remove(uid);
//                     ToDo; Since items are so well splices over different places we need to clean up a bit better?
                else if (type == Models::UnitType::Item) game->items->remove(uid);
                else if (type == Models::UnitType::Tile) game->tiles->remove(uid);
                break;
            }

            case GameHandshake: {
                uid = rdword(2);
                game->ego->uid = uid;
                std::cout << "Debug, shouldnt get this packet anymore" << std::endl;
                break;
            }

            case NPCInfo:
            case NPCHit: {
                type = rtype(1);
                uid = rword(2);

                Monster *monster = type != Models::UnitType::Monster ? nullptr : game->monsters->get(uid);
                if (monster) {
                    if (packetId == NPCHit) {
                        monster->health = rbyte(8) * 100 / 128;
                        game->emitMonster(MonsterEvents::hit, monster);
                    } else if (packetId == uid) {
                        for (int i = 0; i < 8; i++) {
                            memcpy(monster->info, data + 6 + (i * 4), 4/*byte+byte+word*/);
                        }
                    }
                }

                break;
            }

            case SetSkill:
            case PlayerToTarget:
            case PlayerMove:
            case PlayerStop: {
                type = rtype(1);
                uid = rword(2);
                Player *player = type != Models::UnitType::Player ? nullptr : game->players->get(uid);

                if (player) {
                    if (packetId == PlayerStop) {
                        // Unknown byte 6
                        player->x = rword(7);
                        player->y = rword(9);
                        // Unknown byte 11
                        player->health = rword(12);
                    } else if (packetId == PlayerMove) {
                        player->x = rword(12);
                        player->x = rword(14);

                        player->targetx = rword(14);
                        player->targety = rword(14);
                    } else if (packetId == SetSkill) {
                        dword itemUid = rdword(12);

                        auto hand = player->selectedSkill[rbyte(6)];
                        hand->item = (itemUid != 0xFFFFFFFF) ? game->items->get(itemUid) : nullptr;
                        hand->skill = rword(6);
                    }
                }

                break;
            }
            case ObjectState: {
                type = rtype(1);
                uid = rword(2);

                Object *object = type != Models::UnitType::Object ? nullptr : game->objects->get(uid);
                if (object) {
                    object->portalFlags = rbyte(6);
                    object->targetable = rbyte(7);
                    object->mode = rdword(8);
                }

                break;
            }

            case ReassignPlayer:
            case ReportKill: {
                type = rtype(1);
                uid = rword(2);

                Models::Living *living = nullptr;
                if (type == Models::UnitType::Player) living = game->players->get(uid);
                else if (type == Models::UnitType::Monster) living = game->monsters->get(uid);

                if (living) {
                    if (packetId == ReassignPlayer) {
                        living->x = rword(6);
                        living->x = rword(8);
                    } else if (packetId == ReportKill) {
                        living->killCount++;
                    }
                }
                break;
            }
            case Unknown0x16:
            case Unknown0x17:
            case Unknown0x40:
                std::cout << "Unknown packet, need some investigation" << std::endl;
                break;
            case PlayerHPMP: {
                BitReader br(data, 1);
                Ego *ego = game->ego;

                // The real value of the 3 orbs
                ego->life->current = br.ReadUInt16(15);
                ego->mana->current = br.ReadUInt16(15);
                ego->stamina->current = br.ReadUInt16(15);

                ego->life->regen = br.ReadUInt16(7);
                ego->mana->regen = br.ReadUInt16(7);

                ego->x = br.ReadUInt16();
                ego->y = br.ReadUInt16();
                break;
            }
            case GoldtoInvByte: {
                Ego *ego = game->ego;
                ego->gold += rbyte(1);
                break;
            }
            case AddExpWord:
            case AddExpByte: {
                game->ego->experience += packetId == AddExpByte ? rbyte(1) : rword(1);
                break;
            }
            case AddExpDword: {
                game->ego->experience = dword(1);
                break;
            }
            case BaseAttributeByte:
            case BaseAttributeWord:
            case BaseAttributeDword: {
                uchar attr = rbyte(1);
                dword value =
                        packetId == BaseAttributeByte ? rbyte(2) : (packetId == BaseAttributeWord ? rword(2) : dword(
                                2));
                Ego *ego = game->ego;

                switch (attr) {
                    case StatType::Gold:
                        ego->gold = value;
                        break;
                    case StatType::GoldBank:
                        ego->goldBank = value;
                        break;
                    case StatType::MaxMana:
                    case StatType::MaxLife:
                        std::cout << value << std::endl; //ToDo; figure out the flagged id's
                        break;
                }
                break;
            }

            case AttributeUpdate: {
                //ToDo; need to find a reallife packet for this
                break;
            }
            case UpdateItemOskill:
            case UpdateItemSkill: {
                word unknown = rword(1);
                uid = rdword(3); // <-- not item but player
                word skillId = rbyte(7);

                Player *player = game->players->get(uid);
                if (player) {
                    Models::Skill *skill = player->skills->get(skillId);
                    bool isNew = skill == nullptr;
                    if (isNew) skill = new Models::Skill();

                    skill->id = (Models::Skills::Type) skillId;
                    skill->baseLevel = rbyte(9);

                    if (isNew) player->skills->add(skill);
                }

                break;
            }
            case GameChat: {
                const char *playerName = reinterpret_cast<const char *>(data + 10);
                int charNameOffset = int(strlen((char *) playerName)) + 11;
                const char *text = reinterpret_cast<const char *>(data + charNameOffset);

                std::cout << "<" << playerName << "> " << text << std::endl;

                break;
            }
            case QuestInfo:
            case GameQuestInfo: {
                int offset = packetId == GameQuestInfo ? 1 : 7;
                for (int i = 0; i < 41; i++) {
                    game->ego->quests[i]->type = (QuestType::QuestType) i;
                    game->ego->quests[i]->state = (QuestState::QuestState) data[offset + i * 2];
                    game->ego->quests[i]->standing = (QuestStanding::QuestStanding) data[offset + 1 + i * 2];
                }
                break;
            }
            case NPCTransaction: {

                type = rtype(1);
                auto result = (Models::TradeResult::TradeResult) rbyte(1);
                dword unknown = rdword(3); //ItemId?
                dword goldLeft = rdword(11);
                game->ego->gold = goldLeft;

                switch (result) {
                    case Models::TradeResult::Purchased:
                    case Models::TradeResult::Sold:
                        break;
                    default:
                        break;
                }
                break;
            }
            case PlaySound: {
                std::cout << "Playing sound. SoundId: " << rword(6) << std::endl;
                break;
            }
            case UpdateItemStats: {
                // Update stats
                dword uid;
                bool bBool;
                dword value;
                word addtional;

                BitReader br(data, 2);
//            br.ReadUInt16(16); // skip size and packetid

                char nsize = 0x8; // BYTE
                if (br.ReadBoolean(1)) {
                    nsize = 0x10; // WORD
                    if (br.ReadBoolean(1)) nsize = 0x20; // DWORD
                }

                uid = br.ReadUInt32(nsize);

                // Fetch item
                Item *item = game->items->get(uid);
                if (!item) {
                    break;
                }

                bBool = br.ReadBoolean(1);
                // stats are 9 bits, as there are 358 stats 9 bits is a max of 511
                auto statType = (StatType::StatType) br.ReadUInt16(9);

                nsize = 0x8; // BYTE
                if (br.ReadBoolean(1)) {
                    nsize = 0x10; // WORD
                    if (br.ReadBoolean(1)) nsize = 0x20; // DWORD
                }
                value = br.ReadUInt32(nsize);

                for (int i = 0, l = item->stats.size(); i < l; i++) {
                    if (item->stats[i].Stat->Type == statType) {
                        if (statType == 204) {
                            item->stats[i].Value = br.ReadInt32(6);
                            item->stats[i].Skill = (SkillType::SkillType) br.ReadInt32(10);

                            item->stats[i].Charges = br.ReadByte(8);
                            item->stats[i].MaxCharges = br.ReadByte(8);

                        } else {
                            addtional = br.ReadUInt16(br.ReadBoolean(1) ? 0x10 /*word*/ : 0x08/*byte*/);
                            BaseStat *baseStat = BaseStat::Get(statType);

                            if (baseStat->Signed) {
                                int val = (int) value;
                                if (baseStat->SaveAdd > 0) val -= baseStat->SaveAdd;
                                item->stats[i].Value = val;
                            } else {
                                unsigned int val = (unsigned int) value;
                                if (baseStat->SaveAdd > 0) val -= (unsigned int) baseStat->SaveAdd;
                                item->stats[i].Value = val;
                            }
                        }

                        game->emitItem(ItemEvents::updated, item);
                        break;
                    }
                }
                break;
            }
            case UseStackableItem: {
                uchar type = rbyte(1);
                dword itemgid = rbyte(2);
                word skillid = rbyte(2);
                // ToDo: Seems to only be a cast of ffff, weird need to investigate
                break;
            }
            case ClearCursor: {
//                Player *player = game->players->get(player);
//                if (player) player->cursor = -1;
                break;
            }

            case Relator1:
            case Relator2: /* [BYTE UnitType] [BYTE Gap] [DWORD Unit Id] [BYTE[4] Unused] */
                break;
            case UnitSkillonTarget: {
                type = rtype(1);

                Models::BaseUnit *unit = game->getGenericUnit(type, rdword(2));
                if (unit) unit->emit(Models::UnitEvents::attacking);

                unit = game->getGenericUnit(type, rdword(10));
                if (unit) unit->emit(Models::UnitEvents::attacked);
                break;
            }
            case UnitCastSkill: {
                type = rtype(1);
                uid = rtype(1);
                break;
            }

            case MercForHire:
            case ClearMercList:
                break;

            case QuestSpecial:
                break;
            case AssignObject:
                break;
            case PlayerQuestLog:
                break;
            case Darkness:
                break;
            case NPCEnchants:
                break;
            case OpenUserInterface:
                break;
            case AssignPlayer: {
                dword uid = rdword(1);
                word classid = rbyte(5);
                const char *playerName = reinterpret_cast<const char *>(data + 6);
                word x = rdword(22);
                word y = rdword(24);

                // Fetch new player, or ego if not set (first AssignPlayer is always the ego)
                Player *player = !game->ego->uid ? game->ego : new Player(game);

                player->uid = uid;
                player->name = playerName;
                player->classid = classid;
                player->x = x;
                player->y = y;

                game->players->add(player);
                break;
            }
            case EventMessages:
                break;
            case PlayerInGame:
                break;
            case PlayerLeftGame:
                break;
            case QuestItemState:
                break;
            case GameQuestAvailability:
                break;
            case Unknown0x5F:
                break;
            case TownportalState:
                break;
            case CanGoToAct:
                break;
            case Unknown0x62:
                break;
            case WaypointMenu:
                break;
            case PlayerKillCount:
                break;
            case NPCMove:
                break;
            case NPCMovetoTarget:
                break;
            case NPCState:
                break;
            case Unknown0x6A:
                break;
            case NPCAction:
                break;
            case NPCAttack:
                break;
            case NPCStop:
                break;
            case Unknown0x73:
                break;
            case PlayerCorpseAssign:
                break;
            case PlayerPartyInfo:
                break;
            case PlayerInProximity:
                break;
            case ButtonActions:
                break;
            case TradeAccepted:
                break;
            case GoldinTrade:
                break;
            case PetAction:
                break;
            case AssignSkillHotkey:
                break;
            case UseScroll:
                break;
            case SetItemFlags:
                break;
            case CMNCOF:
                break;
            case AllyPartyInfo:
                break;
            case AssignMerc:
                break;
            case PortalOwnership:
                break;
            case SpecialQuestEvent:
                break;
            case NPCWantstoInteract:
                break;
            case PlayerRelationship:
                break;
            case RelationshipUpdate:
                break;
            case AssignPlayerToParty:
                break;
            case CorpseAssign:
                break;
            case Pong:
                break;
            case PartyAutomapInfo:
                break;
            case SetNPCGossipAct:
                break;
            case RemoveUnitDisplay:
                break;
            case Unknown0x93:
                break;
            case BaseSkillLevels: {

                // Fetch correct player
                Models::Player *player = game->players->get(rdword(2));

                if (player) {
                    uchar length = rbyte(1);

                    for (int i = 0; i < length; i++) {
                        auto *skill = new Models::Skill();

                        skill->id = (Models::Skills::Type) rword(6 + i * 3);
                        skill->baseLevel = rbyte(8 + i * 3);

                        player->skills->add(skill);
                    }
                }
            }
                break;
            case LifeandManaUpdate:
                break;
            case WalkVerify:
                break;
            case WeaponSwitch:
                break;
            case UpdateNPCUnknownField40:
                break;
            case SkillCastonUnit:
                break;
            case SkillCastonXandY:
                break;
            case MercReviveCost:
                break;
            case ItemActionWorld:
            case ItemActionOwned: // Items
            {

                dword uid = rdword(4);
                bool update = (bool) (game->items->contains(uid));

                // Either fetch an existing item, or create one
                Item *item = (update ? game->items->get(uid) : new Item())->updateFromPacket(data);
                item->updated = update;

                // Trigger some low level event for items
                game->emitItem(packetId == ItemActionWorld ? ItemEvents::packet_0x9C : ItemEvents::packet_0x9D, item);

                if (!update) game->newItem(item);
                break;
            }


            case SetMercStatByte:
                break;
            case SetMercStatWord:
                break;
            case SetMercStatDWORD:
                break;
            case AddMercExpByte:
                break;
            case AddMercExpWord:
                break;
            case SkillAuraStat:
                break;
            case NextBaalWaveNPCClassId:
                break;
            case StateSkillMove:
                break;
            case Unknown0xA6:
                break;
            case DelayedState:
                break;
            case SetState:
                break;
            case EndState:
                break;
            case AddUnit:
                break;
            case NPCHeal:
                break;
            case AssignNPC:
                break;
            case WardenRequest:
                break;
            case ConnectionInfo:
                game->world->compressed = data[1];
                break;
            case GameConnectionTerminated:
                break;
            case GamesInfo:
                break;
            case DownloadSave:
                break;
            case TimeOut:
                break;
        }
    }
}


#endif //D2PacketBased_SERVERCLIENT_H
