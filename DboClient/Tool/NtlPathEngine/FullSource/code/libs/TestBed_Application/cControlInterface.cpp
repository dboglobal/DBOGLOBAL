//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "cControlInterface.h"
#include "KeyBuffer.h"
#include "KeyCodes.h"
#include "common/interface/Assert.h"
#include <windows.h>

cControlInterface&
cControlInterface::refInstance()
{
    static cControlInterface theInstance;
    return theInstance;
}

void
cControlInterface::updateMouseDeltas()
{
    _mouseDeltaX = _mouseX - _previousMouseX;
    _mouseDeltaY = _mouseY - _previousMouseY;
    _previousMouseX = _mouseX;
    _previousMouseY = _mouseY;
}

void
cControlInterface::setMouseDeltasTo(tSigned32 dx, tSigned32 dy)
{
    _mouseDeltaX = dx;
    _mouseDeltaY = dy;
}

tSigned32
cControlInterface::getversion(tSigned32 min, tSigned32 max)
{
    assertD(min<=max);
    assertD(min);
    assertD(max);
    if(min>2)
        return -1;
    if(max>=2)
        return 2;
    return 1;
}

tSigned32 cControlInterface::getmousex()
{
    return _mouseX;
}
tSigned32 cControlInterface::getmousey()
{
    return _mouseY;
}

tSigned32 cControlInterface::getMouseDX()
{
    return _mouseDeltaX;
}
tSigned32 cControlInterface::getMouseDY()
{
    return _mouseDeltaY;
}

tSigned32 cControlInterface::getlmbdown()
{
    return _leftMouseDown;
}
tSigned32 cControlInterface::getmmbdown()
{
    return _middleMouseDown;
}
tSigned32 cControlInterface::getrmbdown()
{
    return _rightMouseDown;
}
tSigned32 cControlInterface::getismousevalid()
{
    return 1;
}
void cControlInterface::reserved_mouse0()
{
    invalid();
}
void cControlInterface::reserved_mouse1()
{
    invalid();
}
void cControlInterface::reserved_mouse2()
{
    invalid();
}
void cControlInterface::reserved_mouse3()
{
    invalid();
}
void cControlInterface::reserved_mouse4()
{
    invalid();
}
void cControlInterface::reserved_mouse5()
{
    invalid();
}
void cControlInterface::reserved_mouse6()
{
    invalid();
}
void cControlInterface::reserved_mouse7()
{
    invalid();
}
void cControlInterface::reserved_mouse8()
{
    invalid();
}
void cControlInterface::reserved_mouse9()
{
    invalid();
}

tSigned32
cControlInterface::getkeystate(const char* keyString)
{
    int keyCode;
    short rc;
    keyCode = CodeForKeyString(keyString);
    if(keyCode == 0)
    {
        return 0;
    }
    rc = GetAsyncKeyState(keyCode);
    return (rc & 0x8000) != 0;
}

char*
cControlInterface::getkeymessage()
{
    return nKeyBuffer::getKeyMessage();
}

void cControlInterface::reserved_kbd0()
{
    invalid();
}
void cControlInterface::reserved_kbd1()
{
    invalid();
}
void cControlInterface::reserved_kbd2()
{
    invalid();
}
void cControlInterface::reserved_kbd3()
{
    invalid();
}
void cControlInterface::reserved_kbd4()
{
    invalid();
}
void cControlInterface::reserved_kbd5()
{
    invalid();
}
void cControlInterface::reserved_kbd6()
{
    invalid();
}
void cControlInterface::reserved_kbd7()
{
    invalid();
}
void cControlInterface::reserved_kbd8()
{
    invalid();
}
void cControlInterface::reserved_kbd9()
{
    invalid();
}

