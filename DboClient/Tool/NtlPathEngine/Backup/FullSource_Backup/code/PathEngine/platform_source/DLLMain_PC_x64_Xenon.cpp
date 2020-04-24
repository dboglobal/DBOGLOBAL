//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "common/MemoryLog.h"
#include "common/MemoryTracking.h"
#include "common/MemoryOverwriteChecking.h"
#include "common/MemoryOverwriteChecking2.h"
#include "common/LoggingHooks.h"
#include "platform_common/WindowsOSHeader.h"
#include "common/interface/Error.h"
#include "i_pathengine.h"

extern "C"
{
    iPathEngine* __stdcall DLLExport_GetIPathEngine(iErrorHandler* handler)
    {
        if(!nSingletons::isInitialised_StandAlone())
        {
            nSingletons::init_StandAlone();
        }
        iPathEngine* pathEngine = &nSingletons::pathEngineI();
        pathEngine->setErrorHandler(handler);
        return pathEngine;
    }
    iPathEngine* __stdcall DllExport_GetIPathEngine(iErrorHandler* handler)
    {
        if(!nSingletons::isInitialised_StandAlone())
        {
            nSingletons::init_StandAlone();
        }
        iPathEngine* pathEngine = &nSingletons::pathEngineI();
        pathEngine->setErrorHandler(handler);
        Error("Warning", "Obtaining the PathEngine DLL entrypoint by name, with GetProcAddress(hInstance, \"_DllExport_GetIPathEngine@4\") is depreciated. Use GetProcAddress(hInstance, (LPCSTR)1) instead.");
        return pathEngine;
    }
}

BOOL APIENTRY DllMain(HINSTANCE hi, DWORD reasonForCall, LPVOID)
{
    switch(reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        if(nSingletons::isInitialised_StandAlone())
        {
            nSingletons::shutDown_StandAlone();
        }
        break;
    }
    return true;
}
