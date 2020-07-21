// CollisionMap.cpp: implementation of the CCollisionMap class.
//
//////////////////////////////////////////////////////////////////////
#include "CollisionMap.h"
#include "../../../D2/D2Map.h"
#include "objects.h"
#include "../d2data/AreaLevel.h"
#include "../d2data/NpcCode.h"
#include "TeleportPath.h"
#include "WalkPath.h"
#include <limits>
#include <time.h>

bool dumpMaps = false;
//////////////////////////////////////////////////////////////////////
// Static Variables
//////////////////////////////////////////////////////////////////////

CriticalSection CCollisionMap::loading;
CCollisionMap *CCollisionMap::last;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCollisionMap::CCollisionMap(Act *lpAct, DWORD ldwAreaId) {
    this->m_iCurMap = 0x00;
    this->dwLevelId = 0x00;
    this->pAct = lpAct;
    this->dwAreaId = ldwAreaId;
    ::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
}

CCollisionMap::~CCollisionMap() {
    //Abort();
    //Join();
    std::cout << "Destructing " << AreaLevel::toString((AreaLevel::AreaLevel) dwAreaId) << std::endl;
}

void CCollisionMap::AddCollisionData(const CollMap *pCol) {
    if (pCol == NULL)
        return;

    int x = pCol->dwPosGameX - m_ptLevelOrigin.x;
    int y = pCol->dwPosGameY - m_ptLevelOrigin.y;
    int cx = pCol->dwSizeGameX;
    int cy = pCol->dwSizeGameY;

    if (!IsValidIndex(x, y)) {
        return;
    }

    int nLimitX = x + cx;
    int nLimitY = y + cy;

    WORD *p = pCol->pMapStart;
    for (int j = y; j < nLimitY; j++) {
        for (int i = x; i < nLimitX; i++) {
            m_map(i, j) = *p;
            if (m_map(i, j) == 1024)
                m_map(i, j) = MAP_DATA_AVOID;
/*
			if (m_aCollisionTypes.Find(*p) == -1)
			{
				m_aCollisionTypes.Add(*p);
				m_aCollisionTypes.Sort();
			}
*/
            p++;
        }
    }
}

void CCollisionMap::AddUnitData(const Room2 *pRoom2) {
    AddUnitData(pRoom2, 2); // default is 2
}

void CCollisionMap::AddUnitData(const Room2 *pRoom2, unsigned thickenBy) {
    for (PresetUnit *pPresetUnit = pRoom2->pPreset; pPresetUnit; pPresetUnit = pPresetUnit->pPresetNext) {
        // npcs
        if (pPresetUnit->dwType == UNIT_TYPE_NPC) {
            std::cout << "Npc = " << pPresetUnit->dwTxtFileNo << ", x = " << pRoom2->dwPosX * 5 + pPresetUnit->dwPosX
                      << " y = " << pRoom2->dwPosY * 5 + pPresetUnit->dwPosY << std::endl;
            npcs[pPresetUnit->dwTxtFileNo].push_back(
                    std::pair<short, short>((short) pRoom2->dwPosX * 5 + pPresetUnit->dwPosX,
                                            (short) pRoom2->dwPosY * 5 + pPresetUnit->dwPosY));
            /*
            for (int x = 0-1; x <= 0+1; x++)
            {
                for (int y = 0-1; y <= 0+1; y++)
                {
                    if (x != y)
                    {
                        POINT p = {pRoom2->dwPosX*5+pPresetUnit->dwPosX + x, pRoom2->dwPosY*5+pPresetUnit->dwPosY + y};

                        AbsToRelative(p);

                        if (IsValidIndex(p))
                        {
                            m_map(p.x, p.y) = 1;
                        }
                    }
                }
            }*/

        }

        // objects
        if (pPresetUnit->dwType == UNIT_TYPE_OBJECT) {
            if (thickenBy < 0) // make sure?
                thickenBy = 0;

            std::cout << "Object = " << pPresetUnit->dwTxtFileNo << ", x = " << pRoom2->dwPosX * 5 + pPresetUnit->dwPosX
                      << " y = " << pRoom2->dwPosY * 5 + pPresetUnit->dwPosY << std::endl;
            objects[pPresetUnit->dwTxtFileNo].push_back(
                    std::pair<short, short>((short) pRoom2->dwPosX * 5 + pPresetUnit->dwPosX,
                                            (short) pRoom2->dwPosY * 5 + pPresetUnit->dwPosY));

            GameObjectID::GameObjectID oid = static_cast <GameObjectID::GameObjectID> (pPresetUnit->dwTxtFileNo);
            if (!isGoodObject(oid) && thickenBy > 0) {
                Invalidate(pRoom2->dwPosX * 5 + pPresetUnit->dwPosX, pRoom2->dwPosY * 5 + pPresetUnit->dwPosY);
                /*for (int x = -thickenBy; x <= thickenBy; x++)
                {
                    for (int y = -thickenBy; y <= thickenBy; y++)
                    {
                        POINT p = {pRoom2->dwPosX*5+pPresetUnit->dwPosX + x, pRoom2->dwPosY*5+pPresetUnit->dwPosY + y};

                        AbsToRelative(p);

                        if (IsValidIndex(p))
                        {
                            // why we adding x twice...  its added in line 118 already  :)  this could have caused crashes... if an object was on the edge of a map...
                            m_map(p.x, p.y) = 1;
                        }
                    }
                }*/
            }
        }

        // level exits
        if (pPresetUnit->dwType == UNIT_TYPE_TILE) {
            for (RoomTile *pRoomTile = pRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext) {
                if (*pRoomTile->nNum == pPresetUnit->dwTxtFileNo) {
                    std::cout << pRoom2->dwPosX * 5 + pPresetUnit->dwPosX << "/"
                              << pRoom2->dwPosY * 5 + pPresetUnit->dwPosY << " = "
                              << AreaLevel::toString((AreaLevel::AreaLevel) pLevel->dwLevelNo) << "->"
                              << AreaLevel::toString((AreaLevel::AreaLevel) pRoomTile->pRoom2->pLevel->dwLevelNo)
                              << std::endl;

                    std::cout << "pRoom2->dwPosX = " << pRoom2->dwPosX << std::endl;
                    std::cout << "pRoom2->dwPosX*5 = " << pRoom2->dwPosX * 5 << std::endl;
                    std::cout << "pPresetUnit->dwPosX = " << pPresetUnit->dwPosX << std::endl;
                    std::cout << "pRoom2->dwPosX*5+pPresetUnit->dwPosX = " << pRoom2->dwPosX * 5 + pPresetUnit->dwPosX
                              << std::endl;

                    std::cout << "pRoom2->dwPosY = " << pRoom2->dwPosY << std::endl;
                    std::cout << "pRoom2->dwPosY*5 = " << pRoom2->dwPosY * 5 << std::endl;
                    std::cout << "pPresetUnit->dwPosY = " << pPresetUnit->dwPosY << std::endl;
                    std::cout << "pRoom2->dwPosY*5+pPresetUnit->dwPosY = " << pRoom2->dwPosY * 5 + pPresetUnit->dwPosY
                              << std::endl;

                    exits[pRoomTile->pRoom2->pLevel->dwLevelNo].first =
                            (short) pRoom2->dwPosX * 5 + pPresetUnit->dwPosX;
                    exits[pRoomTile->pRoom2->pLevel->dwLevelNo].second =
                            (short) pRoom2->dwPosY * 5 + pPresetUnit->dwPosY;
                }
            }
        }
    }
}

BOOL CCollisionMap::IsValidIndex(long x, long y) const {
    return x >= 0 && x < m_map.size1() && y >= 0 && y < m_map.size2();
}

BOOL CCollisionMap::IsValidIndex(POINT p) const {
    return IsValidIndex(p.x, p.y);
}

BOOL CCollisionMap::IsValidAbsLocation(long x, long y) const {
    x -= m_ptLevelOrigin.x;
    y -= m_ptLevelOrigin.y;

    return IsValidIndex(x, y);
}

BOOL CCollisionMap::IsValidAbsLocation(POINT p) const {
    p.x -= m_ptLevelOrigin.x;
    p.y -= m_ptLevelOrigin.y;

    return IsValidIndex(p);
}

WORD CCollisionMap::GetMapData(long x, long y, BOOL bAbs) const {
    if (bAbs) {
        x -= m_ptLevelOrigin.x;
        y -= m_ptLevelOrigin.y;
    }

    WORD wVal = MAP_DATA_INVALID;

    if (IsValidIndex(x, y))
        wVal = m_map(x, y);

    return wVal;
}


BOOL CCollisionMap::BuildMapData(DWORD AreaId) {
    dwLevelId = AreaId;
    pLevel = GetLevel(pAct->pMisc, AreaId);

    if (!pLevel)
        return FALSE;

    if (!pLevel->pRoom2First)
        D2COMMON_InitLevel(pLevel);

    if (!pLevel->pRoom2First)
        return NULL;

    dwLevelId = AreaId;

    m_ptLevelOrigin.x = pLevel->dwPosX * 5;
    m_ptLevelOrigin.y = pLevel->dwPosY * 5;

    m_map.resize(pLevel->dwSizeX * 5, pLevel->dwSizeY * 5, false);

    for (unsigned int x = 0; x < m_map.size1(); x++) {
        for (unsigned int y = 0; y < m_map.size2(); y++) {
            m_map(x, y) = MAP_DATA_INVALID;
        }
    }

    //DwordArray aSkip;
    //Search(pLevel->pRoom2First, NULL, aSkip);

    for (Room2 *pRoom2 = pLevel->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Next) {
        BOOL bAdded = FALSE;

        if (!pRoom2->pRoom1) {
            bAdded = TRUE;
            D2COMMON_AddRoomData(pAct, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, NULL);
        }

        // levels near
        for (int i = 0; i < pRoom2->dwRoomsNear; i++) {
            DWORD prev = pLevel->dwLevelNo;
            if (pLevel->dwLevelNo != pRoom2->pRoom2Near[i]->pLevel->dwLevelNo) {
                std::pair<POINT, POINT> newlevel;

                newlevel.first.x = pRoom2->pRoom2Near[i]->pLevel->dwPosX * 5;
                newlevel.first.y = pRoom2->pRoom2Near[i]->pLevel->dwPosY * 5;
                newlevel.second.x = pRoom2->pRoom2Near[i]->pLevel->dwSizeX * 5;
                newlevel.second.y = pRoom2->pRoom2Near[i]->pLevel->dwSizeY * 5;

                if (prev != pRoom2->pRoom2Near[i]->pLevel->dwLevelNo) {
                    std::cout << pRoom2->pRoom2Near[i]->pLevel->dwPosX * 5 << "/"
                              << pRoom2->pRoom2Near[i]->pLevel->dwPosY * 5 << " = "
                              << AreaLevel::toString((AreaLevel::AreaLevel) pLevel->dwLevelNo) << "->"
                              << AreaLevel::toString((AreaLevel::AreaLevel) pRoom2->pRoom2Near[i]->pLevel->dwLevelNo)
                              << std::endl;
                    prev = pRoom2->pRoom2Near[i]->pLevel->dwLevelNo;
                }
                std::cout << "Level Near = " << pRoom2->pRoom2Near[i]->pLevel->dwLevelNo << std::endl;
                levelsnear[pRoom2->pRoom2Near[i]->pLevel->dwLevelNo].push_back(newlevel);
            }
        }

        if (pRoom2->pRoom1)
            AddCollisionData(pRoom2->pRoom1->Coll);

        switch (AreaId) {
            case AreaLevel::Harrogath:
                AddUnitData(pRoom2, 2);
                break;
            case AreaLevel::ThePandemoniumFortress:
            case AreaLevel::KurastDocks:
            case AreaLevel::LutGholein:
                AddUnitData(pRoom2, 0);
                break;
            case AreaLevel::RogueEncampment:
                AddUnitData(pRoom2, 2);
                break;
            case AreaLevel::CatacombsLevel4:
            case AreaLevel::ThroneOfDestruction:
                AddUnitData(pRoom2, 0);
                break;
            default:
                AddUnitData(pRoom2, 0);
                break;
        }
        /*
        if(AreaId == AreaLevel::Harrogath) // we want the stairs to be walkable
            AddUnitData(pRoom2, 1);        // hence we thicken objects by only 1
        //else
        //	AddUnitData(pRoom2,0);           // thicken by 2 is default
        */
        if (bAdded)
            D2COMMON_RemoveRoomData(pAct, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, NULL);

    }

    return TRUE;
}

int CCollisionMap::Routine() {
    int result = 0;

    clock_t start = clock();
    CCollisionMap::loading.Lock();

    try {
        std::cout << "had to wait " << clock() - start << " ms before loading "
                  << AreaLevel::toString((AreaLevel::AreaLevel) dwAreaId) << std::endl;

        result = BuildMapData(dwAreaId);
        //CHAR szMapName[64] = "";
        CHAR szFileName[64] = "";
        //sprintf_s(szMapName, sizeof(szMapName), "%s", D2COMMON_GetLevelTxt(dwLevelId)->szName);
        sprintf_s(szFileName, sizeof(szFileName), "maps/Act(%d) - %s - %X.txt", pAct->dwAct + 1,
                  AreaLevel::toString((AreaLevel::AreaLevel) dwAreaId).c_str(), pAct->dwMapSeed);
        DumpMap(szFileName, std::vector<POINT>());

        std::cout << "Finished Loading map " << AreaLevel::toString((AreaLevel::AreaLevel) dwAreaId) << std::endl;

    }
    catch (...) {
        CCollisionMap::loading.Unlock();
        throw "stupid CCollisionMap crash";
    }

    CCollisionMap::last = this;
    CCollisionMap::loading.Unlock();

    return result;
}

POINT CCollisionMap::GetMapOrigin() const {
    return m_ptLevelOrigin;
}

void CCollisionMap::AbsToRelative(POINT &pt) const {
    pt.x -= m_ptLevelOrigin.x;
    pt.y -= m_ptLevelOrigin.y;
}

void CCollisionMap::RelativeToAbs(POINT &pt) const {
    pt.x += m_ptLevelOrigin.x;
    pt.y += m_ptLevelOrigin.y;
}

BOOL CCollisionMap::DumpMap(LPCSTR lpszFilePath, std::vector<POINT> ptPath) const {
    if (!dumpMaps)
        return false;
//	if (Log::ReportingLevel < logDEBUG4)
//		return FALSE;

    if (lpszFilePath == NULL)
        return FALSE;

    FILE *fp = fopen(lpszFilePath, "w+");
    if (fp == NULL)
        return FALSE;

    fprintf(fp, "%s -- (Origin: %d,%d) -- (Size: %dx%d)\n", AreaLevel::toString((AreaLevel::AreaLevel) dwLevelId),
            m_ptLevelOrigin.x, m_ptLevelOrigin.y, m_map.size1(), m_map.size2());
/*
	for (int i = 0; i < m_aCollisionTypes.GetSize(); i++)
	{
		fprintf(fp, "%d, ", m_aCollisionTypes[i]);
	}
*/
    fprintf(fp, "\n\n");

    for (DWORD i = 0; i < ptPath.size(); i++) {
        AbsToRelative(ptPath[i]);
    }

    const int CX = m_map.size1();
    const int CY = m_map.size2();

    for (int y = 0; y < CY; y++) {
        for (int x = 0; x < CX; x++) {
            char ch = IsMarkPoint(NULL, x, y, ptPath);

            if (!ch)
                ch = (m_map(x, y) % 2) ? 'X'/*m_map(x,y)*/ : ' ';

            fprintf(fp, "%C", ch); // X - unreachable
        }

        fprintf(fp, "%c", '\n');
    }

    fclose(fp);

    return TRUE;
}

BOOL CCollisionMap::CheckCollision(INT x, INT y) {
    if (x > m_map.size1() || y > m_map.size2())
        return FALSE;
    BOOL Works = FALSE;
    Works = (m_map(x, y) % 2) ? FALSE : TRUE;
    return Works;
}

char CCollisionMap::IsMarkPoint(INT asd, int x, int y, const std::vector<POINT> ptPath) const {
    char ch = 0;

    for (DWORD i = 0; i < ptPath.size(); i++) {
        if (ptPath[i].x == x && ptPath[i].y == y) {
            if (i == 0)
                return 'S'; // start
            else if (i == ptPath.size() - 1)
                return 'E'; // end
            else
                return 'a' + i;//'*'; // nodes
        }
    }

    return ch;
}

SIZE CCollisionMap::GetMapSize() const {
    SIZE cz = {0};
    cz.cx = m_map.size1();
    cz.cy = m_map.size2();
    return cz;
}

int CCollisionMap::GetMapQSize() const {
    int s = 0;
    s = m_map.size1();
    s *= m_map.size2();
    return s;
}

SIZE CCollisionMap::CopyMapData(WORD **ppBuffer, int cx, int cy) const {
    SIZE copied = {0};
    //m_map.Lock();
//	copied = m_map.ExportData(ppBuffer, cx, cy);
    //m_map.Unlock();
    return copied;
}

void CCollisionMap::Invalidate(int x, int y) {
    /*
    POINT p = {x, y};
    AbsToRelative(p);

    if (IsValidIndex(p))
    {
        //m_map(p.x, p.y) = 1;
    }
    */

    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            POINT p = {x + i, y + j};
            AbsToRelative(p);

            if (IsValidIndex(p)) {
                m_map(p.x, p.y) = 1;
            }
        }
    }
}

BOOL CCollisionMap::IsGap(int x, int y) {
    if (m_map(x, y) % 2)
        return FALSE;

    int nSpaces = 0;
    int i = 0;

    // Horizontal check
    for (i = x - 2; i <= x + 2 && nSpaces < 3; i++) {
        if (i < 0 || i >= m_map.size1() || (m_map(i, y) % 2))
            nSpaces = 0;
        else
            nSpaces++;
    }

    if (nSpaces < 3)
        return TRUE;

    // Vertical check
    nSpaces = 0;
    for (i = y - 2; i <= y + 2 && nSpaces < 3; i++) {
        if (i < 0 || i >= m_map.size2() || (m_map(x, i) % 2))
            nSpaces = 0;
        else
            nSpaces++;
    }

    return nSpaces < 3;
}

void CCollisionMap::FillGaps() {
    const int CX = m_map.size1();
    const int CY = m_map.size2();

    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            if (IsGap(x, y)) {
                m_map(x, y) = MAP_DATA_FILLED;
            }
        }
    }
}

BOOL CCollisionMap::ThickenWalls(int nThickenBy) {
    if (nThickenBy <= 0) {
        return FALSE;
    }

    const int CX = m_map.size1();
    const int CY = m_map.size2();

    for (int i = 0; i < CX; i++) {
        for (int j = 0; j < CY; j++) {
            if ((m_map(i, j) % 2) == 0 || m_map(i, j) == MAP_DATA_THICKENED)
                continue;

            for (int x = i - nThickenBy; x <= i + nThickenBy; x++) {
                for (int y = j - nThickenBy; y <= j + nThickenBy; y++) {
                    if (!IsValidIndex(x, y))
                        continue;

                    if ((m_map(x, y) % 2) == 0)
                        m_map(x, y) = MAP_DATA_THICKENED;
                }
            }
        }
    }

    return TRUE;
}

void CCollisionMap::DestroyMap() {
    //m_map.Lock();
    //m_map.Destroy();
    m_map.clear();
    //m_map.Unlock();
    m_iCurMap = 0x00;
    //m_aCollisionTypes.RemoveAll();
    ::memset(&m_ptLevelOrigin, 0, sizeof(POINT));
}

POINT CCollisionMap::pathToLevel(int id) throw(...) {
    POINT p = {0, 0};

    if (exits.find(id) != exits.end()) {
        std::cout << "Level Exit Found ( " << AreaLevel::toString((AreaLevel::AreaLevel) id) << ") " << exits[id].first
                  << "/" << exits[id].second << std::endl;
        p.x = exits[id].first;
        p.y = exits[id].second;
        return p;
    } else // non warp levels
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        POINT ptExitPoints[4][2];
        int nTotalPoints = 0, nCurrentExit = 0;

        for (unsigned int i = 0; i < m_map.size1(); i++) {
            if (!(m_map(i, 0) % 2) && !(m_map(i, 2) % 2)) {
                ptExitPoints[nTotalPoints][0].x = i;
                ptExitPoints[nTotalPoints][0].y = 0;

                for (i++; i < m_map.size1(); i++) {
                    if (m_map(i, 0) % 2 && m_map(i, 2) % 2) {
                        ptExitPoints[nTotalPoints][1].x = i - 1;
                        ptExitPoints[nTotalPoints][1].y = 0;
                        break;
                    }
                }

                nTotalPoints++;
                break;
            }
        }

        for (unsigned int i = 0; i < m_map.size1(); i++) {
            if (!(m_map(i, m_map.size2() - 1) % 2) && !(m_map(i, m_map.size2() - 3) % 2)) {
                ptExitPoints[nTotalPoints][0].x = i;
                ptExitPoints[nTotalPoints][0].y = (LONG) m_map.size2() - 1;

                for (i++; i < m_map.size1(); i++) {
                    if ((m_map(i, m_map.size2() - 1) % 2) && (m_map(i, m_map.size2() - 3) % 2)) {
                        ptExitPoints[nTotalPoints][1].x = i - 1;
                        ptExitPoints[nTotalPoints][1].y = (LONG) m_map.size2() - 1;
                        break;
                    }
                }

                nTotalPoints++;
                break;
            }
        }

        for (unsigned int i = 0; i < m_map.size2(); i++) {
            if (!(m_map(0, i) % 2) && !(m_map(2, i) % 2)) {
                ptExitPoints[nTotalPoints][0].x = 0;
                ptExitPoints[nTotalPoints][0].y = i;

                for (i++; i < m_map.size2(); i++) {
                    if ((m_map(0, i) % 2) && (m_map(2, i) % 2)) {
                        ptExitPoints[nTotalPoints][1].x = 0;
                        ptExitPoints[nTotalPoints][1].y = i - 1;
                        break;
                    }
                }
                nTotalPoints++;
                break;
            }
        }

        for (unsigned int i = 0; i < m_map.size2(); i++) {
            if (!(m_map(m_map.size1() - 1, i) % 2) && !(m_map(m_map.size1() - 3, i) % 2)) {
                ptExitPoints[nTotalPoints][0].x = (LONG) m_map.size1() - 1;
                ptExitPoints[nTotalPoints][0].y = i;

                for (i++; i < m_map.size2(); i++) {
                    if ((m_map(m_map.size1() - 1, i) % 2) && (m_map(m_map.size1() - 3, i) % 2)) {
                        ptExitPoints[nTotalPoints][1].x = (LONG) m_map.size1() - 1;
                        ptExitPoints[nTotalPoints][1].y = i - 1;
                        break;
                    }
                }

                nTotalPoints++;
                break;
            }
        }

        LPPOINT ptCenters = new POINT[nTotalPoints];
        for (int i = 0; i < nTotalPoints; i++) {
            INT nXDiff = ptExitPoints[i][1].x - ptExitPoints[i][0].x;
            INT nYDiff = ptExitPoints[i][1].y - ptExitPoints[i][0].y;
            INT nXCenter = 0, nYCenter = 0;

            if (nXDiff > 0) {
                if (nXDiff % 2)
                    nXCenter = ptExitPoints[i][0].x + ((nXDiff - (nXDiff % 2)) / 2);
                else
                    nXCenter = ptExitPoints[i][0].x + (nXDiff / 2);
            }

            if (nYDiff > 0) {
                if (nYDiff % 2)
                    nYCenter = ptExitPoints[i][0].y + ((nYDiff - (nYDiff % 2)) / 2);
                else
                    nYCenter = ptExitPoints[i][0].y + (nYDiff / 2);
            }

            ptCenters[i].x = nXCenter ? nXCenter : ptExitPoints[i][0].x;
            ptCenters[i].y = nYCenter ? nYCenter : ptExitPoints[i][0].y;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (levelsnear.find(id) != levelsnear.end()) {
            for (std::vector<std::pair<POINT, POINT>>::iterator iter = levelsnear[id].begin();
                 iter != levelsnear[id].end(); iter++) {
                for (int j = 0; j < nTotalPoints; j++) {
                    if ((ptCenters[j].x + m_ptLevelOrigin.x) >= (*iter).first.x - 5 &&
                        (ptCenters[j].x + m_ptLevelOrigin.x) - 5 <= ((*iter).first.x + (*iter).second.x)) {
                        if ((ptCenters[j].y + m_ptLevelOrigin.y) >= (*iter).first.y - 5 &&
                            (ptCenters[j].y + m_ptLevelOrigin.y) - 5 <= ((*iter).first.y + (*iter).second.y)) {
                            std::cout << "Level Near Found (id: " << AreaLevel::toString((AreaLevel::AreaLevel) id)
                                      << ") " << ptCenters[j].x + m_ptLevelOrigin.x << "/"
                                      << ptCenters[j].y + m_ptLevelOrigin.y << std::endl;

                            p.x = ptCenters[j].x + m_ptLevelOrigin.x;
                            p.y = ptCenters[j].y + m_ptLevelOrigin.y;
                            delete[] ptCenters;
                            return p;
                        }
                    }
                }
            }
        }

        delete[] ptCenters;
    }

    std::cout << "Level Not Found (id: " << id << ")" << std::endl;
    return p;
}

POINT CCollisionMap::pathToObject(int id) throw(...) {
    POINT p = {0, 0};

    if (objects.find(id) != objects.end()) {
        std::cout << "Object Found (id: " << id << ")" << std::endl;
        p.x = objects[id][0].first;
        p.y = objects[id][0].second;
        return p;
    }

    std::cout << "Object Not Found (id: " << id << ")" << std::endl;
    return p;
}

POINT CCollisionMap::pathToNpc(int id) throw(...) {
    POINT p = {0, 0};

    if (npcs.find(id) != npcs.end()) {
        std::cout << "Npc Found (id: " << id << ")" << std::endl;
        p.x = npcs[id][0].first;
        p.y = npcs[id][0].second;
        return p;
    }

    std::cout << "Npc Not Found (id: " << id << ")" << std::endl;
    return p;
}

int CCollisionMap::getWp() throw(...) {
    int p = 0;

    static const short WPList[] = {0x0077, 0x009D, 0x009C, 0x0143, 0x0120, 0x0192, 0x00ED, 0x0144, 0x018E, 0x00EE,
                                   0x01AD, 0x01F0, 0x01FF, 0x01EE};

    for (int i = 0; i < 14; i++) {
        if (objects.find(WPList[i]) != objects.end()) {
            std::cout << "Wp Found (id: " << WPList[i] << ")" << std::endl;
            p = WPList[i];
            return p;
        }
    }

    std::cout << "Wp Not Found" << std::endl;
    return p;
}

extern long CalculateDistance(long x1, long y1, long x2, long y2);

extern long CalculateDistance(const POINT &pt1, const POINT &pt2);


POINT findAndRemoveClosest(POINT p, std::vector<POINT> &ptPath) {
    std::vector<POINT>::iterator closest = ptPath.begin();
    long distance = CalculateDistance(p, *closest);

    for (std::vector<POINT>::iterator i = ptPath.begin(); i != ptPath.end(); i++) {
        long new_distance = CalculateDistance(p, *i);

        if (new_distance < distance) {
            closest = i;
            distance = new_distance;
        } else if (new_distance == distance) {
            if (closest->x > i->x) {
                continue;
            }

            if (closest->y > i->y) {
                continue;
            }

            closest = i;
            distance = new_distance;
        }
    }

    POINT result = *closest;
    ptPath.erase(closest);

    return result;
}

std::vector<POINT> CCollisionMap::explore(int x, int y) throw(...) {
    std::vector<POINT> points;
//pLevel->dwSizeX

    int temp = 0;

    for (Room2 *pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next) {
        temp++;

        POINT pt = {pRoom->dwPosX * 5 + (pRoom->dwSizeX / 2), pRoom->dwPosY * 5 + (pRoom->dwSizeY / 2)};

        AbsToRelative(pt);

        //for (int i = 0; i < 20; i++) //original
        for (int i = 0; i < 20; i += 5) // should help make less points?
        {
            if (IsValidIndex(pt.x - i, pt.y) && m_map(pt.x - i, pt.y) % 2 == 0) {
                pt.x -= i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x + i, pt.y) && m_map(pt.x + i, pt.y) % 2 == 0) {
                pt.x += i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x, pt.y - i) && m_map(pt.x, pt.y - i) % 2 == 0) {
                pt.y -= i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x, pt.y + i) && m_map(pt.x, pt.y + i) % 2 == 0) {
                pt.y += i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x + i, pt.y + i) && m_map(pt.x + i, pt.y + i) % 2 == 0) {
                pt.x += i;
                pt.y += i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x - i, pt.y - i) && m_map(pt.x - i, pt.y - i) % 2 == 0) {
                pt.x -= i;
                pt.y -= i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x + i, pt.y - i) && m_map(pt.x + i, pt.y - i) % 2 == 0) {
                pt.x += i;
                pt.y -= i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
            if (IsValidIndex(pt.x - i, pt.y + i) && m_map(pt.x - i, pt.y + i) % 2 == 0) {
                pt.x -= i;
                pt.y += i;
                RelativeToAbs(pt);
                points.push_back(pt);
                break;
            }
        }
    }

//	TLogLevel orig = Log::ReportingLevel;
//	Log::ReportingLevel = logDEBUG4;
//	DumpMap("maps/explore1.log", points);
//	Log::ReportingLevel = orig;
/*

	std::vector<POINT> ptPath;
	POINT start = {x, y};

	while (points.size() > 0)
	{
		POINT end = findAndRemoveClosest(start, points);
		std::vector<POINT> subPath = pathTo(start.x, start.y, end.x, end.y);
		ptPath.insert(ptPath.end(), subPath.begin(), subPath.end());
		start = end;
	}

//	orig = Log::ReportingLevel;
//	Log::ReportingLevel = logDEBUG4;
//	DumpMap("maps/explore2.log", ptPath);
//	Log::ReportingLevel = orig;


	std::vector<POINT>::iterator it = ptPath.begin() + 1;

	while (it != ptPath.end() - 1)
	{
		POINT start = *(it-1);
		POINT end = *(it+1);

		if (CalculateDistance(start, end) < 25)
		{
			ptPath.erase(it);
			it = ptPath.begin();
		}

		it++;
	}
*/
//	orig = Log::ReportingLevel;
//	Log::ReportingLevel = logDEBUG4;
//	DumpMap("maps/explore3.log", ptPath);
//	Log::ReportingLevel = orig;

//	for (size_t i = 0; i < ptPath.size(); i++)
//	{
//		std::cout << std::endl;
//	}

    return points;
}

std::vector<POINT>
CCollisionMap::pathTo(int start_x, int start_y, int end_x, int end_y, bool teleport, int range) throw(...) {
    POINT ptStart = {start_x, start_y};
    POINT ptEnd = {end_x, end_y};

    //std::cout << std::endl;
    //std::cout << std::endl;

    AbsToRelative(ptStart);
    AbsToRelative(ptEnd);

    std::cout << "Pathing (" << ptStart.x << "," << ptStart.y << ") to (" << ptEnd.x << "," << ptEnd.y << ")"
              << std::endl;
/*
	// clear the area around start and destination
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			if (IsValidIndex(ptStart.x+i, ptStart.y+j))
			{
				m_map(ptStart.x+i, ptStart.y+j) = MAP_DATA_CLEANED;
			}
			if (IsValidIndex(ptEnd.x+i, ptEnd.y+j))
			{
				m_map(ptEnd.x+i, ptEnd.y+j) = MAP_DATA_CLEANED;
			}
		}
	}
*/
    if (!IsValidIndex(ptStart) || !IsValidIndex(ptEnd)) {
        std::cout << "Start or Destination point is invalid (" << ptStart.x << "," << ptStart.y << ") -> (" << ptEnd.x
                  << "," << ptEnd.y << ")" << std::endl;
        return std::vector<POINT>();
    }

    //m_map(ptStart.x, ptStart.y) = MAP_DATA_CLEANED;
    //m_map(ptStart.x, ptStart.y) = MAP_DATA_CLEANED;

    std::vector<POINT> Buffer(255);
    int PathAmount = 0;

    clock_t start = clock();

    if (teleport) {
        CTeleportPath tp(m_map, m_map.size1(), m_map.size2(), range);
        PathAmount = tp.FindTeleportPath(ptStart, ptEnd, Buffer, 255);
    } else {
        CWalkPath wp(m_map, m_map.size1(), m_map.size2(), range);
        PathAmount = wp.FindWalkPath(ptStart, ptEnd, Buffer, 255);
    }

    std::cout << "time to generate path was " << clock() - start << " ms" << std::endl;

    if (PathAmount == 0) {
        std::cout << "Path length is 0" << std::endl;
        std::vector<POINT> ptPath;
        ptPath.push_back(ptStart);
        ptPath.push_back(ptEnd);

        for (UINT i = 0; i < ptPath.size(); i++) {
            RelativeToAbs(ptPath[i]);
        }

        CHAR szMapName[64] = "";
        sprintf_s(szMapName, sizeof(szMapName), "maps/Path from %d,%d to %d,%d.log", start_x, start_y, end_x, end_y);
        DumpMap(szMapName, ptPath);

        std::cout << "Path length is 0" << std::endl;
        return std::vector<POINT>();
    }

    Buffer.resize(PathAmount);

    for (UINT i = 0; i < Buffer.size(); i++) {
        std::cout << std::endl;

        if (Buffer[i].x == 0 || Buffer[i].y == 0 || Buffer[i].x == m_map.size1() - 1 ||
            Buffer[i].y == m_map.size2() - 1) {
            std::cout << "Level Near" << std::endl;
            POINT pt = Buffer[i];
            pt.x += Buffer[i].x == 0 ? -20 : 0;
            pt.y += Buffer[i].y == 0 ? -20 : 0;
            pt.x += Buffer[i].x == m_map.size1() - 1 ? 20 : 0;
            pt.y += Buffer[i].y == m_map.size2() - 1 ? 20 : 0;
            Buffer.push_back(pt);
        }

        RelativeToAbs(Buffer[i]);
    }

    CHAR szMapName[64] = "";
    sprintf_s(szMapName, sizeof(szMapName), "maps/Path from %d,%d to %d,%d.log", start_x, start_y, end_x, end_y);
    DumpMap(szMapName, Buffer);


    if (Buffer.size() > 1) {
        Buffer.erase(Buffer.begin());
    }

    return Buffer;
}

POINT CCollisionMap::teleportTo(std::vector<POINT> &points, int x, int y, int dist) throw(...) {
    // lets build a kinda smart map
    boost::numeric::ublas::matrix<WORD> a_map;
    a_map.resize(2 * dist + 1, 2 * dist + 1, false);

    // get your relative position
    POINT pt = {x, y};
    AbsToRelative(pt);

    for (int x = 0; x < 2 * dist + 1; x++) {
        for (int y = 0; y < 2 * dist + 1; y++) {
            int x1 = pt.x + x - dist;
            int y1 = pt.y + y - dist;

            if (IsValidIndex(x1, y1)) {
                if (m_map(x1, y1) % 2 == 0) {
                    if (m_map(x1, y1) != MAP_DATA_CLEANED) {
                        // safe and clear, assign min threat
                        a_map(x, y) = 1;
                    } else {
                        // don't want to go back to where we have been already...
                        a_map(x, y) = 0xFFFF;
                    }
                } else {
//					if (m_map(x1, y1) != MAP_DATA_FILLED && m_map(x1, y1) != MAP_DATA_THICKENED)
//					{
                    // can't go here, assign max int
                    a_map(x, y) = 0xFFFF;
//					}
//					else
//					{
                    // likely in a corner or against a wall, assign a threat level of 100 (prob need to adjust later)
//						a_map(x, y) = 100;
//					}
                }
            } else {
                // can't go here, assign max int
                a_map(x, y) = 0xFFFF;
            }
        }
    }

    ///////////////////////////////////////////////////////////
    /*
    char lpszFilePath[100];
    sprintf_s(lpszFilePath, sizeof(lpszFilePath), "teleportTo(%d,%d) - %d - %d.txt", x, y, dwLevelId, time(NULL));
    FILE *fp = fopen(lpszFilePath, "w+");

    for (int x = 0; x < 2 * dist + 1; x++)
    {
        for (int y = 0; y < 2 * dist + 1; y++)
        {
            fprintf(fp, "%4d", a_map(x,y));
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    */
    ///////////////////////////////////////////////////////////

    for (int i = 0; i < points.size(); i++) {
        AbsToRelative(points[i]);

        for (int x = -dist; x <= dist; x++) {
            for (int y = -dist; y <= dist; y++) {
                if (IsValidIndex(points[i].x + x, points[i].y + y)) {
                    int x1 = pt.x - (points[i].x + x);
                    int y1 = pt.y - (points[i].y + y);

                    if (abs(x1) <= dist && abs(y1) <= dist) {
                        if (a_map(x1 + dist, y1 + dist) != 0xFFFF) {
                            a_map(x1 + dist, y1 + dist) += (2 * dist) - abs(x) + abs(y);
                        }
                    }
                }
            }
        }
    }

    POINT result = {2 * dist, dist};

    for (int x = 0; x < 2 * dist + 1; x++) {
        for (int y = 0; y < 2 * dist + 1; y++) {
//			if ((abs (x - dist) > dist / 2) && (abs (x - dist) > dist / 2))
//			{
            if (a_map(result.x, result.y) > a_map(x, y)) {
                result.x = x;
                result.y = y;
            }
//			}
        }
    }
/*
	for (int o = -5; o <= 5; o++)
	{
		for (int r = 0; r < 360; r++)
		{
			POINT pt = {25,25};
			pt = CalculatePointOnTrack(pt, dist, r + o);

			if (a_map(result.x, result.y) < a_map(pt.x, pt.y))
			{
				result = pt;
			}

			a_map(pt.x, pt.y) = 999;
		}
	}
*/
    a_map(result.x, result.y) = 9999;

    result.x += pt.x - dist;
    result.y += pt.y - dist;
    RelativeToAbs(result);

    ///////////////////////////////////////////////////////////
    /*
    for (int x = 0; x < 2 * dist + 1; x++)
    {
        for (int y = 0; y < 2 * dist + 1; y++)
        {
            fprintf(fp, "%4d", a_map(x,y));
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    */
    ///////////////////////////////////////////////////////////

    return result;
}

extern long CalculateDistance(long x1, long y1, long x2, long y2);

extern POINT CalculatePointOnTrack(int x, int y, int nRadius, int nAngle);

extern long CalculateAngle(long x1, long y1, long x2, long y2);

bool CCollisionMap::CheckLineOfSight(int x1, int y1, int x2, int y2) {
    long angle = CalculateAngle(x1, y1, x2, y2);
    int length = CalculateDistance(x1, y1, x2, y2);
    bool ret = true;

    for (int l = 0; l < length; l++) {
        POINT pt = CalculatePointOnTrack(x1, y1, l, angle);

        AbsToRelative(pt);


        if (!IsValidIndex(pt)) {
            ret = false;
        }
        if (m_map(pt.x, pt.y) % 2 != 0)
            ret = false;
        if (!ret) {
            pt = CalculatePointOnTrack(x1, y1, l--, angle);
//			awesomo->me->lastValid.x = pt.x; //ToDo; store this somewhere?
//			awesomo->me->lastValid.y = pt.y;
            return ret;
        }
    }

    return ret;
}
