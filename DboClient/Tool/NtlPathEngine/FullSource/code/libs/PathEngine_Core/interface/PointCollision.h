//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINT_COLLISION_INCLUDED
#define POINT_COLLISION_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

class cElementInFaceEntry;
class cCutRegistryEntry;
class cExternalSectionEntry;
class cQueryContext;
class cFlagVector;

// versions for use when all circuits are internal cuts

bool
TestPointCollision(
            cQueryContext& qc,
            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
            bool externalEdgesAreContained,
            tFace startFace, const tPoint& startPoint,
            cFlagVector& elementFlags
            );

bool
TestPointCollision(
            cQueryContext& qc,
            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
            bool externalEdgesAreContained,
            tFace startFace, const tPoint& startPoint
            );

bool
TestPointCollision(
            cQueryContext& qc,
            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
            bool externalEdgesAreContained,
            tFace startFace, const tIntersection& startPoint,
            cFlagVector& elementFlags            
            );

// can only guarantee to return the colliding agent when edges are not contained
//bool
//TestPointCollision_ReportAgent_ExternalEdgesAreNotContained(
//            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
//            tFace startFace, const tIntersection& startPoint, tSigned32& collidingAgent);

bool
TestPointCollision(const std::vector<std::list<cCutRegistryEntry> >& lookup,
            tFace startFace, const tIntersection& startPoint
            );

// used for dynamic obstacle circuits

//tSigned32
//CountContainingCircuits(
//        const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
//        cFlagVector& elementFlags,
//        const std::vector<std::list<cExternalSectionEntry> >& externalSectionLookup,
//        const std::vector<tSigned32>& externalFaceContainedCounts,
//        tFace startFace, const tPoint& startP);

#endif
