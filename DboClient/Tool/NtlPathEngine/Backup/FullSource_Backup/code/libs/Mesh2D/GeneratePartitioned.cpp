//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/GeneratePartitioned.h"
#include "interface/AddInternalVertices.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "platform_common/PointMultiplication.h"

cMeshWithTrackedTransform<tMesh>*
GeneratePartitioned(
        tMesh& mesh, 
        const tMeshCircuits& circuits
        )
{
    cMeshWithTrackedTransform<tMesh>* result;
    result = new cMeshWithTrackedTransform<tMesh>(&mesh);
    AddInternalVertices(*result, circuits, 30);
    result->consolidate();
    return result;
}

cMeshWithTrackedTransform<tMesh>*
GeneratePartitioned_VerticesInsideObstructions(
        tMesh& mesh, 
        const tMeshCircuits& circuits
        )
{
    cMeshWithTrackedTransform<tMesh>* result;
    result = new cMeshWithTrackedTransform<tMesh>(&mesh);
    AddInternalVerticesInsideObstructions(
            *result,
            circuits,
            30
            );
    result->consolidate();
    return result;
}

cMeshWithTrackedTransform<tMesh>*
GeneratePartitioned(
        tMesh& mesh, 
        const std::vector<cInternalPosition>& positions,
        const std::vector<tMesh::tVertexData>& vertexData
        )
{
    cMeshWithTrackedTransform<tMesh>* result;
    result = new cMeshWithTrackedTransform<tMesh>(&mesh);
    AddInternalVertices(*result, positions, vertexData);
    result->consolidate();
    return result;
}
