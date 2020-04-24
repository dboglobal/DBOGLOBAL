//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INFINITESSIMALLY_OFFSET_POINTLIKE_HAS_BEEN_INCLUDED
#define INFINITESSIMALLY_OFFSET_POINTLIKE_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"

template <class tPointLike>
class cInfinitessimallyOffset_PointLike : public iPointLike
{
    tPointLike _p;
    tPoint _offset;

public:

    cInfinitessimallyOffset_PointLike()
    {
    }
    cInfinitessimallyOffset_PointLike(const tPointLike& p, const tPoint& offset) :
      _p(p),
      _offset(offset)
    {
    }

    eSide sideOfLine(const tLine& line) const
    {
        eSide side = _p.sideOfLine(line);
        if(side == SIDE_CENTRE)
        {
            side = line.axis().side(_offset);
        }
        return side;
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return false;
    }
};

#endif
