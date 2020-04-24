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
#include "WrapperManagement.h"
#include "AllWrappers.h"

iPathEngine_Wrapper gPathEngineWrapper;

iPathEngine*
WrapSingleton(iPathEngine* base)
{
    assertR(base);
    gPathEngineWrapper._target = base;
    return &gPathEngineWrapper;
}
iPathEngine*
ExtractWrappedSingleton()
{
    return gPathEngineWrapper._target;
}

void
ExtractWrappers(iAgent** resultsBuffer, tSigned32& numberOverlapped)
{
    tSigned32 i;
    for(i = 0; i != numberOverlapped; ++i)
    {
        ExtractWrapper<iAgent_Wrapper>(resultsBuffer[i]);
    }
}


