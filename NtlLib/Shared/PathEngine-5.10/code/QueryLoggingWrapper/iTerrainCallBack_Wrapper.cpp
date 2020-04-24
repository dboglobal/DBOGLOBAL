//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iTerrainCallBack_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
float
iTerrainCallBack_Wrapper::getHeightAt(tSigned32 x, tSigned32 y)
{
    tUnsigned32 queryCount = gLogger.methodEntry(8, 0, (void*)_target);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    float result = _target->getHeightAt(x, y);
    gLogger.methodExit(8, 0, queryCount, (void*)_target);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

float
iTerrainCallBack_Wrapper::getHeightAtWithPrecision(tSigned32 x, tSigned32 y, float precisionX, float precisionY)
{
    tUnsigned32 queryCount = gLogger.methodEntry(8, 1, (void*)_target);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.log(precisionX);
    gLogger.log(precisionY);
    gLogger.methodEntry_Completed();
    float result = _target->getHeightAtWithPrecision(x, y, precisionX, precisionY);
    gLogger.methodExit(8, 1, queryCount, (void*)_target);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}
void iTerrainCallBack_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
