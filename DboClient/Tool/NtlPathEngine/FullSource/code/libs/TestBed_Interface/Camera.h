//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/Vector3F.h"

#ifdef NOEXTERN
#define extern
#endif
extern cVector3F g_target_position;
extern float g_camera_distance;
extern float g_camera_angle1;
extern float g_camera_angle2;
#undef extern

void InitCamera(tSigned32 windowWidth, tSigned32 windowHeight);
void LookAt(const cVector3F& target, float angle, float elevation, float distance);

void StartZoom();
void Zoom(float percent);
void StartDolly();
void Dolly(float percent);
void StartPan();
void Pan(tSigned32 dx, tSigned32 dy);
void StartRotate();
void Rotate(float dtheta1, float dtheta2);

cVector3F GetCameraPosition();
cVector3F ReverseProject(tSigned32 screenX, tSigned32 screenY);
