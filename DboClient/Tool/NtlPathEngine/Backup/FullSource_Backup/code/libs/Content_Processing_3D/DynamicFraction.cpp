//**********************************************************************
//
// Copyright (c) 2000-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "DynamicFraction.h"

cDynamicFraction::cDynamicFraction(const cLargeInteger2& value) :
 numerator(value),
 denominator((tUnsigned32)(!numerator.isZero()))
{
}

float
cDynamicFraction::asFloat() const
{
    if(isZero())
    {
        return 0.f;
    }
    return numerator.asFloat() / denominator.asFloat();
}

cDynamicFraction operator+(const cDynamicFraction &di1,const cDynamicFraction &di2)
{
	cDynamicFraction result=di1;
	return result+=di2;
}
cDynamicFraction operator-(const cDynamicFraction &di1,const cDynamicFraction &di2)
{
	cDynamicFraction result=di1;
	return result-=di2;
}
cDynamicFraction operator*(const cDynamicFraction &di1,const cDynamicFraction &di2)
{
	cDynamicFraction result=di1;
	return result*=di2;
}
cDynamicFraction operator/(const cDynamicFraction &di1,const cDynamicFraction &di2)
{
	cDynamicFraction result=di1;
	return result/=di2;
}

void cDynamicFraction::dividebyhcf()
{
	assertD(!isZero());
	int remainderflag;
	if(denominator.isIdentity() || numerator.isIdentity())
		return;
//.. optimise - obtain quotients directly from hcf algorithm
	cDynamicInteger hcf=highestcommonfactor(numerator,denominator);
	if(hcf.isIdentity())
		return;
#ifdef ASSERTIONS_ON
//.. useful for debugging
//.. slows down debug verion
	cDynamicInteger beforedivide;
	beforedivide=numerator;
#endif
//.. optimise - drop out early from division as it is known there will be no remainder?
	remainderflag=numerator.divideBy(hcf);
	assertD(remainderflag==0);
#ifdef ASSERTIONS_ON
//.. useful for debugging
//.. slows down debug verion
	beforedivide=denominator;
#endif
	remainderflag=denominator.divideBy(hcf);
	assertD(remainderflag==0);
}

