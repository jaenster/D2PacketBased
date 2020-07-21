#include "Maps.h"
#include "..\..\..\D2\D2Ptrs.h"
#include <time.h>

Maps::Maps() {
    mapid = 0;
    difficulty = 0;
    townPreloadDone = false;
}

Maps::~Maps() {
    if (this->mapid) {
        for (int i = 0; i < 5; i++) {
            try {
                D2COMMON_UnloadAct(pAct[i]);
                std::cout << "Unloaded Act " << i + 1 << std::endl;
            }
            catch (...) {
                std::cout << "Failed to Unload Act " << i + 1 << std::endl;
            }
        }
    }

    for (std::map<AreaLevel::AreaLevel, CCollisionMap *>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
        delete iter->second;
    }
}

int Maps::Routine() {
    CCollisionMap::loading.Lock();

    try {
        BYTE bActLevels[] = {1, 40, 75, 103, 109, 137};

        for (int i = 0; i < 5; i++) {
            pAct[i] = D2COMMON_LoadAct(i, mapid, TRUE, FALSE, difficulty, NULL, bActLevels[i], D2CLIENT_LoadAct_1,
                                       D2CLIENT_LoadAct_2);
            std::cout << "Loaded Act " << i + 1 << " pointer = " << pAct[i] << std::endl;
        }

        std::cout << "Loading map " << AreaLevel::toString(AreaLevel::RogueEncampment) << std::endl;
        maps[AreaLevel::RogueEncampment] = new CCollisionMap(pAct[0], AreaLevel::RogueEncampment);
        maps[AreaLevel::RogueEncampment]->Create(false);

        std::cout << "Loading map " << AreaLevel::toString(AreaLevel::LutGholein) << std::endl;
        maps[AreaLevel::LutGholein] = new CCollisionMap(pAct[1], AreaLevel::LutGholein);
        maps[AreaLevel::LutGholein]->Create(false);

        std::cout << "Loading map " << AreaLevel::toString(AreaLevel::KurastDocks) << std::endl;
        maps[AreaLevel::KurastDocks] = new CCollisionMap(pAct[2], AreaLevel::KurastDocks);
        maps[AreaLevel::KurastDocks]->Create(false);

        std::cout << "Loading map " << AreaLevel::toString(AreaLevel::ThePandemoniumFortress) << std::endl;
        maps[AreaLevel::ThePandemoniumFortress] = new CCollisionMap(pAct[3], AreaLevel::ThePandemoniumFortress);
        maps[AreaLevel::ThePandemoniumFortress]->Create(false);


        std::cout << "Loading map " << AreaLevel::toString(AreaLevel::Harrogath) << std::endl;
        maps[AreaLevel::Harrogath] = new CCollisionMap(pAct[4], AreaLevel::Harrogath);
        maps[AreaLevel::Harrogath]->Create(false);

    }
    catch (...) {
        CCollisionMap::loading.Unlock();
        throw "error loading act pointers";
    }

    CCollisionMap::loading.Unlock();
    return 1;
}

void Maps::JoinAll() {
    Join();

    for (std::map<AreaLevel::AreaLevel, CCollisionMap *>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
        clock_t start = clock();
        iter->second->Abort();
        iter->second->Join();

        if (clock() - start > 0)
            std::cout << "had to wait " << clock() - start << " ms for " << AreaLevel::toString(iter->first)
                      << " to finish loading" << std::endl;
    }
}

void Maps::setMapId(unsigned int mapid, unsigned int difficulty) throw(...) {
    _maps.clear();

    if (!this->mapid) {
        this->mapid = mapid;
        this->difficulty = difficulty;
        Create(false);
        SwitchToThread();
    }
}

void Maps::setLevel(AreaLevel::AreaLevel level) throw(...) {
    std::cout << "Changed Levels from " << AreaLevel::toString(this->level) << " to " << AreaLevel::toString(level)
              << std::endl;
    this->level = level;
}

AreaLevel::AreaLevel Maps::getLevel() throw(...) {
    return level;
}

void Maps::setPosition(int x, int y) throw(...) {
    this->x = x;
    this->y = y;

    for (std::map<AreaLevel::AreaLevel, std::set<std::pair<int, int>>>::iterator iter = _maps.begin();
         iter != _maps.end(); iter++) {
        if (level != iter->first && iter->second.size() > 0) {
            if (!maps[iter->first]->IsRunning() && maps[iter->first]->IsValidAbsLocation(x, y)) {
                setLevel(iter->first);
            }
        }
    }

}

bool Maps::loadMap(int act, AreaLevel::AreaLevel level) throw(...) {
    bool result = false;

    try {
        Join();

        if (maps.find(level) != maps.end()) {
            //std::cou << std::endlt
            result = true;
        } else if (mapid) {
            if (pAct[act - 1]) {
                maps[level] = new CCollisionMap(pAct[act - 1], level);
                maps[level]->Create(false);
                std::cout << "Loading map " << AreaLevel::toString(level) << std::endl;
                SwitchToThread();
                result = true;
            }
        }
    }
    catch (...) {
        std::cout << "Exception Caught In loadMap" << std::endl;
    }

    return result;
}

int getAct(AreaLevel::AreaLevel level) {
    char bActLevels[] = {1, 40, 75, 103, 109, 137};

    for (int i = 0; i < 5; i++) {
        if (level < bActLevels[i + 1]) {
            return i;
        }
    }

    return 0;
}

CCollisionMap *Maps::getMap(AreaLevel::AreaLevel level) throw(...) {
    Join();
    CCollisionMap *map = maps[level];
    map->Join();

    if (map->GetResult()) {
        delete map;
        maps[level] = new CCollisionMap(pAct[getAct(level)], level);
        maps[level]->Create(false);
        maps[level]->Join();
    }

    return map;
}

void Maps::Invalidate(int x, int y) throw(...) {
    getMap(level)->Invalidate(x, y);
}

POINT Maps::pathToLevel(AreaLevel::AreaLevel id) throw(...) {
    CCollisionMap *map = getMap(level);
    return map->pathToLevel(id);
}

POINT Maps::pathToObject(int id) throw(...) {
    return getMap(level)->pathToObject(id);
}

POINT Maps::pathToNpc(int id) throw(...) {
    return getMap(level)->pathToNpc(id);
}

int Maps::getWp() throw(...) {
    return getMap(level)->getWp();
}

std::vector<POINT> Maps::explore() throw(...) {
    return getMap(level)->explore(x, y);
}

std::vector<POINT> Maps::pathTo(int x, int y, bool teleport, int range) throw(...) {
    return getMap(level)->pathTo(this->x, this->y, x, y, teleport, range);
}

POINT Maps::getDistantPoint(std::vector<POINT> &points, int x, int y, int dist) throw(...) {
    return getMap(level)->teleportTo(points, x, y, dist);
}

bool Maps::CheckLineOfSight(int a, int b, int x, int y) {
    return getMap(level)->CheckLineOfSight(a, b, x, y);
}

bool Maps::IsValidIndex(AreaLevel::AreaLevel level, long x, long y) throw(...) {
    return getMap(level)->IsValidIndex(x, y);
}

bool Maps::IsValidAbsLocation(AreaLevel::AreaLevel level, long x, long y) throw(...) {
    return getMap(level)->IsValidAbsLocation(x, y);
}

void Maps::mapAdd(AreaLevel::AreaLevel level, int x, int y) throw(...) {
    _maps[level].insert(std::pair<int, int>(x, y));
}

void Maps::mapRemove(AreaLevel::AreaLevel level, int x, int y) throw(...) {
    _maps[level].erase(std::pair<int, int>(x, y));
}

AreaLevel::AreaLevel Maps::GetCorrectTomb() throw(...) {
    return (AreaLevel::AreaLevel) pAct[1]->pMisc->dwStaffTombLevel;
}

std::vector<AreaLevel::AreaLevel> Maps::getPath(AreaLevel::AreaLevel from, AreaLevel::AreaLevel to) {
    std::vector<AreaLevel::AreaLevel> result;

    if (to == AreaLevel::DurielsLair) {
        result = dijkstra::getPath(from, GetCorrectTomb()); // get the needed levels
        result.push_back(AreaLevel::DurielsLair);
    } else {
        result = dijkstra::getPath(from, to); // get the needed levels
    }

    std::cout << "Preloading needed maps..." << std::endl;

    for (int i = 0; i < result.size(); i++) {
        if ((int) result[i] <= 39) // Act 1 map
            loadMap(1, result[i]);
        else if ((int) result[i] <= 74) // Act 2 map
            loadMap(2, result[i]);
        else if ((int) result[i] <= 102) // Act 3 map
            loadMap(3, result[i]);
        else if ((int) result[i] <= 108) // Act 4 map
            loadMap(4, result[i]);
        else if ((int) result[i] <= 136) // Act 5 map
            loadMap(5, result[i]);


    }

    return result;
}

bool Maps::inTown() {
    AreaLevel::AreaLevel level = this->getLevel();
    return (level == AreaLevel::RogueEncampment || level == AreaLevel::LutGholein || level == AreaLevel::KurastDocks ||
            level == AreaLevel::ThePandemoniumFortress || level == AreaLevel::Harrogath);
}