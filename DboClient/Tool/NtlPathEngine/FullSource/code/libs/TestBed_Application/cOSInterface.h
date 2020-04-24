//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "interface/iOSInterface.h"

class iApplicationCallBacks
{
public:
    virtual ~iApplicationCallBacks() {}
    virtual bool isMinimised() = 0;
    virtual void update() = 0;
    virtual void update_DontBeginScene() = 0;
    virtual void update_Redraw() = 0;
    virtual void beginScene() = 0;
    virtual void exit() = 0;
};

class cOSInterface : public osinterface
{
    tSigned32 _windowSizeX, _windowSizeY;
    iApplicationCallBacks* _callBacks;

//    cOSInterface() {}

public:

    static cOSInterface& refInstance();

    void setWindowSize(tSigned32 sizeX, tSigned32 sizeY);

    void setApplicationCallBacks(iApplicationCallBacks* callBacks)
    {
        _callBacks = callBacks;
    }

// interface

	tSigned32 __stdcall getversion(tSigned32 min, tSigned32 max);
	void* __stdcall getmoduleinterface(char*);
	iGraphicsInterface* __stdcall getiGraphicsInterface();
	controlinterface* __stdcall getcontrolinterface();
	void* __stdcall reservedgetinterface0();
	void* __stdcall reservedgetinterface1();
	void* __stdcall reservedgetinterface2();
	void* __stdcall reservedgetinterface3();
	void* __stdcall reservedgetinterface4();
	void* __stdcall reservedgetinterface5();
	void* __stdcall reservedgetinterface6();
	void* __stdcall reservedgetinterface7();
	void* __stdcall reservedgetinterface8();
	void* __stdcall reservedgetinterface9();
	tSigned32 __stdcall getwinx();
	tSigned32 __stdcall getwiny();
	bool __stdcall isMinimised();
	void __stdcall reservedgetcontextinfo1();
	void __stdcall reservedgetcontextinfo2();
	void __stdcall reservedgetcontextinfo3();
	void __stdcall reservedgetcontextinfo4();
	void __stdcall reservedgetcontextinfo5();
	void __stdcall reservedgetcontextinfo6();
	void __stdcall reservedgetcontextinfo7();
	void __stdcall reservedgetcontextinfo8();
	void __stdcall reservedgetcontextinfo9();
	void __stdcall update();
	void __stdcall exit();
	void __stdcall update_DontBeginScene();
	void __stdcall update_Redraw();
	void __stdcall beginScene();
	void __stdcall sleep(tSigned32 milliseconds);
};
