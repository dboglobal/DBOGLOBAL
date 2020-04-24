//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef STATIC_LINE_COLLISION_TESTER_INCLUDED
#define STATIC_LINE_COLLISION_TESTER_INCLUDED

#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "ElementLookupCollisionTester.h"
#include "interface/LocalisedConvexShapeset.h"

class cStaticLineCollisionTester : public iLineCollisionTester
{
    cElementLookupCollisionTester _target1;
    const cLocalisedConvexShapeSet* _target2;

public:

    cStaticLineCollisionTester(
            cElementLookupCollisionTester target1,
            const cLocalisedConvexShapeSet* target2) :
     _target1(target1),
     _target2(target2)
    {
    }
    
    bool
    testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
    {
        if(_target1.testCollision(qc, from, to))
        {
            return true;
        }
        if(_target2->testCollision(qc, from, to))
        {
            return true;
        }
        return false;
    }
    bool
    testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
    {
        if(_target1.testCollisionToXY(qc, startF, line))
        {
            return true;
        }
        if(_target2->testCollisionToXY(qc, startF, line))
        {
            return true;
        }
        return false;
    }
    bool
    testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
    {
        if(_target1.testCollisionToXY(qc, startF, line, endF))
        {
            return true;
        }
        if(_target2->testCollisionToXY(qc, startF, line))
        {
            return true;
        }
        return false;
    }
    bool
    testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
    {
        if(_target1.testForSnagsToSegmentEnd(qc, segmentLine, approximation))
        {
            return true;
        }
        if(_target2->testForSnagsToSegmentEnd(qc, segmentLine, approximation))
        {
            return true;
        }
        return false;
    }
};

#endif
