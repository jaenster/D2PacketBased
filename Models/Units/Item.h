//
// Created by jaenster on 15/07/2020.
//


#ifndef D2PacketBased_ITEM_H
#define D2PacketBased_ITEM_H

#include <map>
#include <vector>
#include "Unit.h"
#include "../../Tools/BitReader.h"
#include "../D2Data/Stat.h"
#include "../D2Data/item/ItemSet.h"
#include "../D2Data/item/BaseItem.h"
#include "../D2Data/item/ItemAffix.h"
#include "../D2Data/item/SetItem.h"
#include "../D2Data/item/BaseRuneword.h"
#include "../D2Data/item/UniqueItem.h"
#include "../D2Data/item/ItemStat.h"
#include "../Collections/StatCollection.h"

namespace Models {
#define checkFlag(x) (packet->flags.flags.x & ItemFlags::x) == ItemFlags::x

    class Item : public Unit<Item> {
    private:
//        typedef Collection<Item, StatBase, StatType::StatType> StatsCollection;

    public:
        bool updated;
        std::vector<Item *> *sockets;
        Item *parent;

        ItemActionType::ItemActionType action;
        ItemCategory::ItemCategory category;
        UnitType::UnitType ownerType;
        dword ownerUID;
        ItemVersion::ItemVersion version;
        Flags flags;

        // Ear
        uchar charClass: 3;
        uchar level;
        std::string name;

        // Basic's
        BaseItem *baseItem;

        unsigned char unknown1;
        ItemDestination::ItemDestination destination;
        unsigned short x;
        unsigned short y;
        EquipmentLocation::EquipmentLocation location;
        ItemContainer::ItemContainer container;
        unsigned char usedSockets;
        ItemQuality::ItemQuality quality;
        unsigned char graphic;
        int color;
        ItemAffix prefix;
        SuperiorItemType::SuperiorItemType superiorType;
        ItemAffix suffix;
        BaseSetItem *setItem;
        BaseUniqueItem *uniqueItem;
        unsigned short runewordID;
        unsigned short runewordParam;
        BaseRuneword *runeword;
        unsigned char use;

//        StatCollection<Item> *stats;
//        StatCollection<Item> *mods;

        std::vector<StatBase> stats;
        std::vector<StatBase> mods;

        std::vector<MagicPrefixType::MagicPrefixType> magicPrefixes;
        std::vector<MagicSuffixType::MagicSuffixType> magicSuffixes;

        Item() {
            this->Unit<Item>::base = this;
            this->Unit<Item>::type = UnitType::Item;
            this->sockets = new std::vector<Item *>();
            this->parent = nullptr;
        }

        ~Item() {
            delete this->sockets;
        }

        inline static StatBase *ReadStat(BitReader &br) {
            int statID = br.ReadInt32(9);
            if (statID == 0x1FF)
                return NULL;

            BaseStat *baseStat = BaseStat::Get(statID);

            if (baseStat->SaveParamBits != -1) {
                switch (baseStat->Type) {
                    // Multiple param stats
                    case StatType::Reanimate: {
                        unsigned int monster = br.ReadUInt32(baseStat->SaveParamBits);
                        unsigned int val = br.ReadUInt32(baseStat->SaveBits);

                        return new ReanimateStat(baseStat, monster, val);
                    }
                    case StatType::ElementalSkillBonus: {
                        int element = br.ReadInt32(baseStat->SaveParamBits);
                        int val = br.ReadInt32(baseStat->SaveBits);

                        return new ElementalSkillsBonusStat(baseStat, element, val);
                    }
                    case StatType::ClassSkillsBonus: {
                        int charClass = br.ReadInt32(baseStat->SaveParamBits);
                        int val = br.ReadInt32(baseStat->SaveBits);

                        return new ClassSkillsBonusStat(baseStat, charClass, val);
                    }
                    case StatType::Aura: {
                        int skill = br.ReadInt32(baseStat->SaveParamBits);
                        int val = br.ReadInt32(baseStat->SaveBits);

                        return new AuraStat(baseStat, skill, val);
                    }
                    case StatType::SingleSkill:
                    case StatType::NonClassSkill: {
                        int skill = br.ReadInt32(baseStat->SaveParamBits);
                        int val = br.ReadInt32(baseStat->SaveBits);

                        return new SkillBonusStat(baseStat, skill, val);
                    }
                    case StatType::ChargedSkill: {
                        int level = br.ReadInt32(6);
                        int skill = br.ReadInt32(10);
                        int charges = br.ReadInt32(8);
                        int maxCharges = br.ReadInt32(8);

                        return new ChargedSkillStat(baseStat, level, skill, charges, maxCharges);
                    }
                    case StatType::SkillOnAttack:
                    case StatType::SkillOnKill:
                    case StatType::SkillOnDeath:
                    case StatType::SkillOnStriking:
                    case StatType::SkillOnLevelUp:
                    case StatType::SkillOnGetHit: {
                        int level = br.ReadInt32(6);
                        int skill = br.ReadInt32(10);
                        int chance = br.ReadInt32(baseStat->SaveBits);

                        return new SkillOnEventStat(baseStat, level, skill, chance);
                    }
                    case StatType::SkillTabBonus: {
                        int tab = br.ReadInt32(3);
                        int charClass = br.ReadInt32(3);
                        int unknown = br.ReadInt32(10);
                        int val = br.ReadInt32(baseStat->SaveBits);

                        return new SkillTabBonusStat(baseStat, tab, charClass, unknown, val);
                    }
                    default:
                        // Shouldn't get here...
                        throw "StatBase ReadStat(BitReader br)";
                }
            }

            // Per level stats: val / (1 << stat.OpParam) per Level
            if (baseStat->OpBase == StatType::Level)
                return new PerLevelStat(baseStat, br.ReadInt32(baseStat->SaveBits));

            switch (baseStat->Type) {
                // Range stats:
                case StatType::MinDamagePercent:
                case StatType::MaxDamagePercent: {
                    int min = br.ReadInt32(baseStat->SaveBits);
                    int max = br.ReadInt32(baseStat->SaveBits);

                    return new DamageRangeStat(baseStat, min, max);
                }
                case StatType::FireMinDamage:
                case StatType::LightMinDamage:
                case StatType::MagicMinDamage: {
                    int min = br.ReadInt32(baseStat->SaveBits);
                    int max = br.ReadInt32(BaseStat::Get(baseStat->Index + 1)->SaveBits);

                    return new DamageRangeStat(baseStat, min, max);
                }
                case StatType::ColdMinDamage: {
                    int min = br.ReadInt32(baseStat->SaveBits);
                    int max = br.ReadInt32(BaseStat::Get(StatType::ColdMaxDamage)->SaveBits);
                    int frames = br.ReadInt32(BaseStat::Get(StatType::ColdLength)->SaveBits);

                    return new ColdDamageStat(baseStat, min, max, frames);
                }
                case StatType::PoisonMinDamage: {
                    int min = br.ReadInt32(baseStat->SaveBits);
                    int max = br.ReadInt32(BaseStat::Get(StatType::PoisonMaxDamage)->SaveBits);
                    int frames = br.ReadInt32(BaseStat::Get(StatType::PoisonLength)->SaveBits);

                    return new PoisonDamageStat(baseStat, min, max, frames);
                }

                    // Single param stats:
                case StatType::ReplenishDurability:
                case StatType::ReplenishQuantity: {
                    int val = br.ReadInt32(baseStat->SaveBits);

                    return new ReplenishStat(baseStat, val);
                }

                default: {
                    if (baseStat->Signed) {
                        int val = br.ReadInt32(baseStat->SaveBits);
                        if (baseStat->SaveAdd > 0)
                            val -= baseStat->SaveAdd;
                        return StatBase::Signed(baseStat, val);
                    } else {
                        unsigned int val = br.ReadUInt32(baseStat->SaveBits);
                        if (baseStat->SaveAdd > 0)
                            val -= (unsigned int) baseStat->SaveAdd;
                        return StatBase::Unsigned(baseStat, val);
                    }
                }
            }
        }


        inline static Item *fromPacket(uchar *data) {
            Item *item = new Item();
            item->updateFromPacket(data);
            return item;
        }

        Item* updateFromPacket(uchar *data) {
//            bool updateItem = !!(items->contains(uid));

            // We update an item, or add a new one
//            Item *item = updateItem ? game->items->get(uid) : new Item();
//            this->updated = updateItem;

            try {
                BitReader br(data, 1);

                this->action = (ItemActionType::ItemActionType) br.ReadByte();
                br.SkipBytes(1); // length
                this->category = (ItemCategory::ItemCategory) br.ReadByte();
                this->uid = br.ReadUInt32();

                if (data[0] == 0x9d) {
                    this->ownerType = (UnitType::UnitType) br.ReadByte();
                    this->ownerUID = br.ReadUInt32();
                } else {
                    this->ownerType = UnitType::Invalid;
                    this->ownerUID = 0;
                }

                ItemFlags::ItemFlags flags = (ItemFlags::ItemFlags) br.ReadUInt32();
                this->version = (ItemVersion::ItemVersion) br.ReadByte();


                this->flags.None = (flags & ItemFlags::None) == ItemFlags::None;
                this->flags.Equipped = (flags & ItemFlags::Equipped) == ItemFlags::Equipped;
                this->flags.InSocket = (flags & ItemFlags::InSocket) == ItemFlags::InSocket;
                this->flags.Identified = (flags & ItemFlags::Identified) == ItemFlags::Identified;
                this->flags.x20 = (flags & ItemFlags::x20) == ItemFlags::x20;
                this->flags.SwitchedIn = (flags & ItemFlags::SwitchedIn) == ItemFlags::SwitchedIn;
                this->flags.SwitchedOut = (flags & ItemFlags::SwitchedOut) == ItemFlags::SwitchedOut;
                this->flags.Broken = (flags & ItemFlags::Broken) == ItemFlags::Broken;
                this->flags.Duplicate = (flags & ItemFlags::Duplicate) == ItemFlags::Duplicate;
                this->flags.Socketed = (flags & ItemFlags::Socketed) == ItemFlags::Socketed;
                this->flags.OnPet = (flags & ItemFlags::OnPet) == ItemFlags::OnPet;
                this->flags.x2000 = (flags & ItemFlags::x2000) == ItemFlags::x2000;
                this->flags.NotInSocket = (flags & ItemFlags::NotInSocket) == ItemFlags::NotInSocket;
                this->flags.Ear = (flags & ItemFlags::Ear) == ItemFlags::Ear;
                this->flags.StartItem = (flags & ItemFlags::StartItem) == ItemFlags::StartItem;
                this->flags.Compact = (flags & ItemFlags::Compact) == ItemFlags::Compact;
                this->flags.Ethereal = (flags & ItemFlags::Ethereal) == ItemFlags::Ethereal;
                this->flags.Any = (flags & ItemFlags::Any) == ItemFlags::Any;
                this->flags.Personalized = (flags & ItemFlags::Personalized) == ItemFlags::Personalized;
                this->flags.Gamble = (flags & ItemFlags::Gamble) == ItemFlags::Gamble;
                this->flags.Runeword = (flags & ItemFlags::Runeword) == ItemFlags::Runeword;
                this->flags.x8000000 = (flags & ItemFlags::x8000000) == ItemFlags::x8000000;


                //TODO: May be part of version... never seen those bits set.
                this->unknown1 = br.ReadByte(2);

                this->destination = (ItemDestination::ItemDestination) br.ReadByte(3);

                if (this->destination == ItemDestination::Ground) {
                    this->x = br.ReadUInt16();
                    this->y = br.ReadUInt16();
                } else {
                    this->location = (EquipmentLocation::EquipmentLocation) br.ReadByte(4);
                    this->x = br.ReadByte(4);
                    this->y = br.ReadByte(3);
                    this->container = (ItemContainer::ItemContainer) br.ReadByte(4);
                }

                // Buffer to container mapping (sanitizes NPC tabs IDs and coords and changes belt location to X, Y)
                if (this->action == ItemActionType::AddToShop || this->action == ItemActionType::RemoveFromShop) {
                    int buff = (int) this->container | 0x80;
                    if ((buff & 1) == 1) {
                        buff--;
                        this->y += 8;
                    }
                    this->container = (ItemContainer::ItemContainer) buff;
                } else if (this->container == ItemContainer::Unspecified) {
                    if (this->location == EquipmentLocation::NotApplicable) {
                        if ((flags & ItemFlags::InSocket) == ItemFlags::InSocket) {
                            this->container = ItemContainer::Item;
                            this->y = -1;
                        } else if (this->action == ItemActionType::PutInBelt ||
                                   this->action == ItemActionType::RemoveFromBelt) {
                            this->container = ItemContainer::Belt;
                            this->y = this->x / 4;
                            this->x = this->x % 4;
                        }
                    } else {
                        this->x = -1;
                        this->y = -1;
                    }
                }

                BaseStat *baseStat;
                int val;

                if ((flags & ItemFlags::Ear) == ItemFlags::Ear) {
                    this->charClass = (CharacterClass::CharacterClass) br.ReadByte(3);
                    this->level = br.ReadByte(7);
                    this->name = br.ReadString(7, '\0', 16);

                    this->baseItem = BaseItem::Get(ItemType::Ear);
                    return this;
                }

                this->baseItem = BaseItem::GetByID(this->category, br.ReadUInt32());

                // Big Pile : 1
                // Quantity : Big Pile ? 32 : 12
                if (this->baseItem->type == ItemType::Gold) {
                    this->stats.push_back(SignedStat(BaseStat::Get(StatType::Quantity),
                                                     br.ReadInt32(br.ReadBoolean(1) ? 32 : 12)));
                    return this;
                }

                // Used Sockets : 3
                this->usedSockets = br.ReadByte(3);

                // Ends here if SimpleItem or Gamble
                if ((int) (flags & (ItemFlags::Compact | ItemFlags::Gamble)) != 0)
                    return this;

                // ILevel : 7
                this->level = br.ReadByte(7);

                // Quality : 4
                this->quality = (ItemQuality::ItemQuality) br.ReadByte(4);

                // Graphic : 1 : 3
                if (br.ReadBoolean(1))
                    this->graphic = br.ReadByte(3);

                // Color : 1 : 11
                if (br.ReadBoolean(1))
                    this->color = br.ReadInt32(11);

                // Quality specific information
                if ((flags & ItemFlags::Identified) == ItemFlags::Identified) {
                    switch (this->quality) {
                        case ItemQuality::Inferior:
                            this->prefix = ItemAffix(ItemAffixType::InferiorPrefix, br.ReadByte(3));
                            break;

                        case ItemQuality::Superior:
                            this->prefix = ItemAffix(ItemAffixType::SuperiorPrefix, 0);
                            this->superiorType = (SuperiorItemType::SuperiorItemType) br.ReadByte(3);
                            break;

                        case ItemQuality::Magic:
                            this->prefix = ItemAffix(ItemAffixType::MagicPrefix, br.ReadUInt16(11));
                            this->suffix = ItemAffix(ItemAffixType::MagicSuffix, br.ReadUInt16(11));
                            break;

                        case ItemQuality::Rare:
                        case ItemQuality::Crafted:
                            this->prefix = ItemAffix(ItemAffixType::RarePrefix, br.ReadByte(8));
                            this->suffix = ItemAffix(ItemAffixType::RareSuffix, br.ReadByte(8));
                            break;

                        case ItemQuality::Set:
                            this->setItem = BaseSetItem::Get(br.ReadUInt16(12));
                            break;

                        case ItemQuality::Unique:
                            if (this->baseItem->code != "std" && this->baseItem->code != "hdm" &&
                                this->baseItem->code != "te1" && this->baseItem->code != "te2" &&
                                this->baseItem->code != "te3" &&
                                this->baseItem->code != "te4") // TODO: add UniqueItem entry to parse mod (req lvl 90)
                                this->uniqueItem = BaseUniqueItem::Get(br.ReadUInt16(12));
                            break;
                    }
                }

                if (this->quality == ItemQuality::Rare || this->quality == ItemQuality::Crafted) {
                    //			this->magicPrefixes = new std::list<MagicPrefixType::MagicPrefixType>();
                    //			this->magicSuffixes = new std::list<MagicSuffixType::MagicSuffixType>();
                    for (int i = 0; i < 3; i++) {
                        if (br.ReadBoolean(1))
                            this->magicPrefixes.push_back((MagicPrefixType::MagicPrefixType) br.ReadUInt16(11));
                        if (br.ReadBoolean(1))
                            this->magicSuffixes.push_back((MagicSuffixType::MagicSuffixType) br.ReadUInt16(11));
                    }
                }

                // Runeword Info : 16
                if ((flags & ItemFlags::Runeword) == ItemFlags::Runeword) {
                    //HACK: this is probably very wrong, but works for all the runewords I tested so far...
                    //TODO: remove these fields once testing is done
                    this->runewordID = br.ReadUInt16(12);
                    this->runewordParam = br.ReadUInt16(4);

                    val = -1;
                    if (this->runewordParam == 5) //TODO: Test cases where ID is around 100...
                    {
                        val = this->runewordID - this->runewordParam * 5;
                        if (val < 100) val--;
                    } else if (this->runewordParam == 2) //TODO: Test other runewords than Delirium...
                    {
                        val = ((this->runewordID & 0x3FF) >> 5) + 2;
                    }

                    //TODO: Test other runewords, find real shift / add params...
                    br.byteOffset -= 2;
                    this->runewordParam = br.ReadUInt16();
                    this->runewordID = val;

                    if (val != -1)
                        this->runeword = BaseRuneword::Get(val);
                    else
                        throw "Unknown Runeword";
                }

                // Personalized Name : 7 * (NULLSTRING Length)
                if ((flags & ItemFlags::Personalized) == ItemFlags::Personalized)
                    this->name = br.ReadString(7, '\0', 16);

                if (this->baseItem->isArmor()) {
                    baseStat = BaseStat::Get(StatType::ArmorClass);
                    this->stats.push_back(SignedStat(baseStat, br.ReadInt32(baseStat->SaveBits) - baseStat->SaveAdd));
                }

                // Even items marked NoDurability have a durability, it just normally isn't used (e.g. bows...)
                if (this->baseItem->isArmor() || this->baseItem->isWeapon()) {
                    baseStat = BaseStat::Get(StatType::MaxDurability);
                    val = br.ReadInt32(baseStat->SaveBits);
                    this->stats.push_back(SignedStat(baseStat, val));

                    // 0 max durability means indestructible and that there's no current durability.
                    // This is only found on old items subject to the "zod bug"...
                    if (val > 0) {
                        baseStat = BaseStat::Get(StatType::Durability);
                        this->stats.push_back(SignedStat(baseStat, br.ReadInt32(baseStat->SaveBits)));
                    }
                }

                if ((flags & ItemFlags::Socketed) == ItemFlags::Socketed) {
                    baseStat = BaseStat::Get(StatType::Sockets);
                    this->stats.push_back(SignedStat(baseStat, br.ReadInt32(baseStat->SaveBits)));
                }

                // Useable : 5; Quantity : 9
                if (this->baseItem->stackable) {
                    if (this->baseItem->useable)
                        this->use = br.ReadByte(5);

                    baseStat = BaseStat::Get(StatType::Quantity);
                    this->stats.push_back(SignedStat(baseStat, br.ReadInt32(9)));
                }

                if ((flags & ItemFlags::Identified) != ItemFlags::Identified)
                    return this;

                // Set Bonus Stats
                int setMods = this->quality == ItemQuality::Set ? br.ReadByte(5) : -1;

                StatBase *stat;
                //this->mods = new List<StatBase>();

                if (this->baseItem->code != "std" && this->baseItem->code != "hdm" && this->baseItem->code != "te1" &&
                    this->baseItem->code != "te2" && this->baseItem->code != "te3" && this->baseItem->code != "te4") {

                    while ((stat = ReadStat(br)) != NULL) {
                        this->mods.push_back(*stat);
                    }

                    if ((flags & ItemFlags::Runeword) == ItemFlags::Runeword)
                        while ((stat = ReadStat(br)) != NULL) {
                            this->mods.push_back(*stat);
                        }

                    if (setMods > 0) {
                        //this->setBonuses = new std::list<StatBase>[5];
                        for (int i = 0; i < 5; i++) {
                            if ((setMods & (1 << i)) != 0) {
                                //this->setBonuses[i] = new std::list<StatBase>();
                                while ((stat = ReadStat(br)) != NULL) {
                                    this->mods.push_back(*stat);
                                }
                            }
                        }
                    }
                }
            }
            catch (char *message) {
                std::cout << "Exception caught parsing item packet: " << message << std::endl;
            }
            catch (...) {
                std::cout << "Exception caught parsing item packet " << data << std::endl;
            }

            return this;
        };
    };
}

#endif //D2PacketBased_ITEM_H

