//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iContentChunk_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iContentChunk_Wrapper::destroy()
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(10, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iContentChunk_Wrapper::preValidate(tSigned32 maximumScale, tSigned32 translationRange)
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 1, (void*)this);
    gLogger.log(maximumScale);
    gLogger.log(translationRange);
    gLogger.methodEntry_Completed();
    bool result = _target->preValidate(maximumScale, translationRange);
    gLogger.methodExit(10, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iContentChunk_Wrapper::save(iOutputStream* outputStream) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 2, (void*)this);
    gLogger.methodEntry_Completed();
    _target->save(outputStream);
    gLogger.methodExit(10, 2, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iContentChunkInstance*
iContentChunk_Wrapper::instantiate(tSigned32 rotation, tSigned32 scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID)
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 3, (void*)this);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.log(sectionID);
    gLogger.methodEntry_Completed();
    iContentChunkInstance* result = _target->instantiate(rotation, scale, translationX, translationY, translationZ, sectionID);
    result = WrapNew<iContentChunkInstance_Wrapper>(result);
    gLogger.methodExit(10, 3, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iContentChunkInstance*
iContentChunk_Wrapper::instantiate_FloatRotateAndScale(float rotation, float scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID)
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 4, (void*)this);
    gLogger.log(rotation);
    gLogger.log(scale);
    gLogger.log(translationX);
    gLogger.log(translationY);
    gLogger.log(translationZ);
    gLogger.log(sectionID);
    gLogger.methodEntry_Completed();
    iContentChunkInstance* result = _target->instantiate_FloatRotateAndScale(rotation, scale, translationX, translationY, translationZ, sectionID);
    result = WrapNew<iContentChunkInstance_Wrapper>(result);
    gLogger.methodExit(10, 4, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iContentChunk_Wrapper::deleteAllInstances()
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 5, (void*)this);
    gLogger.methodEntry_Completed();
    _target->deleteAllInstances();
    gLogger.methodExit(10, 5, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iContentChunk_Wrapper::hasGroundComponent() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 6, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->hasGroundComponent();
    gLogger.methodExit(10, 6, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iContentChunk_Wrapper::hasAnchorsAndShapesComponent() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 7, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->hasAnchorsAndShapesComponent();
    gLogger.methodExit(10, 7, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iContentChunk_Wrapper::numberOfConnectingEdges() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(10, 8, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->numberOfConnectingEdges();
    gLogger.methodExit(10, 8, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}
void iContentChunk_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
