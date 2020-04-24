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
#include "Extrude.h"
#include "tMesh_Simple3D.h"
#include "FacingDirection.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/STL/vector.h"

static void
DoSplit(tMesh_Simple3D &hull, tEdge_Simple3D& e1, tEdge_Simple3D& e2)
{
    hull.splitVertex(e1, e2);
    assertD(ValidateMeshConnections(hull));
    *e1.prev() = *e1;
    *e2.prev() = *e2;
    --e1;
    --e2;
    hull.breakEdge(e1);
}

void
SplitHullForExtrusion(tMesh_Simple3D& hull, std::vector<tVertex_Simple3D>& verticesBelow)
{
    assertD(!hull.empty());
    assertD(verticesBelow.empty());
    tSigned32 i;
    
// determine facing direction of each face

    std::vector<eSide> sides(hull.faceSize());
    tFace_Simple3D f=hull.beginFace();
    do
    {
        sides[f.index()] = FacingDirection(f);
    }
    while((++f).valid());

// get vertices connected to downwards faces

    verticesBelow.resize(hull.vertices());
    verticesBelow.clear();
    {
        tVertex_Simple3D v = hull.beginVertex();
        do
        {
            tEdge_Simple3D e = tMesh_Simple3D::firstEdge(v);
            do
            {
                if(sides[e.face().index()] == SIDE_LEFT)
                {
                    verticesBelow.push_back(v);
                    break;
                }
            }
            while(e.toNextAroundVertex());
        }
        while((++v).valid());
    }

// get a sequence of edges for the boundary between facing up and other faces

    std::vector<tEdge_Simple3D> boundary;
    {
    // get an edge to start the boundary
        tEdge_Simple3D e = hull.beginEdge();
        while(sides[e.face().index()] == SIDE_RIGHT || sides[e.twin().face().index()] != SIDE_RIGHT)
        {
            e.toNextInMesh();
        }

    // for SIDE_CENTRE case, traverse backwards along the boundary if necessary to ensure edge is first in face
    // for the SIDE_LEFT case, we dont care about this
    // also note that for the SIDE_LEFT case this face could be the only face on the boundary
    // in which case the while loop would never terminate
        if(sides[e.face().index()] == SIDE_CENTRE)
        {
            while(sides[e.prev().twin().face().index()] == SIDE_RIGHT)
            {
                --e;
            }
        }

    // follow the boundary around the hull
        do
        {
            boundary.push_back(e);
            ++e;

            if(sides[e.face().index()] == SIDE_CENTRE)
            {
            // skip any further boundary edges in this face
            // the whole face will be dealt with in one go and have only one entry in left[] and right[]
                while(sides[e.twin().face().index()] == SIDE_RIGHT)
                {
                    ++e;
                }
            }

            while(sides[e.twin().face().index()] != SIDE_RIGHT)
            {
                e = e.twin().next();
            }
        }
        while(e != boundary.front());
    }

// add faces or modify existing faces around the boundary

    std::vector<tEdge_Simple3D> left(boundary.size()), right(boundary.size());
    std::vector<char> at_vertex(boundary.size(), false);

    //.. optimise - construct a set of free vertices first and then add faces around these
    tMesh_Simple3D::tEdgeData edges[4];
    tMesh_Simple3D::tVertexData vertices[4];
    for(i = 0; i < SizeL(boundary); i++)
    {
        tEdge_Simple3D e = boundary[i];
        tFace_Simple3D f = e.face();
        if(sides[f.index()] == SIDE_LEFT)
        {
        // new face is added at this edge
            tEdge_Simple3D e_twin = e.twin();
            hull.breakEdge(e);
            edges[0] = *(e.next());
            edges[1] = *e;
            vertices[0] = *(e.next().vertex());
            vertices[1] = *e.vertex();
            tFace_Simple3D added = hull.addFace(tMesh_Simple3D::tFaceData(), 2, edges,vertices);
            hull.connectEdges(e, tMesh_Simple3D::beginEdge(added));
            hull.connectEdges(e_twin, tMesh_Simple3D::beginEdge(added).next());
            left[i] = tMesh_Simple3D::beginEdge(added).next();
            right[i] = tMesh_Simple3D::beginEdge(added);
            at_vertex[i] = true;
        }
        else
        {
        // face is vertical
        // add two new edges to existing face
        // (except in special cases for vertical edges)

            tEdge_Simple3D start_e = e;

            // skip past edges on the boundary
            do
            {
                ++e;
            }
            while(e.twin().valid() && sides[e.twin().face().index()] == SIDE_RIGHT);

            tPoint3 axis = GetEdgeAxis3(e);
            right[i] = e;

            do
            {
                ++e;
            }
            while(e != start_e.prev());

            axis = GetEdgeAxis3(e);
            if(axis.getX() != 0 || axis.getY() != 0)
            {
                at_vertex[i] = true;
                left[i] = e.next();
            }
            else
            {
            // case for when two vertical faces connect at a vertical edge
            // at left hand side of a face
                left[i] = e;
            }

        }
    }

// split the vertex for any at_vertex cases

    if(at_vertex.front())
    {
        DoSplit(hull, right.back(), left.front());
    }
    for(i = 0; i + 1 < SizeL(left); i++)
    {
        if(at_vertex[i + 1])
        {
            DoSplit(hull, right[i], left[i + 1]);
        }
    }

// sew new faces together

    assertD(GetVertexPoint3(left.front()) == GetVertexPoint3(right.back().next()));
    assertD(GetVertexPoint3(left.front().next()) == GetVertexPoint3(right.back()));
    if(!left.front().twin().valid())
    {
        hull.connectEdges(left.front(), right.back());
    }
    for(i = 0; i + 1 < SizeL(left); i++)
    {
        assertD(GetVertexPoint3(left[i+1]) == GetVertexPoint3(right[i].next()));
        assertD(GetVertexPoint3(left[i+1].next()) == GetVertexPoint3(right[i]));
        if(!right[i].twin().valid())
        {
            hull.connectEdges(right[i], left[i+1]);
        }
    }

//// remove any inline edges
//    for(i=0;i<left.size();i++)
//    {
//        tPoint3 axis=GetVertexPoint3(left[i])-GetVertexPoint3(left[i].prev());
//        if(axis.getX()==0 && axis.getY()==0)
//            left[i].prev().unSplit();
//    }

    assertD(ValidateMeshConnections(hull));
}

//....... declare maximum range for this extrude, also
void
Extrude(tMesh_Simple3D& hull, tSigned32 amount)
{
    assertD(amount > 0);
    assertD(!hull.empty());

    assertD(hull.faces() >= 4);

    std::vector<tVertex_Simple3D> verticesBelow;
    SplitHullForExtrusion(hull, verticesBelow);
    tPoint3 offset(0, 0, -amount, amount);
    tSigned32 i;
    for(i = 0; i != SizeL(verticesBelow); ++i)
    {
        verticesBelow[i]->_p += offset;
    }
}
