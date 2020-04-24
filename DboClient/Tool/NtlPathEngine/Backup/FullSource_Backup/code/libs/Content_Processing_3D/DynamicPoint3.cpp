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
#include "DynamicPoint3.h"
#include "libs/Geometry/interface/tPoint3.h"

cDynamicPoint3::cDynamicPoint3(const tPoint3& p) :
 _x(p.getX()),
 _y(p.getY()),
 _z(p.getZ())
{
}

void
cDynamicPoint3::fastNormalise()
{
    if(!_x.isZero())
    {
        if(_x.isNegative())
        {
            _x.negate();
            _y /= _x;
            _z /= _x;
            _x = cDynamicFraction(-1L);
        }
        else
        {
            _y /= _x;
            _z /= _x;
            _x = cDynamicFraction(1L);
        }
        return;
    }
    if(!_y.isZero())
    {
        if(_y.isNegative())
        {
            _y.negate();
            _x /= _y;
            _z /= _y;
            _y = cDynamicFraction(-1L);
        }
        else
        {
            _x /= _y;
            _z /= _y;
            _y = cDynamicFraction(1L);
        }
        return;
    }
    if(!_z.isZero())
    {
        if(_z.isNegative())
        {
            _z.negate();
            _x /= _z;
            _y /= _z;
            _z = cDynamicFraction(-1L);
        }
        else
        {
            _x /= _z;
            _y /= _z;
            _z = cDynamicFraction(1L);
        }
        return;
    }
}

cDynamicFraction
cDynamicPoint3::dotProduct(const tThis& rhs) const
{
    cDynamicFraction result(_x);
    result *= rhs._x;
    cDynamicFraction product1(_y);
    product1 *= rhs._y;
    cDynamicFraction product2(_z);
    product2 *= rhs._z;
    result += product1;
    result += product2;
    return result;
}
void
cDynamicPoint3::crossProduct(const tThis& rhs, tThis& result) const
{
//    return tThis(
//        getY()*rhs.getZ() - getZ()*rhs.getY(),
//        getZ()*rhs.getX() - getX()*rhs.getZ(),
//        getX()*rhs.getY() - getY()*rhs.getX(),

    cDynamicFraction temp;

    result.x() = y();
    result.x() *= rhs.z();
    temp = z();
    temp *= rhs.y();
    result.x() -= temp;

    result.y() = z();
    result.y() *= rhs.x();
    temp = x();
    temp *= rhs.z();
    result.y() -= temp;

    result.z() = x();
    result.z() *= rhs.y();
    temp = y();
    temp *= rhs.x();
    result.z() -= temp;
}

bool
cDynamicPoint3::inLine(const tThis& rhs) const
{
    cDynamicFraction d1, d2;
    d1 = _x;
    d1 *= rhs._y;
    d2 = rhs._x;
    d2 *= _y;
    if(d1 != d2)
    {
        return false;
    }
    d1 = _x;
    d1 *= rhs._z;
    d2 = rhs._x;
    d2 *= _z;
    if(d1 != d2)
    {
        return false;
    }
    d1 = _y;
    d1 *= rhs._z;
    d2 = rhs._y;
    d2 *= _z;
    if(d1 != d2)
    {
        return false;
    }
    return true;
}

eDirection
cDynamicPoint3::direction_KnownToBeInline(const tThis& rhs) const
{
    if(!_x.isZero())
    {
        assertD(!rhs._x.isZero());
        if(_x.isNegative() == rhs._x.isNegative())
        {
            return DIRECTION_FORWARD;
        }
        return DIRECTION_BACK;
    }
    if(!_y.isZero())
    {
        assertD(!rhs._y.isZero());
        if(_y.isNegative() == rhs._y.isNegative())
        {
            return DIRECTION_FORWARD;
        }
        return DIRECTION_BACK;
    }
    if(!_z.isZero())
    {
        assertD(!rhs._z.isZero());
        if(_z.isNegative() == rhs._z.isNegative())
        {
            return DIRECTION_FORWARD;
        }
        return DIRECTION_BACK;
    }
    invalid(); // making a direction query implies lhs is non-zero
    return DIRECTION_NONE;
}

bool
cDynamicPoint3::isOnAxis_NotInclusive_KnownToBeInline(const tThis& rhs) const
{
    if(rhs._x.isNegative())
    {
        return _x.isNegative() && _x > rhs._x;
    }
    if(!rhs._x.isZero())
    {
        return !_x.isNegative() && !_x.isZero() && _x < rhs._x;
    }
    if(rhs._y.isNegative())
    {
        return _y.isNegative() && _y > rhs._y;
    }
    if(!rhs._y.isZero())
    {
        return !_y.isNegative() && !_y.isZero() && _y < rhs._y;
    }
    if(rhs._z.isNegative())
    {
        return _z.isNegative() && _z > rhs._z;
    }
    if(!rhs._z.isZero())
    {
        return !_z.isNegative() && !_z.isZero() && _z < rhs._z;
    }
    return false; // null vector
}

bool
cDynamicPoint3::isOnAxis_NotInclusive(const tThis& rhs) const
{
    if(!inLine(rhs))
    {
        return false;
    }
    return isOnAxis_NotInclusive_KnownToBeInline(rhs);
}

bool
cDynamicPoint3::isZero() const
{
    return _x.isZero() && _y.isZero() && _z.isZero();
}

cVector3F
cDynamicPoint3::as3F() const
{
    return cVector3F(_x.asFloat(), _y.asFloat(), _z.asFloat());
}

tPoint3
cDynamicPoint3::approximated(tSigned32 range) const
{
    return tPoint3(_x.asLong(), _y.asLong(), _z.asLong(), range);
}

