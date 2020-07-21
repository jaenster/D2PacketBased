//////////////////////////////////////////////////////////////////////
// TeleportPath.h
//
// Diablo II game path finding(teleport) algorithms. 
//
// I must give credits to both Niren7 and Ninjai, their code helped me
// start this class to say the least.
//
// Ustc_tweeg also helped me a lot on completing this algorithm.
//
// Written by Abin(abinn32@yahoo.com)
// Sep 10th, 2004
////////////////////////////////////////////////////////////////////////

#ifndef __TELEPORTPATH_H__
#define __TELEPORTPATH_H__

#include <boost/numeric/ublas/matrix.hpp>
#include <windows.h>

class CTeleportPath {
public:

    CTeleportPath(boost::numeric::ublas::matrix<WORD> pCollisionMap, int cx, int cy, int range);

    virtual ~CTeleportPath();

    DWORD
    FindTeleportPath(POINT ptStart, POINT ptEnd, std::vector<POINT> &lpBuffer, DWORD dwMaxCount); // Calculate path

private:

    static int TP_RANGE;

    BOOL DumpDistanceTable(LPCSTR lpszFilePath) const;

    static int GetRedundancy(const std::vector<POINT> lpPath, DWORD dwMaxCount, const POINT &pos);

    void Block(POINT pos, int nRange);

    BOOL GetBestMove(POINT &rResult, int nAdjust = 2);

    BOOL MakeDistanceTable();

    BOOL IsValidIndex(int x, int y) const;

    boost::numeric::ublas::matrix<WORD> m_ppTable;    // Distance table
    POINT m_ptStart;
    POINT m_ptEnd;
    int m_nCX;
    int m_nCY;

};

#endif // __TELEPORTPATH_H__
