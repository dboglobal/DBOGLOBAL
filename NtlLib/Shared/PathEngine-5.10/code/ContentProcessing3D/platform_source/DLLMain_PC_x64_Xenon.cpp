//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "cContentProcessing3D.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "common/MemoryLog.h"
#include "common/MemoryTracking.h"
#include "common/MemoryOverwriteChecking.h"
#include "common/MemoryOverwriteChecking2.h"
#include "common/LoggingHooks.h"
#include "platform_common/WindowsOSHeader.h"
#include "common/interface/Error.h"
#include "i_pathengine.h"

cContentProcessing3D gContentProcessing3D;

extern "C"
{
    iContentProcessing3D* __stdcall DLLExport_GetIContentProcessing3D(iErrorHandler* handler)
    {
        if(!nSingletons::isInitialised_StandAlone())
        {
            nSingletons::init_StandAlone();
        }
        iPathEngine* pathEngine = &nSingletons::pathEngineI();
        pathEngine->setErrorHandler(handler);
        return &gContentProcessing3D;
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
