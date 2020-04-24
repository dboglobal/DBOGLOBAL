//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifdef ASSERTIONS_ON
#include "platform_common/Header.h"
#include "ValidateFaceDynamic.h"
#include "tMesh_Dynamic.h"
#include "SideOfProjectedTo2D.h"
#include "SideOfPlane.h"
#include "common/TemplateMesh/Helper.h"

static bool
ValidateFaceCommon(
        const cBSPVertices& vertices,
        tFace_Dynamic f
        )
{
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    if(CountEdgesAround(e) < 3)
    {
        return false;
    }
    int projectionType = GetProjectionType(e.vertex()->dynamicPoint(vertices), e.next().vertex()->dynamicPoint(vertices), e.next().next().vertex()->dynamicPoint(vertices));
    do
    {
        const cDynamicPoint3& v1 = e.vertex()->dynamicPoint(vertices);
        tEdge_Dynamic nextE = e;
        ++nextE;
        const cDynamicPoint3& v2 = nextE.vertex()->dynamicPoint(vertices);
        tEdge_Dynamic e2 = tMesh_Dynamic::beginEdge(f);
        do
        {
            if(e2 == e || e2 == nextE)
            {
                continue;
            }
            const cDynamicPoint3& v3 = e2.vertex()->dynamicPoint(vertices);
            eSide side = SideOfProjectedTo2D(projectionType, v1, v2, v3);
            if(side != SIDE_RIGHT)
            {
                return false;
            }
        }
        while(e2.toNextInFace());
    }
    while(e.toNextInFace());
    return true;
}

static void
GetOriginAndNormal_Dynamic(const cBSPVertices& vertices,
                           tFace_Dynamic f, cDynamicPoint3& origin, cDynamicPoint3& normal)
{
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    origin = e.vertex()->dynamicPoint(vertices);
    cDynamicPoint3 axis1 = e.next().vertex()->dynamicPoint(vertices);
    axis1 -= origin;
    assertR(!axis1.isZero());
    cDynamicPoint3 axis2 = e.prev().vertex()->dynamicPoint(vertices);
    axis2 -= origin;
    assertR(!axis2.isZero());
    axis2.crossProduct(axis1, normal);
}
static bool
CheckOnPlane(const cDynamicPoint3& origin, const cDynamicPoint3& normal, const cDynamicPoint3& test)
{
    cDynamicPoint3 offset = test;
    offset -= origin;
    cDynamicFraction dp = normal.dotProduct(offset);
    return dp.isZero();
}

bool
ValidateFaceDynamic(
        const cBSPVertices& vertices,
        tFace_Dynamic f
        )
{
    if(!ValidateFaceCommon(vertices, f))
    {
        return false;
    }
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    cDynamicPoint3 planeOrigin, planeNormal;
    GetOriginAndNormal_Dynamic(vertices, f, planeOrigin, planeNormal);
    do
    {
        const cDynamicPoint3& v1 = e.vertex()->dynamicPoint(vertices);
        if(!CheckOnPlane(planeOrigin, planeNormal, v1))
        {
            return false;
        }
    }
    while(e.toNextInFace());
    return true;
}

bool
ValidateFaceDynamic(
        const cBSPVertices& vertices,
        const cPlane& splitter,
        tFace_Dynamic f
        )
{
    if(!ValidateFaceCommon(vertices, f))
    {
        return false;
    }
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    do
    {
        const cDynamicPoint3& p = e.vertex()->dynamicPoint(vertices);
        if(SideOfPlane(splitter, p) != ON_PLANE)
        {
            return false;
        }
    }
    while(e.toNextInFace());
    return true;
}

bool
ValidateFaceDynamic_Ground(
        const cBSPVertices& vertices,
        tFace_Dynamic f
        )
{
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    cDynamicPoint3 planeOrigin, planeNormal;
    GetOriginAndNormal_Dynamic(vertices, f, planeOrigin, planeNormal);
    if(planeNormal.z().isZero() || planeNormal.z().isNegative())
    {
        return false;
    }
    do
    {
        const cDynamicPoint3& v1 = e.vertex()->dynamicPoint(vertices);
        if(!CheckOnPlane(planeOrigin, planeNormal, v1))
        {
            return false;
        }
    }
    while(e.toNextInFace());
    return true;
}

#endif
