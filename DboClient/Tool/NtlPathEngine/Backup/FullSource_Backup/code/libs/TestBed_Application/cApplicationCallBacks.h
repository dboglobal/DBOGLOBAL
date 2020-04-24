#ifndef __C_APPLICATION_CALLBACKS_H__
#define __C_APPLICATION_CALLBACKS_H__

#include "cOSInterface.h"

#include <string.h>
#include <windows.h>
#include <stdio.h>

class cConfig
{
public:
	static cConfig& GetInstance()
	{
		static cConfig instance;
		return instance;
	}

	bool useDInput;
	tSigned32 sizeX;
	tSigned32 sizeY;

	cConfig();
};

class cApplicationCallBacks : public iApplicationCallBacks
{
public:
	cApplicationCallBacks(HWND hWnd);
	virtual ~cApplicationCallBacks() {}

	virtual bool isMinimised();
	virtual void update();
	virtual void update_DontBeginScene();
	virtual void update_Redraw();
	virtual void beginScene();
	virtual void exit();

protected:
	HWND hWnd;
};

#endif