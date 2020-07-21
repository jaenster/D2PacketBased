cmake_minimum_required(VERSION 3.15)
project(D2PacketBase)

# tell the compiler its a 32 bit process
set(CMAKE_SYSTEM_PROCESSOR "i686")

add_compile_definitions(WIN32_LEAN_AND_MEAN) # dont add weird crap

set(CMAKE_CXX_STANDARD 20)


file(GLOB D2PacketBase_Hooks "Hooks/*.cpp")

add_executable(D2PacketBase
        main.cpp

        Models/Game.cpp

        Tools/Compression.cpp

        D2/D2Map.cpp
        D2/Offset.cpp

        # AOBot

        Models/AOBot/D2Data/item/BaseArmor.cpp
        Models/AOBot/D2Data/item/BaseArmor.h
        Models/AOBot/D2Data/item/BaseItem.cpp
        Models/AOBot/D2Data/item/BaseItem.h
        Models/AOBot/D2Data/item/BaseItemType.cpp
        Models/AOBot/D2Data/item/BaseItemType.h
        Models/AOBot/D2Data/item/BaseMiscItem.cpp
        Models/AOBot/D2Data/item/BaseMiscItem.h
        Models/AOBot/D2Data/item/BaseProperty.cpp
        Models/AOBot/D2Data/item/BaseProperty.h
        Models/AOBot/D2Data/item/BaseRuneword.cpp
        Models/AOBot/D2Data/item/BaseRuneword.h
        Models/AOBot/D2Data/item/BaseWeapon.cpp
        Models/AOBot/D2Data/item/BaseWeapon.h
        Models/AOBot/D2Data/item/ItemAffix.cpp
        Models/AOBot/D2Data/item/ItemAffix.h
        Models/AOBot/D2Data/item/ItemEnums.h
        Models/AOBot/D2Data/item/ItemKind.h
        Models/AOBot/D2Data/item/ItemSet.cpp
        Models/AOBot/D2Data/item/ItemSet.h
        Models/AOBot/D2Data/item/ItemSetType.h
        Models/AOBot/D2Data/item/ItemStat.h
        Models/AOBot/D2Data/item/ItemType.h
        Models/AOBot/D2Data/item/MagicPrefixType.h
        Models/AOBot/D2Data/item/MagicSuffixType.h
        Models/AOBot/D2Data/item/RunewordType.h
        Models/AOBot/D2Data/item/SetItem.cpp
        Models/AOBot/D2Data/item/SetItem.h
        Models/AOBot/D2Data/item/SetItemType.h
        Models/AOBot/D2Data/item/UniqueItem.cpp
        Models/AOBot/D2Data/item/UniqueItem.h
        Models/AOBot/D2Data/item/UniqueItemType.h

        Models/AOBot/D2Data/AreaLevel.h
        Models/AOBot/D2Data/BaseLevel.cpp
        Models/AOBot/D2Data/BaseLevel.h
        Models/AOBot/D2Data/BaseStat.cpp
        Models/AOBot/D2Data/BaseStat.h
        Models/AOBot/D2Data/BattleNet.h
        Models/AOBot/D2Data/CharacterClass.h
        Models/AOBot/D2Data/DamageType.h
        Models/AOBot/D2Data/Game.h
        Models/AOBot/D2Data/GameObjects.h
        Models/AOBot/D2Data/item
        Models/AOBot/D2Data/Messaging.h
        Models/AOBot/D2Data/NPCCode.h
        Models/AOBot/D2Data/NPCType.h
        Models/AOBot/D2Data/Players.h
        Models/AOBot/D2Data/Quests.h
        Models/AOBot/D2Data/SkillInfo.h
        Models/AOBot/D2Data/SkillTab.h
        Models/AOBot/D2Data/SkillType.h
        Models/AOBot/D2Data/Stat.h
        Models/AOBot/D2Data/StateType.h
        Models/AOBot/D2Data/StatType.h
        Models/AOBot/D2Data/UnitType.h
        Models/AOBot/D2Data/WarpType.h


        ${D2PacketBase_Hooks}
        Models/Units/Intermediate/UnitEmitter.h Hooks/LogEvents.cpp Models/Concepts/Storable.h Models/Concepts/Store.h Hooks/TestPacket.cpp Models/Events/SelfEventEmitter.h Models/Concepts/Orb.h)

# create folder @ debug folder with data and fill it
execute_process(COMMAND "data\\createDataFolder.cmd" )