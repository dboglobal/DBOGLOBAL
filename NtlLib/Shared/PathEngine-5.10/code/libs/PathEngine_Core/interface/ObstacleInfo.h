//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef OBSTACLE_INFO_INCLUDED
#define OBSTACLE_INFO_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/InternalPosition.h"

class cConvexPoly;
class iOutputStream;

class cObstacleInfo
{
public:

    cInternalPosition _position;
    float _costToTraverse;
    tPoint _directionForCostToTraverse;

    cObstacleInfo()
    {
    }

    cObstacleInfo(
        const cInternalPosition& position,
        float costToTraverse,
        const tPoint& directionForCostToTraverse
        ) :
     _position(position),
     _costToTraverse(costToTraverse),
     _directionForCostToTraverse(directionForCostToTraverse)
    {
    }

    // shape is passed in here to avoid changing the checksum
    // (since this shape is now stored elsewhere)
    void checksum(iOutputStream& os, const cConvexPoly& shape) const;
};

#endif
