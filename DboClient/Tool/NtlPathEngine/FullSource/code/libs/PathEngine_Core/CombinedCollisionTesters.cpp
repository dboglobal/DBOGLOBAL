//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CombinedCollisionTesters.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h" //COMPILERERROR vc gave a strange error about converting a tFace to a tFace before including this

bool
cCombinedLineCollisionTester::testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    if(_target1->testCollision(qc, from, to))
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
cCombinedLineCollisionTester::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
{
    if(_target1->testCollisionToXY(qc, startF, line))
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
cCombinedLineCollisionTester::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
{
    if(_target1->testCollisionToXY(qc, startF, line, endF))
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
cCombinedLineCollisionTester::testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
{
    if(_target1->testForSnagsToSegmentEnd(qc, segmentLine, approximation))
    {
        return true;
    }
    if(_target2->testForSnagsToSegmentEnd(qc, segmentLine, approximation))
    {
        return true;
    }
    return false;
}

void
cCombinedFirstCollisionTester::firstCollision(cQueryContext& qc, tFace startF, const tLine& line, cElementCollisionInfo& info) const
{
    _target1.firstCollision(qc, startF, line, info);
    _target2.firstCollision(qc, startF, line, info);
}


bool
cCombinedPointCollisionTester::testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const
{
    if(_target1.testPointCollision(qc, f, p))
    {
        return true;
    }
    return _target2.testPointCollision(qc, f, p);
}
bool
cCombinedPointCollisionTester::testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const
{
    if(_target1.testPointCollision(qc, f, p))
    {
        return true;
    }
    return _target2.testPointCollision(qc, f, p);
}


