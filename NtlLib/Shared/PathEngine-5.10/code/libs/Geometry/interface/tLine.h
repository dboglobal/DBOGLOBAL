//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GEOMETRY_T_LINE_INCLUDED
#define GEOMETRY_T_LINE_INCLUDED

#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/Line.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h" //... why is this required here

inline eSide
SideOfLine(const tLine& line, const tPoint& p)
{
    return line.side(p);
}

#endif
