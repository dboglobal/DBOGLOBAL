//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_POINT_HAS_BEEN_INCLUDED
#define I_POINT_HAS_BEEN_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/SideEnum.h"

class iPointLike
{
public:
    virtual ~iPointLike() {};
    virtual eSide sideOfLine(const tLine& line) const = 0;
    virtual bool isEquivalentTo(const tPoint& p) const = 0;
};

inline eSide SideOfLine(const tLine& line, const iPointLike& p)
{
    return p.sideOfLine(line);
}

#endif
