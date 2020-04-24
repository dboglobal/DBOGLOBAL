//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMBINE_CIRCUITS_INCLUDED
#define COMBINE_CIRCUITS_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "common/STL/vector.h"

template <class T> class cMeshWithTrackedTransform;
class cQueryContext;
class cObstacleInfo;
class cConvexPoly;
class cLocalisedConvexShapeSet;

//.... to go
void
CombineCircuits_WithInternalVertices_KeepPartitioned(
        cQueryContext& qc,
        tMesh& mesh, 
        tMeshCircuits& edgeExpansionPieces,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expansionShape,
        tMeshCircuits& result,
        cMeshWithTrackedTransform<tMesh>*& partitioned
        );

//.... to go
void
CombineCircuits_WithInternalVertices(
        cQueryContext& qc,
        tMesh& mesh, 
        tMeshCircuits& edgeExpansionPieces,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expansionShape,
        tMeshCircuits& result
        );

// edgeExpansion is destroyed during this call
void
ConnectBaseObstacleCircuits(
        cQueryContext& qc,
        tMeshCutSequence& edgeExpansion,
        const cLocalisedConvexShapeSet& baseObstacleExpansions,
        tMeshCircuits& result,
        bool keepPartitioned,
        bool justGeneratePartitioned,
        cMeshWithTrackedTransform<tMesh>*& partitioned
        );

#endif
