//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRIANGULATE_FACE_INCLUDED
#define TRIANGULATE_FACE_INCLUDED

#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Geometry/interface/InsideCorner.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "common/TemplateMesh/Helper.h"

template <class tEdge>
class cMesh2DSideChecker
{
public:
    bool
    quadSplitIsValid(tEdge eSplitStart, tEdge eSplitEnd, tEdge mustBeOnLeft, tEdge mustBeOnRight)
    {
        tLine line(GetVertexPoint(eSplitStart), GetVertexPoint(eSplitEnd));
        return line.side(GetVertexPoint(mustBeOnLeft)) == SIDE_LEFT && line.side(GetVertexPoint(mustBeOnRight)) == SIDE_RIGHT;
    }
    eSide
    sideOfLine(tEdge eLineStart, tEdge eLineEnd, tEdge eTest)
    {
        tLine line(GetVertexPoint(eLineStart), GetVertexPoint(eLineEnd));
        return line.side(GetVertexPoint(eTest));
    }
};

template <class tMesh, class tSideChecker> void
TriangulateFace_ConvexWithInlineEdges(tMesh& mesh, typename tMesh::face f, tSideChecker sideChecker)
{
    typedef typename tMesh::edge tEdge;
    tSigned32 edges = CountEdgesInFace(mesh, f);
    assertD(edges >= 3);
#ifdef ASSERTIONS_ON
    {
        // could use a more complicated loop to avoid tests here, but clarity is more important in assertions!
        tEdge e1 = tMesh::beginEdge(f);
        do
        {
            tEdge e2 = tMesh::beginEdge(f);
            do
            {
                eSide side = sideChecker.sideOfLine(e1, e1.next(), e2);
                assertD(side != SIDE_LEFT);
            }
            while(e2.toNextInFace());
        }
        while(e1.toNextInFace());
    }
#endif
    while(edges > 3)
    {
      // split off non-inline corners until face is a tri
        typename tMesh::edge e = tMesh::beginEdge(f);
        while(sideChecker.sideOfLine(e, e.next(), e.next().next()) == SIDE_CENTRE
            || sideChecker.sideOfLine(e, e.prev(), e.next().next()) == SIDE_CENTRE // this condition added as a bugfix
            )
        {
            bool iterate = e.toNextInFace();
            assertD(iterate);
        }
        mesh.splitFace(e, e.next().next());
        f = e.prev().twin().face();
        --edges;
    }
}

template <class tMesh>
void TriangulateFace_ConvexWithInlineEdges(tMesh &mesh, const typename tMesh::face &f)
{
    cMesh2DSideChecker<typename tMesh::edge> checker;
    TriangulateFace_ConvexWithInlineEdges(mesh, f, checker);
}

template <class tMesh> bool
CheckForSplitCrossings(
        const tLine &line,
        typename tMesh::edge begin_e,
        typename tMesh::edge end_e
        )
{
    while(begin_e != end_e)
    {
        tLine test = GetEdgeLine(begin_e);
        if(line.side(test.start()) != line.side(test.end()))
        {
            if(test.side(line.start()) == SIDE_RIGHT
                && test.side(line.end()) == SIDE_LEFT)
            {
                return true;
            }
            if(test.side(line.start()) == SIDE_LEFT
                && test.side(line.end()) == SIDE_RIGHT)
            {
                return true;
            }
        }
        ++begin_e;
    }
    return false;
}

template <class tMesh> bool
LineIsValidSplit(
        typename tMesh::edge e,
        typename tMesh::edge e2,
        const tLine &line
        )
{
    if(!InsideCorner(GetEdgeLine(e.prev()), GetEdgeLine(e), line.end()))
    {
        return false;
    }
    if(!InsideCorner(GetEdgeLine(e2.prev()), GetEdgeLine(e2), line.start()))
    {
        return false;
    }
    if(CheckForSplitCrossings<tMesh>(line, e.next(), e2.prev()))
    {
        return false;
    }
    if(CheckForSplitCrossings<tMesh>(line.reversed(), e2.next(), e.prev()))
    {
        return false;
    }
    return true;
}

template <class tMesh> bool
TriangulateFace_NonConvex(tMesh& mesh, typename tMesh::face f)
{
    tSigned32 i, j;
    typename tMesh::edge e = tMesh::beginEdge(f);
    tSigned32 edges = CountEdgesAround(e);
    for(i = 0; i < edges; i++, ++e)
    {
        typename tMesh::edge e2 = e.next().next();
        for(j = 0; j < edges - 3; j++, ++e2)
        {
            tLine line(GetVertexPoint(e), GetVertexPoint(e2));

        // test potential split for collisions

            if(!LineIsValidSplit<tMesh>(e, e2, line))
            {
                continue;
            }

            mesh.splitFace(e, e2);

            return TriangulateFace_NonConvex(mesh, e.face()) &&
                TriangulateFace_NonConvex(mesh, e2.face());
        }
    }
    return CountEdgesInFace(mesh, f) == 3;
}

#endif
