//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRIANGULATE_INCLUDED
#define TRIANGULATE_INCLUDED

#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/TriangulateFace.h"
#include "libs/Mesh_Common/interface/Vertex.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/TemplateMesh/Helper.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

template <class T>
class cNullVertexFilter
{
public:
    bool operator()(T)
    {
        return false;
    }
};

template <class tMesh>
void Triangulate(tMesh &mesh)
{
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        if(!f->isExternal())
            TriangulateFace_ConvexWithInlineEdges(mesh,f);
        f++;
    }
}

template <class tEdge> bool
EdgeCanBeFlipped(tEdge e)
{
    assertF(e.next().next().next() == e);
    assertF(e.twin().next().next().next() == e.twin());
    tLine line(GetVertexPoint(e.prev()), GetVertexPoint(e.twin().prev()));
    return line.side(GetVertexPoint(e)) == SIDE_LEFT && line.side(GetVertexPoint(e.twin())) == SIDE_RIGHT;
}

template <class tEdge, class tSideChecker> bool
EdgeCanBeFlipped(tEdge e, tSideChecker sideChecker)
{
    assertF(e.next().next().next() == e);
    assertF(e.twin().next().next().next() == e.twin());
    return sideChecker.quadSplitIsValid(e.prev(), e.twin().prev(), e, e.twin());
}

// assumes that the mesh is a triangulation
template <class tMesh, class tSideChecker> void
RemoveInternalVertex(tMesh& mesh, typename tMesh::vertex v, tSideChecker sideChecker)
{
    assertD(!tMesh::lastEdge(v).valid()); // must be an internal vertex!
    typedef typename tMesh::edge tEdge;

    std::list<tEdge> spokes;
    tEdge e = tMesh::firstEdge(v);
    do
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
    }
    while(e.toNextAroundVertex());

    while(spokes.size() > 3)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i, sideChecker))
        {
            ++i;
            if(i == spokes.end())
            {
                assertD(spokes.size() == 4);
              // special case handling for situations where one edge in the triangulation must cross the vertex
              // e.g. when the vertex is at the centre of a square
              // or on the axis of a chevron
              // in both of these case there two spokes diametrically opposite either other which prevent valid flips
                tEdge toJoin1, toJoin2;
                tEdge toUnsplit;
                i = spokes.begin();
                ++i;
//                if(GetEdgeLine(*i).side(GetEdgeLine(spokes.back())) == SIDE_CENTRE)
                if(sideChecker.sideOfLine(i->twin(), spokes.back().twin(), *i) == SIDE_CENTRE)
                {
                  // spokes[1] and spokes[3]
                    toUnsplit = *i;
                    toJoin1 = spokes.front();
                    ++i;
                    toJoin2 = *i;
                }
                else
                {
                  // spokes[0] and spokes[2]
                    toUnsplit = spokes.front();
                    toJoin1 = *i;
                    ++i;
//                    assertD(GetEdgeLine(spokes.front()).side(GetEdgeLine(*i)) == SIDE_CENTRE);
                    assertD(sideChecker.sideOfLine(spokes.front().twin(), i->twin(), *i) == SIDE_CENTRE);
                    toJoin2 = spokes.back();
                }
                mesh.joinFaces(toJoin1);
                mesh.joinFaces(toJoin2);
                mesh.unsplitEdge(toUnsplit.twin());
                return;
            }
        }

        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 3);

    mesh.joinFaces(v);
}

template <class tMesh> void
RemoveInternalVertex(tMesh& mesh, typename tMesh::vertex v)
{
    cMesh2DSideChecker<typename tMesh::edge> sideChecker;
    RemoveInternalVertex(mesh, v, sideChecker);
}

template <class tMesh>
void RemoveInternalVertices(tMesh &mesh)
{
    tSigned32 numberremoved=0;
restart:
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::edge e=tMesh::beginEdge(f);
        do
        {
            if(VertexIsExternal(e))
                continue;

            RemoveInternalVertex(mesh, e.vertex());
//            assertD(ValidateMeshConnections(mesh));

            numberremoved++;
            goto restart;
        }
        while(++e!=tMesh::beginEdge(f));
        f++;
    }
}

// assumes that the mesh is a triangulation
template <class tMesh, class tSideChecker> void
RemoveInlineBoundaryVertex(tMesh& mesh, typename tMesh::edge eBoundary1, typename tMesh::edge eBoundary2, tSideChecker sideChecker, tSideChecker sideChecker2)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::vertex tVertex;
    tVertex v = eBoundary2.vertex();
    assertD(v == eBoundary1.next().vertex());
    assertD(!tMesh::lastEdge(v).valid()); // boundary vertex must be internal

    std::list<tEdge> spokes;

    tEdge e;
    
    e = eBoundary2;
    assertD(e.next().next().next() == e);
    e = e.prev().twin();
    assertD(e != eBoundary1.twin());
    do
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
        e = e.prev().twin();
    }
    while(e != eBoundary1.twin());

    while(spokes.size() > 1)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i, sideChecker))
        {
            ++i;
            assertD(i != spokes.end());
        }
        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 1);
    mesh.joinFaces(spokes.front());
    
    spokes.clear();

    e = eBoundary1.twin();
    assertD(e.next().next().next() == e);
    e = e.prev().twin();
    assertD(e != eBoundary2);
    do
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
        e = e.prev().twin();
    }
    while(e != eBoundary2);

    while(spokes.size() > 1)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i, sideChecker2))
        {
            ++i;
            assertD(i != spokes.end());
        }
        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 1);
    mesh.joinFaces(spokes.front());

    mesh.unsplitEdge(eBoundary1);
}
template <class tMesh> void
RemoveInlineBoundaryVertex(tMesh& mesh, typename tMesh::edge firstInternal)
{
    cMesh2DSideChecker<typename tMesh::edge> sideChecker;
    RemoveInlineBoundaryVertex(mesh, firstInternal, sideChecker, sideChecker);
}

// assumes that the mesh is a triangulation
template <class tMesh, class tSideChecker> void
RemoveInlineEdgeVertex_NoExternalFaces(tMesh& mesh, typename tMesh::vertex v, tSideChecker sideChecker)
{
    assertD(tMesh::lastEdge(v).valid()); // must be an external vertex!
    typedef typename tMesh::edge tEdge;

    std::list<tEdge> spokes;

    tEdge e = tMesh::firstEdge(v);
    assertD(e.next().next().next() == e);
    while(e.toNextAroundVertex())
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
    }

    while(spokes.size() > 1)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i, sideChecker))
        {
            ++i;
            assertD(i != spokes.end());
        }
        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 1);

    mesh.joinFaces(spokes.front());
    mesh.unsplitEdge(tMesh::firstEdge(v).prev());
}
template <class tMesh> void
RemoveInlineEdgeVertex_NoExternalFaces(tMesh& mesh, typename tMesh::edge firstInternal)
{
    cMesh2DSideChecker<typename tMesh::edge> sideChecker;
    RemoveInlineEdgeVertex_NoExternalFaces(mesh, firstInternal, sideChecker);
}

// assumes that the mesh is a triangulation
template <class tMesh, class tSideChecker> void
RemoveInlineEdgeVertex(tMesh& mesh, typename tMesh::edge firstInternal, tSideChecker sideChecker)
{
    assertD(!tMesh::lastEdge(firstInternal.vertex()).valid()); // this version required a mesh with external faces, so all verts must be internal!
    assertD(firstInternal.twin().face()->isExternal());
    typedef typename tMesh::edge tEdge;

    std::list<tEdge> spokes;

    assertD(firstInternal.next().next().next() == firstInternal);

    tEdge e = firstInternal.prev().twin();
    while(!e.face()->isExternal())
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
        e = e.prev().twin();
    }

    while(spokes.size() > 1)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i, sideChecker))
        {
            ++i;
            assertD(i != spokes.end());
        }

        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 1);

    mesh.joinFaces(spokes.front());
    mesh.unsplitEdge(firstInternal.prev());
}
template <class tMesh> void
RemoveInlineEdgeVertex(tMesh& mesh, typename tMesh::edge firstInternal)
{
    cMesh2DSideChecker<typename tMesh::edge> sideChecker;
    RemoveInlineEdgeVertex(mesh, firstInternal, sideChecker);
}

template <class tMesh, class tVertexFilter>
void RemoveInlineEdgeVertices(tMesh &mesh, tVertexFilter filter)
{
restart:
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::edge e=tMesh::beginEdge(f);
        do
        {
            if(!e.twin().face()->isExternal())
                continue;

            if(filter(e.vertex()))
            {
                continue;
            }
            
            if(!ExternalVertexIsInLine(e.twin().next()))
            {
                continue;
            }

            RemoveInlineEdgeVertex(mesh, e);
//            assertD(ValidateMeshConnections(mesh));

            goto restart;
        }
        while(++e!=tMesh::beginEdge(f));
        f++;
    }
}

template <class tMesh, class tEdgeFilter>
void SimplifyMesh(tMesh &mesh, tEdgeFilter filter)
{
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::edge next(mesh.beginEdge(f));
        bool joinedaface=false;
        do
        {
            typename tMesh::edge e(next);
            ++next;

            if(filter(e))
            {
                continue;
            }

            eDirection directionleft=GetEdgeLine(e.prev()).direction(GetEdgeLine(e.twin().next()));
            if(directionleft!=DIRECTION_RIGHT)
                continue;
            eDirection directionright=GetEdgeLine(e.twin().prev()).direction(GetEdgeLine(e.next()));
            if(directionright!=DIRECTION_RIGHT)
                continue;

            typename tMesh::edge beforeleft(e.prev());
            typename tMesh::edge beforeright(e.twin().prev());
            mesh.joinFaces(e);

            //.... on a mesh with external faces added, all twins are valid
            //.... so check if we can simplify this
            if(directionleft==DIRECTION_FORWARD)
            {
                if(
                    (!beforeleft.twin().valid() && !beforeleft.next().twin().valid())
                    ||
                    (beforeleft.twin().valid() && beforeleft.next().twin().valid() && beforeleft.twin().prev()==beforeleft.next().twin())
                    )
                    mesh.unsplitEdge(beforeleft);
            }
            if(directionright==DIRECTION_FORWARD)
            {
                if(
                    (!beforeright.twin().valid() && !beforeright.next().twin().valid())
                    ||
                    (beforeright.twin().valid() && beforeright.next().twin().valid() && beforeright.twin().prev()==beforeright.next().twin())
                    )
                    mesh.unsplitEdge(beforeright);
            }

            joinedaface=true;
            break;
        }
        while(next!=mesh.beginEdge(f));

        if(!joinedaface)
            ++f;
    }
}

template <class tMesh>
bool Attempt_RemoveInternalVertex(  tMesh &mesh, 
                                    const typename tMesh::edge &edge, 
                                    tSigned32 max_error)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;
    typedef typename tMesh::tFaceData tFaceData;

    tPoint p=GetVertexPoint(edge);
    tSigned32 heightBefore=GetVertexHeight(edge);

    std::vector<tEdge> edges_out;
    tFace f=RemoveInternalVertex<tMesh>(mesh,edge,edges_out,tFaceData());

    std::vector<tEdge> edges_in;
    {
        tEdge e=tMesh::beginEdge(f);
        do
            edges_in.push_back(e.twin());
        while(e.nextInFace());
    }

    std::vector<tFace> faces_added;
    TriangulateFace_NonConvex<tMesh>(mesh,f,faces_added);

    tFace containing_f;
    {
        containing_f.clear();
        size_t i;
        for(i=0;i<faces_added.size();i++)
        {
            if(PointInFace_Inclusive(tMesh::beginEdge(faces_added[i]),p))
            {
                containing_f=faces_added[i];
                break;
            }
        }
    }
    assertD(containing_f);

    CalculatePlaneEquation(containing_f);
    tSigned32 heightAfter=containing_f->heightFor(p);

    tSigned32 difference=heightAfter-heightBefore;
    if(difference<0)
        difference=-difference;
    if(difference<=max_error)
    {
        // finalise the change
        EraseInternalVertex(mesh,edge);
        return true;
    }
    else
    {
        // undo the change
        size_t i;
        for(i=0;i<faces_added.size();i++)
            mesh.eraseFace(faces_added[i]);
        assertD(edges_in.size()==edges_out.size());
        for(i=0;i<edges_in.size();i++)
            edges_in[i].connect(edges_out[i]);
        return false;
    }
}

template <class tMesh>
bool Attempt_RemoveExternalVertex(  tMesh &mesh, 
                                    const typename tMesh::edge &edge, 
                                    tSigned32 max_error)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;
    typedef typename tMesh::tFaceData tFaceData;

    assertD(edge.twin().face()->isExternal());

    tLine line(
        GetVertexPoint(edge.twin().next().next()),
        GetVertexPoint(edge.next())
        );

    if(!ExternalVertexIsInLine(edge.twin().next()))
    {
        return false;
    }

    tPoint p=GetVertexPoint(edge);
    tSigned32 heightBefore=GetVertexHeight(edge);

    tSigned32 longest_axis=line.axis().longestAxis();
    tPoint offset=p-line.start();
    tPoint::T n,d;
    n=offset.getPtr()[longest_axis];
    d=line.axis().getPtr()[longest_axis];
    if(d<0)
    {
        n=-n;
        d=-d;
    }

    tSigned32 startz=GetVertexHeight(edge.twin().next().next());
    tSigned32 endz=GetVertexHeight(edge.next());
    tSigned32 zmid=heightBefore-startz;
    tSigned32 ztotal=endz-startz;
    tSigned32 maxAllowed=zmid+max_error;
    tSigned32 minAllowed=zmid-max_error;

    tPoint::tDistance lhs,rhs;
    lhs=ztotal;
    lhs*=n;
    rhs=maxAllowed;
    rhs*=d;
    if(lhs>rhs)
        return false;
    rhs=minAllowed;
    rhs*=d;
    if(lhs<rhs)
        return false;

    RemoveEdgeVertex(mesh,edge,line);
    return true;
}

template <class tMesh> typename tMesh::vertex
AddVertexBySplittingEdge(tMesh& mesh, typename tMesh::edge edge, const typename tMesh::tVertexData& vertexData)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::tEdgeData tEdgeData;

    mesh.splitEdge(edge, vertexData, tEdgeData());
    {
        tEdge after1 = edge.next();
        tEdge after2 = after1.next().next();
        mesh.splitFace(after1, after2);
    }
    {
        tEdge after1 = edge.twin();
        tEdge after2 = after1.next().next();
        mesh.splitFace(after1, after2);
    }
    return edge.next().vertex();
}

template <class tMesh> typename tMesh::vertex
AddVertexInsideFace(tMesh& mesh, typename tMesh::face f, const typename tMesh::tVertexData& vertexData)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::tEdgeData tEdgeData;

    tEdge e = tMesh::beginEdge(f);
    tSigned32 numberOfSplits = CountEdgesAround(e);
    std::vector<tEdgeData> edgeData(numberOfSplits * 2);
    std::vector<tEdge> after(numberOfSplits);

    size_t i = 0;
    do
    {
        edgeData[i * 2 + 1] = *(e.next());
        after[i++] = e;
    }
    while(e.toNextInFace());

    return mesh.splitFaceWithVertex(
        f, numberOfSplits, vertexData, &edgeData.front(), &after.front());
}

#endif
