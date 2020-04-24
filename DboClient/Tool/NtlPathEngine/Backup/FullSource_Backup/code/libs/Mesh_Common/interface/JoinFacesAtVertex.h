//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef JOIN_FACES_AT_VERTEX_INCLUDED
#define JOIN_FACES_AT_VERTEX_INCLUDED

#include "libs/Mesh_Common/interface/TriangulateFace.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "common/TemplateMesh/Helper.h"

template <class tEdge> bool
VertexWillBeConvex(tEdge e)
{
    return GetEdgeAxis(e.twin().prev()).side(GetEdgeAxis(e.next())) == SIDE_RIGHT;
}

template <class tMesh> void
HandleOnEdgeSpecialCase(tMesh& mesh, typename tMesh::edge e)
{
    typedef typename tMesh::edge tEdge;

    tEdge e2 = e.prev().twin();
    do
    {
        assertD(GetEdgeAxis(e).side(GetEdgeAxis(e2)) == SIDE_RIGHT);
        mesh.joinFaces(e2);
        e2 = e.prev().twin();
    }
    while(GetEdgeAxis(e).side(GetEdgeAxis(e2)) != SIDE_CENTRE);

    tEdge e3 = e2.prev().twin();
    assertD(e3 != e);
    do
    {
        mesh.joinFaces(e3);
        e3 = e2.prev().twin();
    }
    while(e3 != e);
    mesh.unsplitEdge(e.twin());
}

template <class tMesh, class tInterpolator> void
JoinFacesAtVertex_KeepConvex(tMesh& mesh, typename tMesh::vertex v, tInterpolator& interpolator)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;

    tEdge e;
    e = tMesh::firstEdge(v);
    do
    {
        if(CountEdgesAround(e) != 3)
        {
            tEdge e1 = e.prev();
            tEdge e2 = e.next();
            tLine line(GetVertexPoint(e1), GetVertexPoint(e2));
            mesh.splitFace(e1, e2);
        }
    }
    while(e.toNextAroundVertex());

restart:
    
    e = tMesh::firstEdge(v);
    do
    {
        if(EdgeCanBeFlipped(e))
        {
            mesh.flipEdge(e);
            interpolator.interpolateEdge(e);
            interpolator.interpolateEdge(e.twin());
            goto restart;
        }
    }
    while(e.toNextAroundVertex());

    e = tMesh::firstEdge(v);
    do
    {
        if(!VertexWillBeConvex(e))
        {
            HandleOnEdgeSpecialCase(mesh, e);
            return;
        }
    }
    while(e.toNextAroundVertex());

    tFace f = mesh.joinFaces(v);
    TriangulateFace_ConvexWithInlineEdges(mesh, f);
}

class cNullInterpolator
{
public:
    template <class tEdge> void
    interpolateEdge(tEdge)
    {
    }
};

template <class tMesh> void
JoinFacesAtVertex_KeepConvex(tMesh& mesh, typename tMesh::vertex v)
{
    cNullInterpolator nullInterpolator;
    JoinFacesAtVertex_KeepConvex(mesh, v, nullInterpolator);
}

#endif
