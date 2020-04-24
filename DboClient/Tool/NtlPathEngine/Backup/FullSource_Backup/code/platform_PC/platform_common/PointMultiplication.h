//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINT_MULTIPLICATION_INCLUDED
#define POINT_MULTIPLICATION_INCLUDED

#include "platform_support/LargeIntegers.h"

class cLongMultiplier
{
    typedef tSigned32 T;
    typedef cLargeInteger2 tDistance;
    typedef cLargeInteger4 tMultipliedDistance;

    static tSigned32 topBit(tUnsigned32 value)
    {
        tSigned32 result=0;
        while(value)
        {
            value/=2;
            result++;
        }
        return result;
    }

public:

    static tSigned32 topBit(tSigned32 value)
    {
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
    static tSigned32 topBit(const tDistance& value)
    {
        int i = 2;
        while(--i)
        {
            if(value.digits[i])
                return topBit(value.digits[i])+32*i;
        }
        return topBit(value.digits[0]);
    }
    static tSigned32 topBit(const tMultipliedDistance& value)
    {
        int i = 4;
        while(--i)
        {
            if(value.digits[i])
                return topBit(value.digits[i])+32*i;
        }
        return topBit(value.digits[0]);
    }

    static void multiply(T a1, T a2, tDistance& result)
    {
        nLargeIntegers::multiply(a1, a2, result);
    }
    static void multiply(const tDistance& a1, const tDistance& a2, tMultipliedDistance& result)
    {
        nLargeIntegers::multiply(a1, a2, result);
    }
    static void multiply(const tDistance& a1, T a2, tMultipliedDistance& result)
    {
        nLargeIntegers::multiply(a1, a2, result);
    }
    static bool divide(const tMultipliedDistance& dividend, const tDistance& divisor, T& quotientResult)
    {
        return nLargeIntegers::divide(4,2, dividend.digits, divisor.digits, quotientResult);
    }
    static bool divide(const tDistance& dividend, T divisor, T& quotientResult)
    {
        tUnsigned32 divisorDigits[2];
        nLargeIntegers::constructFromLong(1, divisorDigits, divisor);
        return nLargeIntegers::divide(2, 1, dividend.digits, divisorDigits, quotientResult);
    }

    static tSigned32 compare(const tDistance& a1, const tDistance& a2)
    {
        return nLargeIntegers::compare(2, 2, a1.digits, a2.digits);
    }
    static tSigned32 compare(const tMultipliedDistance& a1, const tMultipliedDistance& a2)
    {
        return nLargeIntegers::compare(4, 4, a1.digits, a2.digits);
    }

    static bool isZero(const tDistance& value)
    {
        return nLargeIntegers::isZero(2, value.digits);
    }
    static bool isZero(const tMultipliedDistance& value)
    {
        return nLargeIntegers::isZero(4, value.digits);
    }
    static bool isNegative(const tDistance& value)
    {
        return nLargeIntegers::isNegative(2, value.digits);
    }
    static bool isNegative(const tMultipliedDistance& value)
    {
        return nLargeIntegers::isNegative(4, value.digits);
    }
    static void negate(tDistance& value)
    {
        nLargeIntegers::doNegate(2, value.digits);
    }
    static void negate(tMultipliedDistance& value)
    {
        nLargeIntegers::doNegate(4, value.digits);
    }
    static tDistance absoluteValueOf(const tDistance& value)
    {
        tDistance result=value;
        if(isNegative(result))
            negate(result);
        return result;
    }

//    static T choppedDivide(T lhs, T rhs)
//    {
//        return lhs/rhs;
//    }
//
    static T asT(const tDistance &value)
    {
        return nLargeIntegers::asLong(2, value.digits);
    }
    static float asFloat(const tDistance& value)
    {
        return nLargeIntegers::asFloat(2, value.digits);
    }
    static double asDouble(const tDistance& value)
    {
        return nLargeIntegers::asDouble(2, value.digits);
    }
};

#endif
