//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_MUTEX_INCLUDED
#define PLATFORM_COMMON_MUTEX_INCLUDED

//#include <windows.h>

// the relevant headers have been ripped from windows.h, as follows, for fast linkage
// in case of any problems with this linkage on other platforms (or on new versions of windows, or whatever) simply replace with the standard header, above
struct sCriticalSectionStandIn
{
    void* a;
    tSigned32 b;
    tSigned32 c;
    void* d;
    void* e;
    tUnsigned32 f;
};
struct _RTL_CRITICAL_SECTION;
extern "C"
{
__declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
}

class cMutex
{
    mutable sCriticalSectionStandIn _cs;

    // prevent copying and assignment
    cMutex(const cMutex&);
    const cMutex& operator=(const cMutex&);

public:

    cMutex()
    {
        InitializeCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }
    ~cMutex()
    {
        DeleteCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }

    void lock() const
    {
        EnterCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }
    void unlock() const
    {
        LeaveCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }
};

#endif
