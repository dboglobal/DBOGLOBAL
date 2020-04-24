//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "BSPVertices.h"
#include "platform_common/CoordinateRanges.h"

void
cBSPVertices::approximateX()
{
    tSigned32 i;
    for(i = SizeL(_originalPoints); i != SizeL(_dynamicPoints); ++i)
    {
        _dynamicPoints[i].x() = _dynamicPoints[i].x().asLong_RoundToNearest();
    }
}

void
cBSPVertices::approximateY()
{
    tSigned32 i;
    for(i = SizeL(_originalPoints); i != SizeL(_dynamicPoints); ++i)
    {
        _dynamicPoints[i].y() = _dynamicPoints[i].y().asLong_RoundToNearest();
    }
}

void
cBSPVertices::approximateZ()
{
    tSigned32 i;
    for(i = SizeL(_originalPoints); i != SizeL(_dynamicPoints); ++i)
    {
        _dynamicPoints[i].z() = _dynamicPoints[i].z().asLong_RoundToNearest();
    }
}

const tPoint
cBSPVertices::lookupApproximated2D(tSigned32 index) const
{
    const cDynamicPoint3& p = lookupDynamic(index);
    tSigned32 x, y;
    x = p.x().asLong();
    assertD(x == p.x());
    y = p.y().asLong();
    assertD(y == p.y());
    return tPoint(x, y, WORLD_RANGE);
}
