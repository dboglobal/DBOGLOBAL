//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tPoint_Header.h"

class cQueryContext;
class cConvexPoly;
class cLocalisedConvexShapeSet;
class cCompoundContextCollisionPreprocess;
class cInternalPosition;
class cAgent;

bool
TestPointCollision_NoPreprocess(
        cQueryContext& qc,
        const cConvexPoly& shape,
        const cLocalisedConvexShapeSet* baseObstacles,
        const cCompoundContextCollisionPreprocess* contextObstacles,
        const cInternalPosition& p,
        const cAgent* contextAgentToIgnore
        );

bool
TestLineCollision_NoPreprocess(
        cQueryContext& qc,
        const cConvexPoly& shape,
        const cLocalisedConvexShapeSet* baseObstacles,
        const cCompoundContextCollisionPreprocess* contextObstacles,
        const cInternalPosition& start,
        const cInternalPosition& end,
        const cAgent* contextAgentToIgnore
        );

bool
TestLineCollision_XY_NoPreprocess(
        cQueryContext& qc,
        const cConvexPoly& shape,
        const cLocalisedConvexShapeSet* baseObstacles,
        const cCompoundContextCollisionPreprocess* contextObstacles,
        const cInternalPosition& start,
        const tPoint& targetP,
        tFace& endF,
        const cAgent* contextAgentToIgnore
        );
