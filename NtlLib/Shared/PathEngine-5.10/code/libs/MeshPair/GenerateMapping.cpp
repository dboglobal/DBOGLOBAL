//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/GenerateMapping.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/Delaunay.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "libs/ParseXML/interface/ParsedXMLElement.h"
#include "common/TemplateMesh/ElementIndexing.h"

tMesh*
GenerateIdentityMapping(tMesh_3D& mesh3D)
{
    tMesh* result = new tMesh;
    result->copyStructure(mesh3D);
    tSigned32 i;
    for(i = 0; i != mesh3D.faces(); ++i)
    {
        if(mesh3D.faceDataAt(i).isExternal())
        {
            result->faceDataAt(i) = tMesh::tFaceData::externalFace();
        }
    }
    for(i = 0; i != mesh3D.vertices(); ++i)
    {
        result->vertDataAt(i)._p = GetVertexPoint(mesh3D.vertAt(i));
        result->vertDataAt(i).setElementOn3DIndex(ElementIndexForTypeAndSubIndex(bMeshElement::VERTEX, i));
    }
    return result;
}

void
OptimiseMapping(tMesh& mapping)
{
    RemoveInternalVertices(mapping);
    RemoveInlineEdgeVertices(mapping, cNullVertexFilter<tVertex>());
    cDelaunayFilter_NoEdgesOnExternalFaces<tMesh> filter;
    Delaunay(mapping, filter);
    SimplifyMesh(mapping, filter);
}
