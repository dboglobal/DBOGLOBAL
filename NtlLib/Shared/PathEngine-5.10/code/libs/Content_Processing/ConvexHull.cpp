//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ConvexHull.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include <algorithm>
#include <utility>

using namespace std;

static tSigned32
compare(const tPoint& p1, const tPoint& p2)
{
    if(p2.getX() < p1.getX())
    {
        return -1;
    }
    if(p2.getX() > p1.getX())
    {
        return 1;
    }
    if(p2.getY() < p1.getY())
    {
        return -1;
    }
    if(p2.getY() > p1.getY())
    {
        return 1;
    }
    return 0;
}

class CoordinatePredicate
{
public:

    // returns true if p2 is greater than p1
    bool operator()(const tPoint& p1, const tPoint& p2);
};

bool
CoordinatePredicate::operator()(const tPoint& p1, const tPoint& p2)
{
    return compare(p1, p2) == 1;
}

class AnglePredicate
{
    tPoint _centre;
public:

    AnglePredicate(const tPoint& centre);

    // returns true if p2 is greater than p1
    bool operator()(const tPoint& p1, const tPoint& p2);
};

AnglePredicate::AnglePredicate(const tPoint& centre) :
    _centre(centre)
{
}

bool
AnglePredicate::operator()(const tPoint& p1, const tPoint& p2)
{
    if(p1 == _centre || p2 == _centre)
    {
        assertD(p1 != _centre || p2 != _centre);
        return p2 == _centre; // centre comes last
    }
    tPoint offset1 = p1 - _centre;
    tPoint offset2 = p2 - _centre;
    eSide side = offset1.side(offset2);
    if(side != SIDE_CENTRE)
    {
        return side == SIDE_RIGHT;
    }
    tPoint::tDistance distance1 = offset1.lengthSquared();
    tPoint::tDistance distance2 = offset2.lengthSquared();
    return distance2 < distance1;
}

void
ConvexHull(const std::vector<tPoint>& points, std::vector<tPoint>& result)
{
    assertD(!points.empty());

    vector<tPoint> sorted = points;
    sort(sorted.begin(), sorted.end(), CoordinatePredicate());

// remove repeated points
    vector<tPoint>::iterator afterRepeats = unique(sorted.begin(), sorted.end());
    sorted.erase(afterRepeats, sorted.end());

    sort(sorted.begin(), sorted.end(), CoordinatePredicate());

    do
    {
        result.push_back(sorted.front());

        AnglePredicate anglePredicate(result.back());
        sort(sorted.begin(), sorted.end(), anglePredicate);
    }
    while(sorted.front() != result.front());
}

//void
//addPointsToHullIfNecessaryToEnsureNonZeroArea(std::vector<tPoint>& hull)
//{
//	if(hull.size() == 1)
//	{
//		tPoint p = hull.front();
//		hull.push_back(p + tPoint(0, 1, 1));
//		hull.push_back(p + tPoint(1, 0, 1));
//	}
//	else
//	if(hull.size() == 2)
//	{
//		tPoint p1 = hull.front();
//		tPoint p2 = hull.back();
//		tLine l(p1, p2);
//		tPoint candidate = p1 + tPoint(0, 1, 1);
//		if(l.side(candidate) != SIDE_RIGHT)
//		{
//			candidate = p1 + tPoint(1, 0, 1);
//			if(l.side(candidate) != SIDE_RIGHT)
//			{
//				candidate = p1 + tPoint(0, -1, 1);
//				if(l.side(candidate) != SIDE_RIGHT)
//				{
//					candidate = p1 + tPoint(-1, 0, 1);
//					assertD(l.side(candidate) == SIDE_RIGHT);
//				}
//			}
//		}
//		hull.push_back(candidate);			
//	}
//}

