//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_TRAVERSAL_COMMON_DEFINED
#define MESH_TRAVERSAL_COMMON_DEFINED

#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/Vertex.h"
#include "libs/Mesh_Common/interface/GeometryOverlay.h"
#include "common/TemplateMesh/MeshElement.h"

template <class tEdge> void
FacingRange_EntryUnknown(
    const tLine &line,
    const tEdge &edge,
    tEdge &firstfacing,
    tEdge &lastfacing
    )
{
    if(line.side(GetEdgeLine(edge))==SIDE_RIGHT)
    {
        firstfacing=edge;
        tEdge prev=edge.prev();
        while(line.side(GetEdgeLine(prev))==SIDE_RIGHT)
        {
            firstfacing=prev;
            --prev;
        }
        lastfacing=edge;
    }
    else
    {
        firstfacing=edge;
        while(line.side(GetEdgeLine(firstfacing))!=SIDE_RIGHT)
        {
            ++firstfacing;
            assertD(firstfacing!=edge);
        }
        lastfacing=firstfacing;
    }
    tEdge next;
    next=lastfacing;
    ++next;
    while(next!=firstfacing.prev() && line.side(GetEdgeLine(next))==SIDE_RIGHT)
    {
        lastfacing=next;
        ++next;
    }
}

template <class tEdge, class tOverlay> void
FacingRange_EntryUnknown(
    const tLine &line,
    const tEdge &edge,
    tEdge &firstfacing,
    tEdge &lastfacing, 
    const tOverlay& overlay
    )
{
    if(line.side(overlay.getEdgeLine(edge))==SIDE_RIGHT)
    {
        firstfacing=edge;
        tEdge prev=edge.prev();
        while(line.side(overlay.getEdgeLine(prev))==SIDE_RIGHT)
        {
            firstfacing=prev;
            --prev;
        }
        lastfacing=edge;
    }
    else
    {
        firstfacing=edge;
        while(line.side(overlay.getEdgeLine(firstfacing))!=SIDE_RIGHT)
        {
            ++firstfacing;
            assertD(firstfacing!=edge);
        }
        lastfacing=firstfacing;
    }
    tEdge next;
    next=lastfacing;
    ++next;
    while(next!=firstfacing.prev() && line.side(overlay.getEdgeLine(next))==SIDE_RIGHT)
    {
        lastfacing=next;
        ++next;
    }
}

template <class tEdge> void
FacingRange_Direction_EntryUnknown(
    const tPoint &direction,
    const tEdge &edge,
    tEdge &firstfacing,
    tEdge &lastfacing
    )
{
    if(direction.side(GetEdgeLine(edge).axis())==SIDE_RIGHT)
    {
        firstfacing=edge;
        tEdge prev=edge.prev();
        while(direction.side(GetEdgeLine(prev).axis())==SIDE_RIGHT)
        {
            firstfacing=prev;
            --prev;
        }
        lastfacing=edge;
    }
    else
    {
        firstfacing=edge;
        while(direction.side(GetEdgeLine(firstfacing).axis())!=SIDE_RIGHT)
        {
            ++firstfacing;
            assertD(firstfacing!=edge);
        }
        lastfacing=firstfacing;
    }
    tEdge next;
    next=lastfacing;
    ++next;
    while(next!=firstfacing.prev() && direction.side(GetEdgeLine(next).axis())==SIDE_RIGHT)
    {
        lastfacing=next;
        ++next;
    }
}

template <class tEdge> tEdge
ExitEdge_CrossesVertex(
    const tLine& line,
    tEdge firstfacing,
    tEdge lastfacing,
    bool& crossesvertex
    )
{
    tEdge next;
    eSide side;

    // check side of start of first
    side=line.side(GetVertexPoint(firstfacing));
    assertD(side!=SIDE_RIGHT);
    if(side==SIDE_CENTRE)
    {
        crossesvertex=true;
        return firstfacing;
    }

    while(firstfacing!=lastfacing)
    {
    // check side of intersection
        
        next=firstfacing;
        ++next;

        side=line.side(GetVertexPoint(next));
        if(side==SIDE_CENTRE)
        {
            crossesvertex=true;
            return next;
        }
        if(side==SIDE_RIGHT)
        {
            crossesvertex=false;
            return firstfacing;
        }
        firstfacing=next;
    }

    // check side of end of last
    next=lastfacing;
    ++next;
    side=line.side(GetVertexPoint(next));
    assertD(side!=SIDE_LEFT);
    if(side==SIDE_CENTRE)
    {
        crossesvertex=true;
        return lastfacing.next();
    }

    crossesvertex=false;
    return lastfacing;
}

template <class tEdge, class tOverlay> tEdge
ExitEdge_CrossesVertex(
    const tLine& line,
    tEdge firstfacing,
    tEdge lastfacing,
    bool& crossesvertex,
    const tOverlay& overlay
    )
{
    tEdge next;
    eSide side;

    // check side of start of first
    side=line.side(overlay.getVertexPoint(firstfacing));
    assertD(side!=SIDE_RIGHT);
    if(side==SIDE_CENTRE)
    {
        crossesvertex=true;
        return firstfacing;
    }

    while(firstfacing!=lastfacing)
    {
    // check side of intersection
        
        next=firstfacing;
        ++next;

        side=line.side(overlay.getVertexPoint(next));
        if(side==SIDE_CENTRE)
        {
            crossesvertex=true;
            return next;
        }
        if(side==SIDE_RIGHT)
        {
            crossesvertex=false;
            return firstfacing;
        }
        firstfacing=next;
    }

    // check side of end of last
    next=lastfacing;
    ++next;
    side=line.side(overlay.getVertexPoint(next));
    assertD(side!=SIDE_LEFT);
    if(side==SIDE_CENTRE)
    {
        crossesvertex=true;
        return lastfacing.next();
    }

    crossesvertex=false;
    return lastfacing;
}

template <class tMesh> bool
TraverseTowardsTarget_OverlappingFaces(typename tMesh::face& f, const tLine& line, eSide spinThroughSide, const iPointLike& target, typename tMesh::edge& crossed, bool& atVertex, bool& exited)
{
    typedef typename tMesh::edge tEdge;
    assertD(!f->isExternal());
    tEdge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
    crossed = ExitEdge_CrossesVertex(line, firstFacing, lastFacing, atVertex);
    if(atVertex)
    {
        tPoint p = GetVertexPoint(crossed);
        tLine bound(p, p + line.axis().right90());
        if(target.sideOfLine(bound) != SIDE_LEFT)
        {
            exited = false;
            return false; // completed at or before vertex
        }
        if(spinThroughSide == SIDE_RIGHT)
        {
            tEdge e = crossed.twin().next();
            f = e.face();
            if(f->isExternal())
            {
                exited = true;
                return false;
            }
            while(line.axis().side(GetEdgeAxis(e)) == SIDE_RIGHT)
            {
                e = e.twin().next();
                f = e.face();
            }
        }
        else
        {
            assertD(spinThroughSide == SIDE_LEFT);
            tEdge e = crossed.prev().twin().prev();
            f = e.face();
            if(f->isExternal())
            {
                exited = true;
                return false;
            }
            while(line.axis().side(GetEdgeAxis(e.twin())) == SIDE_LEFT)
            {
                assertD(!f->isExternal());
                e = e.twin().prev();
                f = e.face();
            }
        }
    }
    else
    {
        assertD(line.side(GetEdgeLine(crossed)) == SIDE_RIGHT);
        eSide side = target.sideOfLine(GetEdgeLine(crossed));
        if(side != SIDE_LEFT)
        {
            exited = false;
            return false; // traversal completed
        }
        f = crossed.twin().face();
        if(f->isExternal())
        {
            exited = true;
            return false;
        }
    }
    return true;
}

template <class T, class tOverlay>
bool ExternalVertexIsInLine(T last, const tOverlay& overlay)
{
    assertD(last.face()->isExternal());
    T first = last.prev().twin();
    if(overlay.getEdgeAxis(first).side(overlay.getEdgeAxis(last)) == SIDE_CENTRE)
    {
        return SpinCountAroundVertex(last) == 1;
    }
    return false;
}

#endif
