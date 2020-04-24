//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// This file to be included in just one .obj
// Defines global new and delete operators if enabled by the relevant define
// Only one such set of global operators should be enabled for any given build

#ifdef MEMORY_LOG

#include "common/MemoryLog_OperatorCode.h"
#include "platform_common/WorkingDirectory.h"

#include <stdlib.h>
#include <stdio.h>

//#define assert

static FILE* fp = 0;
static bool insideCall = false;
static tSigned32 callCounter = 0;
const tSigned32 callStackDepth = 1;

template <class T> void
doFWrite(const T& toWrite)
{
    fwrite(&toWrite, sizeof(toWrite), 1, fp);
}

void init()
{
    fp = fopen(STATIC_STRING_FOR_FILE_IN_WORKING_DIRECTORY("memlog.bin"), "wb");
    doFWrite(callStackDepth);
}

static void
writeReferenceAddress()
{
    if(!fp)
    {
        init();
    }
    doFWrite(OC_REFERENCE_ADDRESS);
    doFWrite(callCounter++);
}

class ReferenceAddressWriter
{
public:

    ReferenceAddressWriter();
};
ReferenceAddressWriter::ReferenceAddressWriter()
{
    writeReferenceAddress();
}
static ReferenceAddressWriter theWriter;

void
writeCallCounter()
{
    doFWrite(callCounter++);
}

void*
operator new(size_t s)
{
    if(s == 0)
    {
        return 0;
    }

    insideCall = true;

    void* ptr = malloc(s);

    if(!fp)
    {
        init();
    }
    doFWrite(OC_NEW);
    doFWrite(s);
    doFWrite(ptr);
    writeCallCounter();

    insideCall = false;
    return ptr;
}
void*
operator new[](size_t s)
{
    if(s == 0)
    {
        return 0;
    }

    insideCall = true;

    void* ptr = malloc(s);

    if(!fp)
    {
        init();
    }
    doFWrite(OC_NEW_ARRAY);
    doFWrite(s);
    doFWrite(ptr);
    writeCallCounter();

    insideCall = false;
    return ptr;
}

void
operator delete(void *ptr)
{
    if(!ptr)
    {
        return;
    }

    insideCall = true;

    free(ptr);

    if(!fp)
    {
        init();
    }
    doFWrite(OC_DELETE);
    doFWrite(ptr);
    writeCallCounter();

    insideCall = false;
}

void
operator delete[](void *ptr)
{
    if(!ptr)
    {
        return;
    }

    insideCall = true;

    free(ptr);

    if(!fp)
    {
        init();
    }
    doFWrite(OC_DELETE_ARRAY);
    doFWrite(ptr);
    writeCallCounter();

    insideCall = false;
}

#endif
