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
#define NOEXTERN
#include "camera.h"
#undef NOEXTERN
#include "interface/Singletons_TestBed.h"
#include "cTestBed.h"
#include "libs/Geometry/interface/Point3.h"
#include "platform_common/PointMultiplication.h"
#include "i_pathengine.h"

#include <math.h>

const float PI=3.1428f;

static tSigned32 gWindowWidth, gWindowHeight;

void InitCamera(tSigned32 windowWidth, tSigned32 windowHeight)
{
    g_target_position=cVector3F(static_cast<float>(windowWidth) / 2, static_cast<float>(windowHeight) / 2, 0.0f);
    g_camera_distance=1280.0f;
    g_camera_angle1=PI;
    g_camera_angle2=PI/2.0f;
    gWindowWidth = windowWidth;
    gWindowHeight = windowHeight;
}

void
LookAt(const cVector3F& target, float angle, float elevation, float distance)
{
    g_target_position = target;
    g_camera_angle1 = angle;
    g_camera_angle2 = elevation;
    g_camera_distance = distance;
}

void VectorForAngles(float angle1, float angle2, float distance, cVector3F &vector)
{
    float xy_component=cosf(angle2)*distance;
    vector=cVector3F(
        sinf(angle1)*xy_component,
        cosf(angle1)*xy_component,
        sinf(angle2)*distance);
}

double g_log_of_start_distance;
void StartZoom()
{
    g_log_of_start_distance=log(g_camera_distance);
}
void Zoom(float percent)
{
    double zoomedlog=g_log_of_start_distance*percent;
    g_camera_distance=static_cast<float>(pow(2.7182818,zoomedlog));
}

cVector3F g_dolly_vector;
cVector3F g_dolly_start;
void StartDolly()
{
    VectorForAngles(g_camera_angle1,g_camera_angle2,-g_camera_distance,g_dolly_vector);
    g_dolly_start=g_target_position+g_dolly_vector;
}
void Dolly(float percent)
{
    g_target_position=g_dolly_start-g_dolly_vector*percent;
}


cVector3F g_target_start_position;
cVector3F g_up_vector;
cVector3F g_right_vector;
void StartPan()
{
    cVector3F cameravector;
    VectorForAngles(g_camera_angle1,g_camera_angle2,-1.0f,cameravector);

    {
        cPosition position=nSingletons::testBed().positionAtMouse();
        if(position.cell!=-1)
        {
        // move target to be at same distance along cameravector as this point
            tSigned32 height=nSingletons::testBed().getMesh()->heightAtPosition(position);
            cVector3F p(
                static_cast<float>(position.x),
                static_cast<float>(position.y),
                static_cast<float>(height)
                );
            cVector3F offset_to_p = p-g_target_position;
            float distance = offset_to_p.dotProduct(cameravector);
            g_camera_distance += distance;
            g_target_position += cameravector*distance;
        }
    }

    cVector3F cross,up,right;
    VectorForAngles(g_camera_angle1,g_camera_angle2+PI/2.0f,1.0f,up);
    cross=cameravector.crossProduct(up);
    right=cross;
//    SetLength(right,1.0f);
    right.scaleToUnitLength();

    float ratio = g_camera_distance/1280.0f;
    g_up_vector=up*ratio;
    g_right_vector=right*ratio;
    g_target_start_position=g_target_position;
}
void Pan(tSigned32 dx, tSigned32 dy)
{
    float fdx=static_cast<float>(dx);
    float fdy=static_cast<float>(dy);
    g_target_position=g_target_start_position-g_right_vector*fdx-g_up_vector*fdy;
}

float g_camera_start_angle1;
float g_camera_start_angle2;
void StartRotate()
{
    g_camera_start_angle1=g_camera_angle1;
    g_camera_start_angle2=g_camera_angle2;
}
void Rotate(float dtheta1, float dtheta2)
{
    g_camera_angle1=g_camera_start_angle1+dtheta1;
    g_camera_angle1=static_cast<float>(fmod(g_camera_angle1,PI*2.0f));
    g_camera_angle2=g_camera_start_angle2+dtheta2;
    g_camera_angle2=static_cast<float>(fmod(g_camera_angle2,PI*2.0f));
}

cVector3F
GetCameraPosition()
{
    cVector3F cameraVector;
    VectorForAngles(g_camera_angle1, g_camera_angle2, g_camera_distance, cameraVector);
    return g_target_position + cameraVector;
}

cVector3F
ReverseProject(tSigned32 screenX, tSigned32 screenY)
{
    cVector3F cameraVector;
    VectorForAngles(g_camera_angle1, g_camera_angle2, -1.0f, cameraVector);
    cVector3F toScreenCentre = cameraVector * (1280.0f - g_camera_distance);
    cVector3F cross,up,right;
    VectorForAngles(g_camera_angle1,g_camera_angle2+PI/2.0f,1.0f,up);
    cross = cameraVector.crossProduct(up);
    right=cross;
    right.scaleToUnitLength();
    return g_target_position +
        toScreenCentre +
        right * static_cast<float>(screenX - gWindowWidth / 2) +
        up * static_cast<float>(screenY - gWindowHeight / 2);
}
