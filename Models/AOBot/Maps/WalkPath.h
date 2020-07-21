#include <boost/numeric/ublas/matrix.hpp>
#include <vector>
#include <windows.h>

class CWalkPath {
public:
    int WALK_RANGE;
    int dest_x;
    int dest_y;

    CWalkPath(boost::numeric::ublas::matrix<WORD> pCollisionMap, int cx, int cy, int range);

    DWORD FindWalkPath(POINT ptStart, POINT ptEnd, std::vector<POINT> &lpBuffer, DWORD dwMaxCount); // Calculate path
    int GetMap(unsigned int x, unsigned int y);

private:
    boost::numeric::ublas::matrix<WORD> mapa;
};

