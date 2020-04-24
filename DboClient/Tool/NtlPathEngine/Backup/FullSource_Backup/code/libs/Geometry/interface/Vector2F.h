//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_2F_INCLUDED
#define VECTOR_2F_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "platform_common/SquareRoot.h"

class cVector2F
{
public:

    float x, y;

    cVector2F()
    {
    }

    cVector2F(float x, float y) :
     x(x),
     y(y)
    {
    }

    cVector2F(const tPoint& p) :
     x(static_cast<float>(p.getX())),
     y(static_cast<float>(p.getY()))
    {
    }

    float length() const
    {
        return SquareRoot(x * x + y * y);
    }

    float dotProduct(const cVector2F& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }

    void operator-=(const cVector2F& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }
    void operator+=(const cVector2F& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }
    void operator*=(float multiplier)
    {
        x *= multiplier;
        y *= multiplier;
    }
    void operator/=(float divisor)
    {
        x /= divisor;
        y /= divisor;
    }
    cVector2F operator+(const cVector2F& rhs) const
    {
        cVector2F result(*this);
        result += rhs;
        return result;
    }
    cVector2F operator-(const cVector2F& rhs) const
    {
        cVector2F result(*this);
        result -= rhs;
        return result;
    }
    cVector2F operator*(float rhs) const
    {
        cVector2F result(*this);
        result *= rhs;
        return result;
    }
    cVector2F operator/(float rhs) const
    {
        cVector2F result(*this);
        result /= rhs;
        return result;
    }
};

#endif
