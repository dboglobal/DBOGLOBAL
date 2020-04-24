//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef JUST_AFTER_INTERSECTION_POINTLIKE_HAS_BEEN_INCLUDED
#define JUST_AFTER_INTERSECTION_POINTLIKE_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/tLine.h"

class cJustAfterIntersection_PointLike : public iPointLike
{
    tLine _line1, _line2;

public:

    cJustAfterIntersection_PointLike()
    {
    }
    // the point like is just after line2 in the direction of line1
    cJustAfterIntersection_PointLike(const tLine& line1, const tLine& line2) :
     _line1(line1),
     _line2(line2)
    {
        eSide side = _line1.side(_line2);
        assertD(side != SIDE_CENTRE);
        if(side == SIDE_LEFT)
        {
            _line2.reverse();
        }
    }

    eSide sideOfLine(const tLine& line) const
    {
        eSide side = _line1.side(line);
        if(side == SIDE_CENTRE)
        {
            return line.side(_line1.start());
        }
        eSide result;
        if(side == SIDE_RIGHT)
        {
            result = _line1.intersectionOrder_Clockwise(line, _line2);
        }
        else
        {
            result = OtherSide(_line1.intersectionOrder_Clockwise(line.reversed(), _line2));
        }
        if(result == SIDE_CENTRE)
        {
            result = SIDE_LEFT;
        }
        return result;
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return false;
    }
};

inline eSide SideOfLine(const tLine& line, const cJustAfterIntersection_PointLike& p)
{
    return p.sideOfLine(line);
}

#endif
