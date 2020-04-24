//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// This file to be included in just one .obj

#include "common/interface/Error.h"

class iAPIObjectWrapper;

extern "C"
{
    iAPIObjectWrapper** __stdcall DLLExport_RefWrapperPointer(tSigned32 interfaceIndex, void* p)
    {
        Error("Fatal", "This build of the PathEngine DLL does not support automatic query logging.");
        return 0;
    }
}
