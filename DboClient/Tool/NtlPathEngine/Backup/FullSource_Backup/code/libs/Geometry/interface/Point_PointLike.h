//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINT_POINTLIKE_HAS_BEEN_INCLUDED
#define POINT_POINTLIKE_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"

class cPoint_PointLike : public iPointLike
{
    tPoint _p;

public:

    cPoint_PointLike() {}
    cPoint_PointLike(const tPoint& p) :
      _p(p)
    {
    }

    tPoint& p() {return _p;}
    const tPoint& p() const {return _p;}

    eSide sideOfLine(const tLine& line) const
    {
        return line.side(_p);
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return _p == p;
    }
};

inline eSide
SideOfLine(const tLine& line, const cPoint_PointLike& p)
{
    return p.sideOfLine(line);
}
inline eDirection
DirectionFromLine(const tLine& line, const cPoint_PointLike& p)
{
    return line.direction(p.p());
}

#endif
