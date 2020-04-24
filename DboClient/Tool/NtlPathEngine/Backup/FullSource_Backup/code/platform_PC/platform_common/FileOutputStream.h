//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FILE_OUTPUT_STREAM_INCLUDED
#define FILE_OUTPUT_STREAM_INCLUDED

#include "common/interface/Assert.h"
#include "i_pathengine.h"

#include <stdio.h>

class cFileOutputStream : public iOutputStream
{
    FILE* _file;

public:

    cFileOutputStream(const char* name)
    {
        _file = fopen(name, "wb");
        assertR(_file);
    }
    ~cFileOutputStream()
    {
        int errorOccurred = fclose(_file);
        assertR(!errorOccurred);
    }
    void put(const char* data, tUnsigned32 dataSize)
    {
        size_t written = fwrite(data, 1, dataSize, _file);
        assertR(written == dataSize);
        int flushResult = fflush(_file);
        assertR(flushResult == 0);
    }
};

#endif
