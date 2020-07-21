#include "WalkPath.h"
#include "stlastar.h"

//int CWalkPath::WALK_RANGE;

//int CWalkPath::dest_x;
//int CWalkPath::dest_y;

#define PI 3.1415926535897932384626433832795

void NormalizeAngle(int &rAngle) {
    if (::abs(rAngle) >= 360)
        rAngle %= 360;

    if (rAngle < 0)
        rAngle += 360;
}

long CalculateDistance(long x1, long y1, long x2, long y2) {
    return (long) ::sqrt((double) ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

long CalculateDistance(const POINT &pt1, const POINT &pt2) {
    return CalculateDistance(pt1.x, pt1.y, pt2.x, pt2.y);
}

POINT CalculatePointOnTrack(int x, int y, int nRadius, int nAngle) {
    NormalizeAngle(nAngle);

    POINT pt = {x, y};

    if (nAngle == 0)
        pt.x += nRadius;
    else if (nAngle == 90)
        pt.y += nRadius;
    else if (nAngle == 180)
        pt.x -= nRadius;
    else if (nAngle == 270)
        pt.y -= nRadius;
    else {
        pt.x = LONG(double(x) + ::cos((double) nAngle * PI / 180.0) * (double) nRadius);
        pt.y = LONG(double(y) - ::sin((double) nAngle * PI / 180.0) * (double) nRadius);
    }

    return pt;
}

POINT CalculatePointOnTrack(const POINT &ptOrigin, int nRadius, int nAngle) {
    return CalculatePointOnTrack(ptOrigin.x, ptOrigin.y, nRadius, nAngle);
}

long CalculateAngle(long x1, long y1, long x2, long y2) {
    // mathematic stuff, now thanks God I haven't forgot all of them...
    if (x1 == x2 && y1 == y2)
        return 0;

    double fAngle = 0.0;

    if (x1 == x2) {
        // vertical special case
        fAngle = y2 > y1 ? 270.0 : 90.0;
    } else if (y1 == y2) {
        // horizontal special case
        fAngle = x2 > x1 ? 0.0 : 180.0;
    } else {
        // common case
        fAngle = ::atan(((double) y2 - (double) y1) / ((double) x2 - (double) x1)) * 180.0 / PI;

        // determine the phases (1-4)
        // Phases allocation figure:
        /*
                   y
                   |
            P2     |    P1
                   |
        -----------+----------->x
                   |
            P3	   |    P4
                   |
        */
        int nPhase = 0;
        if (y2 < y1)
            nPhase = x2 > x1 ? 1 : 2;
        else
            nPhase = x2 > x1 ? 4 : 3;

        // adjust the angle according to phases
        switch (nPhase) {
            case 1:
                fAngle = -fAngle;
                break;

            case 2:
            case 3:
                fAngle = 180.0 - fAngle;
                break;

            case 4:
                fAngle = 360.0 - fAngle;
                break;

            default:
                fAngle = 0.0;
                break;
        }
    }

    return (long) fAngle;
}

long CalculateAngle(const POINT &pt1, const POINT &pt2) {
    return CalculateAngle(pt1.x, pt1.y, pt2.x, pt2.y);
}


// Definitions

class MapSearchNode {
private:
    CWalkPath *walkpath;

public:
    unsigned int x;     // the (x,y) positions of the node
    unsigned int y;

    MapSearchNode() {
        x = y = 0;
        this->walkpath = NULL;
    }

    MapSearchNode(CWalkPath *walkpath) {
        x = y = 0;
        this->walkpath = walkpath;
    }

    MapSearchNode(CWalkPath *walkpath, unsigned int px, unsigned int py) {
        x = px;
        y = py;
        this->walkpath = walkpath;
    }

    float GoalDistanceEstimate(MapSearchNode &nodeGoal);

    bool IsGoal(MapSearchNode &nodeGoal);

    bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node, bool failed = false);

    float GetCost(MapSearchNode &successor);

    bool IsSameState(MapSearchNode &rhs);

    void PrintNodeInfo();
};

bool MapSearchNode::IsSameState(MapSearchNode &rhs) {

    // same state in a maze search is simply when (x,y) are the same
    if ((x == rhs.x) &&
        (y == rhs.y)) {
        return true;
    } else {
        return false;
    }

}

void MapSearchNode::PrintNodeInfo() {
    char str[100];
    sprintf_s(str, 100, "Node position : (%d,%d)\n", x, y);

    std::cout << str;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal) {
    return CalculateDistance(x, y, nodeGoal.x, nodeGoal.y);
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal) {
    if ((abs((int) (x - nodeGoal.x)) <= 5) && (abs((int) (y - nodeGoal.y)) <= 5)) {
        return true;
    }

    return false;
}

struct ffs {
    POINT p;
    bool valid;
    bool stop;
    int angle;
    int cost;

    ffs(int x, int y, int a) {
        p.x = x;
        p.y = y;
        valid = false;
        stop = false;
        angle = a;
        cost = 10000;
    }
};

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node, bool failed) {

    int parent_x = -1;
    int parent_y = -1;

    if (parent_node) {
        parent_x = parent_node->x;
        parent_y = parent_node->y;
    }


    MapSearchNode NewNode(walkpath);

    /*
    for (int i = -CWalkPath::WALK_RANGE; i <= CWalkPath::WALK_RANGE; i+=2)
    {
        for (int j = -CWalkPath::WALK_RANGE; j <= CWalkPath::WALK_RANGE; j+=2)
        {
            if ( (!i && j) || (i && !j) || (i == j && i))
            {
                if ((GetMap(x+i, y+j) < 9) && !((parent_x == x+i) && (parent_y == y+j)))
                {
                    NewNode = MapSearchNode( x+i, y+j );
                    astarsearch->AddSuccessor( NewNode );
                }
            }
        }
    }
    */

    int CURRENT_WALK_RANGE = min(CalculateDistance(x, y, walkpath->dest_x, walkpath->dest_y), walkpath->WALK_RANGE);
    long angle = CalculateAngle(x, y, walkpath->dest_x, walkpath->dest_y);

    std::vector<ffs> points;

    points.push_back(ffs(x, y, 0));
    points.push_back(ffs(x, y, 90));
    points.push_back(ffs(x, y, -90));
    points.push_back(ffs(x, y, 180));

    points.push_back(ffs(x, y, angle));
    points.push_back(ffs(x, y, angle + 45));
    points.push_back(ffs(x, y, angle - 45));

    //points.push_back(ffs(x, y, angle + 30));
    //points.push_back(ffs(x, y, angle + 45));
    //points.push_back(ffs(x, y, angle + 90));
    //points.push_back(ffs(x, y, angle + 135));
    //points.push_back(ffs(x, y, angle - 30));
    //points.push_back(ffs(x, y, angle - 45));
    //points.push_back(ffs(x, y, angle - 90));
    //points.push_back(ffs(x, y, angle - 135));

    for (int r = 0; r < CURRENT_WALK_RANGE; r++) {
        for (int i = 0; i < points.size(); i++) {
            if (!points[i].stop) {
                POINT p = CalculatePointOnTrack(x, y, r, points[i].angle);
                int cost = CalculateDistance(p.x, p.y, walkpath->dest_x, walkpath->dest_y);

                if (walkpath->GetMap(p.x, p.y) < 9) {
                    if (r > 3 && points[i].cost > cost) {
                        points[i].p = p;
                        points[i].valid = true;
                        points[i].cost = cost;
                    }
                } else {
                    if (!failed || r > 3) {
                        points[i].stop = true;
                    }
                }
            }
        }
    }

    bool foundone = false;

    for (int i = 0; i < points.size(); i++) {
        if (points[i].valid) {
            NewNode = MapSearchNode(walkpath, points[i].p.x, points[i].p.y);
            astarsearch->AddSuccessor(NewNode);
            foundone = true;
        }
    }

    if (!foundone && !parent_node && !failed) {
        return GetSuccessors(astarsearch, parent_node, true);
    }

    return points.size() > 0;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor) {
    //return (float) GetMap( x, y );
/*
	int points = 0;

	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			if (GetMap(x+i, y+j) == 9)
			{
				points += 10;
			}
		}
	}
*/
    return /*points + */CalculateDistance(x, y, walkpath->dest_x, walkpath->dest_y);
}


CWalkPath::CWalkPath(boost::numeric::ublas::matrix<WORD> pCollisionMap, int cx, int cy, int range) {
    CWalkPath::WALK_RANGE = range;
    mapa = pCollisionMap;
}

DWORD CWalkPath::FindWalkPath(POINT ptStart, POINT ptEnd, std::vector<POINT> &lpBuffer, DWORD dwMaxCount) {
    CWalkPath::dest_x = ptEnd.x;
    CWalkPath::dest_y = ptEnd.y;

    lpBuffer[0] = ptStart; // start point

    AStarSearch<MapSearchNode> astarsearch;

    unsigned int SearchCount = 0;

    const unsigned int NumSearches = 1;

    while (SearchCount < NumSearches) {
        // Create a start state
        MapSearchNode nodeStart(this);
        nodeStart.x = ptStart.x;
        nodeStart.y = ptStart.y;

        // Define the goal state
        MapSearchNode nodeEnd(this);
        nodeEnd.x = ptEnd.x;
        nodeEnd.y = ptEnd.y;

        // Set Start and goal states
        astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

        unsigned int SearchState;
        unsigned int SearchSteps = 0;

        do {
            SearchState = astarsearch.SearchStep();
            SearchSteps++;

//			if (SearchSteps > 1000)
//			{
//				SearchState = AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED;
//			}
        } while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

        int steps = 1;

        if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {
            MapSearchNode *node = astarsearch.GetSolutionStart();

            //node->PrintNodeInfo();
            for (;;) {
                node = astarsearch.GetSolutionNext();

                if (!node) {
                    break;
                }

                POINT p = {node->x, node->y};
                lpBuffer[steps] = p;
                //node->PrintNodeInfo();
                steps++;

            };

            // Once you're done with the solution you can free the nodes up
            astarsearch.FreeSolutionNodes();

            // Add end point if we arent using range of 1
//			if (CWalkPath::WALK_RANGE != 1)
//			{
//				lpBuffer[steps] = ptEnd;
//				steps ++;
//			}
        } else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED) {
            std::cout << "Info: Search terminated. Did not find goal state" << std::endl;
        }

        SearchCount++;
        astarsearch.EnsureMemoryFreed();

        return steps;
    }
    return 0;
}

// map helper functions

int CWalkPath::GetMap(unsigned int x, unsigned int y) {
    if (x < 0 || x >= mapa.size1() || y < 0 || y >= mapa.size2()) {
        return 9;
    }

    return mapa(x, y) % 2 == 0 ? 1 : 9;//(y*MAP_WIDTH)+x];
}
