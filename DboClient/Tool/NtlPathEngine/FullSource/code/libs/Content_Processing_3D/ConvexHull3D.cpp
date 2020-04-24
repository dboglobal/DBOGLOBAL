//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ConvexHull3D.h"
#include "GetOriginAndNormal.h"
#include "SideOfPlane.h"
#include "tMesh_Simple3D.h"
#include "Plane.h"
#include "DynamicPoint3.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/PointMultiplication.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"

typedef tMesh_Simple3D::tFaceData tFaceData;
typedef tMesh_Simple3D::tEdgeData tEdgeData;
typedef tMesh_Simple3D::tVertexData tVertexData;

static void
ConvexHull3D_BasePyramidPoints(
        tSigned32 points, const tPoint3* pointArray,
        tPoint3* pyramid,
        std::list<tPoint3>& remainingPoints
        )
{
    assertD(points > 0);
    assertD(remainingPoints.empty());

    std::list<tPoint3>::iterator it;
    tSigned32 i;

    pyramid[0] = pointArray[0];
    for(i = 1; i < points; i++)
    {
        remainingPoints.push_front(pointArray[i]);
    }

// remove any points coincident with pyramid[0]
    for(it = remainingPoints.begin(); it != remainingPoints.end(); )
    {
        if(*it == pyramid[0])
        {
            it = remainingPoints.erase(it);
        }
        else
        {
            ++it;
        }
    }
    if(remainingPoints.empty())
    {
    // point cloud reduces to a single point
    // add points to force volume
        pyramid[1] = pointArray[0] + tPoint3(1, 0, 0, 1);
        pyramid[2] = pointArray[0] + tPoint3(0, 1, 0, 1);
        pyramid[3] = pointArray[0] + tPoint3(0, 0, 1, 1);
        return;
    }

    pyramid[1] = remainingPoints.front();
    remainingPoints.pop_front();

    tPoint3 axis1 = pyramid[1] - pyramid[0];

// find first remaining point not on line formed by first two
    for(it = remainingPoints.begin(); it != remainingPoints.end(); ++it)
    {
        tPoint3 offset = *it;
        offset -= pyramid[0];
        if(!axis1.inLine(offset))
        {
            break;
        }
    }

    if(it == remainingPoints.end())
    {
    // point cloud reduces to a line
    // add points to force volume
        if(axis1.inLine(tPoint3(1, 0, 0, 1)))
        {
            pyramid[2] = pointArray[0] + tPoint3(0, 1, 0, 1);
            pyramid[3] = pointArray[0] + tPoint3(0, 0, 1, 1);
        }
        else if(axis1.inLine(tPoint3(0, 1, 0, 1)))
        {
            pyramid[2] = pointArray[0] + tPoint3(1, 0, 0, 1);
            pyramid[3] = pointArray[0] + tPoint3(0, 0, 1, 1);
        }
        else
        {
            pyramid[2] = pointArray[0] + tPoint3(1, 0, 0, 1);
            pyramid[3] = pointArray[0] + tPoint3(0, 1, 0, 1);
        }
        return;
    }

    pyramid[2] = *it;
    remainingPoints.erase(it);
    
    tPoint3 axis2 = pyramid[2] - pyramid[0];
//    tPoint3 normal = axis2.crossProduct(axis1);
    cDynamicPoint3 normal;
    cDynamicPoint3(axis2).crossProduct(cDynamicPoint3(axis1), normal);

// find first remaining point not on plane formed by first three
//    tPoint3::tDistance dp;
    cDynamicFraction dp;
    for(it = remainingPoints.begin(); it != remainingPoints.end(); ++it)
    {
        tPoint3 offset = *it;
        offset -= pyramid[0];
        dp = normal.dotProduct(offset);
//        if(!tPoint3::tMultiplier::isZero(dp))
        if(!dp.isZero())
        {
            break;
        }
    }
    if(it == remainingPoints.end())
    {
    // point cloud reduces to a plane
    // add one point to force volume
        tPoint3 offset = tPoint3(1, 0, 0, 1);
        dp = normal.dotProduct(offset);
        if(dp.isZero())
        {
            offset = tPoint3(0, 1, 0, 1);
            dp = normal.dotProduct(offset);
            if(dp.isZero())
            {
                offset = tPoint3(0, 0, 1, 1);
                dp = normal.dotProduct(offset);
                assertD(!dp.isZero());
            }
        }
        pyramid[3] = pointArray[0] + offset;
    }
    else
    {
        pyramid[3] = *it;
        remainingPoints.erase(it);
    }
    if(!dp.isNegative())
    {
    // pyramid[3] must be behind the face formed by the other 3 points
        std::swap(pyramid[2], pyramid[3]);
    }
}

static void
ConvexHull3D_BasePyramid(
        tSigned32 points, const tPoint3* pointArray,
        tMesh_Simple3D& result,
        std::list<tPoint3>& remainingPoints
        )
{
    assertD(result.empty());

    tPoint3 pyramid[4];
    ConvexHull3D_BasePyramidPoints(points, pointArray, pyramid, remainingPoints);

    tFace_Simple3D f;
    tEdge_Simple3D e[12];
    tEdge_Simple3D *ptr = e;
    tEdgeData edges[3];
    tVertexData vertices[3];

    vertices[0] = tVertexData(pyramid[0]);
    vertices[1] = tVertexData(pyramid[1]);
    vertices[2] = tVertexData(pyramid[2]);
    f = result.addFace(tFaceData(), 3, edges,vertices);
    *ptr++ = tMesh_Simple3D::beginEdge(f);
    *ptr++ = tMesh_Simple3D::beginEdge(f).next();
    *ptr++ = tMesh_Simple3D::beginEdge(f).prev();

    vertices[0] = tVertexData(pyramid[1]);
    vertices[1] = tVertexData(pyramid[0]);
    vertices[2] = tVertexData(pyramid[3]);
    f = result.addFace(tFaceData(),3,edges,vertices);
    *ptr++ = tMesh_Simple3D::beginEdge(f);
    *ptr++ = tMesh_Simple3D::beginEdge(f).next();
    *ptr++ = tMesh_Simple3D::beginEdge(f).prev();

    vertices[0] = tVertexData(pyramid[2]);
    vertices[1] = tVertexData(pyramid[1]);
    vertices[2] = tVertexData(pyramid[3]);
    f = result.addFace(tFaceData(),3,edges,vertices);
    *ptr++ = tMesh_Simple3D::beginEdge(f);
    *ptr++ = tMesh_Simple3D::beginEdge(f).next();
    *ptr++ = tMesh_Simple3D::beginEdge(f).prev();

    vertices[0] = tVertexData(pyramid[0]);
    vertices[1] = tVertexData(pyramid[2]);
    vertices[2] = tVertexData(pyramid[3]);
    f = result.addFace(tFaceData(),3,edges,vertices);
    *ptr++ = tMesh_Simple3D::beginEdge(f);
    *ptr++ = tMesh_Simple3D::beginEdge(f).next();
    *ptr++ = tMesh_Simple3D::beginEdge(f).prev();

    result.connectEdges(e[0*3+0], e[1*3+0]);
    result.connectEdges(e[0*3+1], e[2*3+0]);
    result.connectEdges(e[0*3+2], e[3*3+0]);
    result.connectEdges(e[1*3+1], e[3*3+2]);
    result.connectEdges(e[2*3+1], e[1*3+2]);
    result.connectEdges(e[3*3+1], e[2*3+2]);
}

void
ConvexHull3D_AddPoint(tMesh_Simple3D &hull, const tPoint3 &p)
{
    std::vector<eSideOfPlane> sides(hull.faceSize());
    bool outside = false;
    tFace_Simple3D f = hull.beginFace();
    do
    {
        cPlane plane;
        GetOriginAndNormal(f, plane);
        sides[f.index()] = SideOfPlane(plane, p);
        if(sides[f.index()] == IN_FRONT)
        {
            outside = true;
        }
    }
    while((++f).valid());
    
    if(!outside)
    {
        return;
    }

// erase all visible faces

    f = hull.beginFace();
    do
    {
        tFace_Simple3D nextF = f;
        ++nextF;
        if(sides[f.index()] == IN_FRONT)
        {
        // remove this face
            hull.eraseFace(f);
        }
        f = nextF;
    }
    while(f.valid());

// add new faces around boundary
// (or add point to face already there in case of p on plane of face)

    std::vector<tEdge_Simple3D> boundary;
    {
        tEdge_Simple3D e = hull.beginEdge();

        // get first on boundary
        while(e.twin().valid())
        {
            e.toNextInMesh();
        }

        // ensure first in face for ON_PLANE case
        // for the BEHIND case this doesn't matter
        // also note that for BEHIND case this face could be the only face left in the hull
        // in which case the while loop would never terminate
        if(sides[e.face().index()] == ON_PLANE)
        {
            while(!e.next().twin().valid())
            {
                ++e;
            }
        }

        // follow boundary
        do
        {
            // for ON_PLANE case remove boundary edges on this face until there is only one
            if(sides[e.face().index()] == ON_PLANE)
            {
                while(!e.prev().twin().valid())
                {
                    --e;
                    hull.unsplitEdge(e);
                }
            }

            boundary.push_back(e);
            --e;
            while(e.twin().valid())
            {
                e = e.twin().prev();
            }
        }
        while(e != boundary.front());
    }

// add faces to connect the boundary to p

    //.. optimise - construct free vertices first, and then add faces around these

    std::vector<tEdge_Simple3D> left(boundary.size()), right(boundary.size());

    tEdgeData edges[3];
    tVertexData vertices[3];
    vertices[0] = tVertexData(p);
    tSigned32 i;
    for(i = 0; i < SizeL(boundary); i++)
    {
        tEdge_Simple3D e = boundary[i];
        tFace_Simple3D f = e.face();
        if(sides[f.index()] == BEHIND)
        {
        // connect a new face to this edge
            vertices[1] = *(e.next().vertex());
            vertices[2] = *(e.vertex());
            edges[1] = *(e.next());
            edges[2] = *e;
            tFace_Simple3D added = hull.addFace(tFaceData(), 3, edges, vertices);
            hull.connectEdges(e, tMesh_Simple3D::beginEdge(added).next());
            left[i] = tMesh_Simple3D::beginEdge(added);
            right[i] = tMesh_Simple3D::beginEdge(added).prev();
        }
        else
        {
        // p is on plane of this face
        // so split this edge and set mid point to p
            hull.splitEdge(e);
            *(e.next().vertex()) = tVertexData(p);
            left[i] = e.next();
            right[i] = e;
        }
    }

// sew new faces together

    hull.connectEdges(left.front(), right.back());
    for(i = 0; i + 1 < SizeL(left); i++)
    {
        hull.connectEdges(right[i], left[i + 1]);
    }

// check for and remove any inline edges

    for(i = 0; i < SizeL(left); i++)
    {
        tEdge_Simple3D e = left[i];
        tPoint3 axis1 = GetVertexPoint3(e.next()) - GetVertexPoint3(e);
        tPoint3 axis2 = GetVertexPoint3(e.next().next()) - GetVertexPoint3(e.next());
        if(axis1.inLine(axis2))
        {
            hull.unsplitEdge(e);
        }
    }
}

void
ConvexHull3D(tSigned32 points, const tPoint3* pointArray, tMesh_Simple3D& result)
{
    std::list<tPoint3> remainingPoints;
    ConvexHull3D_BasePyramid(points, pointArray, result, remainingPoints);
    if(result.empty())
    {
    //.. check for failure in the case of point sets with no volume
        return;
    }
    while(!remainingPoints.empty())
    {
        ConvexHull3D_AddPoint(result, remainingPoints.front());
        remainingPoints.pop_front();
    }
}
