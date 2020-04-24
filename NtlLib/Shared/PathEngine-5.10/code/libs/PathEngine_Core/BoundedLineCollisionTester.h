//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BOUNDED_LINE_COLLISION_TESTER_INCLUDED
#define BOUNDED_LINE_COLLISION_TESTER_INCLUDED

#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"

template <class T>
class cBoundedLineCollisionTester : public iLineCollisionTester
{
    const T* _target;
    tPoint _boundsMin, _boundsMax;
    bool _boundsValid;

public:

    cBoundedLineCollisionTester(
        const T* target,
        const tPoint& boundsMin,
        const tPoint& boundsMax,
        bool boundsValid
        )
    {
        _target = target;
        _boundsMin = boundsMin;
        _boundsMax = boundsMax;
        _boundsValid = boundsValid;
    }

    bool
    testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
    {
        if(_boundsValid && !to.p.insideOrOnBounds(_boundsMin, _boundsMax))
        {
            return true;
        }
        if(_target->testCollision(qc, from, to))
        {
            return true;
        }
        return false;
    }
    bool
    testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
    {
        if(_boundsValid && !line.end().insideOrOnBounds(_boundsMin, _boundsMax))
        {
            return true;
        }
        if(_target->testCollisionToXY(qc, startF, line))
        {
            return true;
        }
        return false;
    }
    bool
    testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
    {
        if(_boundsValid && !line.end().insideOrOnBounds(_boundsMin, _boundsMax))
        {
            return true;
        }
        if(_target->testCollisionToXY(qc, startF, line, endF))
        {
            return true;
        }
        return false;
    }

    bool
    testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
    {
        if(_target->testForSnagsToSegmentEnd(qc, segmentLine, approximation))
        {
            return true;
        }
        return false;
    }
};

#endif
