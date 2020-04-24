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

#ifdef MEMORY_OVERWRITE_CHECKING2
#include "common/interface/Assert.h"
#include <stdlib.h>

template <class T, tSigned32 maxSize>
class cSimpleQueue
{
    T _queue[maxSize];
    tSigned32 _currentPosition;
    tSigned32 _currentSize;

public:

    cSimpleQueue()
    {
        _currentPosition = 0;
        _currentSize = 0;
    }

    bool full() const
    {
        return _currentSize == maxSize;
    }
    T pop_front()
    {
        assertD(_currentSize);
        T result = _queue[_currentPosition];
        --_currentSize;
        ++_currentPosition;
        if(_currentPosition == maxSize)
        {
            _currentPosition = 0;
        }
        return result;
    }
    void push_back(T value)
    {
        assertD(!full());
        tSigned32 destination = _currentPosition + _currentSize;
        if(destination >= maxSize)
        {
            destination -= maxSize;
        }
        _queue[destination] = value;
        ++_currentSize;
    }
};

static void
CheckedDelayedFree(char* ptr, tSigned32 size)
{
    static cSimpleQueue<char*, 50> ptrQueue;
    static cSimpleQueue<tSigned32, 50> sizeQueue;
    const char fillByte = '~';    
    if(ptrQueue.full())
    {
        char* ptr = ptrQueue.pop_front();
        tSigned32 size = sizeQueue.pop_front();
        tSigned32 i;
        for(i = 0; i < size; ++i)
        {
            assertR(ptr[i] == fillByte);
        }
        free(ptr);
    }
    tSigned32 i;
    for(i = 0; i < size; ++i)
    {
        ptr[i] = fillByte;
    }
    ptrQueue.push_back(ptr);
    sizeQueue.push_back(size);
}

void*
operator new(size_t size)
{
    if(size == 0)
    {
        return 0;
    }
    size_t sizeToMalloc = sizeof(tSigned32) + size;
    tSigned32* result = reinterpret_cast<tSigned32*>(malloc(sizeToMalloc));
    *result = size;
    return result + 1;
}
void
operator delete(void* ptr)
{
    if(ptr == 0)
    {
        return;
    }
    tSigned32* ptrToLong = reinterpret_cast<tSigned32*>(ptr);
    --ptrToLong;
    tSigned32 size = *ptrToLong;
    char* ptrToChar = reinterpret_cast<char*>(ptrToLong);
    CheckedDelayedFree(ptrToChar, size + sizeof(tSigned32));
}
void*
operator new[](size_t size)
{
    if(size == 0)
    {
        return 0;
    }
    size_t sizeToMalloc = sizeof(tSigned32) + size;
    tSigned32* result = reinterpret_cast<tSigned32*>(malloc(sizeToMalloc));
    *result = size;
    return result + 1;
}
void
operator delete[](void* ptr)
{
    if(ptr == 0)
    {
        return;
    }
    tSigned32* ptrToLong = reinterpret_cast<tSigned32*>(ptr);
    --ptrToLong;
    tSigned32 size = *ptrToLong;
    char* ptrToChar = reinterpret_cast<char*>(ptrToLong);
    CheckedDelayedFree(ptrToChar, size + sizeof(tSigned32));
}

#endif
