#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include "../d2data/AreaLevel.h"

class dijkstra { // tolua_export

private:
	static int predecessor[132][132];

	static void pathto(int from, int to, std::vector<AreaLevel::AreaLevel> *ptPath);

public:
	//tolua_begin
	static std::vector<AreaLevel::AreaLevel> getPath(AreaLevel::AreaLevel from, AreaLevel::AreaLevel to);
};
// tolua_end