//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GRAPH__NO_CONTEXT_DYNAMIC_COMPONENT_INCLUDED
#define GRAPH__NO_CONTEXT_DYNAMIC_COMPONENT_INCLUDED

class cGraph_NoContextDynamicComponent
{
public:

    cGraph_NoContextDynamicComponent()
    {
    }

// graph interface

	static tSigned32
	points()
	{
        return 0;
	}

// point and corner info lookup

    static tPoint
    point(tSigned32 pointIndex)
    {
        invalid();
        return tPoint();
    }

    static void
    attachObstacle(tSigned32 obstacleIndex)
    {
        invalid();
    }
	static tSigned32
    obstacleFirstCorner(tSigned32 obstacle)
	{
        invalid();
        return 0;
	}
	static tSigned32
    obstacleCornerAfter(tSigned32 obstacle)
	{
        invalid();
		return 0;
	}

    static bool
    pointIsAttached(tSigned32 pointIndex)
    {
        invalid();
        return false;
    }

    static cInternalPosition
    position(tSigned32 pointIndex)
    {
        invalid();
        return cInternalPosition();
    }
    static bool
    pointIsCorner(tSigned32 pointIndex)
    {
        invalid();
        return false;
    }
    static tPoint
    axisBefore(tSigned32 pointIndex)
    {
        invalid();
        return tPoint();
    }
    static tPoint
    axisAfter(tSigned32 pointIndex)
    {
        invalid();
        return tPoint();
    }

    static tUnsigned32
    getAdditionalCost(
            cQueryContext& qc,
            const cInternalPosition& from, const tPoint& to,
            bool& blocked
            )
    {
        blocked = false;
        return 0;
    }

// collision

	static bool
    checkCollision(
            tFace startF, const tLine& line,
            tSigned32& blockingUnattachedIndex
            )
    {
        return false;
    }
    static bool
    testPointCollision(cQueryContext& qc, const cInternalPosition& p)
    {
        return false;
    }
};

#endif
