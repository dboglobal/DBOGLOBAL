//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLANE_INCLUDED
#define PLANE_INCLUDED

#include "DynamicPoint3.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "libs/Geometry/interface/tPoint.h"

class cPlane
{
public:

    tPoint3 _origin;
    cDynamicPoint3 _normal;
};

#endif
