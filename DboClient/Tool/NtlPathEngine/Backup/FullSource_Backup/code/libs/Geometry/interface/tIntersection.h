//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GEOMETRY_T_INTERSECTION_INCLUDED
#define GEOMETRY_T_INTERSECTION_INCLUDED

#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Geometry/interface/Intersection2.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"

inline eSide
SideOfLine(const tLine& line, const tIntersection& p)
{
    return line.side(p);
}

#endif
