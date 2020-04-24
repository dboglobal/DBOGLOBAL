//**********************************************************************
//
// Copyright (c) 2004
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_MUTEX_INCLUDED
#define PLATFORM_COMMON_MUTEX_INCLUDED

#include <windows.h>

class cMutex
{
    mutable CRITICAL_SECTION _cs;

    // prevent copying and assignment
    cMutex(const cMutex&);
    const cMutex& operator=(const cMutex&);

public:

    cMutex()
    {
        InitializeCriticalSection(&_cs);
    }
    ~cMutex()
    {
        DeleteCriticalSection(&_cs);
    }

    void lock() const
    {
        EnterCriticalSection(&_cs);
    }
    void unlock() const
    {
        LeaveCriticalSection(&_cs);
    }
};

#endif
