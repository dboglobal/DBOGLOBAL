//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INT128_INCLUDED
#define INT128_INCLUDED

#include "platform_support/LargeIntegers.h"

class cInt128
{
    int compareMagnitudes(const cInt128& rhs) const
    {
        if(_high != rhs._high)
        {
            if(_high < rhs._high)
            {
                return 1;
            }
            if(_high > rhs._high)
            {
                return -1;
            }
            return 0;
        }
        if(_low < rhs._low)
        {
            return 1;
        }
        if(_low > rhs._low)
        {
            return -1;
        }
        return 0;
    }

    // used to support representing coordinates of intersections as high precision fractions
    void addMagnitude(const cInt128& rhs)
    {
        _low += rhs._low;
        if(_low < rhs._low)
        {
            _high++;;
        }
        _high += rhs._high;
    }

    // used to support representing coordinates of intersections as high precision fractions
    void subtractMagnitude(const cInt128& rhs)
    {
        _high -= rhs._high;
        tUnsigned64 before = _low;
        _low -= rhs._low;
        if(_low > before)
        {
            _high--;
        }
    }
    // used to support representing coordinates of intersections as high precision fractions
    void subtractMagnitudeFrom(const cInt128& rhs)
    {
        _high = rhs._high - _high;
        _low = rhs._low - _low;
        if(_low > rhs._low)
        {
            _high--;
        }
    }

    bool _isNegative;
    tUnsigned64 _high;
    tUnsigned64 _low;

public:

    cInt128()
    {
    }

    explicit cInt128(tSigned32 value)
    {
        _high = 0;
        if(value < 0)
        {
            assertD(value != LONG_MIN);
            _isNegative = true;
            _low = -value;
        }
        else
        {
            _isNegative = false;
            _low = value;
        }
    }
    explicit cInt128(tSigned64 value)
    {
        _high = 0;
        if(value < 0)
        {
            assertD(value != LLONG_MIN);
            _isNegative = true;
            _low = -value;
        }
        else
        {
            _isNegative = false;
            _low = value;
        }
    }

    void
    initialiseAsProduct(const tSigned64& lhs, const tSigned64& rhs)
    {
        nLargeIntegers::multiplyTo128Bits(lhs, rhs, _isNegative, _high, _low);
    }

    bool
    divideTo32BitQuotient(tSigned64 divisor, tSigned32& quotientResult) const
    {
        bool hasRemainder;
        nLargeIntegers::divideFrom128Bits(
                _isNegative, _high, _low,
                divisor,
                quotientResult, hasRemainder
                );
        return hasRemainder;
    }

    // used to support representing coordinates of intersections as high precision fractions
    void operator*=(tSigned64 rhs)
    {
        if(_high == 0 && _low == 0)
        {
            // note that this conditional prevents zero values getting marked as negative
            return;
        }
        if(rhs < 0)
        {
            rhs = -rhs;
            _isNegative = !_isNegative;
        }
        tUnsigned64 rhsU = static_cast<tUnsigned64>(rhs);
        tUnsigned64 addToHigh = __umulh(rhsU, _low);
        _high *= rhsU;
        _high += addToHigh;
        _low *= rhsU;
        //... check for overflow
    }

    // used to support representing coordinates of intersections as high precision fractions
    void operator+=(const cInt128& rhs)
    {
        if(_isNegative == rhs._isNegative)
        {
            addMagnitude(rhs);
        }
        else
        {
            int comparison = compareMagnitudes(rhs);
            if(comparison == 0)
            {
                _high = 0;
                _low = 0;
                _isNegative = false;
                return;
            }
            if(comparison == -1)
            {
                subtractMagnitude(rhs);
            }
            else
            {
                subtractMagnitudeFrom(rhs);
                _isNegative = rhs._isNegative;
            }
        }
    }

    void operator-=(const cInt128& rhs)
    {
        _isNegative = !_isNegative;
        (*this) += rhs;
        _isNegative = !_isNegative;
    }

    void negate()
    {
        if(_high || _low)
        {
            _isNegative = !_isNegative;
        }
    }

    bool isNegative() const
    {
        return _isNegative;
    }
    bool isZero() const
    {
        return _high == 0 && _low == 0;
    }

    tSigned32 topBit() const
    {
        if(_high)
        {
            tUnsigned64 value = _high;
            tSigned32 result = 64;
            while(value)
            {
                value /= 2;
                result++;
            }
            return result;
        }
        tUnsigned64 value = _low;
        tSigned32 result = 0;
        while(value)
        {
            value /= 2;
            result++;
        }
        return result;
    }

    int compare(const cInt128& rhs) const
    {
        if(_isNegative != rhs._isNegative)
        {
            if(_isNegative)
            {
                return 1;
            }
            return -1;
        }
        if(_isNegative)
        {
            return -compareMagnitudes(rhs);
        }
        return compareMagnitudes(rhs);
    }

    bool operator<(const cInt128& rhs) const
    {
        return compare(rhs) == 1;
    }
    bool operator>(const cInt128& rhs) const
    {
        return compare(rhs) == -1;
    }

    // this depends on _isNegative not being set in the case of zero values
    bool operator ==(const cInt128& rhs) const
    {
        assertF(_high != 0 || _low != 0 || !_isNegative); // zero values are not negative
        return
            _isNegative == rhs._isNegative
            && _high == rhs._high
            && _low == rhs._low;
    }
    bool operator!=(const cInt128& rhs) const
    {
        return !((*this) == rhs);
    }
};

#endif
