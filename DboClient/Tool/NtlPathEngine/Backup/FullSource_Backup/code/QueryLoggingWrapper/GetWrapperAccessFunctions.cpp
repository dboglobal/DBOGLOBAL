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
#include "GetWrapperPointerAccessFunction.h"
#include "RefWrapperPointer.h"
#include <stdio.h>

typedef iAPIObjectWrapper** (__stdcall* tAccessFunction)(tSigned32, void*);
static tAccessFunction gAccessFunction;

bool
GetWrapperPointerAccessFunction(HINSTANCE hi)
{
    FARPROC procAddress = GetProcAddress(hi, (LPCSTR)2);
    if(!procAddress)
    {
        return false;
    }
    gAccessFunction = (tAccessFunction)procAddress;
    return true;
}

// provide functions headered in WrapperPointerAccess.h

iAPIObjectWrapper*&
RefWrapperPointer(tSigned32 interfaceIndex, void* interfacePointer)
{
    return *gAccessFunction(interfaceIndex, interfacePointer);
}
