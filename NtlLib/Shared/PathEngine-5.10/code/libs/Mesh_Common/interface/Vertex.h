//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VERTEX_INCLUDED
#define VERTEX_INCLUDED

#include "libs/Geometry/interface/SideEnum.h"
#include "libs/Geometry/interface/SpinCount.h"
#include "common/TemplateMesh/MeshElement.h"
#include "libs/Mesh_Common/interface/Mesh.h"

//template <class T, class P>
//T TraverseFromVertex_AntiClockwise(const T first_left, const P& target)
//{
//    T left = first_left;
//    assertD(!left.face()->isExternal());
//    assertD(target != GetVertexPoint(left));
//    while(SideWithPriority(left, target) != SIDE_RIGHT
//        || SideWithPriority(left.prev().twin(), target) != SIDE_LEFT)
//    {
//        left = left.twin().next();
//        if(left.face()->isExternal())
//        {
//            return left;
//        }
//    }
//    return left;
//}
//
//template <class T, class P>
//T TraverseFromVertex_Clockwise(const T first_left, const P& target)
//{
//    T left = first_left;
//    assertD(!left.face()->isExternal());
//    assertD(target != GetVertexPoint(left));
//    while(SideWithPriority(left, target) != SIDE_RIGHT
//        || SideWithPriority(left.prev().twin(), target) != SIDE_LEFT)
//    {
//        left = left.prev().twin();
//        if(left.face()->isExternal())
//        {
//            return left;
//        }
//    }
//    return left;
//}

template <class tEdge> tSigned32
SpinCountAroundVertex(const tEdge edge)
{
    tEdge firstE = ExternalEdgeForVertex(edge);
    firstE = firstE.prev().twin();

    tPoint firstAxis = GetEdgeAxis(firstE);

    tSigned32 spinCount = 0;
    eSide currentSide = SIDE_RIGHT;
    tEdge e = firstE;
    while(e != edge)
    {
        assertD(!e.face()->isExternal());
        e = e.prev().twin();

        eSide side = firstAxis.side(GetEdgeAxis(e));
        if(side != currentSide)
        {
            currentSide = OtherSide(currentSide);
            spinCount++;
        }
    }
    return spinCount;
}

//template <class T, class P>
//T TraverseFromVertex(const T vertex, const P& target, tSigned32 target_spin_count)
//{
//    T first_e = ExternalEdgeForVertex(vertex);
//    first_e = first_e.prev().twin();
//
//    tPoint first_axis = GetEdgeAxis(first_e);
//
//    tSigned32 spin_count = 0;
//    eSide current_side = SIDE_RIGHT;
//    T e = first_e;
//    while(1)
//    {
//        if(SideWithPriority(e, target) == SIDE_RIGHT
//           && SideWithPriority(e.prev().twin(), target) == SIDE_LEFT)
//        {
//            tSigned32 actual_spin_count = spin_count;
//            if(GetEdgeLine(e).side(target) != SIDE_CENTRE)
//            {
//                eSide side = GetEdgeLine(first_e).side(target);
//                if(side != current_side)
//                {
//                    actual_spin_count++;
//                }
//            }
//            if(actual_spin_count == target_spin_count)
//            {
//                return e;
//            }
//        }
//    
//        e = e.prev().twin();
//        assertD(!e.face()->isExternal()); // could return an invalid edge instead if desired
//
//        eSide side = first_axis.side(GetEdgeAxis(e));
//        if(side != current_side)
//        {
//            current_side = OtherSide(current_side);
//            spin_count++;
//        }
//    }
//}

template <class tMesh, class tPointLike> cMeshElement<tMesh>
TraverseFromVertex_Elements(const typename tMesh::vertex v, const tPointLike& target, tSigned32 targetSpinCount)
{
    assertD(!target.isEquivalentTo(GetVertexPoint(v)));
    typename tMesh::edge e = ExternalEdgeForVertex(tMesh::firstEdge(v)).prev().twin();
    tPoint baseAxis = GetEdgeAxis(e);
    tSigned32 spinCount = 0;
    while(spinCount != targetSpinCount)
    {
        e = e.prev().twin();
        UpdateSpinCount(baseAxis, spinCount, GetEdgeAxis(e));
    }
    while(target.sideOfLine(GetEdgeLine(e)) == SIDE_RIGHT)
    {
        e = e.prev().twin();
    }
    if(target.sideOfLine(GetEdgeLine(e)) == SIDE_CENTRE)
    {
        return cMeshElement<tMesh>(e);
    }
    return cMeshElement<tMesh>(e.twin().face());
}

template <class T>
bool ExternalVertexIsInLine(T last)
{
//    T last = ExternalEdgeForVertex(vertex);
    assertD(last.face()->isExternal());
    T first = last.prev().twin();
    if(GetEdgeAxis(first).side(GetEdgeAxis(last)) == SIDE_CENTRE)
    {
        return SpinCountAroundVertex(last) == 1;
    }
    return false;
}

template <class T>
bool ExternalVertexIsConcave(T last)
{
    return SpinCountAroundVertex(last) == 0;
}

//template <class T>
//T TraverseFromVertex_Angle(
//    T vertex,
//    const tPoint &direction
//    )
//{
//    // this version has no way to disambiguate at a spinning vertex    
//    assertD(!VertexIsExternal(vertex) || ExternalVertexIsInLine(ExternalEdgeForVertex(vertex)));
//
//    T e=vertex;
//
//    // check faces clockwise
//    while(1)
//    {
//        if(e.face()->isExternal() && GetEdgeLine(e.prev()).side(GetEdgeLine(e))!=SIDE_RIGHT)
//        {
//            if(
//                SideWithPriority_Angle(e,direction)==SIDE_RIGHT
//                ||
//                SideWithPriority_Angle(e.prev().twin(),direction)==SIDE_LEFT
//                )
//                return e;
//        }
//        else
//        if(
//            SideWithPriority_Angle(e,direction)==SIDE_RIGHT
//            &&
//            SideWithPriority_Angle(e.prev().twin(),direction)==SIDE_LEFT
//            )
//            return e;
//        e=e.prev().twin();
//        assertD(e!=vertex);
//    }
//}
//
#endif
