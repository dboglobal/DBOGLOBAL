//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMBINED_COLLISION_TESTERS_INCLUDED
#define COMBINED_COLLISION_TESTERS_INCLUDED

#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "libs/PathEngine_Core/interface/iFirstCollisionTester.h"

class cCombinedLineCollisionTester : public iLineCollisionTester
{
    const iLineCollisionTester* _target1;
    const iLineCollisionTester* _target2;

public:

    void setTargets(const iLineCollisionTester* target1, const iLineCollisionTester* target2)
    {
        _target1 = target1;
        _target2 = target2;
    }

    bool testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const;
    bool testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const;
};

class cCombinedFirstCollisionTester : public iFirstCollisionTester
{
    const iFirstCollisionTester& _target1;
    const iFirstCollisionTester& _target2;

public:

    cCombinedFirstCollisionTester(const iFirstCollisionTester& target1, const iFirstCollisionTester& target2) :
      _target1(target1),
      _target2(target2)
    {
    }

    void firstCollision(cQueryContext& qc, tFace startF, const tLine& line, cElementCollisionInfo& info) const;
};

class cCombinedPointCollisionTester : public iPointCollisionTester
{
    const iPointCollisionTester& _target1;
    const iPointCollisionTester& _target2;

public:

    cCombinedPointCollisionTester(const iPointCollisionTester& target1, const iPointCollisionTester& target2) :
      _target1(target1),
      _target2(target2)
    {
    }

    bool testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const;
    bool testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const;
};

#endif
