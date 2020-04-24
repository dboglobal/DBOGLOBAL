//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// This file to be included in just one .obj
// Defines global new and delete operators if enabled by the relevant define
// Only one such set of global operators should be enabled for any given build

#ifdef MEMORY_OVERWRITE_CHECKING
#include "common/interface/Assert.h"
#include <stdlib.h>

static const tSigned32 BYTES_AT_START = 20;
static const tSigned32 BYTES_AT_END = 20;
static const char FILL_AT_START = '<';
static const char FILL_AT_END = '>';

void*
operator new(size_t size)
{
    if(size == 0)
    {
        return 0;
    }
    size_t sizeToMalloc = BYTES_AT_START + sizeof(size_t) + size + BYTES_AT_END;
    char* result = reinterpret_cast<char*>(malloc(sizeToMalloc));
    tSigned32 i;
    for(i = 0; i < BYTES_AT_START; ++i)
    {
        result[i] = FILL_AT_START;
    }
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(result + BYTES_AT_START);
    *ptrToSizeT = size;
    for(i = 0; i < BYTES_AT_END; ++i)
    {
        result[BYTES_AT_START + sizeof(size_t) + size + i] = FILL_AT_END;
    }
    return result + BYTES_AT_START + sizeof(size_t);
}
void
operator delete(void* ptr)
{
    if(ptr == 0)
    {
        return;
    }
    char* ptrToChar = reinterpret_cast<char*>(ptr);
    ptrToChar -= BYTES_AT_START + sizeof(size_t);
    tSigned32 i;
    for(i = 0; i < BYTES_AT_START; ++i)
    {
        assertR(ptrToChar[i] == FILL_AT_START);
    }
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(ptrToChar + BYTES_AT_START);
    size_t size = *ptrToSizeT;
    for(i = 0; i < BYTES_AT_END; ++i)
    {
        assertR(ptrToChar[BYTES_AT_START + sizeof(size_t) + size + i] == FILL_AT_END);
    }
    free(ptrToChar);
}
void*
operator new[](size_t size)
{
    if(size == 0)
    {
        return 0;
    }
    size_t sizeToMalloc = BYTES_AT_START + sizeof(size_t) + size + BYTES_AT_END;
    char* result = reinterpret_cast<char*>(malloc(sizeToMalloc));
    tSigned32 i;
    for(i = 0; i < BYTES_AT_START; ++i)
    {
        result[i] = FILL_AT_START;
    }
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(result + BYTES_AT_START);
    *ptrToSizeT = size;
    for(i = 0; i < BYTES_AT_END; ++i)
    {
        result[BYTES_AT_START + sizeof(size_t) + size + i] = FILL_AT_END;
    }
    return result + BYTES_AT_START + sizeof(size_t);
}
void
operator delete[](void* ptr)
{
    if(ptr == 0)
    {
        return;
    }
    char* ptrToChar = reinterpret_cast<char*>(ptr);
    ptrToChar -= BYTES_AT_START + sizeof(size_t);
    tSigned32 i;
    for(i = 0; i < BYTES_AT_START; ++i)
    {
        assertR(ptrToChar[i] == FILL_AT_START);
    }
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(ptrToChar + BYTES_AT_START);
    size_t size = *ptrToSizeT;
    for(i = 0; i < BYTES_AT_END; ++i)
    {
        assertR(ptrToChar[BYTES_AT_START + sizeof(size_t) + size + i] == FILL_AT_START);
    }
    free(ptrToChar);
}

#endif
