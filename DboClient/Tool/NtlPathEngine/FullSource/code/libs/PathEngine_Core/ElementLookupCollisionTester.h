//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ELEMENT_LOOKUP_COLLISION_TESTER_INCLUDED
#define ELEMENT_LOOKUP_COLLISION_TESTER_INCLUDED

#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "libs/PathEngine_Core/interface/iFirstCollisionTester.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/vector.h"

class cElementInFaceEntry;
class cQueryContext;

class cElementLookupCollisionTester : public iPointCollisionTester, public iLineCollisionTester, public iFirstCollisionTester
{
    const std::vector<std::vector<cElementInFaceEntry> >& _lookup;

public:

    cElementLookupCollisionTester(
        const std::vector<std::vector<cElementInFaceEntry> >& lookup,
        tSigned32 elementCount
        );

// interface for iPointCollisionTester

    bool testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const;
    bool testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const;

// interface for iLineCollisionTester

    bool testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const;
    bool testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const;

// interface for iFirstCollisionTester

    void firstCollision(cQueryContext& qc, tFace startF, const tLine& line, cElementCollisionInfo& info) const;
};

#endif
