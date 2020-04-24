//**********************************************************************
//
// Copyright (c) 2000-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DYNAMIC_FRACTION_INCLUDED
#define DYNAMIC_FRACTION_INCLUDED

#include "libs/Content_Processing_3D/DynamicInteger.h"

class cDynamicFraction
{
	cDynamicInteger numerator;
	cDynamicInteger denominator;
	void normalise()
	{
		if(denominator.isNegative())
		{
			denominator.negate();
			numerator.negate();
		}
	}
	void dividebyhcf();
public:
	cDynamicFraction()
	{
	}
    cDynamicFraction(const cDynamicInteger di) :
     numerator(di),
     denominator((tUnsigned32)(!numerator.isZero()))
	{
	}
    cDynamicFraction(const tSigned32 l) :
     numerator(l),
     denominator((tUnsigned32)(l != 0))
	{
	}

    cDynamicFraction(const cLargeInteger2& value);

	cDynamicFraction(const cDynamicFraction &d1)
	{
		numerator=d1.numerator;
		denominator=d1.denominator;
	}
	cDynamicFraction& operator=(const cDynamicFraction &d1)
	{
		numerator=d1.numerator;
		denominator=d1.denominator;
		return *this;
	}

    void
    swap(cDynamicFraction& rhs)
    {
        numerator.swap(rhs.numerator);
        denominator.swap(rhs.denominator);
    }

	cDynamicFraction operator+=(const cDynamicFraction &di2)
	{
		if(di2.isZero())
			return *this;
		if(isZero())
		{
			*this=di2;
			return *this;
		}
		if(denominator==di2.denominator)
		{
			numerator+=di2.numerator;
			if(numerator.isZero())
			{
				denominator=(tUnsigned32)0;
				return *this;
			}
		}
		else
		{
			numerator*=di2.denominator;
			assertD(!numerator.isZero());
			numerator+=di2.numerator*denominator;
			if(numerator.isZero())
			{
				denominator=(tUnsigned32)0;
				return *this;
			}
			denominator*=di2.denominator;
		}
		dividebyhcf();
		return *this;
	}
	cDynamicFraction operator-=(const cDynamicFraction &di2)
	{
		negate();
		*this+=di2;
		negate();
		return *this;
	}
	cDynamicFraction operator*=(const cDynamicFraction &di2)
	{
		if(isZero() || di2.isIdentity())
			return *this;
		if(di2.isZero())
		{
			numerator=(tUnsigned32)0;
			denominator=(tUnsigned32)0;
			return *this;
		}
		numerator*=di2.numerator;
		denominator*=di2.denominator;
		dividebyhcf();
		return *this;
	}
	cDynamicFraction operator/=(const cDynamicFraction &di2)
	{
		assertD(!di2.isZero());
		if(isZero() || di2.isIdentity())
			return *this;
		numerator*=di2.denominator;
		denominator*=di2.numerator;
		normalise();
		dividebyhcf();
		return *this;
	}
	cDynamicFraction operator-() const
	{
		cDynamicFraction returnvalue=*this;
		returnvalue.negate();
		return returnvalue;
	}


	friend bool operator==(const cDynamicFraction &s1, const cDynamicFraction &s2)
	{
		return s1.numerator==s2.numerator && s1.denominator==s2.denominator;
	}
	friend bool operator!=(const cDynamicFraction &s1, const cDynamicFraction &s2)
	{
		return !(s1==s2);
	}

    int
    compare(const cDynamicFraction& rhs) const
    {
        if(isNegative() != rhs.isNegative())
        {
			if(rhs.isNegative())
            {
                return -1;
            }
            return 1;
        }
		if(isZero() || rhs.isZero())
		{
			if(isZero() && rhs.isZero())
            {
                return 0;
            }
			if(isZero())
            {
                if(rhs.isNegative())
                {
                    return -1;
                }
                return 1;
            }
			if(isNegative())
            {
                return 1;
            }
            return -1;
		}
        int denominatorCompare = denominator.compare(rhs.denominator);
        int absoluteNumeratorCompare = numerator.compareAbsoluteValues(rhs.numerator);
        if(absoluteNumeratorCompare != denominatorCompare)
        {
            int absoluteValueCompare;
            if(absoluteNumeratorCompare == 0)
            {
                absoluteValueCompare = -denominatorCompare;
            }
            else
            {
                // denominators are equal or different but with the same effect as the numerator difference
                absoluteValueCompare = absoluteNumeratorCompare;
            }
            if(isNegative())
            {
                return absoluteValueCompare * -1;
            }
            return absoluteValueCompare;
        }
		cDynamicInteger lhsMultipliedOut = numerator * rhs.denominator;
		cDynamicInteger rhsMultipliedOut = rhs.numerator * denominator;
        return lhsMultipliedOut.compare(rhsMultipliedOut);
    }

	friend bool operator>(const cDynamicFraction &s1, const cDynamicFraction &s2)
	{
//		if(s1.isZero() || s2.isZero())
//		{
//			if(s1.isZero())
//				return s2.isNegative();
//			return !s1.isNegative();
//		}
//		if(s1.denominator==s2.denominator)
//			return s1.numerator>s2.numerator;
//		return s1.numerator*s2.denominator>s2.numerator*s1.denominator;
        return s1.compare(s2) == -1;
	}
	friend bool operator>=(const cDynamicFraction &s1, const cDynamicFraction &s2)
	{
//		return s1==s2 || s1>s2;
        return s1.compare(s2) != 1;
	}
	friend bool operator<(const cDynamicFraction &s1, const cDynamicFraction &s2)
	{
//		return s1!=s2 &! (s1>s2);
        return s1.compare(s2) == 1;
	}
	friend bool operator<=(const cDynamicFraction &s1, const cDynamicFraction &s2)
	{
//		return !(s1>s2);
        return s1.compare(s2) != -1;
	}

	bool isZero() const
	{
		return numerator.isZero();
	}
	bool isNegative() const
	{
		return numerator.isNegative();
	}
	void negate()
	{
		numerator.negate();
	}
	bool isIdentity() const
	{
		return denominator.isIdentity() && numerator.isIdentity();
	}

	tSigned32 asLong() const
	{
		if(numerator.isZero())
        {
			return 0;
        }
		cDynamicInteger quotient;
		quotient = numerator;
		quotient.divideBy(denominator);
		return quotient.asLong();
	}
	bool isValidLong() const
	{
	//.. optimise - try and check this without divide
		if(numerator.isZero())
        {
			return 1;
        }
		cDynamicInteger quotient;
		quotient = numerator;
		quotient.divideBy(denominator);
		return quotient.isValidLong();
	}

	tSigned32 asLong_RoundToNearest() const
	{
		if(numerator.isZero())
        {
			return 0;
        }
		cDynamicInteger quotient;
        quotient = denominator;
        quotient.shiftright();
        if(numerator.isNegative())
        {
            quotient.negate();
        }
		quotient += numerator;
		quotient.divideBy(denominator);
		return quotient.asLong();
	}

    float asFloat() const;
};

cDynamicFraction operator+(const cDynamicFraction &di1,const cDynamicFraction &di2);
cDynamicFraction operator-(const cDynamicFraction &di1,const cDynamicFraction &di2);
cDynamicFraction operator*(const cDynamicFraction &di1,const cDynamicFraction &di2);
cDynamicFraction operator/(const cDynamicFraction &di1,const cDynamicFraction &di2);

#endif
