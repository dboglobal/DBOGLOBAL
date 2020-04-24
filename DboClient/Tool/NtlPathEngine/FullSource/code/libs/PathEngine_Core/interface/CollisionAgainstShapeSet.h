//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COLLISION_AGAINST_SHAPE_SET_INCLUDED
#define COLLISION_AGAINST_SHAPE_SET_INCLUDED

#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "common/STL/vector.h"

class cQueryContext;
class cLocalisedConvexShapeSet;
class cLocalisedConvexShape;
class cAxisFraction;
class cInternalPosition;
class cAgent;
class cBitVector;

// used by first collision query
bool
LineCollision_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32& collidingShape,
        tSigned32& collidingLineIndex,
        cAxisFraction& distanceAlong,
        tFace& endF
        );

// used by first collision testing where start and end of test cannot be specified as points
bool
LineCollision_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32& collidingShape,
        tSigned32& collidingLineIndex,
        cAxisFraction& distanceAlong
        );

bool
TestForSnagsToSegmentEnd_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cLocalisedConvexShapeSet& shapeSet,
        const tLine& segmentLine
        );


bool
CollisionToXY_AgainstShapeSet(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& line, tFace& endF
        );

// this version is used for collision against dynamic obstacles during pathfinding
bool
CollisionToXY_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cBitVector& shapeIsAttached,
        tFace startF, const tLine& line,
        tSigned32& blockingUnattachedIndex
        );

bool
PointCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p,
        tSigned32& collidingShape
        );
bool
PointCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32 faceIndex, const tIntersection& p,
        tSigned32& collidingShape
        );
bool
PointCollision_AgainstShapeSet_IgnoringOneShape(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p,
        tSigned32 shapeToIgnore
        );

void
FirstCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& line, bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        tSigned32& collidingShape
        );
void
FirstCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF,
        const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        tSigned32& collidingShape
        );

void
LastCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& testLine, bool& collides, cAxisFraction& distanceAlong
        );
void
GetAllShapesOverlapped_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p, cBitVector& isOverlapped
        );

// only used for small convex shape set, at time of writing
// so doesn't need to support compound shape sets
// or to be fast against compount shape sets
void
GetShapesPotentiallyOverlappedBy(
        const cLocalisedConvexShapeSet& shapeSet,
        const cLocalisedConvexShape& shape, std::vector<tSigned32>& appendTo
        );

template <class tShapeSet> bool
PointCollision_AgainstShapeSet(
        const tShapeSet& shapeSet,
        tSigned32 faceIndex, const tIntersection& p,
        const cLocalisedConvexShape*& collidingShapePtr
        )
{
    typename tShapeSet::tShapesInFaceIterator it;
    shapeSet.initShapesInFaceIterator(faceIndex, it);
    const cLocalisedConvexShape* candidateShapePtr;
    while(shapeSet.nextShapeInFace(it, candidateShapePtr))
    {
        if(candidateShapePtr->poly().pointCollides2D(p))
        {
            collidingShapePtr = candidateShapePtr;
            return true;
        }
    }
    return false;
}

template <class tShapeSet> bool
PointCollision_AgainstShapeSet(
        const tShapeSet& shapeSet,
        const cInternalPosition& p
        )
{
    typename tShapeSet::tShapesInFaceIterator it;
    shapeSet.initShapesInFaceIterator(p.cell(), it);
    const cLocalisedConvexShape* candidateShapePtr;
    while(shapeSet.nextShapeInFace(it, candidateShapePtr))
    {
        if(candidateShapePtr->poly().pointCollides2D(p.p))
        {
            return true;
        }
    }
    return false;
}

#endif
