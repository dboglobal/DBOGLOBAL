//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iCollisionInfo_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iCollisionInfo_Wrapper::destroy()
{
    tUnsigned32 queryCount = gLogger.methodEntry(7, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(7, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionInfo_Wrapper::getCollidingLine(tSigned32* coords) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(7, 1, (void*)this);
    gLogger.methodEntry_Completed();
    _target->getCollidingLine(coords);
    gLogger.methodExit(7, 1, queryCount, (void*)this);
    gLogger.logCollidingLine(coords);
    gLogger.methodExit_Completed();
}

iAgent*
iCollisionInfo_Wrapper::getCollidingAgent() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(7, 2, (void*)this);
    gLogger.methodEntry_Completed();
    iAgent* result = _target->getCollidingAgent();
    ExtractWrapper<iAgent_Wrapper>(result);
    gLogger.methodExit(7, 2, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}
void iCollisionInfo_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
