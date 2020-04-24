//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESHTRAVERSAL_INCLUDED
#define MESHTRAVERSAL_INCLUDED

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Mesh_Common/interface/Vertex.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

template <class T> class iMeshTranslator;

bool
TraversalExitsThrough(const tLine& line, const iPointLike& target, tEdge exit);

bool
LineExitsThrough(const tLine& line, tEdge exit);

bool LineCrossesAndExitsFace(
        tFace face,
        const tLine& line,
        tEdge &entryEdge, bool &atVertex
        );

bool InfiniteLineEntersFace(
        tFace face,
        const tLine& line,
        tEdge &entryEdge, bool &atVertex
        );

//template <class tEdge>
//tEdge ExitEdge_StdIota(
//    const tLine &line,
//    tEdge firstfacing,
//    tEdge lastfacing
//    )
//{
//    while(firstfacing!=lastfacing)
//    {
//        tEdge next;
//        eSide side;
//        next=firstfacing;
//        ++next;
//        side=line.side_NegativeStdIotas(GetVertexPoint(next));
//        assertD(side!=SIDE_CENTRE);
//        if(side==SIDE_RIGHT)
//            return firstfacing;
//        firstfacing=next;
//    }
//    return lastfacing;
//}



//================================
// NextInTraversal_ type traversal

//template <class tMesh>
//cMeshElement<tMesh> NextInTraversal_AlongInfiniteLine(
//        const cMeshElement<tMesh> &element,
//        const tLine &line
//        )
//{
//    typedef typename tMesh::edge tEdge;
//    typedef typename tMesh::face tFace;
//
//    tEdge e = element.asEdge();
//    if(element.isVertex())
//    {
//        e = TraverseFromVertex_Angle(e,line.axis());
//        return cMeshElement<tMesh>(e.face(), e);
//    }
//    else
//    {
//        bool crossesvertex;
//        assertD(!e.face()->isExternal());
//        tEdge firstfacing,lastfacing;
//        FacingRange_EntryUnknown(line,e,firstfacing,lastfacing);
//        tEdge exitedge=ExitEdge_CrossesVertex(line,firstfacing,lastfacing,crossesvertex);
//
//        if(crossesvertex)
//        {
//            return cMeshElement<tMesh>(exitedge.vertex(), exitedge);
//        }
//        return cMeshElement<tMesh>(exitedge.twin().face(), exitedge.twin());
//    }
//}


template <class tMesh>
cMeshElement<tMesh> NextInTraversal_ToIntersection(
        const cMeshElement<tMesh> &element,
        const tLine &line,
        const tLine &intersecting_line
        )
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;

    tEdge e = element.getEdge();
    if(element.isVertex())
    {
        if(intersecting_line.side(GetVertexPoint(e))==SIDE_CENTRE)
        {
        // ended on this vertex
            cMeshElement<tMesh> result;
            result.setInvalid();
            return result;
        }
        e=TraverseFromVertex_Angle(e,line.axis());
        return cMeshElement<tMesh>(e.face(), e);
    }
    else
    {
        bool crossesvertex;
        assertD(!e.face()->isExternal());
        tEdge firstfacing,lastfacing;
        FacingRange_EntryUnknown(line,e,firstfacing,lastfacing);
        tEdge exitedge=ExitEdge_CrossesVertex(line,firstfacing,lastfacing,crossesvertex);

        if(crossesvertex)
        {
            eSide side=intersecting_line.side(GetVertexPoint(exitedge));
            if(side==SIDE_LEFT)
            {
            // ended before vertex
                cMeshElement<tMesh> result;
                result.setInvalid();
                return result;
            }
            return cMeshElement<tMesh>(exitedge.vertex(), exitedge);
        }
        eSide side=line.intersectionOrder_Clockwise(intersecting_line,GetEdgeLine(exitedge));
        if(side==SIDE_CENTRE && EdgeHasPriority(exitedge))
            side=SIDE_LEFT;
        if(side==SIDE_LEFT)
        {
        // ended before exit
            cMeshElement<tMesh> result;
            result.setInvalid();
            return result;
        }
        return cMeshElement<tMesh>(exitedge.twin().face(), exitedge.twin());
    }
}
template <class tMesh>
cMeshElement<tMesh> NextInTraversal_ToLineEnd(
        const cMeshElement<tMesh> &element,
        const tLine &line
        )
{
    tLine intersecting_line=line.reversed().left90();
    return NextInTraversal_ToIntersection(element,line,intersecting_line);
}



//template <class tMesh>
//cMeshElement<tMesh> NextInTraversal_ToApproximation(
//        const cMeshElement<tMesh> &element,
//        const tPoint &target
//        )
//{
//    typedef typename tMesh::edge tEdge;
//    typedef typename tMesh::face tFace;
//
//    if(element.isVertex())
//    {
//        assertD(GetVertexPoint(element.getEdge())==target);
//        cMeshElement<tMesh> result;
//        result.setInvalid();
//        return result;
//    }
//    assertD(!element.getEdge().face()->isExternal());
//
//// check if the target point is on a vertex in the current face
//// if so this vertex is next in the traversal
//// (and the traversal will subsequently end)
////.. optimise - use traversor objects instead of these functions
//
//     tEdge e;
//     
//     e=element.getEdge();
//     do
//     {
//         if(GetVertexPoint(e)==target)
//            return e;
//     }
//     while(++e!=element.getEdge());
//
//// check if the target point is behind any edge on this face
//
//     e=element.getEdge();
//     do
//     {
//         if(SideWithPriority(e,target)==SIDE_LEFT)
//            return e.twin().face();
//     }
//     while(++e!=element.getEdge());
//
//// traversal ends
//
//    cMeshElement<tMesh> result;
//    result.setInvalid();
//    return result;
//}
//
//
//template <class tMesh>
//cMeshElement<tMesh>
//NextInTraversal_IntersectionAndOffset(
//        const cMeshElement<tMesh>& element,
//        const tIntersection &startpoint,
//        const tPoint& offset
//        )
//{
//    tEdge e = element.asEdge();
//    if(element.isVertex())
//    {
//        tPoint p = GetVertexPoint(e);
//        if(p-offset == startpoint)
//        {
//        // ended on this vertex
//            cMeshElement<tMesh> result;
//            result.setInvalid();
//            return result;
//        }
//        e=TraverseFromVertex_Angle(e,offset);
//        return cMeshElement<tMesh>(e.face(), e);
//    }
//    else
//    {
//        bool crossesvertex;
//        assertD(!e.face()->isExternal());
//        tEdge firstfacing,lastfacing;
//        FacingRange_Angle_EntryUnknown(offset,e,firstfacing,lastfacing);
//        tEdge exitedge=ExitEdge_IntersectionAndOffset_CrossesVertex(
//                        startpoint,offset,firstfacing,lastfacing,crossesvertex);
//
//        if(crossesvertex)
//        {
//            tEdge bound=exitedge;
//            if(offset.side(GetEdgeAxis(exitedge))==SIDE_CENTRE)
//                --bound;
//            tLine boundline=GetEdgeLine(bound)-offset;
//            if(boundline.side(startpoint)==SIDE_RIGHT)
//            {
//            // ended before vertex
//                cMeshElement<tMesh> result;
//                result.setInvalid();
//                return result;
//            }
//            return cMeshElement<tMesh>(exitedge.vertex(), exitedge);
//        }
//
//        tLine boundline=GetEdgeLine(exitedge)-offset;
//        eSide side=boundline.side(startpoint);
//        if(side==SIDE_CENTRE && EdgeHasPriority(exitedge))
//            side=SIDE_RIGHT;
//        if(side==SIDE_RIGHT)
//        {
//        // ended before exit
//            cMeshElement<tMesh> result;
//            result.setInvalid();
//            return result;
//        }
//        return cMeshElement<tMesh>(exitedge.twin().face(), exitedge.twin());
//    }
//}




//================================
// traverse objects

//template <class tMesh>
//class iTraverse
//{
//public:
//    virtual ~iTraverse()
//    {
//    }
//    virtual bool toNext(cMeshElement<tMesh>& pos)=0;
//    virtual bool entersThrough(const cElementEdge<tMesh>& entry)=0;
//};
//
//template <class tMesh>
//class cTraverse_ToLineEnd
//{
//    typedef typename tMesh::edge tEdge;
//    const tLine* m_line;
//public:
//    cTraverse_ToLineEnd(const tLine& line) :
//      m_line(&line)
//    { 
//        assertD(line.start()!=line.end());
//    }
//
//    // face and vertex version
//    bool toNext(cMeshElement<tMesh>& pos)
//    {
//        if(pos.isInExternalFace())
//            return false;
//        cMeshElement<tMesh> next_pos = 
//            NextInTraversal_ToLineEnd(pos,*m_line);
//        if(!next_pos.valid())
//            return false;
//        pos=next_pos;
//        return true;
//    }
//};
//
//template <class tMesh>
//class cTraverse_ToIntersection : public iTraverse<tMesh>
//{
//    typedef typename tMesh::edge tEdge;
//    const tLine* m_line;
//    const tLine* m_intersecting_line;
//public:
//    cTraverse_ToIntersection(const tLine& line, const tLine& intersecting_line) :
//      m_line(&line),
//      m_intersecting_line(&intersecting_line)
//    { 
//        assertD(line.start()!=line.end());
//        assertD(intersecting_line.start()!=intersecting_line.end());
//        assertD(line.side(intersecting_line)==SIDE_RIGHT);
//    }
//
//    // face and vertex version
//    bool toNext(cMeshElement<tMesh>& pos)
//    {
//        if(pos.isInExternalFace())
//            return false;
//        cMeshElement<tMesh> next_pos = 
//            NextInTraversal_ToIntersection(pos,*m_line,*m_intersecting_line);
//        if(!next_pos.valid())
//            return false;
//        pos=next_pos;
//        return true;
//    }
//
//    bool entersThrough(const cElementEdge<tMesh>& entry)
//    {
//        assertD(entry.twin().element().isExternal());
//        if(entry.type() == cElementEdge<tMesh>::VERTEX_INTERNAL)
//        {
//            return false;
//        }
//        if(entry.type() == cElementEdge<tMesh>::VERTEX_EXTERNAL)
//        {
//            tPoint p = GetVertexPoint(entry.basisEdge());
//            if(m_line->side(p) != SIDE_CENTRE)
//            {
//                return false;
//            }
//            tPoint axis = m_line->axis();
//            tEdge left = entry.element().asEdge();
//            if(SideWithPriority_Angle(left, axis) != SIDE_RIGHT)
//            {
//                return false;
//            }
//            if(SideWithPriority_Angle(left.prev().twin(), axis) != SIDE_LEFT)
//            {
//                return false;
//            }
//            if(m_intersecting_line->side(p) != SIDE_RIGHT)
//            {
//                return false;
//            }
//            return true;
//        }
//        tEdge e = entry.asEdge();
//        if(m_line->side(GetVertexPoint(e)) != SIDE_RIGHT)
//        {
//            return false;
//        }
//        if(m_line->side(GetVertexPoint(e.next())) != SIDE_LEFT)
//        {
//            return false;
//        }
//        return m_line->intersectionOrder_Clockwise(GetEdgeLine(e.twin()), *m_intersecting_line) != SIDE_RIGHT;
//    }
//};
//
//template <class tMesh>
//class cTraverse_AlongInfiniteLine : public iTraverse<tMesh>
//{
//    typedef typename tMesh::edge tEdge;
//    const tLine* m_line;
//public:
//    cTraverse_AlongInfiniteLine(const tLine& line) :
//      m_line(&line)
//    { 
//        assertD(line.start()!=line.end());
//    }
//
//    bool toNext(cMeshElement<tMesh>& pos)
//    {
//        if(pos.isInExternalFace())
//            return false;
//        cMeshElement<tMesh> next_pos = 
//            NextInTraversal_AlongInfiniteLine(pos,*m_line);
//        if(!next_pos.valid())
//            return false;
//        pos=next_pos;
//        return true;
//    }
//
//    bool entersThrough(const cElementEdge<tMesh>& entry)
//    {
//        assertD(entry.twin().element().isExternal());
//        if(entry.type() == cElementEdge<tMesh>::VERTEX_INTERNAL)
//        {
//            return false;
//        }
//        if(entry.type() == cElementEdge<tMesh>::VERTEX_EXTERNAL)
//        {
//            tPoint p = GetVertexPoint(entry.basisEdge());
//            if(m_line->side(p) != SIDE_CENTRE)
//            {
//                return false;
//            }
//            tPoint axis = m_line->axis();
//            tEdge left = entry.element().asEdge();
//            if(SideWithPriority_Angle(left, axis) != SIDE_RIGHT)
//            {
//                return false;
//            }
//            if(SideWithPriority_Angle(left.prev().twin(), axis) != SIDE_LEFT)
//            {
//                return false;
//            }
//            return true;
//        }
//        tEdge e = entry.asEdge();
//        if(m_line->side(GetVertexPoint(e)) != SIDE_RIGHT)
//        {
//            return false;
//        }
//        if(m_line->side(GetVertexPoint(e.next())) != SIDE_LEFT)
//        {
//            return false;
//        }
//        return true;
//    }
//};
//
//class cTraverse_ToApproximation
//{
//    const tIntersection* m_intersection; //... maybe can do without this
//    const tPoint* m_target;
//public:
//    cTraverse_ToApproximation(const tIntersection& start, const tPoint& end) :
//      m_intersection(&start),
//      m_target(&end)
//    { 
//    }
//    bool toNext(cMeshElement<tMesh>& pos);
//};
//
//class cTraverse_IntersectionAndOffset
//{
//    const tIntersection* m_start;
//    const tPoint* m_offset;
//public:
//    cTraverse_IntersectionAndOffset(const tIntersection& start, const tPoint& offset) :
//      m_start(&start),
//      m_offset(&offset)
//    { 
//    }
//    bool toNext(cMeshElement<tMesh>& pos);
//};


void FacingRange(
    const tLine &line,
    const tMesh::edge &entryedge,
    tMesh::edge &firstfacing,
    tMesh::edge &lastfacing
    );

tMesh::edge ExitEdge_WhicheverSide(
    const tLine &line,
    tMesh::edge firstfacing,
    tMesh::edge lastfacing
    );

#endif //MESHTRAVERSAL_INCLUDED
