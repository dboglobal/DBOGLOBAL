//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINT_TRAITS_INCLUDED
#define POINT_TRAITS_INCLUDED

#include "common/interface/Assert.h"
#include "platform_common/PointMultiplication.h"

class cPointTraits
{
public:

    typedef tSigned32 T;
    typedef cLargeInteger2 tDistance;
    typedef cLargeInteger4 tMultipliedDistance;
    typedef cLongMultiplier tMultiplier;

    static T
    getMaximumSupported()
    {
        return 16777215L;  // 2 to the 24 - 1, this limit is required for multiplying out vector fractions
    }
    static void
    checkRange(T range)
    {
        assertR(range >= 0 && range <= getMaximumSupported());
    }
    static void
    checkInRange(T range, T value)
    {
        assertR(value >= -range && value <= range);
    }
    static void
    addRange(T source, T& dest)
    {
        tDistance sum(source);
        sum += dest;
        assertR(sum <= tDistance(getMaximumSupported()));
        dest = tMultiplier::asT(sum);
    }
    static void
    multiplyRange(T& dest, tSigned32 multiplier)
    {
        tDistance product;
        tMultiplier::multiply(dest, multiplier, product);
        assertR(product <= tDistance(getMaximumSupported()));
        dest = tMultiplier::asT(product);
    }
    static void
    multiplyRange(T& dest, float multiplier)
    {
        double product = dest;
        product *= multiplier;
        assertR(product <= getMaximumSupported());
        dest = static_cast<T>(product);
    }
    static void
    divideRange(T& dest, tSigned32 divisor)
    {
        dest = dest / divisor;
    }
    static void
    checkRangeMultiply(T lhs, T rhs)
    {
        tDistance product;
        tMultiplier::multiply(lhs, rhs, product);
        assertR(product <= tDistance(getMaximumSupported()));
    }
};

#endif
