
#ifndef FILE_OUTPUT_STREAM_INCLUDED
#define FILE_OUTPUT_STREAM_INCLUDED

#include "i_pathengine.h"
#include "error.h"
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
    void put(const char* data, unsigned long dataSize)
    {
        size_t written = fwrite(data, 1, dataSize, _file);
        assertR(written == dataSize);
        int flushResult = fflush(_file);
        assertR(flushResult == 0);
    }
};

#endif
