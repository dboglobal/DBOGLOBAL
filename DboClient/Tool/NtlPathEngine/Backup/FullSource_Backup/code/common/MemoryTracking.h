//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// This file to be included in just one .obj
// Defines global new and delete operators if enabled by the relevant define
// Only one such set of global operators should be enabled for any given build

#include "common/MemoryTracking_Interface.h"
#include "common/interface/Error.h"

#ifdef MEMORY_TRACKING

#include "common/MutexLock.h"
#include "platform_common/Directory.h"

#include <stdlib.h>
#include <stdio.h>

static tUnsigned32 totalAllocated = 0;
static tUnsigned32 maximumAllocated = 0;
static tUnsigned32 totalAllocations = 0;

#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
static cMutex mutex;
#endif

#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
static void
SetReturnToZeroFlag(char value)
{
    FILE* fp;
    fp = fopen(STATIC_STRING_FOR_FILE_IN_WORKING_DIRECTORY("mem_tracking_zero_flag.bin"), "wb");
    fwrite(&value, 1, 1, fp);
    fclose(fp);
}
#endif

void PathEngine_DebugBreak();

void*
operator new(size_t size)
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    totalAllocations++;
    size_t sizeToMalloc = size + sizeof(size_t);
    size_t* result = reinterpret_cast<size_t*>(malloc(sizeToMalloc));
    if(sizeToMalloc && !result)
    {
        Error("Fatal", "Memory allocation failure");
    }
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0 && size)
    {
        SetReturnToZeroFlag(1);
    }
#endif
    totalAllocated += static_cast<tUnsigned32>(size);
    if(totalAllocated > maximumAllocated)
    {
        maximumAllocated = totalAllocated;
//        if(maximumAllocated > 400000000)
//        {
//            PathEngine_DebugBreak();
//        }
    }
    *result = size;
    return result + 1;
}
void
operator delete(void* ptr)
{
    if(!ptr)
    {
        return;
    }
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(ptr);
    --ptrToSizeT;
    totalAllocated -= static_cast<tUnsigned32>(*ptrToSizeT);
    free(ptrToSizeT);
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0)
    {
        SetReturnToZeroFlag(0);
    }
#endif
}
void*
operator new[](size_t size)
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    totalAllocations++;
    size_t sizeToMalloc = size + sizeof(size_t);
    size_t* result = reinterpret_cast<size_t*>(malloc(sizeToMalloc));
    if(sizeToMalloc && !result)
    {
        Error("Fatal", "Memory allocation failure");
    }
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0 && size)
    {
        SetReturnToZeroFlag(1);
    }
#endif
    totalAllocated += static_cast<tUnsigned32>(size);
    if(totalAllocated > maximumAllocated)
    {
        maximumAllocated = totalAllocated;
//        if(maximumAllocated > 400000000)
//        {
//            PathEngine_DebugBreak();
//        }
    }
    *result = size;
    return result + 1;
}
void
operator delete[](void* ptr)
{
    if(!ptr)
    {
        return;
    }
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(ptr);
    --ptrToSizeT;
    totalAllocated -= static_cast<tUnsigned32>(*ptrToSizeT);
    free(ptrToSizeT);
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0)
    {
        SetReturnToZeroFlag(0);
    }
#endif
}

bool MemoryTrackingIsEnabled()
{
    return true;
}
tUnsigned32 GetTotalMemoryAllocated()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    return totalAllocated;
}
tUnsigned32 GetMaximumMemoryAllocated()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    return maximumAllocated;
}
void ResetMaximumMemoryAllocated()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    maximumAllocated = totalAllocated;
}
tUnsigned32 GetTotalNumberOfAllocations()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    return totalAllocations;
}

#else // memory tracking is not enabled

bool MemoryTrackingIsEnabled() {return false;}
tUnsigned32 GetTotalMemoryAllocated() {return 0;}
tUnsigned32 GetMaximumMemoryAllocated() {return 0;}
void ResetMaximumMemoryAllocated() {}
tUnsigned32 GetTotalNumberOfAllocations() {return 0;}

#endif
