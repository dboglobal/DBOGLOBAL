//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef STANDARD_IOTAS_INCLUDED
#define STANDARD_IOTAS_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/interface/Assert.h"

inline eSide
SideOfLine_SI(const tLine& l)
{
    eSide result = l.axis().side(tPoint(1, 0, 1));
    if(result == SIDE_CENTRE)
    {
        result = l.axis().side(tPoint(0, 1, 1));
        assertD(result != SIDE_CENTRE);
    }
    return result;
}
inline eSide
SideOfAxis_SI(const tPoint& axis)
{
    eSide result = axis.side(tPoint(1, 0, 1));
    if(result == SIDE_CENTRE)
    {
        result = axis.side(tPoint(0, 1, 1));
        assertD(result != SIDE_CENTRE);
    }
    return result;
}

#endif
