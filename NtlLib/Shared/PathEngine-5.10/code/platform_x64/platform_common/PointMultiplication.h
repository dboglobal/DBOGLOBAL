//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINT_MULTIPLICATION_INCLUDED
#define POINT_MULTIPLICATION_INCLUDED

#include "platform_support/Int128.h"

class cLongMultiplier
{
    typedef tSigned32 T;
    typedef __int64 tDistance;
    typedef cInt128 tMultipliedDistance;

public:

    static tSigned32 topBit(tSigned32 value)
    {
        assertD(value != LONG_MIN);
        if(value<0)
            value=-value;
        tSigned32 result=0;
        while(value)
        {
            value/=2;
            result++;
        }
        return result;
    }
    static tSigned32 topBit(const tDistance& valueConst)
    {
        tDistance value = valueConst;
        assertD(value != LLONG_MIN);
        if(value<0)
            value=-value;
        tSigned32 result=0;
        while(value)
        {
            value/=2;
            result++;
        }
        return result;
    }
    static tSigned32 topBit(const tMultipliedDistance& value)
    {
        return value.topBit();
    }

    static void multiply(T a1, T a2, tDistance& result)
    {
        result = tSigned64(a1) * a2;
    }
    static void multiply(const tDistance& a1, const tDistance& a2, tMultipliedDistance& result)
    {
        result.initialiseAsProduct(a1, a2);
    }
    static void multiply(const tDistance& a1, T a2, tMultipliedDistance& result)
    {
        result.initialiseAsProduct(a1, tDistance(a2));
    }
    static bool divide(const tMultipliedDistance& dividend, const tDistance& divisor, T& quotientResult)
    {
        return dividend.divideTo32BitQuotient(divisor, quotientResult);
    }
    static bool divide(const tDistance& dividend, T divisor, T& quotientResult)
    {
        quotientResult = static_cast<T>(dividend / divisor);
        if(tSigned64(quotientResult) * divisor != dividend)
        {
            return true;
        }
        return false;
    }

    static tSigned32 compare(const tDistance& a1, const tDistance& a2)
    {
        if(a2 < a1)
        {
            return -1;
        }
        if(a2 > a1)
        {
            return 1;
        }
        return 0;
    }
    static tSigned32 compare(const tMultipliedDistance& a1, const tMultipliedDistance& a2)
    {
        return a1.compare(a2);
    }

    static bool isZero(const tDistance& value)
    {
        return value == 0;
    }
    static bool isZero(const tMultipliedDistance& value)
    {
        return value.isZero();
    }
    static bool isNegative(const tDistance& value)
    {
        return value < 0;
    }
    static bool isNegative(const tMultipliedDistance& value)
    {
        return value.isNegative();
    }
    static void negate(tDistance& value)
    {
        value = -value;
    }
    static void negate(tMultipliedDistance& value)
    {
        value.negate();
    }
    static tDistance absoluteValueOf(const tDistance& value)
    {
        tDistance result=value;
        if(isNegative(result))
            negate(result);
        return result;
    }

    static T asT(const tDistance &value)
    {
        return static_cast<T>(value);
    }
    static float asFloat(const tDistance& value)
    {
        return static_cast<float>(value);
    }
    static double asDouble(const tDistance& value)
    {
        return static_cast<double>(value);
    }
};

#endif
