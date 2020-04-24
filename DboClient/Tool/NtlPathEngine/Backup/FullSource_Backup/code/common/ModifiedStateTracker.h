//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMMON_MODIFIED_STATE_TRACKER_INCLUDED
#define COMMON_MODIFIED_STATE_TRACKER_INCLUDED

#include "common/interface/Assert.h"
#include "common/CLib/limits.h"

class cModifiedStateTracker
{
    tUnsigned32 _lastPublishedState;
    bool _modifiedSinceLastPublishedState;

    // prevent copying and assignment
    cModifiedStateTracker(const cModifiedStateTracker&);
    const cModifiedStateTracker& operator=(const cModifiedStateTracker&);

    void increment()
    {
        // platform independant tUnsigned32 max is used in case this needs to be saved
        assertC(ULONG_MAX >= 0xffffffffUL);
        if(_lastPublishedState == 0xffffffffUL)
        {
            _lastPublishedState = 0;
        }
        else
        {
            _lastPublishedState++;
        }
    }

public:

    cModifiedStateTracker()
    {
        _lastPublishedState = 0;
        _modifiedSinceLastPublishedState = false;
    }

    void notifyStateModified()
    {
        _modifiedSinceLastPublishedState = true;
    }

    tUnsigned32 getStateHandle()
    {
        if(_modifiedSinceLastPublishedState)
        {
            increment();
            _modifiedSinceLastPublishedState = false;
        }
        return _lastPublishedState;
    }

    bool hasBeenModifiedSince(tUnsigned32 stateHandle) const
    {
        if(_modifiedSinceLastPublishedState)
        {
            return true;
        }
        return stateHandle == _lastPublishedState;
    }
};

#endif
