//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef T_SHARED_TARGET_INCLUDED
#define T_SHARED_TARGET_INCLUDED

#include "common/interface/Assert.h"

class bHandleTarget
{
    int _ref;

    // prevent copying and assignment
    bHandleTarget(const bHandleTarget&);
    const bHandleTarget& operator=(const bHandleTarget&);

public:

    bHandleTarget() :
        _ref(0)
    {
    }

    virtual ~bHandleTarget()
    {
    }

    void incRef()
    {
        assertD(_ref >= 0);
        ++_ref;
    }

    void decRef()
    {
        assertD(_ref > 0);
        if(--_ref == 0)
        {
            delete this;
        }
    }

    int getRef() const
    {
        return _ref;
    }
};

#endif
