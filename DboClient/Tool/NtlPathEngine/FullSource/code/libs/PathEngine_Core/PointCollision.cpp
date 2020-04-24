//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/PointCollision.h"
#include "ElementInFaceLookup.h"
#include "interface/LineCollision.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"

// if a line from the point doesnt cross any circuits before exiting then
//  the point is obstructed
// otherwise it is not
// this assumes that there are no contained or overlapping circuits
bool
TestPointCollision(
            cQueryContext& qc,
            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
            bool externalEdgesAreContained,
            tFace startFace, const tPoint& startPoint,
            cFlagVector& elementFlags
            )
{
    tLine line(startPoint, startPoint + tPoint(1, 0, 1));
    bool exitedMesh, isEntry;
    FirstElementCrossing(lookup, elementFlags, startFace, line, exitedMesh, isEntry);
    if(exitedMesh)
    {
        return externalEdgesAreContained;
    }
    return !isEntry;
}
bool
TestPointCollision(
            cQueryContext& qc,
            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
            bool externalEdgesAreContained,
            tFace startFace, const tPoint& startPoint
            )
{
    cFlagVector& elementFlags = qc.lockElementFlagVector();
    bool result = TestPointCollision(qc, lookup, externalEdgesAreContained, startFace, startPoint, elementFlags);
    qc.releaseFlagVector();
    return result;
}

bool
TestPointCollision(
            cQueryContext& qc,
            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
            bool externalEdgesAreContained,
            tFace startFace, const tIntersection& startPoint,
            cFlagVector& elementFlags
            )
{
    bool exitedMesh, isEntry;
    FirstElementCrossing(lookup, elementFlags, startFace, startPoint.refLine(), startPoint, exitedMesh, isEntry);
    if(exitedMesh)
    {
        return externalEdgesAreContained;
    }
    return !isEntry;
}


//bool
//TestPointCollision(
//            cQueryContext& qc,
//            const std::vector<std::vector<cElementInFaceEntry> >& lookup,
//            bool externalEdgesAreContained,
//            tFace startFace, const tIntersection& startPoint)
//{
//    cFlagVector& elementFlags = qc.lockElementFlagVector();
//
//    bool exitedMesh, isEntry;
//    FirstElementCrossing(lookup, elementFlags, startFace, startPoint.refLine(), startPoint, exitedMesh, isEntry);
//
//    qc.releaseFlagVector();
//
//    if(exitedMesh)
//    {
//        return externalEdgesAreContained;
//    }
//    return !isEntry;
//}

bool
TestPointCollision(const std::vector<std::list<cCutRegistryEntry> >& lookup,
            tFace startFace, const tIntersection& startPoint
            )
{
    const tLine& line = startPoint.refLine();
    tPoint::tDistance startN, startD;
    startPoint.getAxisFraction(startN, startD);
    bool enters;

    tFace f = startFace;

    // first face
    if(FirstElementCrossed_FirstFace(lookup[f.index()], line, startN, startD, enters))
    {
        return !enters;
    }

    tEdge crossed;
    bool iterate = TraverseToExit_SI<tMesh>(f, line, crossed);
    if(!iterate)
    {
        return true;
    }

    do
    {
    // face not at start or end of traversal
        if(FirstElementCrossed_MidFace(lookup[f.index()], line, enters))
        {
            return !enters;
        }
    }
    while(TraverseToExit_SI<tMesh>(f, line, crossed));

    return true;
}
