//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/InfinitessimallyOffset_PointLike.h"
#include "platform_common/PointMultiplication.h"

bool
TraversalExitsThrough(const tLine& line, const iPointLike& target, tEdge exit)
{
    tLine edgeLine = GetEdgeLine(exit);
    if(line.side(edgeLine) != SIDE_RIGHT)
    {
        return false;
    }
    if(line.side(edgeLine.start()) != SIDE_LEFT)
    {
        return false;
    }
    if(line.side(edgeLine.end()) != SIDE_RIGHT)
    {
        return false;
    }
    return SideOfLine(edgeLine, target) == SIDE_LEFT;
}
bool
LineExitsThrough(const tLine& line, tEdge exit)
{
    tLine edgeLine = GetEdgeLine(exit);
    if(line.side(edgeLine) != SIDE_RIGHT)
    {
        return false;
    }
    if(line.side(edgeLine.start()) != SIDE_LEFT)
    {
        return false;
    }
    if(line.side(edgeLine.end()) != SIDE_RIGHT)
    {
        return false;
    }
    return true;
}

void FacingRange(
    const tLine &line,
    const tMesh::edge &entryedge,
    tMesh::edge &firstfacing,
    tMesh::edge &lastfacing
    )
{
    firstfacing=entryedge;
    ++firstfacing;
    assertD(firstfacing!=entryedge);
    while(line.side(GetEdgeLine(firstfacing))!=SIDE_RIGHT)
    {
        ++firstfacing;
        assertD(firstfacing!=entryedge);
    }
    lastfacing=firstfacing;
    tMesh::edge next;
    next=lastfacing;
    ++next;
    while(next!=entryedge && line.side(GetEdgeLine(next))==SIDE_RIGHT)
    {
        lastfacing=next;
        ++next;
    }
}


void FacingRange_Angle_EntryUnknown(
    const tPoint &angle,
    const tMesh::edge &edge,
    tMesh::edge &firstfacing,
    tMesh::edge &lastfacing
    )
{
    if(angle.side(GetEdgeAxis(edge))==SIDE_RIGHT)
    {
        firstfacing=edge;
        tMesh::edge prev=edge.prev();
        while(angle.side(GetEdgeAxis(prev))==SIDE_RIGHT)
        {
            firstfacing=prev;
            --prev;
        }
        lastfacing=edge;
    }
    else
    {
        firstfacing=edge;
        while(angle.side(GetEdgeAxis(firstfacing))!=SIDE_RIGHT)
        {
            ++firstfacing;
            assertD(firstfacing!=edge);
        }
        lastfacing=firstfacing;
    }
    tMesh::edge next;
    next=lastfacing;
    ++next;
    while(next!=firstfacing.prev() && angle.side(GetEdgeAxis(next))==SIDE_RIGHT)
    {
        lastfacing=next;
        ++next;
    }
}

tMesh::edge ExitEdge_WhicheverSide(
    const tLine &line,
    tMesh::edge firstfacing,
    tMesh::edge lastfacing
    )
{
    tMesh::edge next;
    eSide side;

    while(firstfacing!=lastfacing)
    {
    // check side of intersection
        
        next=firstfacing;
        ++next;

        side=line.side(GetVertexPoint(next));
        if(side!=SIDE_LEFT)
            return firstfacing;
        firstfacing=next;
    }
    return lastfacing;
}


tMesh::edge ExitEdge_IntersectionAndOffset_CrossesVertex(
    const tIntersection &startpoint, const tPoint &offset,
    tMesh::edge firstfacing,
    tMesh::edge lastfacing,
    bool &crossesvertex
    )
{
    tMesh::edge next;
    eSide side;
    tPoint p;
    tLine subtractedline;

    // check side of start of first
    p=GetVertexPoint(firstfacing);
    subtractedline=tLine(p-offset,p);
    side=OtherSide(subtractedline.side(startpoint));
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

        p=GetVertexPoint(next);
        subtractedline=tLine(p-offset,p);
        side=OtherSide(subtractedline.side(startpoint));
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
    p=GetVertexPoint(next);
    subtractedline=tLine(p-offset,p);
    side=OtherSide(subtractedline.side(startpoint));
    assertD(side!=SIDE_LEFT);
    if(side==SIDE_CENTRE)
    {
        crossesvertex=true;
        return lastfacing.next();
    }

    crossesvertex=false;
    return lastfacing;
}

tMesh::edge ExitEdge_CrossesVertex_MightNotPassThroughFace(
    const tLine &line,
    tMesh::edge firstfacing,
    tMesh::edge lastfacing,
    bool &crossesvertex,
    bool &passesthrough
    )
{
    tMesh::edge next;
    eSide side;

    passesthrough=true;

    // check side of start of first
    side=line.side(GetVertexPoint(firstfacing));
    if(side==SIDE_RIGHT)
    {
        passesthrough=false;
        return tMesh::endEdge();
    }
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
    if(side==SIDE_LEFT)
    {
        passesthrough=false;
        return tMesh::endEdge();
    }
    if(side==SIDE_CENTRE)
    {
        crossesvertex=true;
        return lastfacing.next();
    }

    crossesvertex=false;
    return lastfacing;
}


//bool LineCrossesAndExitsFace(
//        tFace f,
//        const tLine& line,
//        tEdge& exitEdge, bool &atVertex
//        )
//{
//    tEdge firstFacing, lastFacing;
//    FacingRange_EntryUnknown(line, tMesh::beginEdge(f), firstFacing, lastFacing);
//    eSide side;
//    side = line.side(GetVertexPoint(firstFacing));
//    if(side == SIDE_CENTRE)
//    {
//        if(VertexIsExternal(firstFacing))
//        {
//            if(line.axis().side(GetEdgeAxis(firstFacing.prev())) != SIDE_CENTRE ||
//                !EdgeHasPriority(firstFacing.prev()))
//            {
//                return false;
//            }
//            exitEdge = firstFacing;
//            atVertex = true;
//            return GetEdgeLine(firstFacing).side(line.start()) == SIDE_RIGHT
//                && GetEdgeLine(firstFacing).side(line.end()) != SIDE_RIGHT;
//        }
//        else
//        {
//            side = line.axis().side(tPoint(1, 0, 1));
//            if(side == SIDE_CENTRE)
//            {
//                side = line.axis().side(tPoint(0, 1, 1));
//            }
//            if(side == SIDE_RIGHT)
//            {
//                return false;
//            }
//            exitEdge = firstFacing;
//            atVertex = false;
//            return SideWithPriority(firstFacing, line.start()) == SIDE_RIGHT
//                && SideWithPriority(firstFacing, line.end()) != SIDE_RIGHT;
//        }
//    }
//    if(side == SIDE_RIGHT)
//    {
//        return false;
//    }
//
//    side = line.side(GetVertexPoint(lastFacing.next()));
//    if(side == SIDE_CENTRE)
//    {
//        if(VertexIsExternal(lastFacing.next()))
//        {
//            if(line.axis().side(GetEdgeAxis(lastFacing.next())) != SIDE_CENTRE ||
//                !EdgeHasPriority(lastFacing.next()))
//            {
//                return false;
//            }
//            exitEdge = lastFacing.next();
//            atVertex = true;
//            return GetEdgeLine(lastFacing).side(line.start()) == SIDE_RIGHT
//                && GetEdgeLine(lastFacing).side(line.end()) != SIDE_RIGHT;
//        }
//        else
//        {
//            side = line.axis().side(tPoint(1, 0, 1));
//            if(side == SIDE_CENTRE)
//            {
//                side = line.axis().side(tPoint(0, 1, 1));
//            }
//            exitEdge = lastFacing;
//            atVertex = false;
//            return SideWithPriority(lastFacing, line.start()) == SIDE_RIGHT
//                && SideWithPriority(lastFacing, line.end()) != SIDE_RIGHT;
//        }
//    }
//    if(side == SIDE_LEFT)
//    {
//        return false;
//    }
//
//    while(firstFacing != lastFacing)
//    {
//        side = line.side(GetVertexPoint(firstFacing.next()));
//        if(side == SIDE_RIGHT)
//        {
//            exitEdge = firstFacing;
//            atVertex = false;
//            return SideWithPriority(firstFacing, line.start()) == SIDE_RIGHT
//                && SideWithPriority(firstFacing, line.end()) != SIDE_RIGHT;
//        }
//        if(side == SIDE_CENTRE)
//        {
//            if(VertexIsExternal(firstFacing.next()))
//            {
//                exitEdge = firstFacing.next();
//                atVertex = true;
//                return GetEdgeLine(firstFacing).side(line.start()) == SIDE_RIGHT
//                    && GetEdgeLine(firstFacing).side(line.end()) != SIDE_RIGHT;
//            }
//            side = line.axis().side(tPoint(1, 0, 1));
//            if(side == SIDE_CENTRE)
//            {
//                side = line.axis().side(tPoint(0, 1, 1));
//            }
//            if(side == SIDE_RIGHT)
//            {
//                ++firstFacing;
//            }
//            exitEdge = firstFacing;
//            atVertex = false;
//            return SideWithPriority(firstFacing.next(), line.start()) == SIDE_RIGHT
//                && SideWithPriority(firstFacing.next(), line.end()) != SIDE_RIGHT;
//        }
//        ++firstFacing;
//    }
//    exitEdge = firstFacing;
//    atVertex = false;
//    return SideWithPriority(firstFacing, line.start()) == SIDE_RIGHT
//        && SideWithPriority(firstFacing, line.end()) != SIDE_RIGHT;
//}
