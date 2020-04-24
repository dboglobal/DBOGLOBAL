//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iShape_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iShape_Wrapper::destroy()
{
    tUnsigned32 queryCount = gLogger.methodEntry(1, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(1, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

tSigned32
iShape_Wrapper::size() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(1, 1, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->size();
    gLogger.methodExit(1, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iShape_Wrapper::vertex(tSigned32 index, tSigned32& x, tSigned32& y) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(1, 2, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    _target->vertex(index, x, y);
    gLogger.methodExit(1, 2, queryCount, (void*)this);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodExit_Completed();
}
void iShape_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
