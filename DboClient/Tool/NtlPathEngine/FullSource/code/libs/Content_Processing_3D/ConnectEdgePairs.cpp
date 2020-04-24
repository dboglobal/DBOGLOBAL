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
#include "ConnectEdgePairs.h"
#include "tMesh_Dynamic.h"
#include "common/Containers/CollapsedVectorVector.h"
#include "i_pathengine.h"
#include "common/STL/algorithm.h"
#include "common/STL/vector.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"


//static void
//SplitEdgeByVerts(
//        tMesh_Dynamic& mesh,
//        tEdge_Dynamic e,
//        tEdge_Dynamic* vertsPtr, tSigned32 verts
//        )
//{
//    cDynamicPoint3 base = e.vertex()->dynamicPoint(vertices);
//    cDynamicPoint3 axis = e.next().vertex()->dynamicPoint(vertices);
//    axis -= base;
//    tSigned32 j;
//    for(j = 0; j != verts; ++j)
//    {
//        tVertex_Dynamic v = vertsPtr[j].next().vertex();
//        cDynamicPoint3 offset = v->dynamicPoint(vertices);
//        offset -= base;
//        if(offset.isOnAxis_NotInclusive_KnownToBeInline(axis))
//        {
//            mesh.splitEdge(e, *v);
//            SplitEdgeByVerts(mesh, e.next(), vertsPtr + j + 1, verts - (j + 1));
//            SplitEdgeByVerts(mesh, e, vertsPtr + j + 1, verts - (j + 1));
//            return;
//        }
//    }
//}

class cDistanceAlongSharedAxisPredicate
{
    const cBSPVertices* _vertices;
    cDynamicPoint3* _axis;
public:
    cDistanceAlongSharedAxisPredicate(const cBSPVertices* vertices, cDynamicPoint3* axis) :
      _vertices(vertices),
      _axis(axis)
    {
    }
    bool operator()(tEdge_Dynamic lhs, tEdge_Dynamic rhs)
    {
    //...... optimise - only need to compare one component
        if(lhs == rhs)
        {
            return false;
        }
        cDynamicPoint3 offset = rhs.vertex()->dynamicPoint(*_vertices);
        offset -= lhs.vertex()->dynamicPoint(*_vertices);
        if(offset.isZero())
        {
            return false;
        }
        eDirection direction = _axis->direction_KnownToBeInline(offset);
        return direction == DIRECTION_FORWARD;
    }
};

static void
Update_E1StartMatchesE2End(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        cDistanceAlongSharedAxisPredicate predicate,
        tEdge_Dynamic*& edgesPtr1, tSigned32& edges1,
        tEdge_Dynamic*& edgesPtr2, tSigned32& edges2
        )
{
    tEdge_Dynamic e1 = edgesPtr1[0];
    tEdge_Dynamic e2 = edgesPtr2[0];
    if(e1.next().vertex()->dynamicPoint(vertices) == e2.vertex()->dynamicPoint(vertices))
    {
        mesh.connectEdges(e1, e2);
        edgesPtr1++;
        edges1--;
        edgesPtr2++;
        edges2--;
        return;
    }
    if(predicate(e1.next(), e2))
    {
        mesh.splitEdge(e2, *e1.next().vertex());
        mesh.connectEdges(e1, e2.next());
        edgesPtr1++;
        edges1--;
    }
    else
    {
        mesh.splitEdge(e1, *e2.vertex());
        mesh.connectEdges(e1, e2);
        *edgesPtr1 = e1.next();
        edgesPtr2++;
        edges2--;
    }
}

static void
Update_E1StartBeforeE2End(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        cDistanceAlongSharedAxisPredicate predicate,
        tEdge_Dynamic*& edgesPtr1, tSigned32& edges1,
        tEdge_Dynamic*& edgesPtr2, tSigned32& edges2
        )
{
    tEdge_Dynamic e1 = edgesPtr1[0];
    tEdge_Dynamic e2 = edgesPtr2[0];
    if(predicate(e2.next(), e1.next()))
    {
    // e2 end vertex splits e1
        mesh.splitEdge(e1, *e2.next().vertex());
        *edgesPtr1 = e1.next();
        Update_E1StartMatchesE2End(vertices, mesh, predicate, edgesPtr1, edges1, edgesPtr2, edges2); 
    }
    else
    {
    // e1 is completely before, or ends exactly on e2 end vertex
        edgesPtr1++;
        edges1--;
    }
}

static void
Update_E2EndBeforeE1Start(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        cDistanceAlongSharedAxisPredicate predicate,
        tEdge_Dynamic*& edgesPtr1, tSigned32& edges1,
        tEdge_Dynamic*& edgesPtr2, tSigned32& edges2
        )
{
    tEdge_Dynamic e1 = edgesPtr1[0];
    tEdge_Dynamic e2 = edgesPtr2[0];
    if(predicate(e1, e2))
    {
    // e1 start vertex splits e2
        mesh.splitEdge(e2, *e1.vertex());
        Update_E1StartMatchesE2End(vertices, mesh, predicate, edgesPtr1, edges1, edgesPtr2, edges2); 
    }
    else
    {
    // e2 is completely before, or starts exactly on e1 start vertex
        edgesPtr2++;
        edges2--;
    }
}

static void
ConnectEdgePairs(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        tEdge_Dynamic* edgesPtr1, tSigned32 edges1,
        tEdge_Dynamic* edgesPtr2, tSigned32 edges2
        )
{
    if(edges1 == 0 || edges2 == 0)
    {
        return;
    }

// sort the edges in order of distances along the shared axis

    cDynamicPoint3 axis = GetEdgeAxis3(vertices, edgesPtr1[0]);
    cDistanceAlongSharedAxisPredicate predicate(&vertices, &axis);

    std::sort(edgesPtr1, edgesPtr1 + edges1, predicate);
    std::sort(edgesPtr2, edgesPtr2 + edges2, predicate);

    do
    {
        tEdge_Dynamic e1 = edgesPtr1[0];
        tEdge_Dynamic e2 = edgesPtr2[0];
        if(e1.vertex()->dynamicPoint(vertices) == e2.next().vertex()->dynamicPoint(vertices))
        {
            Update_E1StartMatchesE2End(vertices, mesh, predicate, edgesPtr1, edges1, edgesPtr2, edges2); 
        }
        else
        if(predicate(e1, e2.next()))
        {
            Update_E1StartBeforeE2End(vertices, mesh, predicate, edgesPtr1, edges1, edgesPtr2, edges2);
        }
        else
        {
            Update_E2EndBeforeE1Start(vertices, mesh, predicate, edgesPtr1, edges1, edgesPtr2, edges2);
        }
    }
    while(edges1 && edges2);
}

void
ConnectEdgePairs(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        iProgressCallBack* progressCallBack
        )
{
    assertF(ValidateMeshConnections(mesh));

// count entries first, so we can use cCollapsedVectorVector, and avoid lots of memory overhead

    std::vector<tSigned32> edgesPerIndex;
    tFace_Dynamic f = mesh.beginFace();
    do
    {
        if(f->wasDiscarded())
        {
            continue;
        }        
        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
        do
        {
            if(e->_pairIndex == -1)
            {
                continue;
            }
            assertD(!e.twin().valid());
            assertD(e->_pairIndex >= 0);
            if(e->_pairIndex >= SizeL(edgesPerIndex))
            {
                edgesPerIndex.resize(e->_pairIndex + 1, 0);
            }
            edgesPerIndex[e->_pairIndex]++;
        }
        while(e.toNextInFace());
    }
    while((++f).valid());

// now go ahead and build the lookup

    cCollapsedVectorVector<tEdge_Dynamic> lookup(edgesPerIndex);
    f = mesh.beginFace();
    do
    {
        if(f->wasDiscarded())
        {
            continue;
        }        
        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
        do
        {
            if(e->_pairIndex == -1)
            {
                continue;
            }
            assertD(!e.twin().valid());
            --edgesPerIndex[e->_pairIndex];
            lookup[e->_pairIndex][edgesPerIndex[e->_pairIndex]] = e;
        }
        while(e.toNextInFace());
    }
    while((++f).valid());

// note that this will be rounded down in the case where all pairs for the last entry encountered have been discarded

    tSigned32 pairs = SizeL(edgesPerIndex) / 2;

// process and connect per pair index

    tSigned32 i;
    for(i = 0; i != pairs; ++i)
    {
        if(progressCallBack)
        {
            progressCallBack->updateProgressL("Connecting edge pairs", i, pairs);
        }
        ConnectEdgePairs(
                vertices,
                mesh,
                lookup[i * 2], lookup.subVectorSize(i * 2),
                lookup[i * 2 + 1], lookup.subVectorSize(i * 2 + 1)
                );
    }
}
