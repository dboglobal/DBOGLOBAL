//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

//.... this needs to be extended to catch slivers with any orientation
/*
#include "platform_common/Header.h"
#include "RemoveFragments.h"
#include "tMesh_Dynamic.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

typedef tMesh_Dynamic tMesh;
typedef tMesh::face tFace;
typedef tMesh::edge tEdge;
typedef tMesh::vertex tVertex;

static void
DeleteConnectedFaces(tMesh& mesh, tFace f)
{
    std::vector<tFace> connected;
    tEdge e = tMesh::beginEdge(f);
    do
    {
        if(e.twin().valid())
        {
            connected.push_back(e.twin().face());
        }
    }
    while(e.toNextInFace());
    mesh.eraseFace(f);
    tSigned32 i;
    for(i = 0; i != SizeL(connected); ++i)
    {
        DeleteConnectedFaces(mesh, connected[i]);
    }
}

static void
FillAndGetRange(tFace f, cBitVector& faceChecked, const cBSPVertices& vertices, tPoint& bottomLeft, tPoint& topRight)
{
    faceChecked.setBit(f.index(), true);
    tEdge e = tMesh::beginEdge(f);
    do
    {
        tPoint p = e.vertex()->approximatedPoint2D(vertices);
        bottomLeft.updateAsMinimumBound(p);
        topRight.updateAsMaximumBound(p);
        if(!e.twin().valid())
        {
            continue;
        }
        tFace connectedF = e.twin().face();
        if(faceChecked[connectedF.index()])
        {
            continue;
        }
        FillAndGetRange(connectedF, faceChecked, vertices, bottomLeft, topRight);
    }
    while(e.toNextInFace());
}

void
RemoveFragments(tMesh_Dynamic& mesh, const cBSPVertices& vertices, tSigned32 rangeThreshold)
{
    cBitVector faceChecked(mesh.faceSize(), false);
    tFace f = mesh.beginFace();
    while(f.valid())
    {
        if(faceChecked[f.index()])
        {
            ++f;
            continue;
        }
        tPoint bottomLeft, topRight;
        bottomLeft = tMesh::beginEdge(f).vertex()->approximatedPoint2D(vertices);
        topRight = bottomLeft;
        FillAndGetRange(f, faceChecked, vertices, bottomLeft, topRight);
        tPoint rangeOffset = topRight - bottomLeft;
        if(rangeOffset.getRangeUsed() <= rangeThreshold)
        {
            DeleteConnectedFaces(mesh, f);
        }
        ++f;
    }
}
*/
