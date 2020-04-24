//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "DynamicPoint2.h"
#include "libs/Geometry/interface/tPoint.h"

cDynamicPoint2::cDynamicPoint2(const tPoint& p) :
 _x(p.getX()),
 _y(p.getY())
{
}

cDynamicFraction
cDynamicPoint2::dotProduct(const tThis& rhs) const
{
    cDynamicFraction result(_x);
    result *= rhs._x;
    cDynamicFraction product1(_y);
    product1 *= rhs._y;
    result += product1;
    return result;
}

bool
cDynamicPoint2::isZero() const
{
    return _x.isZero() && _y.isZero();
}
