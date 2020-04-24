//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COUNTING_COLLISION_TESTER_INCLUDED
#define COUNTING_COLLISION_TESTER_INCLUDED
#define I_COLLISIONTESTER_INCLUDED

#include "interface/iCollisionTester.h"

class cCountingLineCollisionTester : public iLineCollisionTester
{
    const iLineCollisionTester& _base;
    mutable tSigned32 _count;

public:

    cCountingLineCollisionTester(const iLineCollisionTester& base) :
      _base(base)
    {
        _count = 0;
    }

    void
    reset()
    {
        _count = 0;
    }

    bool
    testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
    {
        ++_count;
        return _base.testCollision(qc, from, to);
    }
    bool
    testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
    {
        ++_count;
        return _base.testCollisionToXY(qc, startF, line);
    }
    bool
    testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
    {
        ++_count;
        return _base.testCollisionToXY(qc, startF, line, endF);
    }
    bool    
    testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
    {
        ++_count;
        return _base.testForSnagsToSegmentEnd(qc, segmentLine, approximation);
    }
};

#endif

