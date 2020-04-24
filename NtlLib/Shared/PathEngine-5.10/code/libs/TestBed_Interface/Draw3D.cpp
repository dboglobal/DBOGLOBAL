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
#include "Draw3D.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "i_pathengine.h"
#include <math.h>

void
Draw3DLine(iGraphicsInterface* gip, const cVector3F& start, const cVector3F& end)
{
    float data[6];
    data[0]=start.x();
    data[1]=start.y();
    data[2]=start.z();
    data[3]=end.x();
    data[4]=end.y();
    data[5]=end.z();
    iRenderGeometry* rg = gip->newLine(data);
    gip->render(rg);
}

void
Draw3DCross(iGraphicsInterface* gip, const cVector3F& p, float size)
{
//    cVector3F xOffset(static_cast<float>(size), 0.0f, 0.0f);
//    cVector3F yOffset(0.0f, static_cast<float>(size), 0.0f);
//    cVector3F zOffset(0.0f, 0.0f, static_cast<float>(size));
    cVector3F xOffset(size, 0.0f, 0.0f);
    cVector3F yOffset(0.0f, size, 0.0f);
    cVector3F zOffset(0.0f, 0.0f, size);
    Draw3DLine(gip, p - xOffset, p + xOffset);
    Draw3DLine(gip, p - yOffset, p + yOffset);
    Draw3DLine(gip, p - zOffset, p + zOffset);
}

void
DrawAgentWithPrecision(
        iGraphicsInterface* gip,
        const cConvexPoly& placedPoly,
        tSigned32 zAsLong,
        tSigned32 height,
        float precisionX, float precisionY
        )
{
    float data[4 * 3];
    tSigned32 i;
    float z = static_cast<float>(zAsLong);
    data[0 * 3 + 2] = z;
    data[3 * 3 + 2] = z;
    data[2 * 3 + 2] = z + height;
    data[1 * 3 + 2] = z + height;
    for(i = 0; i < placedPoly.sides(); i++)
    {
        tSigned32 x, y, nextX, nextY;
        x = placedPoly.vertex(i).getX();
        y = placedPoly.vertex(i).getY();
        nextX = placedPoly.vertex_WrappedAround(i + 1).getX();
        nextY = placedPoly.vertex_WrappedAround(i + 1).getY();

        data[0 * 3 + 0] = static_cast<float>(nextX) + precisionX;
        data[0 * 3 + 1] = static_cast<float>(nextY) + precisionY;
        data[1 * 3 + 0] = static_cast<float>(nextX) + precisionX;
        data[1 * 3 + 1] = static_cast<float>(nextY) + precisionY;

        data[3 * 3 + 0] = static_cast<float>(x) + precisionX;
        data[3 * 3 + 1] = static_cast<float>(y) + precisionY;
        data[2 * 3 + 0] = static_cast<float>(x) + precisionX;
        data[2 * 3 + 1] = static_cast<float>(y) + precisionY;

        iRenderGeometry* rg = gip->newPolygon(4, data);
        gip->render(rg);
    }

    if(placedPoly.sides() <= 20)
    {
        float data[20 * 3];
        for(i = 0; i < placedPoly.sides(); i++)
        {
            tSigned32 x, y;
            x = placedPoly.vertex(i).getX();
            y = placedPoly.vertex(i).getY();
            data[i * 3 + 0] = static_cast<float>(x) + precisionX;
            data[i * 3 + 1] = static_cast<float>(y) + precisionY;
            data[i * 3 + 2] = z + height;
        }
        iRenderGeometry* rg = gip->newPolygon(placedPoly.sides(), data);
        gip->render(rg);
    }
    //... shape with more than 20 vertices doesnt get a top polygon
}

void
DrawAgentArrowWithPrecision(iGraphicsInterface* gip, const cPosition& p, float direction, tSigned32 _z, float size, tSigned32 height, float precisionX, float precisionY)
{
    float data[3*3];
    float sinof,cosof;
    float z=static_cast<float>(_z);
    sinof=static_cast<float>(sin(double(direction)));
    cosof=static_cast<float>(cos(double(direction)));

    data[0*3+0]=p.x+size*sinof + precisionX;
    data[0*3+1]=p.y+size*cosof + precisionY;
    data[0*3+2]=z+height;

    data[1*3+0]=p.x+size*cosof*0.25f + precisionX;
    data[1*3+1]=p.y-size*sinof*0.25f + precisionY;
    data[1*3+2]=z+height;

    data[2*3+0]=p.x-size*cosof*0.25f + precisionX;
    data[2*3+1]=p.y+size*sinof*0.25f + precisionY;
    data[2*3+2]=z+height;

    iRenderGeometry* rg = gip->newPolygon(3, data);
    gip->render(rg);
}
