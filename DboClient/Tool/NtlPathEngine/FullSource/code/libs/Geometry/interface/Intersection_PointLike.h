//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef T_INTERSECTION_POINTLIKE_HAS_BEEN_INCLUDED
#define T_INTERSECTION_POINTLIKE_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/tLine.h"

class cIntersection_PointLike : public iPointLike
{
    tIntersection _p;

public:

    cIntersection_PointLike()
    {
    }
    cIntersection_PointLike(const tIntersection& p) :
      _p(p)
    {
    }
    cIntersection_PointLike(const tLine& line1, const tLine& line2) :
     _p(line1, line2)
    {
    }

    eSide sideOfLine(const tLine& line) const
    {
        return line.side(_p);
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return _p == p;
    }
};

inline eSide SideOfLine(const tLine& line, const cIntersection_PointLike& p)
{
    return p.sideOfLine(line);
}

#endif
