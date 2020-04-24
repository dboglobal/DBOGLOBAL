//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "interface/iControlInterface.h"

class cControlInterface : public controlinterface
{
    tSigned32 _mouseX, _mouseY;
    bool _leftMouseDown;
    bool _middleMouseDown;
    bool _rightMouseDown;
    tSigned32 _previousMouseX, _previousMouseY;
    tSigned32 _mouseDeltaX, _mouseDeltaY;

    cControlInterface()
    {
        _mouseX = 0;
        _mouseY = 0;
        _leftMouseDown = false;
        _middleMouseDown = false;
        _rightMouseDown = false;
        _previousMouseX = 0;
        _previousMouseY = 0;
        _mouseDeltaX = 0;
        _mouseDeltaY = 0;
    }

public:

    static cControlInterface& refInstance();

    void updateMouseDeltas();
    void setMouseDeltasTo(tSigned32 deltaX, tSigned32 deltaY);

    void setMousePosition(tSigned32 x, tSigned32 y)
    {
        _mouseX = x;
        _mouseY = y;
    }
    void leftMouseDown()
    {
        _leftMouseDown = true;
    }
    void leftMouseUp()
    {
        _leftMouseDown = false;
    }
    void middleMouseDown()
    {
        _middleMouseDown = true;
    }
    void middleMouseUp()
    {
        _middleMouseDown = false;
    }
    void rightMouseDown()
    {
        _rightMouseDown = true;
    }
    void rightMouseUp()
    {
        _rightMouseDown = false;
    }

// interface

	tSigned32 __stdcall getversion(tSigned32 min, tSigned32 max);
	
	tSigned32 __stdcall getmousex();
	tSigned32 __stdcall getmousey();
	tSigned32 __stdcall getlmbdown();
	tSigned32 __stdcall getmmbdown();
	tSigned32 __stdcall getrmbdown();
	tSigned32 __stdcall getismousevalid();
	void __stdcall reserved_mouse0();
	void __stdcall reserved_mouse1();
	void __stdcall reserved_mouse2();
	void __stdcall reserved_mouse3();
	void __stdcall reserved_mouse4();
	void __stdcall reserved_mouse5();
	void __stdcall reserved_mouse6();
	void __stdcall reserved_mouse7();
	void __stdcall reserved_mouse8();
	void __stdcall reserved_mouse9();

	tSigned32 __stdcall getkeystate(const char* keystring);
	char* __stdcall getkeymessage();
	void __stdcall reserved_kbd0();
	void __stdcall reserved_kbd1();
	void __stdcall reserved_kbd2();
	void __stdcall reserved_kbd3();
	void __stdcall reserved_kbd4();
	void __stdcall reserved_kbd5();
	void __stdcall reserved_kbd6();
	void __stdcall reserved_kbd7();
	void __stdcall reserved_kbd8();
	void __stdcall reserved_kbd9();

// extra calls in version 2 interface
	tSigned32 __stdcall getMouseDX();
	tSigned32 __stdcall getMouseDY();
};
