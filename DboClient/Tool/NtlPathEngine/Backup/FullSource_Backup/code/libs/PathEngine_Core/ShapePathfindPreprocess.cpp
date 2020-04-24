//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SHAPE_PATHFIND_PREPROCESS_INCLUDED
#define SHAPE_PATHFIND_PREPROCESS_INCLUDED

#include "platform_common/Header.h"
#include "interface/ShapePathfindPreprocess.h"
#include "interface/PathfindPreprocess.h"
#include "interface/ShapeCollisionPreprocess.h"

bool
cShapePathfindPreprocess::isValidFor(const cInternalPosition& baseShapePosition) const
{
    return _preprocessedPosition == baseShapePosition;
}

void
cShapePathfindPreprocess::initOrReinit(
            const cInternalPosition& baseShapePosition,
            const cPathfindPreprocess& pp,
            const cShapeCollisionPreprocess& scp
            )
{
    _preprocessedPosition = baseShapePosition;
    _smallConvexOverlapped.clear();
    pp.getSmallConvexPotentiallyOverlapped(*scp.getExpansion2D(), _smallConvexOverlapped);
}

#endif
