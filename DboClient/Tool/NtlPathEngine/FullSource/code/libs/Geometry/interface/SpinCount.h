//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SPIN_COUNT_INCLUDED
#define SPIN_COUNT_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "common/interface/Assert.h"

inline void
UpdateSpinCount(const tPoint& baseAxis, tSigned32& spinCount, const tPoint& nextVector)
{
    assertD(spinCount >= 0);
    if(spinCount & 1)
    {
        if(baseAxis.side(nextVector) != SIDE_LEFT)
        {
            ++spinCount;
        }
    }
    else
    {
        if(baseAxis.side(nextVector) != SIDE_RIGHT)
        {
            ++spinCount;
        }
    }
}

#endif
