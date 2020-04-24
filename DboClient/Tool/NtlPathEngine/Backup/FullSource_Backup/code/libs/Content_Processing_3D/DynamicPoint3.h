//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DYNAMIC_POINT3_INCLUDED
#define DYNAMIC_POINT3_INCLUDED

#include "DynamicFraction.h"
#include "libs/Geometry/interface/tPoint3_Header.h"
#include "libs/Geometry/interface/Vector3F.h"
#include "libs/Geometry/interface/SideEnum.h"

class cPlane;

class cDynamicPoint3
{
    typedef cDynamicPoint3 tThis;

    cDynamicFraction _x, _y, _z;

public:

    cDynamicPoint3()
    {
    }
    cDynamicPoint3(const tPoint3& p);

    cDynamicFraction& x() {return _x;}
    const cDynamicFraction& x() const {return _x;}
    cDynamicFraction& y() {return _y;}
    const cDynamicFraction& y() const {return _y;}
    cDynamicFraction& z() {return _z;}
    const cDynamicFraction& z() const {return _z;}

    bool operator==(const tThis& rhs) const
    {
        return _x == rhs._x && _y == rhs._y && _z == rhs._z;
    }
    bool operator!=(const tThis& rhs) const
    {
        return !((*this)==rhs);
    }

    void operator-=(const tThis& rhs)
    {
        _x -= rhs._x;
        _y -= rhs._y;
        _z -= rhs._z;
    }
    void operator+=(const tThis& rhs)
    {
        _x += rhs._x;
        _y += rhs._y;
        _z += rhs._z;
    }
    
    template <class tScalar>
    void operator*=(const tScalar& rhs)
    {
        _x *= rhs;
        _y *= rhs;
        _z *= rhs;
    }
    template <class tScalar>
    void operator/=(const tScalar& rhs)
    {
        _x /= rhs;
        _y /= rhs;
        _z /= rhs;
    }

    // doesn't make this a unit vector, instead divides by the first non-zero component
    // the point being to enable direction vectors to be compared directly for equality
    void fastNormalise();

    cDynamicFraction dotProduct(const tThis& rhs) const;
    void crossProduct(const tThis& rhs, tThis& result) const;

    friend void
    Intersection(
            const cPlane& plane,
            const cDynamicPoint3& lineStart,
            const cDynamicPoint3& lineAxis,
            cDynamicPoint3& result
            );

    bool
    inLine(const tThis& rhs) const;

    eDirection direction_KnownToBeInline(const tThis& rhs) const;

    bool
    isOnAxis_NotInclusive(const tThis& rhs) const;

    bool
    isOnAxis_NotInclusive_KnownToBeInline(const tThis& rhs) const;

    bool
    isZero() const;

    cVector3F as3F() const;
    tPoint3 approximated(tSigned32 range) const;
};

#endif
