//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LINE_COLLISION_INCLUDED
#define LINE_COLLISION_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

class cElementInFaceEntry;
class cCutRegistryEntry;
class cCircuitElement;
class cAgent;
class cElementCollisionInfo;
class cFlagVector;
class cEdgesTraversal;
class cAxisFraction;

bool
TestLineCollisionToXY(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cBitVector& elementBitBuffer,
    tFace startF, const tLine& testLine, tFace& endF);

//... get rid of this and change stuff to use FaceAtEndOfLine() from MeshTraversal_Common.h instead
bool
TestTraversalToXY(
    tFace startF, const tLine& line, tFace& endF);

void
GetTraversalOut(
        const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
        cFlagVector& elementFlags,
        tFace startFace, const tLine& line,
        cEdgesTraversal& traversal,
        cElementCollisionInfo& info,
        eSide bias = SIDE_CENTRE);

// these test collision to x,y at end of line, and set endF from the traversal if no collision

void
FirstLineCollision(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startF, const tLine& line, tFace& endF,
    cElementCollisionInfo& info);
void
FirstLineCollision(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startF,
    const tLine& line,
    const cAxisFraction& startFraction,
    const cAxisFraction& endFraction,
    tFace& endF,
    cElementCollisionInfo& info);

void
LastLineCollision(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startF, const tLine& line, tFace& endF, 
    cElementCollisionInfo& info);

void
FirstElementCrossing(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startFace, const tLine& line, // this line is treated as a halfline
    bool& exitedMesh, bool& isEntry);

void
FirstElementCrossing(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startFace, const tLine& line, const tIntersection& startPointOnLine,  // this line is treated as a halfline starting at the specified point
    bool& exitedMesh, bool& isEntry);

bool
FirstElementCrossed_FirstFace(
    const std::list<cCutRegistryEntry>& elements,
    const tLine& line,
    const tPoint::tDistance& startN,
    const tPoint::tDistance& startD,
    bool& enters);

bool
FirstElementCrossed_MidFace(
    const std::list<cCutRegistryEntry>& elements,
    const tLine& line,
    bool& enters);

bool ExitsThroughExternalSection(
                        tEdge exit,
                        const tLine& exitLine,
                        const tMeshCutSequence& circuits
                        );
bool ExitsThroughExternalSection(
                        tEdge exit,
                        const tLine& exitLine,
                        const tMeshCircuits& circuits
                        );

bool
TestForSnagsToSegmentEnd(
        const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
        cFlagVector& elementFlags,
        tFace startF, const tLine& line,
        const tLine& segmentLine
        );
#endif
