
//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ShapesCostCalculator.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "platform_common/SquareRoot.h"
#include "common/Containers/BitVector.h"
#include "common/Containers/SwapOut.h"

static tFace
ResolveFaceForIntersection_DoesntExit(tFace f,
                const tLine& traverseLine,
                const tLine& intersectLine)
{
    cPoint_PointLike target(traverseLine.end());
    tEdge crossed;
    while(TraverseToTarget_SI<tMesh>(f, traverseLine, target, crossed))
    {
        eSide side = traverseLine.intersectionOrder_Anticlockwise(intersectLine, GetEdgeLine(crossed.twin()));
        if(EdgeHasPriority_SI(crossed))
        {
            if(side != SIDE_RIGHT)
            {
                return crossed.face();;
            }
        }
        else
        {
            if(side == SIDE_LEFT)
            {
                return crossed.face();
            }
        }
    }
    return f;
}

float
cShapesCostCalculator::checkExitAndCalculateCost(
                        const cInternalPosition& from, const tPoint& to,
                        const tLine& segmentLineReversed,
                        float segmentDistance,
                        float startFraction,
                        tSigned32 i                        
                        ) const
{
    float distanceInDirection;
    if(_directionLengths[i] != 0.f)
    {
        tLine l(from.p, from.p + _directions[i].right90());
        if(l.side(to) != SIDE_LEFT)
        {
        // travelling away from (or at right angles to) direction for cost
            return 0.f;
        }
        tPoint travelDirection = to - from.p;
        distanceInDirection = tMultiplier::asFloat(travelDirection.dotProduct(_directions[i]));
        distanceInDirection /= _directionLengths[i];
    }
    else
    {
    // no direction associated with this cost
        distanceInDirection = segmentDistance;
    }

    float endFraction = 1.f;
    tSigned32 collidingLineIndex;
    if(_shapeSet[i].poly().lineCollides2D(segmentLineReversed.start(), segmentLineReversed.axis(), collidingLineIndex))
    {
      // get distance to exit
        tLine intersectLine = _shapeSet[i].poly().getBoundaryLine(collidingLineIndex);
        cAxisFraction fraction(segmentLineReversed, intersectLine);
        endFraction -= fraction.asFloat();
    }

    assertD(endFraction >= startFraction);
    return distanceInDirection * (endFraction - startFraction) * _costs[i];
}

void
cShapesCostCalculator::push_back(const cLocalisedConvexShape* shape, float cost, const tPoint& direction)
{
    assertD(cost > 0.f);
    _shapeSet.push_back(shape);
    _costs.push_back(cost);
    _directions.push_back(direction);
    float directionLength = 0.f;
    if(direction != tPoint(0,0,0))
    {
        float directionLengthSquared = tMultiplier::asFloat(direction.lengthSquared());
        directionLength = SquareRoot(directionLengthSquared);
        assertD(directionLength > 0.f); // minimum length for a non-zero vector is 1 unit
    }
    _directionLengths.push_back(directionLength);
}

//void
//cShapesCostCalculator::swapOut(tSigned32 index)
//{
//    _shapeSet.swapOut(index);
//    SwapOut(_costs, index);
//    SwapOut(_directions, index);
//    SwapOut(_directionLengths, index);
//}

void
cShapesCostCalculator::setCompleted()
{
    _shapeSet.setCompleted();
}

float
cShapesCostCalculator::calculate(
        cQueryContext& qc,
        const cInternalPosition& from, const tPoint& to,
        bool& blocked
        ) const
{
    blocked = false;
    float result = 0.f;
    if(from.p == to)
    {
        return result;
    }

    cBitVector isOverlapped(_shapeSet.size(), false);
    GetAllShapesOverlapped_AgainstShapeSet(_shapeSet, from, isOverlapped);

    tLine segmentLineReversed(to, from.p);
    float segmentDistance = SquareRoot(tMultiplier::asFloat(segmentLineReversed.axis().lengthSquared()));

// for overlapped agents, we need only check if the line exits the shape in 2d
// and add costs for each overlapped agent accordingly

    tSigned32 i;
    for(i = 0; i < _shapeSet.size(); ++i)
    {
        if(!isOverlapped[i])
        {
            continue;
        }
        result += checkExitAndCalculateCost(from, to, segmentLineReversed, segmentDistance, 0.f, i);
    }

// costs for shapes entered partway along the segment

    cBitVector& shapeChecked = isOverlapped; // shapes that overlap the start point cant be entered
    tLine segmentLine(from.p, to);
    cPoint_PointLike target(to);

    tFace f = from.f;
    tEdge edgeCrossed;
    do
    {
        tSigned32 i;
        for(i = 0; i < _shapeSet.shapesInFace(f.index()); ++i)
        {
            tSigned32 shapeIndex = _shapeSet.indexForShapeInFace(f.index(), i);
            if(shapeChecked[shapeIndex])
            {
                continue;
            }
            shapeChecked.setBit(shapeIndex, true);

            tSigned32 collidingLineIndex;
            if(!_shapeSet[shapeIndex].poly().lineCollides2D(segmentLine.start(), segmentLine.axis(), collidingLineIndex))
            {
                continue;
            }
            
            tLine intersectLine = _shapeSet[shapeIndex].poly().getBoundaryLine(collidingLineIndex);            
            tFace intersectF = ResolveFaceForIntersection_DoesntExit(from.f, segmentLine, intersectLine);
            if(!_shapeSet[shapeIndex].overlapsFace(intersectF.index()))
            {
                continue;
            }

            cAxisFraction startFraction(segmentLine, intersectLine);
            result += checkExitAndCalculateCost(from, to, segmentLineReversed, segmentDistance, startFraction.asFloat(), shapeIndex);
        }
    }
    while(TraverseToTarget_SI<tMesh>(f, segmentLine, target, edgeCrossed));


    return result;
}
