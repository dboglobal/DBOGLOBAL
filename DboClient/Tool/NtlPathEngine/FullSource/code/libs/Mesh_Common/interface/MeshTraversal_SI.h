//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_TRAVERSAL_SI_INCLUDED
#define MESH_TRAVERSAL_SI_INCLUDED

#include "libs/Geometry/interface/StandardIotas.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"

template <class tEdge> eSide
SideOfEdge_SI(tEdge e)
{
    return OtherSide(SideOfLine_SI(GetEdgeLine(e)));
}

template <class tEdge> tEdge
NextEdgeCrossed_SI(tEdge e, const tLine& line)
{
    assertD(!e.face()->isExternal());
    tEdge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, e, firstFacing, lastFacing);
    bool atVertex;
    tEdge nextEdgeCrossed = ExitEdge_CrossesVertex(line, firstFacing, lastFacing, atVertex);
    if(atVertex)
    {
        eSide side = line.axis().side(tPoint(1, 0, 1));
        if(side == SIDE_CENTRE)
        {
            side = line.axis().side(tPoint(0, 1, 1));
        }
        if(side == SIDE_RIGHT)
        {
            assertD(nextEdgeCrossed != firstFacing);
            --nextEdgeCrossed;
        }
    }
    assertD(line.side(GetEdgeLine(nextEdgeCrossed)) == SIDE_RIGHT);
    return nextEdgeCrossed;
}

template <class tMesh, class tPointLike> bool
TraverseTowardsTarget_SI(typename tMesh::face& f, const tLine& line, const tPointLike& target, typename tMesh::edge& edgeCrossed)
{
    typename tMesh::edge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
    bool atVertex;
    typename tMesh::edge nextEdgeCrossed = ExitEdge_CrossesVertex(line, firstFacing, lastFacing, atVertex);
    if(atVertex && SideOfLine_SI(line) == SIDE_RIGHT)
    {
        assertD(nextEdgeCrossed != firstFacing);
        --nextEdgeCrossed;
    }
    assertD(line.side(GetEdgeLine(nextEdgeCrossed)) == SIDE_RIGHT);
    if(SideOfEdge_SI(nextEdgeCrossed, target) == SIDE_RIGHT)
    {
        return false;
    }
    edgeCrossed = nextEdgeCrossed;
    f = edgeCrossed.twin().face();
    return !f->isExternal();
}
template <class tMesh, class tPointLike> bool
TraverseToTarget_SI(typename tMesh::face& f, const tLine& line, const tPointLike& target, typename tMesh::edge& crossed)
{
    bool result = TraverseTowardsTarget_SI<tMesh, tPointLike>(f, line, target, crossed);
    assertD(!f->isExternal());
    return result;
}

template <class tMesh> bool
TraverseToExit_SI(typename tMesh::face& f, const tLine& line, typename tMesh::edge& crossed)
{
    typename tMesh::edge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
    bool atVertex;
    crossed = ExitEdge_CrossesVertex(line, firstFacing, lastFacing, atVertex);
    if(atVertex)
    {
        eSide side = line.axis().side(tPoint(1, 0, 1));
        if(side == SIDE_CENTRE)
        {
            side = line.axis().side(tPoint(0, 1, 1));
        }        
        if(side == SIDE_RIGHT)
        {
            --crossed;
        }
    }
    f = crossed.twin().face();
    return !f->isExternal();
}

template <class tEdge, class tPointLike> eSide
SideOfEdge_SI(tEdge e, const tPointLike& p)
{
    eSide result = SideOfLine(GetEdgeLine(e), p);
    if(result == SIDE_CENTRE)
    {
        result = OtherSide(SideOfLine_SI(GetEdgeLine(e)));
    }
    return result;
}

template <class tVertex> eSide
SideOfLine_SI(const tLine& l, tVertex v)
{
    eSide result = l.side(GetVertexPoint(v));
    if(result == SIDE_CENTRE)
    {
        result = SideOfLine_SI(l);
    }
    return result;
}

template <class tMesh> bool
InfiniteLineEntersFace_SI(typename tMesh::face f, const tLine& line, typename tMesh::edge& entryEdge)
{
  // first check whether the line actually crosses this face
    typename tMesh::edge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
    eSide side;
    side = SideOfLine_SI(line, firstFacing.vertex());
    if(side == SIDE_RIGHT)
    {
        return false;
    }
    side = SideOfLine_SI(line, lastFacing.next().vertex());
    if(side == SIDE_LEFT)
    {
        return false;
    }
    entryEdge = lastFacing.next();
    typename tMesh::edge endE = firstFacing.prev();
    while(entryEdge != endE && SideOfLine_SI(line, entryEdge.next().vertex()) == SIDE_RIGHT)
    {
        entryEdge++;
    }
    return true;
}

template <class tMesh, class tPointLike> bool
PointIsInFace(typename tMesh::face f, const tPointLike& p)
{
    typename tMesh::edge e = tMesh::beginEdge(f);
    do
    {
        if(SideOfEdge_SI(e, p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    while(e.toNextInFace());
    return true;
}

template <class tEdge> bool
EdgeHasPriority_SI(tEdge e)
{
    return SideOfEdge_SI(e) == SIDE_RIGHT;
}

template <class tMesh> bool
LineCrossesAndExitsFace_SI(typename tMesh::face f, const tLine& line, typename tMesh::edge& exitEdge)
{
    typename tMesh::edge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
    eSide side;
    side = SideOfLine_SI(line, firstFacing.vertex());
    if(side == SIDE_RIGHT)
    {
        return false;
    }
    side = SideOfLine_SI(line, lastFacing.next().vertex());
    if(side == SIDE_LEFT)
    {
        return false;
    }
    while(firstFacing != lastFacing)
    {
        side = SideOfLine_SI(line, firstFacing.next().vertex());
        if(side == SIDE_RIGHT)
        {
            exitEdge = firstFacing;
            return SideOfEdge_SI(firstFacing, line.start()) == SIDE_RIGHT
                && SideOfEdge_SI(firstFacing, line.end()) == SIDE_LEFT;
        }
        ++firstFacing;
    }
    exitEdge = firstFacing;
    return SideOfEdge_SI(firstFacing, line.start()) == SIDE_RIGHT
        && SideOfEdge_SI(firstFacing, line.end()) == SIDE_LEFT;
}

// Reversed standard iotas

template <class tEdge, class tPointLike> eSide
SideOfEdge_ReverseSI(tEdge e, const tPointLike& p)
{
    eSide result = SideOfLine(GetEdgeLine(e), p);
    if(result == SIDE_CENTRE)
    {
        result = SideOfLine_SI(GetEdgeLine(e));
    }
    return result;
}

template <class tMesh, class tPointLike> bool
TraverseTowardsTarget_ReverseSI(typename tMesh::face& f, const tLine& line, const tPointLike& target, typename tMesh::edge& edgeCrossed)
{
    typename tMesh::edge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
    bool atVertex;
    typename tMesh::edge nextEdgeCrossed = ExitEdge_CrossesVertex(line, firstFacing, lastFacing, atVertex);
    if(atVertex && SideOfLine_SI(line) == SIDE_LEFT)
    {
        assertD(nextEdgeCrossed != firstFacing);
        --nextEdgeCrossed;
    }
    assertD(line.side(GetEdgeLine(nextEdgeCrossed)) == SIDE_RIGHT);
    if(SideOfEdge_ReverseSI(nextEdgeCrossed, target) == SIDE_RIGHT)
    {
        return false;
    }
    edgeCrossed = nextEdgeCrossed;
    f = edgeCrossed.twin().face();
    return !f->isExternal();
}

template <class tMesh, class tPointLike> typename tMesh::face
TranslateToOwned_SI(const cMeshElement<tMesh>& el, const tPointLike& p)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::vertex tVertex;
    switch(el.type())
    {
    default:
        invalid();
    case bMeshElement::FACE:
        return el.asFace();
    case bMeshElement::EDGE:
        {
            tEdge e = el.asEdge();
            if(EdgeHasPriority_SI(e))
            {
                return e.face();
            }
            return e.twin().face();
        }
    case bMeshElement::VERTEX:
        {
            tVertex v = el.asVertex();
            tEdge e = tMesh::firstEdge(v);
            assertD(!VertexIsExternal(e));

            while(1)
            {
                if(SideOfEdge_SI(e) == SIDE_RIGHT && SideOfEdge_SI(e.prev()) == SIDE_RIGHT)
                {
                    return e.face();
                }
                bool iterate = e.toNextAroundVertex();
                assertD(iterate);
            }
        }
    }
}

template <class tMesh> typename tMesh::face
FaceAtEndOfLine(typename tMesh::face f, const tLine& line)
{
    typename tMesh::edge crossed;
    while(TraverseTowardsTarget_SI<tMesh>(f, line, line.end(), crossed))
    {
    }
    return f;
}


#endif
