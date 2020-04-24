//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SIDE_OF_PROJECTED_TO_2D_INCLUDED
#define SIDE_OF_PROJECTED_TO_2D_INCLUDED

#include "libs/Geometry/interface/SideEnum.h"

class cDynamicPoint3;

inline int
ProjectionTypeForUpwardFacing()
{
    return 0;
}

// three vertices clockwise around a (non-zero area) 3D face
int
GetProjectionType(const cDynamicPoint3& v1, const cDynamicPoint3& v2, const cDynamicPoint3& v3);

int
GetProjectionType(const cDynamicPoint3& planeNormal);

eSide
SideOfProjectedTo2D(int projectionType, const cDynamicPoint3& v1, const cDynamicPoint3& v2, const cDynamicPoint3& v3);

#endif
