//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/Containers/FlagVector.h"
#include "common/STL_Helpers.h"

void
cLocalisedConvexShapeSet::initShapesInFaceIterator(tSigned32 faceIndex, tShapesInFaceIterator& it)
{
    it._faceIndex = faceIndex;
    it._indexInFace = 0;
}
bool
cLocalisedConvexShapeSet::nextShapeInFace(tShapesInFaceIterator& it, tSigned32& index, const cLocalisedConvexShape*& shapePtr) const
{
    if(it._indexInFace < shapesInFace(it._faceIndex))
    {
        index = indexForShapeInFace(it._faceIndex, it._indexInFace);
        shapePtr = _expansions[index];
        ++it._indexInFace;
        return true;
    }
    else
    {
        return false;
    }
}
bool
cLocalisedConvexShapeSet::nextShapeInFace(tShapesInFaceIterator& it, const cLocalisedConvexShape*& shapePtr) const
{
    if(it._indexInFace < shapesInFace(it._faceIndex))
    {
        shapePtr = _expansions[indexForShapeInFace(it._faceIndex, it._indexInFace)];
        ++it._indexInFace;
        return true;
    }
    else
    {
        return false;
    }
}

cLocalisedConvexShapeSet::cLocalisedConvexShapeSet(tSigned32 facesInMesh)
{
    _facesInMesh = facesInMesh;
    _setCompletedCalled = false;
}

void
cLocalisedConvexShapeSet::clear()
{
    _expansions.clear();
    _expansionLookup.clear();
    _setCompletedCalled = false;
}

void
cLocalisedConvexShapeSet::deleteAllAndClear()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_expansions); ++i)
    {
        delete _expansions[i];
    }
    clear();
}

void
cLocalisedConvexShapeSet::push_back(const cLocalisedConvexShape* shape)
{
    assertD(!_setCompletedCalled);
    _expansions.push_back(shape);
}

void
cLocalisedConvexShapeSet::setCompleted()
{
    std::vector<tSigned32> entriesPerFace(_facesInMesh, 0);

    tSigned32 i;
    for(i = 0; i != SizeL(_expansions); ++i)
    {
        const std::vector<tSigned32>& overlapped = _expansions[i]->refFacesOverlapped();
        tSigned32 j;
        for(j = 0; j != SizeL(overlapped); ++j)
        {
            entriesPerFace[overlapped[j]]++;
        }
    }

    _expansionLookup.reconstruct(entriesPerFace);

    for(i = 0; i != SizeL(_expansions); ++i)
    {
        const std::vector<tSigned32>& overlapped = _expansions[i]->refFacesOverlapped();
        tSigned32 j;
        for(j = 0; j != SizeL(overlapped); ++j)
        {
            tSigned32 faceIndex = overlapped[j];
            tSigned32 entryInFace = --entriesPerFace[faceIndex];
            _expansionLookup[faceIndex][entryInFace] = i;
        }
    }

    _setCompletedCalled = true;
}

void
cLocalisedConvexShapeSet::shrinkToFit()
{
    ShrinkToFit(_expansions);
    _expansionLookup.shrinkToFit();
}

// interface for iPointCollisionTester

bool
cLocalisedConvexShapeSet::testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const
{
    return PointCollision_AgainstShapeSet(*this, cInternalPosition(f, p));
}
bool
cLocalisedConvexShapeSet::testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const
{
    tSigned32 collidingShape_Ignored;
    return PointCollision_AgainstShapeSet(*this, f.index(), p, collidingShape_Ignored);
}

// interface for iLineCollisionTester

bool
cLocalisedConvexShapeSet::testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    assertD(_setCompletedCalled);

    if(from == to)
    {
        return false;
    }
    if(_expansions.empty())
    {
        return false;
    }
    tLine line(from.p, to.p);
    tFace endF;
    bool collided = CollisionToXY_AgainstShapeSet(
            qc,
            *this,
            from.f, line,
            endF
            );
    return collided || endF != to.f;
}

bool
cLocalisedConvexShapeSet::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
{
    assertD(_setCompletedCalled);

    if(line.start() == line.end())
    {
        return false;
    }
    if(_expansions.empty())
    {
        return false;
    }
    tFace endF_Ignored;
    return CollisionToXY_AgainstShapeSet(
            qc,
            *this,
            startF, line,
            endF_Ignored
            );
}
bool
cLocalisedConvexShapeSet::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
{
    assertD(_setCompletedCalled);
    if(line.start() == line.end())
    {
        endF = startF;
        return false;
    }
//.. must fill in the end face in this case
//.. but could do this with a traversal
//    if(_expansions.empty())
//    {
//        return false;
//    }
    return CollisionToXY_AgainstShapeSet(
            qc,
            *this,
            startF, line,
            endF
            );
}

bool
cLocalisedConvexShapeSet::testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
{
    assertD(_setCompletedCalled);
    tLine line(approximation.p, segmentLine.end());
    assertD(line.start() != line.end());
    return TestForSnagsToSegmentEnd_AgainstShapeSet(
            approximation.f, line,
            *this,
            segmentLine
            );
}
