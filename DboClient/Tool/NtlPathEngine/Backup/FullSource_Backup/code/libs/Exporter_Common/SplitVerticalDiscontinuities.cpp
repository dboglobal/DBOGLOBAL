//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SplitVerticalDiscontinuities.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/STL/vector.h"

void
SplitVerticalDiscontinuities(tMesh_3D_FaceTracked& mesh)
{
    std::vector<tEdge_3D_FaceTracked> splitWithTri;
    std::vector<tEdge_3D_FaceTracked> splitWithQuad;
    tEdge_3D_FaceTracked e = mesh.beginEdge();
    do
    {
        if(e.face()->isExternal() || !e.twin().valid() || e.twin().face()->isExternal())
        {
            continue;
        }
        if(e->getZ() == e.twin().next()->getZ())
        {
            continue;
        }
        if(e.next()->getZ() == e.twin()->getZ())
        {
            splitWithTri.push_back(e);
            continue;
        }
        if(e->getZ() > e.twin().next()->getZ())
        {
            continue;
        }
        assertD(e.next()->getZ() < e.twin()->getZ());
        splitWithQuad.push_back(e);
    }
    while(e.toNextInMesh());

    tSigned32 i;
    for(i = 0; i < SizeL(splitWithTri); ++i)
    {
        tEdge_3D_FaceTracked e1 = splitWithTri[i];
        tEdge_3D_FaceTracked e2 = e1.twin();
        mesh.breakEdge(e1);
        tMesh_3D_FaceTracked::tEdgeData edgeData[3];
        tMesh_3D_FaceTracked::tVertexData vertexData[3];

        vertexData[0] = *e1.next().vertex();
        vertexData[1] = *e1.vertex();
        vertexData[2] = *e2.next().vertex();
        edgeData[0] = *e1.next();
        edgeData[1] = *e1;
        edgeData[2] = *e2.next();
        tFace_3D_FaceTracked added = mesh.addFace(tMesh_3D_FaceTracked::tFaceData(), 3, edgeData, vertexData);
        mesh.connectEdges(e1, tMesh_3D_FaceTracked::beginEdge(added));
        mesh.connectEdges(e2, tMesh_3D_FaceTracked::beginEdge(added).prev());

        // note that third edge is left disconnected
    }
    for(i = 0; i < SizeL(splitWithQuad); ++i)
    {
        tEdge_3D_FaceTracked e1 = splitWithQuad[i];
        tEdge_3D_FaceTracked e2 = e1.twin();
        mesh.breakEdge(e1);
        tMesh_3D_FaceTracked::tEdgeData edgeData[3];
        tMesh_3D_FaceTracked::tVertexData vertexData[3];

        vertexData[0] = *e1.next().vertex();
        vertexData[1] = *e1.vertex();
        vertexData[2] = *e2.next().vertex();
        edgeData[0] = *e1.next();
        edgeData[1] = *e1;
        edgeData[2] = *e2.next();
        tFace_3D_FaceTracked added1 = mesh.addFace(tMesh_3D_FaceTracked::tFaceData(), 3, edgeData, vertexData);
        mesh.connectEdges(e1, tMesh_3D_FaceTracked::beginEdge(added1));

        vertexData[0] = *e2.next().vertex();
        vertexData[1] = *e2.vertex();
        vertexData[2] = *e1.next().vertex();
        edgeData[0] = *e2.next();
        edgeData[1] = *e2;
        edgeData[2] = *e1.next();
        tFace_3D_FaceTracked added2 = mesh.addFace(tMesh_3D_FaceTracked::tFaceData(), 3, edgeData, vertexData);
        mesh.connectEdges(e2, tMesh_3D_FaceTracked::beginEdge(added2));

        mesh.connectEdges(tMesh_3D_FaceTracked::beginEdge(added1).prev(), tMesh_3D_FaceTracked::beginEdge(added2).prev());

        // note that third edge of both tris are left disconnected
    }
}
