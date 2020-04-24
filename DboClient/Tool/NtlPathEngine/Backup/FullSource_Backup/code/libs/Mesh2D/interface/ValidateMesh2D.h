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
#include "common/interface/Error.h"
#include "platform_common/CoordinateRanges.h"
#include "libs/Mesh_Common/interface/Vertex.h"

template <class tMesh>
void ValidateMeshEdge(typename tMesh::edge edge)
{
    typedef typename tMesh::edge tEdge;
    assertD(GetVertexPoint(edge).isPoint());
    assertD(edge.twin().valid());
    assertD(GetEdgeLine(edge)==GetEdgeLine(edge.twin()).reversed());
    assertD(edge.twin().twin()==edge);

    if(edge.face()->isExternal())
    {
        // each vertex may have at most one external face
        {
            tEdge e=edge.twin().next();
            assertD(e!=edge);
            do
            {
                assertD(!e.face()->isExternal());
                e=e.twin().next();
            }
            while(e!=edge);
        }
    }
    else
    {
    // next edge must always turn to right
        assertD(GetEdgeAxis(edge).side(GetEdgeAxis(edge.next()))==SIDE_RIGHT);
    }
}
template <class tMesh>
void ValidateMeshPolygon(typename tMesh::edge start)
{
    typename tMesh::edge edge=start;
    do
    {
        assertD(edge.next().valid());
        ValidateMeshEdge<tMesh>(edge);
        //.. doesn't check for spinning
    }
    while(++edge!=start);
}

template <class tMesh>
void
ValidateMesh2D(tMesh &mesh)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;
    tEdge e;

    assertD(!mesh.empty());
    assertD(!mesh.hasDeadEntries());

    // external faces must all be at end of face ordering
    tFace f = mesh.beginFace();
    while(1)
    {
        assertD(f.valid());
        if(f->isExternal())
        {
            ++f;
            break;
        }
        ++f;
    }
    while(f.valid())
    {
        assertD(f->isExternal());
        ++f;
    }

    e=mesh.beginEdge();
    do
    {
        tSigned32 x,y;
        tPoint p=GetVertexPoint(e);
        x=p.getX();
        y=p.getY();

        assertD(x>=-WORLD_RANGE || x<=WORLD_RANGE && y>=-WORLD_RANGE && y<=WORLD_RANGE);
        e.face();
    }
    while(e.toNextInMesh());

    f = mesh.beginFace();
    while(f.valid())
    {
        ValidateMeshPolygon<tMesh>(mesh.beginEdge(f));
        ++f;
    }
}

