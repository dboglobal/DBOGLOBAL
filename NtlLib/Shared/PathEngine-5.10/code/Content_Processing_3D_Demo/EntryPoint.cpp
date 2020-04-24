//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "Demo.h"
#include "libs/TestBed_Application/interface/ClientEntryPoint.h"
#include "libs/TestBed_Interface/interface/Singletons_TestBed.h"
#include "platform_common/BasicErrorHandler.h"
#include "common/MemoryLog.h"
#include "common/MemoryTracking.h"
#include "common/MemoryOverwriteChecking.h"
#include "common/MemoryOverwriteChecking2.h"
#include "common/interface/Error.h"

#include <windows.h>

void ClientEntryPoint(osinterface *ip, int argc, char **argv)    
{        
    SetGlobalErrorHandler(&cBasicErrorHandler::refInstance());
    nSingletons::init_TestBed(ip);
    Demo();
    nSingletons::shutDown_TestBed();
}
