//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "common/STL/vector.h"

template <class T> class cMeshWithTrackedTransform;
class cObstacleInfo;
class cInternalPosition;

cMeshWithTrackedTransform<tMesh>*
GeneratePartitioned(
        tMesh& mesh,
        const tMeshCircuits& circuits
        );

cMeshWithTrackedTransform<tMesh>*
GeneratePartitioned_VerticesInsideObstructions(
        tMesh& mesh, 
        const tMeshCircuits& circuits
        );

cMeshWithTrackedTransform<tMesh>*
GeneratePartitioned(
        tMesh& mesh, 
        const std::vector<cInternalPosition>& positions,
        const std::vector<tMesh::tVertexData>& vertexData
        );
