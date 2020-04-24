//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_RANDOM_SEED_INCLUDED
#define PLATFORM_COMMON_RANDOM_SEED_INCLUDED

//// headered here to avoid inclusion of windows.h
//__declspec(dllimport) tUnsigned32 __stdcall timeGetTime(void);
//
//namespace nPlatformCommon
//{
//
//tUnsigned32
//generateRandomSeed()
//{
//    tUnsigned32 result = timeGetTime();
//    return result;
//}
//
//}

#include "platform_common/Timer.h"

namespace nPlatformCommon
{

tUnsigned32
generateRandomSeed()
{
    tUnsigned32 high, low;
    DoRDTSC(high, low);
    return low;
}

}

#endif
