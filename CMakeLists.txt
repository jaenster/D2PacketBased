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

        Models/D2Data/item/BaseArmor.cpp
        Models/D2Data/item/BaseArmor.h
        Models/D2Data/item/BaseItem.cpp
        Models/D2Data/item/BaseItem.h
        Models/D2Data/item/BaseItemType.cpp
        Models/D2Data/item/BaseItemType.h
        Models/D2Data/item/BaseMiscItem.cpp
        Models/D2Data/item/BaseMiscItem.h
        Models/D2Data/item/BaseProperty.cpp
        Models/D2Data/item/BaseProperty.h
        Models/D2Data/item/BaseRuneword.cpp
        Models/D2Data/item/BaseRuneword.h
        Models/D2Data/item/BaseWeapon.cpp
        Models/D2Data/item/BaseWeapon.h
        Models/D2Data/item/ItemAffix.cpp
        Models/D2Data/item/ItemAffix.h
        Models/D2Data/item/ItemEnums.h
        Models/D2Data/item/ItemKind.h
        Models/D2Data/item/ItemSet.cpp
        Models/D2Data/item/ItemSet.h
        Models/D2Data/item/ItemSetType.h
        Models/D2Data/item/ItemStat.h
        Models/D2Data/item/ItemType.h
        Models/D2Data/item/MagicPrefixType.h
        Models/D2Data/item/MagicSuffixType.h
        Models/D2Data/item/RunewordType.h
        Models/D2Data/item/SetItem.cpp
        Models/D2Data/item/SetItem.h
        Models/D2Data/item/SetItemType.h
        Models/D2Data/item/UniqueItem.cpp
        Models/D2Data/item/UniqueItem.h
        Models/D2Data/item/UniqueItemType.h

        Models/D2Data/AreaLevel.h
        Models/D2Data/BaseLevel.cpp
        Models/D2Data/BaseLevel.h
        Models/D2Data/BaseStat.cpp
        Models/D2Data/BaseStat.h
        Models/D2Data/BattleNet.h
        Models/D2Data/CharacterClass.h
        Models/D2Data/DamageType.h
        Models/D2Data/Game.h
        Models/D2Data/GameObjects.h
        Models/D2Data/item
        Models/D2Data/Messaging.h
        Models/D2Data/NPCCode.h
        Models/D2Data/NPCType.h
        Models/D2Data/Players.h
        Models/D2Data/Quests.h
        Models/D2Data/SkillInfo.h
        Models/D2Data/SkillTab.h
        Models/D2Data/SkillType.h
        Models/D2Data/Stat.h
        Models/D2Data/StateType.h
        Models/D2Data/StatType.h
        Models/D2Data/UnitType.h
        Models/D2Data/WarpType.h

        ${D2PacketBase_Hooks}
        Models/Units/Intermediate/UnitEmitter.h Hooks/LogEvents.cpp Models/Concepts/Storable.h Models/Concepts/Store.h Hooks/TestPacket.cpp Models/Events/SelfEventEmitter.h Models/Concepts/Orb.h)

# create folder @ debug folder with data and fill it
execute_process(COMMAND "data\\createDataFolder.cmd" )