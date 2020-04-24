//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "i_testbed.h"
#include "libs/TestBed_Application/interface/ClientEntryPoint.h"
#include "libs/TestBed_Application/interface/iOSInterface.h"
#include "libs/TestBed_Application/cOSInterface.h"
#include "libs/TestBed_Application/cApplicationCallBacks.h"
#include "libs/TestBed_Application/D3D.h"
#include "libs/TestBed_Application/MouseDX8.h"
#include "libs/TestBed_Interface/interface/Singletons_TestBed.h"
#include "libs/TestBed_Interface/interface/RunClient.h"
#include "platform_common/BasicErrorHandler.h"
#include "common/MemoryLog.h"
#include "common/MemoryTracking.h"
#include "common/MemoryOverwriteChecking.h"
#include "common/MemoryOverwriteChecking2.h"
#include "common/interface/Error.h"

#include <stdio.h>
#include <string>
#include <windows.h>

iGraphicsInterface *gip;
controlinterface *cip;
osinterface *ip;

extern "C" __declspec(dllexport) 
iTestBed* DLLExport_GetITestBed( HWND hWnd, int nSizeX, int nSizeY )
{
	if(!nSingletons::isInitialised_TestBed())
	{
		HRESULT result = cD3D::refInstance().init(hWnd, nSizeX, nSizeY);
		if(result != S_OK)
		{
			// ErrorLog
			return NULL;
		}

		SetGlobalErrorHandler(&cBasicErrorHandler::refInstance());
		::ip=&cOSInterface::refInstance();
		gip=ip->getiGraphicsInterface();
		cip=ip->getcontrolinterface();

		if(cConfig::GetInstance().useDInput)
		{
			HRESULT result = InitMouseDX8(hWnd);
			if(result != S_OK)
			{
				MessageBox(0, "Failed to setup direct input.", "Error", MB_OK | MB_ICONSTOP);
				exit(EXIT_FAILURE);
			}
		}

		static cApplicationCallBacks callBacks(hWnd);
		cOSInterface::refInstance().setApplicationCallBacks(&callBacks);
		cOSInterface::refInstance().setWindowSize(nSizeX, nSizeY);
		nSingletons::init_TestBed( ip );

		return &nSingletons::testBedI();
	}
	else
	{
		return &nSingletons::testBedI();
	}
}

BOOL APIENTRY DllMain(HINSTANCE hi, DWORD reasonForCall, LPVOID)
{
	switch(reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		if(nSingletons::isInitialised_TestBed())
		{
			nSingletons::shutDown_TestBed();
		}
		break;
	}
	return true;
}

//const char *name=0;
//void ProcessArgument(const char *argument)
//{
//    if(strncmp(argument,"app=",4)==0)
//        name=argument+4;
//}
//
//void ClientEntryPoint(osinterface *ip, int argc, char **argv)    
//{        
//    SetGlobalErrorHandler(&cBasicErrorHandler::refInstance());
//    ::ip=ip;
//    gip=ip->getiGraphicsInterface();
//    cip=ip->getcontrolinterface();
//
//    while(argc)
//    {
//        ProcessArgument(*argv);
//        argc--;
//        argv++;
//    }
//
//    if(name==0)
//        name="testapplication";
//    nSingletons::init_TestBed(ip);
//    RunClient(name);
//    nSingletons::shutDown_TestBed();
//}
