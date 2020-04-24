//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DYNAMIC_POINT2_INCLUDED
#define DYNAMIC_POINT2_INCLUDED

#include "DynamicFraction.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/SideEnum.h"

class cDynamicPoint2
{
    typedef cDynamicPoint2 tThis;

    cDynamicFraction _x, _y;

public:

    cDynamicPoint2()
    {
    }
    cDynamicPoint2(const tPoint& p);

    cDynamicFraction& x() {return _x;}
    const cDynamicFraction& x() const {return _x;}
    cDynamicFraction& y() {return _y;}
    const cDynamicFraction& y() const {return _y;}

    bool operator==(const tThis& rhs) const
    {
        return _x == rhs._x && _y == rhs._y;
    }
    bool operator!=(const tThis& rhs) const
    {
        return !((*this)==rhs);
    }

    void operator-=(const tThis& rhs)
    {
        _x -= rhs._x;
        _y -= rhs._y;
    }
    void operator+=(const tThis& rhs)
    {
        _x += rhs._x;
        _y += rhs._y;
    }
    
    template <class tScalar>
    void operator*=(const tScalar& rhs)
    {
        _x *= rhs;
        _y *= rhs;
    }
    template <class tScalar>
    void operator/=(const tScalar& rhs)
    {
        _x /= rhs;
        _y /= rhs;
    }

    void rotateRight90()
    {
        _x.swap(_y);
        _y.negate();
    }

    cDynamicFraction dotProduct(const tThis& rhs) const;

    bool
    isZero() const;
};

#endif
