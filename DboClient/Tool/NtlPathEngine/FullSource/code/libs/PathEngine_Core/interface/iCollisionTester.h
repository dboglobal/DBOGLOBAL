//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_COLLISIONTESTER_INCLUDED
#define I_COLLISIONTESTER_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"

class cInternalPosition;
class cQueryContext;

class iLineCollisionTester
{
public:

    virtual ~iLineCollisionTester() {}
    virtual bool testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const = 0;
    virtual bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const = 0;
    virtual bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const = 0;
    virtual bool testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const = 0;
};

class cNullLineCollisionTester : public iLineCollisionTester
{
public:

    bool testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
    {
        return false;
    }
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
    {
        return false;
    }
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
    {
        invalid(); // no way to fill in endF properly
        endF = startF;
        return false;
    }
    bool testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
    {
        return false;
    }
};

class iPointCollisionTester
{
public:
    virtual ~iPointCollisionTester() {}
    virtual bool testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const=0;
    virtual bool testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const=0;
};

class cNullPointCollisionTester
{
public:

    bool testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const
    {
        return false;
    }
    bool testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const
    {
        return false;
    }
};

#endif //include guard
