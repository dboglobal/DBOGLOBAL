//**********************************************************************
//
// Copyright (c) 2002-2004
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_THREAD_SAFE_HANDLE_TARGET_INCLUDED
#define PLATFORM_COMMON_THREAD_SAFE_HANDLE_TARGET_INCLUDED

//#include "common/MutexLock.h"
#include <windows.h>

class bThreadSafeHandleTarget
{
    tSigned32 _ref;
//	cMutex _mutex;

    // prevent copying and assignment
    bThreadSafeHandleTarget(const bThreadSafeHandleTarget&);
    const bThreadSafeHandleTarget& operator=(const bThreadSafeHandleTarget&);

public:

    typedef bThreadSafeHandleTarget tTargetBase;

    bThreadSafeHandleTarget() :
        _ref(0)
    {
    }
    virtual ~bThreadSafeHandleTarget()
    {
    }

    void incRef()
    {
        InterlockedIncrement(&_ref);
//		cMutexLock lock(_mutex);
//		_ref++;
    }
    void decRef()
    {
        if(InterlockedDecrement(&_ref) == 0)
        {
            delete this;
        }
//		LONG result = InterlockedDecrement(&_ref);
//		__lwsync();
//		if(result == 0)
//		{
//			delete this;
//		}
//		bool noMoreRefs;
//		{
//			cMutexLock lock(_mutex);
//			--_ref;
//			noMoreRefs = (_ref == 0);
//		}
//		if(noMoreRefs)
//		{
//			delete this;
//		}
    }
    bool hasMoreThanOneRef()
    {
        return InterlockedExchangeAdd(&_ref, 0) > 1;
//		LONG result = InterlockedExchangeAdd(&_ref, 0);
//		__lwsync();
//		return result > 1;
//		cMutexLock lock(_mutex);
//		return _ref > 1;
    }
};

#endif
