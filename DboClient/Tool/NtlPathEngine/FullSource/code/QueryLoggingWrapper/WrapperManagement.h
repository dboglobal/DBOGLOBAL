//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER__WRAPPER_MANAGEMENT_INCLUDED
#define QUERY_LOGGING_WRAPPER__WRAPPER_MANAGEMENT_INCLUDED

#include "RefWrapperPointer.h"
#include "common/interface/Assert.h"

class iPathEngine;
class iAgent;
class iAgent_Wrapper;

iPathEngine* WrapSingleton(iPathEngine*);
iPathEngine* ExtractWrappedSingleton();

template <class tInterfaceWrapper>
typename tInterfaceWrapper::tWrapped*
WrapNew(typename tInterfaceWrapper::tWrapped* base)
{
    if(!base)
    {
        return base;
    }
    iAPIObjectWrapper*& wrapperPointer = RefWrapperPointer(tInterfaceWrapper::interfaceIndex(), base);
    assertR(wrapperPointer == 0);
    tInterfaceWrapper* wrapper = new tInterfaceWrapper();
    wrapper->_target = base;
    wrapperPointer = wrapper;
    return wrapper;
}

template <class tInterfaceWrapper>
void
ExtractWrapper(typename tInterfaceWrapper::tWrapped*& base)
{
    if(!base)
    {
        return;
    }
    iAPIObjectWrapper*& wrapperPointer = RefWrapperPointer(tInterfaceWrapper::interfaceIndex(), base);
    assertR(wrapperPointer);
    base = static_cast<tInterfaceWrapper*>(wrapperPointer);
}

void ExtractWrappers(iAgent** resultsBuffer, tSigned32& numberOverlapped);

template <class tInterfaceWrapper>
const typename tInterfaceWrapper::tWrapped*
ExtractWrapped(const typename tInterfaceWrapper::tWrapped* base)
{
    if(!base)
    {
        return base;
    }
    return static_cast<const tInterfaceWrapper*>(base)->_target;
}

template <class tInterfaceWrapper>
typename tInterfaceWrapper::tWrapped*
ExtractWrapped(typename tInterfaceWrapper::tWrapped* base)
{
    if(!base)
    {
        return base;
    }
    return static_cast<tInterfaceWrapper*>(base)->_target;
}

#endif
