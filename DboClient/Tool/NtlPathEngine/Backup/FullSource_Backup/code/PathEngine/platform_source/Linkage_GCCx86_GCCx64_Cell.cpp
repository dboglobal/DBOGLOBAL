//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "common/interface/Error.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "common/MemoryLog.h"
#include "common/MemoryTracking.h"
#include "common/MemoryOverwriteChecking.h"
#include "common/MemoryOverwriteChecking2.h"
#include "i_pathengine.h"
#include <stdlib.h>
#include <signal.h>

extern "C"
{
iPathEngine* GetIPathEngine(iErrorHandler* handler)
{
    nSingletons::init_StandAlone();
    iPathEngine* pathEngine = &nSingletons::pathEngineI();
    pathEngine->setErrorHandler(handler);
    return pathEngine;
}
void ShutDownPathEngine()
{
    nSingletons::shutDown_StandAlone();
}
}
