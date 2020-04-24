//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iQueryCallBack_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
tSigned32
iQueryCallBack_Wrapper::desiredCallBackFrequency()
{
    tUnsigned32 queryCount = gLogger.methodEntry(9, 0, (void*)_target);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->desiredCallBackFrequency();
    gLogger.methodExit(9, 0, queryCount, (void*)_target);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iQueryCallBack_Wrapper::abort()
{
    tUnsigned32 queryCount = gLogger.methodEntry(9, 1, (void*)_target);
    gLogger.methodEntry_Completed();
    bool result = _target->abort();
    gLogger.methodExit(9, 1, queryCount, (void*)_target);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}
void iQueryCallBack_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
