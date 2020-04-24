//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tIntersection.h"

//.... move stuff into code/common where geometry is not used

template <class tMesh> typename tMesh::edge
EdgeInFaceForVertex(typename tMesh::face f, typename tMesh::vertex v)
{
    typename tMesh::edge result = tMesh::beginEdge(f);
    do
    {
        if(result.vertex() == v)
        {
            return result;
        }
    }
    while(result.toNextInFace());
    return result;
}
template <class tMesh> typename tMesh::edge
EdgeBetweenFaces(typename tMesh::face f1, typename tMesh::face f2)
{
    typename tMesh::edge result = tMesh::beginEdge(f1);
    do
    {
        if(result.twin().face() == f2)
        {
            return result;
        }
    }
    while(result.toNextInFace());
    return result;
}

//template <class T>
//eSide SideWithPriority(const T e, const tPoint &p)
//{
////... optimise - store ownership as bool in cMeshEdge
////... optimise - eliminate SIDE_CENTRE test
//    eSide result=GetEdgeLine(e).side(p);
//    if(result==SIDE_CENTRE)
//    {
////        if(!e.twin().valid() || e.twin().face()->index>e.face()->index)
//        if(EdgeHasPriority(e))
//        // this face has priority
//            return SIDE_RIGHT;
//        return SIDE_LEFT;
//    }
//    return result;
//}
//template <class T>
//eSide SideWithPriority(const T e, const tIntersection &p)
//{
////... optimise - store ownership as bool in cMeshEdge
////... optimise - eliminate SIDE_CENTRE test
//    eSide result=GetEdgeLine(e).side(p);
//    if(result==SIDE_CENTRE)
//    {
////        if(!e.twin().valid() || e.twin().face()->index>e.face()->index)
//        if(EdgeHasPriority(e))
//        // this face has priority
//            return SIDE_RIGHT;
//        return SIDE_LEFT;
//    }
//    return result;
//}
//template <class T>
//eSide SideWithPriority_Angle(const T e, const tPoint &angle)
//{
////... optimise - store ownership as bool in cMeshEdge
////... optimise - eliminate SIDE_CENTRE test
//    eSide result=GetEdgeAxis(e).side(angle);
//    if(result==SIDE_CENTRE)
//    {
////        if(!e.twin().valid() || e.twin().face()->index>e.face()->index)
//        if(EdgeHasPriority(e))
//        // this face has priority
//            return SIDE_RIGHT;
//        return SIDE_LEFT;
//    }
//    return result;
//}

template <class T>
eSide SideWithPreference_Angle(const T e, const tPoint &angle, eSide sideforcentre)
{
    eSide result=GetEdgeAxis(e).side(angle);
    if(result==SIDE_CENTRE)
        return sideforcentre;
    return result;
}


//template <class T>
//T VertexOwner(T vertex)
//{
//    T e=vertex;
//    T owner=vertex;
//    tSigned32 bestindex=owner.face()->index;
//
//    do
//    {
//        if(e.face()->index<bestindex)
//        {
//            owner=e;
//            bestindex=e.face()->index;
//        }
//        e=e.prev().twin();
//    }
//    while(e!=vertex);
//
//    return owner;
//}
//
template <class T> T
ExternalEdgeForVertex(const T vertex)
{
    T e=vertex;
    while(!e.face()->isExternal())
    {
        e=e.twin().next();
        if(e==vertex)
        {
            e.clear();
            return e;
        }
    }
    return e;
}

template <class tEdge> bool
FaceIsConvex(tEdge startE)
{
    assertD(startE.face().size() > 2);
    tEdge e = startE;
    do
    {
        tEdge e2 = e.next().next();
        do
        {
            if(GetEdgeLine(e).side(GetVertexPoint(e2)) != SIDE_RIGHT)
            {
                return false;
            }
            ++e2;
        }
        while(e2 != e);
        ++e;
    }
    while(e != startE);
    return true;
}

#endif
