//**********************************************************************
//
// Copyright (c) 2005-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/Bounds.h"
#include "libs/Geometry/interface/tPoint.h"

bool
BoundsOverlap(
        const tPoint& min1,
        const tPoint& max1,
        const tPoint& min2,
        const tPoint& max2
        )
{
    if(min1.getX() >= max2.getX())
    {
        return false;
    }
    if(min1.getY() >= max2.getY())
    {
        return false;
    }
    if(min2.getX() >= max1.getX())
    {
        return false;
    }
    if(min2.getY() >= max1.getY())
    {
        return false;
    }
    return true;
}

bool
BoundsOverlap_Inclusive(
        const tPoint& min1,
        const tPoint& max1,
        const tPoint& min2,
        const tPoint& max2
        )
{
    if(min1.getX() > max2.getX())
    {
        return false;
    }
    if(min1.getY() > max2.getY())
    {
        return false;
    }
    if(min2.getX() > max1.getX())
    {
        return false;
    }
    if(min2.getY() > max1.getY())
    {
        return false;
    }
    return true;
}
