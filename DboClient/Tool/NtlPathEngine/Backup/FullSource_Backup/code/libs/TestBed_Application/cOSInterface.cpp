//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "cOSInterface.h"
#include "cGraphicsInterface.h"
#include "cControlInterface.h"
#include "common/interface/Assert.h"
#include <windows.h>

cOSInterface&
cOSInterface::refInstance()
{
    static cOSInterface theInstance;
    return theInstance;
}

void
cOSInterface::setWindowSize(tSigned32 sizeX, tSigned32 sizeY)
{
    _windowSizeX = sizeX;
    _windowSizeY = sizeY;
}

tSigned32
cOSInterface::getversion(tSigned32 min, tSigned32 max)
{
    assertD(min<=max);
    assertD(min);
    assertD(max);
    if(min>1)
        return -1;
    return 1;
}

void*
cOSInterface::getmoduleinterface(char *name)
{
    invalid();
    return 0;
}

iGraphicsInterface*
cOSInterface::getiGraphicsInterface()
{
    return &cGraphicsInterface::refInstance();
}
controlinterface*
cOSInterface::getcontrolinterface()
{
    return &cControlInterface::refInstance();
}

void* cOSInterface::reservedgetinterface0()
{
    return 0;
}
void* cOSInterface::reservedgetinterface1()
{
    return 0;
}

void* cOSInterface::reservedgetinterface2()
{
    return 0;
}

void* cOSInterface::reservedgetinterface3()
{
    return 0;
}

void* cOSInterface::reservedgetinterface4()
{
    return 0;
}

void* cOSInterface::reservedgetinterface5()
{
    return 0;
}

void* cOSInterface::reservedgetinterface6()
{
    return 0;
}

void* cOSInterface::reservedgetinterface7()
{
    return 0;
}

void* cOSInterface::reservedgetinterface8()
{
    return 0;
}

void* cOSInterface::reservedgetinterface9()
{
    return 0;
}

tSigned32
cOSInterface::getwinx()
{
    return _windowSizeX;
}
tSigned32
cOSInterface::getwiny()
{
    return _windowSizeY;
}
bool
cOSInterface::isMinimised()
{
    return _callBacks->isMinimised();
}
void cOSInterface::reservedgetcontextinfo1()
{
}
void cOSInterface::reservedgetcontextinfo2()
{
}
void cOSInterface::reservedgetcontextinfo3()
{
}
void cOSInterface::reservedgetcontextinfo4()
{
}
void cOSInterface::reservedgetcontextinfo5()
{
}
void cOSInterface::reservedgetcontextinfo6()
{
}
void cOSInterface::reservedgetcontextinfo7()
{
}
void cOSInterface::reservedgetcontextinfo8()
{
}
void cOSInterface::reservedgetcontextinfo9()
{
}

void
cOSInterface::update()
{
    _callBacks->update();
}
void
cOSInterface::exit()
{
    _callBacks->exit();
}
void
cOSInterface::update_DontBeginScene()
{
    _callBacks->update_DontBeginScene();
}
void
cOSInterface::update_Redraw()
{
    _callBacks->update_Redraw();
}
void
cOSInterface::beginScene()
{
    _callBacks->beginScene();
}
void
cOSInterface::sleep(tSigned32 milliseconds)
{
    Sleep(milliseconds);
}
