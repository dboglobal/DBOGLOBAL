//**********************************************************************
//
// Copyright (c) 2002-2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_TIMER_INCLUDED
#define PLATFORM_COMMON_TIMER_INCLUDED

#include "platform_common/ToAndFromByteBuffer.h"
#include <windows.h>
#include <sys/timeb.h>
#include <time.h>
#include <intrin.h>

class cTimerValue
{
    LARGE_INTEGER _value;

public:

    cTimerValue()
    {
    }

    void
    read()
    {
        QueryPerformanceCounter(&_value);
    }
    void
    operator-=(const cTimerValue& rhs)
    {
        _value.QuadPart -= rhs._value.QuadPart;
    }
    double
    asSeconds() const
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        return static_cast<double>(_value.QuadPart) / static_cast<double>(frequency.QuadPart);
    }
};

static const tSigned32 TIMESTAMP_BYTE_SIZE = 6;
inline void GetTimeStamp(char* buffer)
{
    struct __timeb64 tb;
    _ftime64(&tb);
    // seconds since midnight (00:00:00), January 1, 1970, coordinated universal time (UTC)
    float milliseconds = static_cast<float>(tb.millitm);
    tUnsigned32 afterPoint = static_cast<tUnsigned32>(milliseconds * 65.536f);
    UnsignedToByteBuffer(afterPoint, 2, buffer);
    tUnsigned32 seconds = static_cast<tUnsigned32>(tb.time);
    UnsignedToByteBuffer(seconds, 4, buffer + 2);
}

//..... TODO can this be made volatile in the same way as on gcc?
inline void DoRDTSC(tUnsigned32 &high, tUnsigned32 &low)
{
	tUnsigned64 asI64 = __rdtsc();
	low = static_cast<tUnsigned32>(asI64);
	asI64 >>= 32;
	high = static_cast<tUnsigned32>(asI64);
}

// GCC 486
//inline void DoRDTSC(tUnsigned32 &high, tUnsigned32 &low)
//{
//    __asm__ volatile (".byte 0x0f, 0x31" : "=a" (low), "=d" (high));
//}

// not actually platform specific, but timer related
inline double
ClocksAsDouble(tUnsigned32 high, tUnsigned32 low)
{
    const double TIMER_HIGH_MULTIPLIER = 4294967296.0;
    double result = high;
    result *= TIMER_HIGH_MULTIPLIER;
    result += low;
    return result;
}

#endif
