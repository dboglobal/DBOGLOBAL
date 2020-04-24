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
#include "interface/DrawLineOnGround.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "i_pathengine.h"

void
DrawLineOnGround(tFace_3D f, const tLine& line, eSide spinThroughSide,
                 iPointLike& target, const cVector2F& endP,
                 const tPoint& origin,
                 iRender3DLinesCallBack& callBack)
{
    tEdge_3D crossed;
    bool crossedAtVertex;
    bool exited;
    while(TraverseTowardsTarget_OverlappingFaces<tMesh_3D>(f, line, spinThroughSide, target, crossed, crossedAtVertex, exited))
    {
        cVector2F exitP;
        if(crossedAtVertex)
        {
            exitP = GetVertexPoint(crossed);
        }
        else
        {
            tPoint::tDistance n, d;
            tIntersection intersection(line, GetEdgeLine(crossed));
            intersection.getAxisFraction(n, d);
            exitP = line.start() - origin;
            cVector2F axis = line.axis();
            axis *= tMultiplier::asFloat(n);
            axis /= tMultiplier::asFloat(d);
            exitP += axis;
        }
        float exitHeight = crossed.face()->heightForF(exitP);
        callBack.vertex(exitP.x, exitP.y, exitHeight);
    }
    cVector2F exitP;
    if(!exited)
    {
        exitP = endP;
    }
    else if(crossedAtVertex)
    {
        exitP = GetVertexPoint(crossed);
    }
    else
    {
        tPoint::tDistance n, d;
        tIntersection intersection(line, GetEdgeLine(crossed));
        intersection.getAxisFraction(n, d);
        exitP = line.start() - origin;
        cVector2F axis = line.axis();
        axis *= tMultiplier::asFloat(n);
        axis /= tMultiplier::asFloat(d);
        exitP += axis;
    }

    if(exited)
    {
        f = crossed.face();
    }

    float exitHeight = f->heightForF(exitP);
    callBack.vertex(exitP.x, exitP.y, exitHeight);
}

void
DrawLineOnGround(tFace_3D f, const tLine& line, eSide spinThroughSide,
                 const tPoint& origin, bool connectsToPrevious,
                 iRender3DLinesCallBack& callBack)
{
    cPoint_PointLike target(line.end());
    if(!connectsToPrevious)
    {
        cVector2F startP = line.start() - origin;
        float startHeight = f->heightForF(startP);
        callBack.startVertex(startP.x, startP.y, startHeight);
    }
    DrawLineOnGround(f, line, spinThroughSide,
        target, line.end() - origin,
        origin, 
        callBack
        );
}

