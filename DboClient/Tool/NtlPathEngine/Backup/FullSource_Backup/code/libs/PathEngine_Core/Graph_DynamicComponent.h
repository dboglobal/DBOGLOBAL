//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GRAPH__DYNAMIC_COMPONENT_INCLUDED
#define GRAPH__DYNAMIC_COMPONENT_INCLUDED

#include "interface/CollisionAgainstShapeSet.h"
#include "interface/CombinedCostCalculator.h"
#include "interface/CornerProvider.h"
#include "interface/ContextPathfindPreprocess.h"

class cGraph_DynamicComponent
{
    const cContextPathfindPreprocess& _cpp;
    const cCornerProvider& _corners;
    const cLocalisedConvexShapeSet& _shapeSet;
    const cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator>* _costCalculator;
    tPoint _boundsMin, _boundsMax;
    bool _boundsValid;
    tSigned32 _hardCorners;
    cBitVector _obstacleIsAttached;
    cBitVector _pointIsAttached;

public:

    cGraph_DynamicComponent(
            const cContextPathfindPreprocess& cpp,
            bool attachObstacles
            ) :
     _cpp(cpp),
     _corners(cpp.refCornerProvider()),
     _shapeSet(cpp.refObstaclesAndSmallConvexOverlapped()),
     _costCalculator(cpp.getCostCalculator()),
     _boundsMin(cpp._boundsMin),
     _boundsMax(cpp._boundsMax),
     _boundsValid(cpp._boundsValid),
     _hardCorners(cpp._hardCorners),
     _obstacleIsAttached(cpp.numberOfObstacles(), attachObstacles),
     _pointIsAttached(_cpp._hardCorners, attachObstacles)
    {
    }

// graph interface

	tSigned32
	points() const
	{
        return _corners.size();
	}

    void
    attachObstacle(tSigned32 obstacleIndex)
    {
	    assertD(!_obstacleIsAttached[obstacleIndex]);
	    _obstacleIsAttached.setBit(obstacleIndex);
        tSigned32 i;
        for(i = obstacleFirstCorner(obstacleIndex); i != obstacleCornerAfter(obstacleIndex); ++i)
        {
	        _pointIsAttached.setBit(i);
        }
    }

	tSigned32 obstacleFirstCorner(tSigned32 obstacle) const
	{
		return _cpp._obstacleFirstCorner[obstacle];
	}
	tSigned32 obstacleCornerAfter(tSigned32 obstacle) const
	{
		return _cpp._obstacleFirstCorner[obstacle + 1];
	}

    bool
    pointIsAttached(tSigned32 pointIndex) const
    {
        if(pointIndex >= _hardCorners)
        {
            return true;
        }
        return _pointIsAttached[pointIndex];
    }

// point and corner info lookup

    tPoint
    point(tSigned32 pointIndex) const
    {
        return _corners.cornerPoint(pointIndex);
    }
    cInternalPosition
    position(tSigned32 pointIndex) const
    {
        return cInternalPosition(_corners.cornerFace(pointIndex), _corners.cornerPoint(pointIndex));
    }
    static bool
    pointIsCorner(tSigned32 pointIndex)
    {
        return true;
    }
    tPoint
    axisBefore(tSigned32 pointIndex) const
    {
        return _corners.axisBefore(pointIndex);
    }
    tPoint
    axisAfter(tSigned32 pointIndex) const
    {
        return _corners.axisAfter(pointIndex);
    }

    tUnsigned32
    getAdditionalCost(
            cQueryContext& qc,
            const cInternalPosition& from, const tPoint& to,
            bool& blocked
            ) const
    {
        if(_costCalculator)
        {
            return static_cast<tUnsigned32>(_costCalculator->calculate(qc, from, to, blocked));
        }
        blocked = false;
        return 0;
    }

// collision

	bool
    checkCollision(
            tFace startF, const tLine& line,
            tSigned32& blockingUnattachedIndex
            ) const
    {
        if(_boundsValid && !line.end().insideOrOnBounds(_boundsMin, _boundsMax))
        {
            blockingUnattachedIndex = -1;
            return true;
        }
        return CollisionToXY_AgainstShapeSet(
                _shapeSet,
                _obstacleIsAttached, startF, line,
                blockingUnattachedIndex
                );
    }

    //bool
    //testLineCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
    //{
    //    return _lineCollision.testCollision(qc, from, to);
    //}
    bool
    testPointCollision(cQueryContext& qc, const cInternalPosition& p) const
    {
//        return _pointCollision.testPointCollision(qc, p.f, p.p);
    //... should there be bounds checking here, also?
        return _shapeSet.testPointCollision(qc, p.f, p.p);
    }
};

#endif
