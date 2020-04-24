//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ZoomExtents.h"
#include "Camera.h"
#include "cTestBed.h"
#include "interface/Singletons_TestBed.h"
#include "libs/TestBed_Application/interface/iOSInterface.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"

float DistanceForX(float x, float z, float halfWindowWidth)
{
    if(x<0)
        x=-x;
    return z+x/halfWindowWidth*1280.0f;
}
float DistanceForY(float y, float z, float halfWindowHeight)
{
    if(y<0)
        y=-y;
    return z+y/halfWindowHeight*1280.0f;
}

void
ZoomExtents(const std::vector<tPoint3>& points, tSigned32 windowWidth, tSigned32 windowHeight)
{
    assertD(!points.empty());
    float minx,maxx,miny,maxy,minz,maxz;
    {
        tPoint3 p = points[0];
        minx = static_cast<float>(p.getX());
        maxx = static_cast<float>(p.getX());
        miny = static_cast<float>(p.getY());
        maxy = static_cast<float>(p.getY());
        minz = static_cast<float>(p.getZ());
        maxz = static_cast<float>(p.getZ());
    }
    tSigned32 i;
    for(i = 1; i != points.size(); ++i)
    {
        tPoint3 p = points[i];
        if(p.getX() < minx)
            minx = static_cast<float>(p.getX());
        if(p.getX() > maxx)
            maxx = static_cast<float>(p.getX());
        if(p.getY() < miny)
            miny = static_cast<float>(p.getY());
        if(p.getY() > maxy)
            maxy = static_cast<float>(p.getY());
        if(p.getZ() < minz)
            minz = static_cast<float>(p.getZ());
        if(p.getZ() > maxz)
            maxz = static_cast<float>(p.getZ());
    }

    float range;
    float centreX,centreY,centreZ;
    range=maxx-minx;
    centreX=minx+range/2;
    range=maxy-miny;
    centreY=miny+range/2;
    range=maxz-minz;
    centreZ=minz+range/2;

    InitCamera(windowWidth, windowHeight); // sets angle to view from above
    g_target_position = cVector3F(centreX,centreY,centreZ);
    g_camera_distance=10;

    float halfWindowWidth = static_cast<float>(windowWidth) / 2;
    float halfWindowHeight = static_cast<float>(windowHeight) / 2;

    for(i = 0; i != SizeL(points); ++i)
    {
        tPoint3 p = points[i];
        float z = p.getZ() - centreZ;
        float distance;
        distance = DistanceForX(p.getX()-centreX, z, halfWindowWidth);
        if(distance>g_camera_distance)
            g_camera_distance = distance;
        distance = DistanceForY(p.getY()-centreY, z, halfWindowHeight);
        if(distance>g_camera_distance)
            g_camera_distance = distance;
    }

    g_camera_distance*=1.1f;
}


void
ZoomExtents(tMesh_3D& mesh, tSigned32 windowWidth, tSigned32 windowHeight)
{
    std::vector<tPoint3> points;
    tFace_3D f = mesh.beginFace();
    while(f.valid())
    {
        tEdge_3D e = tMesh_3D::beginEdge(f);
        do
        {
            points.push_back(GetVertexPoint3(e));
        }
        while(e!=tMesh_3D::beginEdge(f));
        ++f;
    }
    ZoomExtents(points, windowWidth, windowHeight);
}

void
ZoomExtents(const std::vector<tPoint3>& points)
{
    cTestBed& testBed = nSingletons::testBed();
    osinterface* ip = testBed.getIP();
    ZoomExtents(points, ip->getwinx(), ip->getwiny());
}
