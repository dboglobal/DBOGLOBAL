//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON__DELETE_FILE_INCLUDED
#define PLATFORM_COMMON__DELETE_FILE_INCLUDED

#include "common/interface/Error.h"
#include <stdio.h>

inline void
DeleteFileWithPath(const char* path)
{
    int returnCode = remove(path);
    if(returnCode != 0)
    {
        const char* attributes[3];
        attributes[0] = "path";
        attributes[1] = path;
        attributes[2] = 0;
        Error("Fatal", "Failed to delete a file.", attributes);
    }
}

#endif
