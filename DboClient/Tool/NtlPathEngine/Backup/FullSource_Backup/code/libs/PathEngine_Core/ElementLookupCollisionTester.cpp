//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ElementLookupCollisionTester.h"
#include "ElementInFaceLookup.h"
#include "ElementCollisionInfo.h"
#include "interface/LineCollision.h"
#include "interface/PointCollision.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/InternalPosition.h"

cElementLookupCollisionTester::cElementLookupCollisionTester(
    const std::vector<std::vector<cElementInFaceEntry> >& lookup,
    tSigned32 elementCount
    ) :
    _lookup(lookup)
{
}

bool
cElementLookupCollisionTester::testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const
{
    cFlagVector& flagVector = qc.lockElementFlagVector();
    bool result = TestPointCollision(qc, _lookup, true, f, p, flagVector);
    qc.releaseFlagVector();
    return result;
}
bool
cElementLookupCollisionTester::testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const
{
    cFlagVector& flagVector = qc.lockElementFlagVector();
    bool result = TestPointCollision(qc, _lookup, true, f, p, flagVector);
    qc.releaseFlagVector();
    return result;
}

bool
cElementLookupCollisionTester::testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    if(from == to)
    {
        return false;
    }
    tLine line(from.p, to.p);
    tFace endF;
    cBitVector& bitBuffer = qc.lockElementBitBuffer();
    bool collides = TestLineCollisionToXY(_lookup, bitBuffer, from.f, line, endF);
    qc.releaseElementBitBuffer();
    if(collides)
    {
        return true;
    }
    return endF != to.f;
}

bool
cElementLookupCollisionTester::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
{
    if(line.start() == line.end())
    {
        return false;
    }
    tFace endF_Ignored;
    cBitVector& bitBuffer = qc.lockElementBitBuffer();
    bool result = TestLineCollisionToXY(_lookup, bitBuffer, startF, line, endF_Ignored);
    qc.releaseElementBitBuffer();
    return result;
}
bool
cElementLookupCollisionTester::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
{
    if(line.start() == line.end())
    {
        return false;
    }
    cBitVector& bitBuffer = qc.lockElementBitBuffer();
    bool result = TestLineCollisionToXY(_lookup, bitBuffer, startF, line, endF);
    qc.releaseElementBitBuffer();
    return result;
}

bool
cElementLookupCollisionTester::testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
{
    tLine line(approximation.p, segmentLine.end());
    assertD(line.start() != line.end());
    cFlagVector& flagVector = qc.lockElementFlagVector();
    bool result = TestForSnagsToSegmentEnd(_lookup, flagVector, approximation.f, line, segmentLine);
    qc.releaseFlagVector();
    return result;
}


void
cElementLookupCollisionTester::firstCollision(cQueryContext& qc, tFace startF, const tLine& line, cElementCollisionInfo& info) const
{
    if(line.start() == line.end())
    {
        return;
    }
    tFace endF_Ignored;
    cFlagVector& flagVector = qc.lockElementFlagVector();
    FirstLineCollision(_lookup, flagVector, startF, line, endF_Ignored, info);
    qc.releaseFlagVector();
}
