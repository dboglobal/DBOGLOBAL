//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SIOFFSET_POINTLIKE_HAS_BEEN_INCLUDED
#define SIOFFSET_POINTLIKE_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/StandardIotas.h"

template <class tPointLike>
class cSIOffset_PointLike : public iPointLike
{
    const tPointLike& _p;

public:

    cSIOffset_PointLike()
    {
    }
  // NOTE: a reference of p is taken so p must remain valid over the lifetime of constructed object
    cSIOffset_PointLike(const tPointLike& p) :
      _p(p)
    {
    }

    eSide sideOfLine(const tLine& line) const
    {
        eSide side = _p.sideOfLine(line);
        if(side == SIDE_CENTRE)
        {
            side = SideOfLine_SI(line);
        }
        return side;
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return false;
    }
};

template <class tPointLike>
class cReverseSIOffset_PointLike : public iPointLike
{
    const tPointLike& _p;

public:

    cReverseSIOffset_PointLike()
    {
    }
  // NOTE: a reference of p is taken so p must remain valid over the lifetime of constructed object
    cReverseSIOffset_PointLike(const tPointLike& p) :
      _p(p)
    {
    }

    eSide sideOfLine(const tLine& line) const
    {
        eSide side = _p.sideOfLine(line);
        if(side == SIDE_CENTRE)
        {
            side = OtherSide(SideOfLine_SI(line));
        }
        return side;
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return false;
    }
};

#endif
