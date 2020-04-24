//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class iGraphicsInterface;
class controlinterface;

class osinterface
{
public:
	virtual tSigned32 __stdcall getversion(tSigned32 min, tSigned32 max) = 0;
	virtual void* __stdcall getmoduleinterface(char*) = 0;
	virtual iGraphicsInterface* __stdcall getiGraphicsInterface() = 0;
	virtual controlinterface* __stdcall getcontrolinterface() = 0;
	virtual void* __stdcall reservedgetinterface0() = 0;
	virtual void* __stdcall reservedgetinterface1() = 0;
	virtual void* __stdcall reservedgetinterface2() = 0;
	virtual void* __stdcall reservedgetinterface3() = 0;
	virtual void* __stdcall reservedgetinterface4() = 0;
	virtual void* __stdcall reservedgetinterface5() = 0;
	virtual void* __stdcall reservedgetinterface6() = 0;
	virtual void* __stdcall reservedgetinterface7() = 0;
	virtual void* __stdcall reservedgetinterface8() = 0;
	virtual void* __stdcall reservedgetinterface9() = 0;
	virtual tSigned32 __stdcall getwinx() = 0;
	virtual tSigned32 __stdcall getwiny() = 0;
	virtual bool __stdcall isMinimised() = 0;
	virtual void __stdcall reservedgetcontextinfo1() = 0;
	virtual void __stdcall reservedgetcontextinfo2() = 0;
	virtual void __stdcall reservedgetcontextinfo3() = 0;
	virtual void __stdcall reservedgetcontextinfo4() = 0;
	virtual void __stdcall reservedgetcontextinfo5() = 0;
	virtual void __stdcall reservedgetcontextinfo6() = 0;
	virtual void __stdcall reservedgetcontextinfo7() = 0;
	virtual void __stdcall reservedgetcontextinfo8() = 0;
	virtual void __stdcall reservedgetcontextinfo9() = 0;
	virtual void __stdcall update() = 0;
	virtual void __stdcall exit() = 0;
	virtual void __stdcall update_DontBeginScene() = 0;
	virtual void __stdcall update_Redraw() = 0;
	virtual void __stdcall beginScene() = 0;
	virtual void __stdcall sleep(tSigned32 milliseconds) = 0;
};
