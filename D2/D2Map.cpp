#include "D2Map.h"
#include "D2Structs.h"
#include <set>
#include <time.h>
#include <fstream>
#include <iostream>
#include "../Models/D2Data/BaseStat.h"
#include "../Models/D2Data/item/BaseProperty.h"
#include "../Models/D2Data/item/BaseItemType.h"
#include "../Models/D2Data/item/BaseItem.h"
#include "../Models/D2Data/item/ItemSet.h"
#include "../Models/D2Data/item/SetItem.h"
#include "../Models/D2Data/item/BaseRuneword.h"
#include "../Models/D2Data/item/UniqueItem.h"

#include <Windows.h>
; // Thanks to the magic of windows.h it sometimes misses a ;

d2client_struct D2Client;

char D2_DIR[MAX_PATH] = "";


HANDLE __stdcall myCreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                              LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                              DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    std::string szPath(lpFileName);


    if (szPath.find('\\') != std::string::npos) {
        szPath = szPath.substr(szPath.rfind('\\') + 1);
    }

    szPath.insert(0, D2_DIR);
    std::cout << "Loading " << lpFileName << std::endl;

    return CreateFile(szPath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
                      dwFlagsAndAttributes, hTemplateFile);
}

void StartGame(const char *dir) {

    BaseStat::init();
    BaseProperty::init();
    BaseItemType::init();
    BaseItem::init();
    ItemSet::init();
    BaseSetItem::init();
    BaseRuneword::init();
    BaseUniqueItem::init();

    sprintf_s(D2_DIR, sizeof(D2_DIR), "%s\\", dir);

    char GAME[MAX_PATH] = "";
    sprintf_s(GAME, sizeof(GAME), "%s\\game.exe", dir);
    std::ifstream ifs(GAME, std::ifstream::in);
    if (!ifs) {
        std::cout << "Incorrect Folder Name";
        exit(1);
    }
    ifs.close();

    CHAR szPath[MAX_PATH] = {0};
    GetCurrentDirectory(MAX_PATH, szPath);
    memset(&D2Client, NULL, sizeof(d2client_struct));
    SetCurrentDirectory(D2_DIR);
    std::cout << "Loading Diablo's Libs .." << std::endl;
    DefineOffsets();

    std::cout << "Override window's myCreateFile to in the middle of MPQ loading" << std::endl;

    DWORD dwOld, bases[] = {(DWORD) GetModuleHandle("FOG.dll") + 0x2505C,
                            (DWORD) GetModuleHandle("Storm.dll") + 0x43230,
                            (DWORD) GetModuleHandle("D2CLIENT.dll") + 0xCFFA4,
                            (DWORD) GetModuleHandle("D2COMMON.dll") + 0x8D11C};
    for (int i = 0; i < 4; i++) {
        VirtualProtect((VOID *) bases[i], 4, PAGE_READWRITE, &dwOld);
        *(DWORD *) bases[i] = (DWORD) myCreateFile;
        VirtualProtect((VOID *) bases[i], 4, dwOld, &dwOld);
    }
    *p_STORM_MPQHashTable = NULL;

    D2Client.dwInit = 1;
    D2Client.fpInit = (DWORD) D2ClientInterface;


    FOG_10021("Diablo II");
    FOG_10101(1, NULL);
    FOG_10089(1);

    if (!FOG_10218()) {
        std::cout << "Game Server Initialize Failed!" << std::endl;
        ExitProcess(NULL);
    }


    if (D2WIN_10174()) {
        if (!D2WIN_10005(NULL, NULL, NULL, &D2Client)) {
            std::cout << "Couldn't load Diablo 2 MPQ files. Please make sure you have a full install of Diablo II and copy the D2XMUSIC.MPQ and D2XVIDEO.MPQ from the Expansion CD" << std::endl;
            ExitProcess(NULL);
        }
    }
    std::cout << "Loading Language Tables..." << std::endl;
    D2LANG_10009(0, "ENG", 0);

    if (!D2COMMON_InitDataTables(0, 0, 0)) {
        std::cout << "Couldn't initialize sqptDataTable" << std::endl;
        ExitProcess(NULL);
    }

    SetCurrentDirectory(szPath);

    std::cout << "Initalized Game Misc..." << std::endl;
}


DWORD D2ClientInterface(VOID) {
    std::cout << "Client asks if its ready.." << std::endl;
    return D2Client.dwInit;
}