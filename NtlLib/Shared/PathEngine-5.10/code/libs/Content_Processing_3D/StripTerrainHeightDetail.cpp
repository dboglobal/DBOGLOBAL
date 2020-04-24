//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "StripTerrainHeightDetail.h"
#include "tMesh_Dynamic.h"
#include "SourceDataLookup.h"
#include "TriangulateDynamic.h"
#include "SideOfProjectedTo2D.h"
#include "i_pathengine.h"

typedef tMesh_Dynamic tMesh;
typedef tMesh::vertex tVertex;
typedef tMesh::edge tEdge;

static bool
IsDetailVertex(tVertex v, tSigned32 numberOfTerrainLayers, const cSourceDataLookup& lookup)
{
    if(!v.isInternalVertex())
    {
        return false;
    }
    tEdge e = tMesh::firstEdge(v);
    const tSigned32 terrainLayer = lookup.faceAttribute(e.face()->_operandIndex, PE_FaceAttribute_SectionID);
    if(terrainLayer < 0 || terrainLayer >= numberOfTerrainLayers)
    {
        return false;
    }
    do
    {
        if(!lookup.faceAttributesMatch(e.face()->_operandIndex, e.twin().face()->_operandIndex))
        {
            return false;
        }
    }
    while(e.toNextAroundVertex());
    return true;
}

void
StripTerrainHeightDetail(tMesh_Dynamic& mesh, const cBSPVertices& vertices, tSigned32 numberOfTerrainLayers, const cSourceDataLookup& lookup)
{
    tVertex v = mesh.beginVertex();
    while(v.valid())
    {
        tVertex candidate = v;
        ++v;
        if(IsDetailVertex(candidate, numberOfTerrainLayers, lookup))
        {
            RemoveInternalVertex_Dynamic(mesh, candidate, vertices, ProjectionTypeForUpwardFacing());
        }
    }
    mesh.consolidate();
}
