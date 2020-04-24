//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/Vector2F.h"
#include "platform_common/PointMultiplication.h"

template <class tMesh> void
MostExtremeXY(typename tMesh::face aFace,
              typename tMesh::edge &e1, 
              typename tMesh::edge &e2)
{
    typename tMesh::edge candidate1,candidate2;
    tPoint::tDistance bestdist,candidatedist;
    candidate1 = tMesh::beginEdge(aFace);
    candidate2 = candidate1.next();
    e1 = candidate1;
    e2 = candidate2;
    bestdist = (GetVertexPoint(e2).point()-GetVertexPoint(e1).point()).lengthSquared();
    ++candidate2;
    do
    {
        while(candidate2!=tMesh::beginEdge(aFace))
        {
            candidatedist=
                (GetVertexPoint(candidate2).point()-GetVertexPoint(candidate1).point()).lengthSquared();
            if(tMultiplier::compare(candidatedist, bestdist) == -1)
            {
                e1=candidate1;
                e2=candidate2;
                bestdist=candidatedist;
            }
            ++candidate2;
        }
        ++candidate1;
        candidate2=candidate1.next();
    }
    while(candidate1!=tMesh::beginEdge(aFace));
}

template <class tMesh> void
CalculatePlaneEquation(typename tMesh::face aFace)
{
// primary axis comes from most extreme points in horizontal
    typename tMesh::edge origin,primary;
    MostExtremeXY<tMesh>(aFace,origin,primary);
    tPoint p_origin=GetVertexPoint(origin).point();
    cVector2F axis1(GetVertexPoint(primary).point() - p_origin);
    float heightfor1 = primary->getZ() - origin->getZ();
    {
        float length = axis1.length();
        axis1/=length;
        heightfor1/=length;
    }

// secondary axis comes from summation of offsets to all other points from origin
//    tPointF axis2=tPointF(0.0f,0.0f,0);
    cVector2F axis2(0.0f, 0.0f);
    float heightfor2=0;
    typename tMesh::edge i=origin.next();
    while(i!=primary)
    {
        axis2 += cVector2F(p_origin - GetVertexPoint(i).point());
        heightfor2 += origin->getZ() - i->getZ();
        i++;
    }
    i=primary.next();
    while(i!=origin)
    {
        axis2 += cVector2F(GetVertexPoint(i).point() - p_origin);
        heightfor2 += i->getZ() - origin->getZ();
        i++;
    }

// make secondary axis normal to primary
    float distalong;
    distalong = axis2.dotProduct(axis1);
    axis2-=axis1*distalong;
    heightfor2-=heightfor1*distalong;
    {
        float length = axis2.length();
        axis2/=length;
        heightfor2/=length;
    }

// get height for unit vectors
    float heightForX = axis1.x * heightfor1 + axis2.x * heightfor2;
    float heightForY = axis1.y * heightfor1 + axis2.y * heightfor2;

// move origin to 0,0
    float heightAtOrigin =
        origin->getZ()
        - p_origin.getX() * heightForX
        - p_origin.getY() * heightForY;

    aFace->setPlaneEquation(heightAtOrigin, heightForX, heightForY);
}

template <class tMesh> void
CalculatePlaneEquations(tMesh& mesh)
{
    typename tMesh::face f = mesh.beginFace();
    while(f.valid())
    {
        if(!f->isExternal())
        {
            CalculatePlaneEquation<tMesh>(f);
        }
        ++f;
    }
}
