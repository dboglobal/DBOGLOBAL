//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FACES_CONTAINED_BY_BOUNDARY_INCLUDED
#define FACES_CONTAINED_BY_BOUNDARY_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

// boundary shape is assumed to be convex

class cFacesContainedByBoundary
{
    cBitVector _edgeEncountered;
    std::vector<tEdge> _topLevelBoundary;
    std::vector<tFace> _result;

    void
    processInsideEdges(
            tEdge eBefore,
            tEdge eAfter,
            std::vector<tEdge>& boundary
            );
    void
    processInsideEdges(
            tEdge boundaryEdge1, tEdge boundaryEdge2,
            tEdge& firstE, tEdge& previousE,
            std::vector<tEdge>& boundary
            );

    void processLoop(const std::vector<tEdge>& boundary, tSigned32 beginI);
    void edgeCrossed(std::vector<tEdge>& boundary, tEdge e);

public:

    cFacesContainedByBoundary(tSigned32 edgesInMesh);

//    bool empty() const;
//    void clear();

    void edgeCrossed(tEdge e);
    void closeAndTakeResult(std::vector<tFace>& swapOutTo);
};

#endif
