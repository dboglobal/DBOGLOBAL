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
#include "cameracontrol.h"
#include "camera.h"
#include "libs/TestBed_Application/interface/iControlInterface.h"
#include "libs/TestBed_Application/interface/iOSInterface.h"

enum eCameraState
{
    STATIC,
    ZOOMING,
    DOLLYING,
    PANNING,
    ROTATING,
};

eCameraState CameraState(controlinterface *cip)
{
    if(cip->getkeystate("1"))
        return ROTATING;
    if(cip->getkeystate("2"))
        return PANNING;
    if(cip->getkeystate("3"))
        if(cip->getkeystate("_LSHIFT"))
            return DOLLYING;
        else
            return ZOOMING;

    if(!cip->getkeystate("_LCONTROL"))
        return STATIC;
    if(cip->getlmbdown() && cip->getmmbdown())
    {
        if(cip->getkeystate("_LSHIFT"))
            return DOLLYING;
        else
            return ZOOMING;
    }
    if(cip->getmmbdown())
        return PANNING;
    if(cip->getlmbdown())
        return ROTATING;
    return STATIC;
}

static eCameraState state=STATIC;

bool ShouldCameraTargetBeDisplayed()
{
    return state!=STATIC;
}

void CameraControl(controlinterface* cip, osinterface* ip)
{
    eCameraState newstate=CameraState(cip);
    static tSigned32 startx,starty;
    static tSigned32 dx;
    static tSigned32 dy;
    if(newstate!=state)
    {
        switch(newstate)
        {
        case ZOOMING:
            StartZoom();
            break;
        case DOLLYING:
            StartDolly();
            break;
        case PANNING:
            StartPan();
            break;
        case ROTATING:
            StartRotate();
            break;
        }
        startx=cip->getmousex();
        starty=cip->getmousey();
        dx=0;
        dy=0;
        state=newstate;
    }
    else
    {
        if(cip->getversion(1,2)==1)
        {
            dx=cip->getmousex()-startx;
            dy=cip->getmousey()-starty;
        }
        else
        {
            dx+=cip->getMouseDX();
            dy+=cip->getMouseDY();
        }
        switch(state)
        {
        case ZOOMING:
            {
                float percent=(ip->getwinx()-dx)/static_cast<float>(ip->getwinx());
                Zoom(percent);
            }
            break;
        case DOLLYING:
            {
                float percent=(ip->getwinx()-dx)/static_cast<float>(ip->getwinx());
                Dolly(percent);
            }
            break;
        case PANNING:
            Pan(dx,dy);
            break;
        case ROTATING:
            float dtheta1=dx*3.14f/static_cast<float>(ip->getwinx());
            float dtheta2=dy*3.14f/static_cast<float>(ip->getwiny());
            Rotate(dtheta1,dtheta2);
            break;
        }
    }
}
