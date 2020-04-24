//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_3F_INCLUDED
#define VECTOR_3F_INCLUDED

#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/SquareRoot.h"

class cVector3F
{
    typedef cVector3F tThis;

public:

    float data[3];

    cVector3F()
    {
    }

    cVector3F(float x, float y, float z)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }

    cVector3F(const tPoint3& p)
    {
        data[0] = static_cast<float>(p.getX());
        data[1] = static_cast<float>(p.getY());
        data[2] = static_cast<float>(p.getZ());
    }

    float& x() {return data[0];}
    float& y() {return data[1];}
    float& z() {return data[2];}
    const float& x() const {return data[0];}
    const float& y() const {return data[1];}
    const float& z() const {return data[2];}

    bool operator==(const tThis& rhs) const
    {
        return data[0] == rhs.data[0] && data[1] == rhs.data[1] && data[2] == rhs.data[2];
    }
    bool operator!=(const tThis& rhs) const
    {
        return !((*this)==rhs);
    }

    void operator-=(const cVector3F& rhs)
    {
        data[0] -= rhs.data[0];
        data[1] -= rhs.data[1];
        data[2] -= rhs.data[2];
    }
    void operator+=(const cVector3F& rhs)
    {
        data[0] += rhs.data[0];
        data[1] += rhs.data[1];
        data[2] += rhs.data[2];
    }
    void operator*=(float multiplier)
    {
        data[0] *= multiplier;
        data[1] *= multiplier;
        data[2] *= multiplier;
    }
    void operator/=(float divisor)
    {
        data[0] /= divisor;
        data[1] /= divisor;
        data[2] /= divisor;
    }
    cVector3F operator+(const cVector3F& rhs) const
    {
        cVector3F result(*this);
        result += rhs;
        return result;
    }
    cVector3F operator-(const cVector3F& rhs) const
    {
        cVector3F result(*this);
        result -= rhs;
        return result;
    }
    cVector3F operator*(float rhs) const
    {
        cVector3F result(*this);
        result *= rhs;
        return result;
    }
    cVector3F operator/(float rhs) const
    {
        cVector3F result(*this);
        result /= rhs;
        return result;
    }

    float lengthSquared() const
    {
        float xD = data[0];
        float yD = data[1];
        float zD = data[2];
        return xD * xD + yD * yD + zD * zD;
    }

    float length() const
    {
        return SquareRoot(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
    }

    float dotProduct(const cVector3F& rhs) const
    {
        return data[0] * rhs.data[0] + data[1] * rhs.data[1] + data[2] * rhs.data[2];
    }

    cVector3F crossProduct(const cVector3F& rhs) const
    {
        return cVector3F(
            y() * rhs.z() - z() * rhs.y(),
            z() * rhs.x() - x() * rhs.z(),
            x() * rhs.y() - y() * rhs.x()
            );
    }

    void scaleToUnitLength()
    {
        float ratio = length();
        x() /= ratio;
        y() /= ratio;
        z() /= ratio;
    }

    bool
    operator<(const cVector3F& rhs) const
    {
        if(data[0] < rhs.data[0])
        {
            return true;
        }
        if(data[0] > rhs.data[0])
        {
            return false;
        }
        if(data[1] < rhs.data[1])
        {
            return true;
        }
        if(data[1] > rhs.data[1])
        {
            return false;
        }
        if(data[2] < rhs.data[2])
        {
            return true;
        }
        return false;
    }
};

#endif
