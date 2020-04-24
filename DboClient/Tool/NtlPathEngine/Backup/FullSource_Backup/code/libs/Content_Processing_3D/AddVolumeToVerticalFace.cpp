//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "AddVolumeToVerticalFace.h"
#include "tMesh_Simple3D.h"
#include "Plane.h"
#include "GetOriginAndNormal.h"
#include "DynamicFraction.h"

void
AddVolumeToVerticalFace(tMesh_Simple3D& mesh)
{
  // find an offset to place a point slightly behind the plane of the face
    cPlane plane;
    GetOriginAndNormal(mesh.beginFace(), plane);
    tPoint3 offset(1, 0, 0, 1);
    cDynamicFraction dp = plane._normal.dotProduct(offset);
    if(dp.isZero())
    {
        offset = tPoint3(0, 1, 0, 1);
        dp = plane._normal.dotProduct(offset);
        assertD(!dp.isZero());
    }
    if(!dp.isNegative())
    {
//        offset = -offset; //... why does this fail to compile?
        offset.reverse();
    }

    std::vector<tMesh_Simple3D::tVertexData> vertexData(3);
    std::vector<tMesh_Simple3D::tEdgeData> edgeData(3);

  // add faces around and connect to the original face

    tEdge_Simple3D e = mesh.beginEdge();
    tPoint3 p = GetVertexPoint3(e) + offset;
    do
    {
        vertexData[0] = *e.next().vertex();
        vertexData[1] = *e.vertex();
        vertexData[2] = p;
        tFace_Simple3D f = mesh.addFace(tMesh_Simple3D::tFaceData(), 3, &edgeData.front(), &vertexData.front());
        mesh.connectEdges(e, tMesh_Simple3D::beginEdge(f));
    }
    while(e.toNextInFace());

  // connect the new faces to each other

    e = mesh.beginEdge();
    do
    {
        mesh.connectEdges(e.twin().next(), e.prev().twin().prev());
    }
    while(e.toNextInFace());
}
