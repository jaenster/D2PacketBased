//
// Created by jaenster on 16/07/2020.
//

#ifndef D2PacketBased_SERVERCLIENT_H
#define D2PacketBased_SERVERCLIENT_H

#include "../../Models.h"
#include "../../UsingModels.h"
#include "Identifiers.h"

namespace GameServer {
    inline void ServerClient(uchar *data, dword size, Models::Game *game) {

        printf("GS: %-5d", size);
        for (int i = 0; i < size; i++) {
            printf("%02x ", (unsigned char) data[i]);
        }
        printf("\n");


        auto packetId = (PacketIdentifiers) data[0];
        switch (packetId) {

            case ConnectionInfo:
                game->world->compressed = data[1];
                break;
            case GameFlags:  //01 02 04 20 10 00 01 00
                game->world->difficulty = (Models::Difficulty) rbyte(1);
                game->world->hardcore = rword(4) == 0;
                game->world->expansion = rword(6) > 0;
                game->world->ladder = rbyte(7) > 0;
                break;
            case GameLoading:
                break;
            case LoadSuccessful:
                break;
            case LoadAct:
                game->world->setMapSeed(rdword(2));
                break;
            case LoadComplete:
                break;
            case UnloadComplete:
                break;
            case GameExitSucessful:
                break;
            case MapReveal:
                break;
            case MapHide:
                break;
            case AssignLevelWarp:
                break;
            case RemoveObject:
                break;
            case GameHandshake:
                break;
            case NPCHit:
                break;
            case PlayerStop:
                break;
            case ObjectState:
                break;
            case PlayerMove:
                break;
            case PlayerToTarget:
                break;
            case ReportKill:
                break;
            case ReassignPlayer:
                break;
            case Unknown0x16:
                break;
            case Unknown0x17:
                break;
            case PlayerHPMP:
                break;
            case GoldtoInvByte:
                break;
            case AddExpByte:
                break;
            case AddExpWord:
                break;
            case AddExp:
                break;
            case BaseAttributBytee:
                break;
            case BaseAttributeWord:
                break;
            case BaseAttribute:
                break;
            case AttributeUpdate:
                break;
            case UpdateItemOskill:
                break;
            case UpdateItemSkill:
                break;
            case SetSkill:
                break;
            case GameChat:
                break;
            case NPCInfo:
                break;
            case QuestInfo:
                break;
            case GameQuestInfo:
                break;
            case NPCTransaction:
                break;
            case PlaySound:
                break;
            case UpdateItemStats:
                break;
            case UseStackableItem:
                break;
            case Unknown0x40:
                break;
            case ClearCursor:
                break;
            case Relator1:
                break;
            case Relator2:
                break;
            case UnitSkillonTarget:
                break;
            case UnitCastSkill:
                break;
            case MercForHire:
                break;
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
                rstring(playerName, 6, 20)
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
                bool update = (bool)(game->items->contains(uid));

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
