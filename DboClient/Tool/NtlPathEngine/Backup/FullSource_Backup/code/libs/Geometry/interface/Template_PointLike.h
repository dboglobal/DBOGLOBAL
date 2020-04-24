//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TEMPLATE_POINTLIKE_HAS_BEEN_INCLUDED
#define TEMPLATE_POINTLIKE_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/iPointLike.h"

template <class tPointLike>
class cTemplate_PointLike : public iPointLike
{
    const tPointLike& _p;

public:

    cTemplate_PointLike()
    {
    }
  // NOTE: a reference of p is taken so p must remain valid over the lifetime of constructed object
    cTemplate_PointLike(const tPointLike& p) :
      _p(p)
    {
    }

    eSide sideOfLine(const tLine& line) const
    {
        return _p.sideOfLine(line);
    }
    bool isEquivalentTo(const tPoint& p) const
    {
        return _p.isEquivalentTo(p);
    }
};

#endif
