//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LARGE_INTEGERS_INCLUDED
#define LARGE_INTEGERS_INCLUDED

#define LARGE_INTEGERS_TYPE_1

#include "common/interface/Assert.h"
#include "platform_common/AssemblyMath.h"
#include "platform_support/LargeIntegers_Header.h"

namespace nLargeIntegers
{

const int MAXN = 14;

inline int
compareMagnitudes(const int N, const int N2, const tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    int i=N2;
    while(--i>=N)
    {
        if(rhsDigits[i])
            return 1;
    }
    i=N;
    while(--i>=N2)
    {
        if(lhsDigits[i])
            return -1;
    }
    while(i)
    {
        if(lhsDigits[i]>rhsDigits[i])
            return -1;
        if(lhsDigits[i]<rhsDigits[i])
            return 1;
        i--;
    }
    if(lhsDigits[0]>rhsDigits[0])
        return -1;
    if(lhsDigits[0]<rhsDigits[0])
        return 1;
    return 0;
}

inline bool
magnitudeGreaterThan(const int N, const int N2, const tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    return compareMagnitudes(N, N2, lhsDigits, rhsDigits) == -1;
}


inline void
addMagnitude(const int N, const int N2, tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    tUnsigned32 carry;
    int i;
    nAssemblyMath::add(rhsDigits[0],lhsDigits[0],carry);
    for(i = 1; i<N && i<N2; i++)
        nAssemblyMath::adc(rhsDigits[i],lhsDigits[i],carry);
    for(; i<N; i++)
    // propagate carry to end if necessary
        nAssemblyMath::adc(0,lhsDigits[i],carry);
    assertD(carry==0);
    for(; i<N2; i++)
    {
        assertD(rhsDigits[i]==0); // this would cause overflow
    }
}

inline void
subtractMagnitude(const int N, const int N2, tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    tUnsigned32 borrow;
    nAssemblyMath::sub(rhsDigits[0],lhsDigits[0],borrow);
    int i;
    for(i = 1; i<N && i<N2; i++)
        nAssemblyMath::sbb(rhsDigits[i],lhsDigits[i],borrow);
    for(; i<N; i++)
    // propagate borrow to end if necessary
        nAssemblyMath::sbb(0,lhsDigits[i],borrow);
    assertD(borrow==0);
    for(; i<N2; i++)
    {
        assertD(rhsDigits[i]==0); // this would cause underflow
    }
}

inline bool
sign(const int N, const tUnsigned32* lhsDigits)
{
    return lhsDigits[N] != 0;
}

inline void
constructFromLong(const int N, tUnsigned32* lhsDigits, tSigned32 value)
{
    if(value>=0)
    {
        lhsDigits[0]=value;
        lhsDigits[N]=false;
    }
    else
    {
        assertD(static_cast<tUnsigned32>(value)!=0x80000000);
        lhsDigits[0]=-value;
        lhsDigits[N]=true;
    }
    int i;
    for(i = 1; i < N; i++)
        lhsDigits[i]=0;
}


inline void
constructFrom(const int N, const int N2, tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    lhsDigits[N]=rhsDigits[N2];
    int i;
    for(i = 0; i<N && i<N2; i++)
        lhsDigits[i]=rhsDigits[i];
    for(; i<N; i++)
        lhsDigits[i]=0;
    for(; i<N2; i++)
    {
        assertD(rhsDigits[i]==0); // value of rhs is too large to be represented
    }
}

inline void
clear(const int N, tUnsigned32* lhsDigits)
{
    int i;
    for(i = 0; i < N; i++)
        lhsDigits[i]=0;
    lhsDigits[N]=false;
}

inline bool
isNegative(const int N, const tUnsigned32* lhsDigits)
{
    return lhsDigits[N] != 0;
}
inline bool
isZero(const int N, const tUnsigned32* lhsDigits)
{
    int i;
    for(i = 0; i < N; i++)
    {
        if(lhsDigits[i])
            return false;
    }
    return true;
}
inline tSigned32
asLong(const int N, const tUnsigned32* lhsDigits)
{
    int i;
    for(i = 1; i < N; i++)
    {
        assertD(lhsDigits[i]==0); // too large to be represented in a tSigned32
    }
    assertD(lhsDigits[0]<=0x7fffffff); 
// note that this doesn't support -0x80000000
    if(lhsDigits[N])
        return -static_cast<tSigned32>(lhsDigits[0]);
    return static_cast<tSigned32>(lhsDigits[0]);
}
inline float
asFloat(const int N, const tUnsigned32* lhsDigits)
{
    int i=N-1;
    while(lhsDigits[i]==0)
    {
        if(i==0)
            return 0.0f;
        --i;
    }
    float f=static_cast<float>(lhsDigits[i]);
    if(i)
    {
        --i;
        f*=0x10000;
        f*=0x10000;
        f+=lhsDigits[i];
    }
    while(i)
    {
        --i;
        f*=0x10000;
        f*=0x10000;
    }
    if(lhsDigits[N])
        f=-f;
    return f;
}
inline double
asDouble(const int N, const tUnsigned32* lhsDigits)
{
    int i=N-1;
    while(lhsDigits[i]==0)
    {
        if(i==0)
            return 0.0;
        --i;
    }
    double f=static_cast<float>(lhsDigits[i]);
    if(i)
    {
        --i;
        f*=0x10000;
        f*=0x10000;
        f+=lhsDigits[i];
    }
    if(i)
    {
        --i;
        f*=0x10000;
        f*=0x10000;
        f+=lhsDigits[i];
    }
    while(i)
    {
        --i;
        f*=0x10000;
        f*=0x10000;
    }
    if(lhsDigits[N])
        f=-f;
    return f;
}
inline void
doNegate(const int N, tUnsigned32* lhsDigits)
{
    lhsDigits[N]=!sign(N, lhsDigits);
    if(sign(N, lhsDigits) && isZero(N, lhsDigits))
        lhsDigits[N]=false;
}

inline void
negated(const int N, const tUnsigned32* lhsDigits, tUnsigned32* resultDigits)
{
    int i;
    for(i = 0; i < N; i++)
        resultDigits[i]=lhsDigits[i];
    resultDigits[N]=!lhsDigits[N];
}

inline bool
equal(const int N, const int N2, const tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    if(lhsDigits[N]!=rhsDigits[N2])
        return false;
    int i;
    for(i = 0; i<N && i<N2; i++)
    {
        if(lhsDigits[i]!=rhsDigits[i])
            return false;
    }
    for(; i<N; i++)
    {
        if(lhsDigits[i])
            return false;
    }
    for(; i<N2; i++)
    {
        if(rhsDigits[i])
            return false;
    }
    return true;
}

inline bool
lessThan(const int N, const int N2, const tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    bool result;
    if(sign(N, lhsDigits)!=sign(N2, rhsDigits))
        result=(sign(N, lhsDigits)==true);
    else
    if(sign(N, lhsDigits)==true)
        result=magnitudeGreaterThan(N, N2, lhsDigits, rhsDigits);
    else
        result=magnitudeGreaterThan(N2, N, rhsDigits, lhsDigits);
    return result;
}

inline int
compare(const int N, const int N2, const tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    int result;
    if(sign(N, lhsDigits)!=sign(N2, rhsDigits))
    {
        if(sign(N, lhsDigits))
        {
            result = 1;
        }
        else
        {
            result = -1;
        }
    }
    else
    if(sign(N, lhsDigits))
        result=compareMagnitudes(N2, N, rhsDigits, lhsDigits);
    else
        result=compareMagnitudes(N, N2, lhsDigits, rhsDigits);
    return result;
}

inline void
multiplyByLong(const int N, tUnsigned32* lhsDigits, tSigned32 rhs)
{
    if(rhs<0)
    {
        assertD(static_cast<tUnsigned32>(rhs)!=0x80000000);
        rhs=-rhs;
        lhsDigits[N]=!sign(N, lhsDigits);
    }
    tUnsigned32 _rhs=static_cast<tUnsigned32>(rhs);
    tUnsigned32 overflow;
    nAssemblyMath::mul(lhsDigits[N-1],_rhs,overflow,lhsDigits[N-1]);
    assertD(overflow==0);
    int i=N-1;
    while(i--)
    {
        nAssemblyMath::mul(lhsDigits[i],_rhs,overflow,lhsDigits[i]);
        tUnsigned32 carry;
        int j=i+1;
        nAssemblyMath::add(overflow,lhsDigits[j],carry);
        while(++j<N)
        {
        // propagate carry
            nAssemblyMath::adc(0,lhsDigits[j],carry);
        }
        assertD(carry==0);
    }
    if(sign(N, lhsDigits) && isZero(N, lhsDigits))
        lhsDigits[N]=false;
}

inline void
multiplyBy(const int N, const int N2, tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
//    tUnsigned32 dest[N];
    tUnsigned32 dest[MAXN];
    tUnsigned32 overflow,carry;
    int i;

// multiply by rhsDigits[0]
    i=N-1;
    do
    {
        nAssemblyMath::mul(rhsDigits[0],lhsDigits[i],overflow,dest[i]);
        if(i==N-1)
        {
            assertD(overflow==0);
        }
        else
        {
            int j=i+1;
            nAssemblyMath::add(overflow,dest[j],carry);
            while(++j<N)
                nAssemblyMath::adc(0,dest[j],carry);
            assertD(carry==0);
        }
    }
    while(i--);

// multiply and add products for remaining lhsDigitss
    for(i = 1; i < N2; i++)
    {
        if(rhsDigits[i]==0)
            continue;

        int j=N-1;

        // skip lhsDigits that would cause overflow
        while(j+i>=N)
        {
            assertD(lhsDigits[j]==0);
            j--;
        }

        while(j>=0)
        {
            int k=j+i;

            tUnsigned32 product;
            nAssemblyMath::mul(rhsDigits[i],lhsDigits[j],overflow,product);

            nAssemblyMath::add(product,dest[k],carry);
            k++;
            if(k==N)
            {
                assertD(overflow==0);
            }
            else
            {
                nAssemblyMath::adc(overflow,dest[k],carry);
                while(++k!=N)
                    nAssemblyMath::adc(0,dest[k],carry);
            }
            assertD(carry==0);

            j--;
        }
    }

// finally copy dest into lhsDigits
    for(i = 0; i < N; i++)
        lhsDigits[i] = dest[i];

    if(rhsDigits[N2])
        lhsDigits[N]=!lhsDigits[N];
}

inline void
addTo(const int N, const int N2, tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    if(rhsDigits[N2]!=lhsDigits[N])
    {
    // subtract
        if(magnitudeGreaterThan(N2, N, rhsDigits, lhsDigits))
        {
        // subtract this from rhs and store result in this
//            tUnsigned32 resultDigits[N2 + 1];
            tUnsigned32 resultDigits[MAXN + 1];
            tSigned32 i;
            for(i = 0; i <= N2; i++)
            {
                resultDigits[i] = rhsDigits[i];
            }
            subtractMagnitude(N2, N, resultDigits, lhsDigits);
            resultDigits[N2]=!lhsDigits[N];
            constructFrom(N, N2, lhsDigits, resultDigits);
        }
        else
        {
        // subtract rhs from this
            subtractMagnitude(N, N2, lhsDigits, rhsDigits);
            if(lhsDigits[N] && isZero(N, lhsDigits))
                lhsDigits[N]=false;
        }
    }
    else
        addMagnitude(N, N2, lhsDigits, rhsDigits);
}
inline void
subtractFrom(const int N, const int N2, tUnsigned32* lhsDigits, const tUnsigned32* rhsDigits)
{
    if(rhsDigits[N2]==lhsDigits[N])
    {
    // subtract
        if(magnitudeGreaterThan(N2, N, rhsDigits, lhsDigits))
        {
        // subtract this from rhs and store result in this
//            tUnsigned32 resultDigits[N2 + 1];
            tUnsigned32 resultDigits[MAXN + 1];
            tSigned32 i;
            for(i = 0; i <= N2; i++)
            {
                resultDigits[i] = rhsDigits[i];
            }
            subtractMagnitude(N2, N, resultDigits, lhsDigits);
            resultDigits[N2]=!lhsDigits[N];
            constructFrom(N, N2, lhsDigits, resultDigits);
        }
        else
        {
        // subtract rhs from this
            subtractMagnitude(N, N2, lhsDigits, rhsDigits);
            if(lhsDigits[N] && isZero(N, lhsDigits))
                lhsDigits[N]=false;
        }
    }
    else
        addMagnitude(N, N2, lhsDigits, rhsDigits);
}
//cLargeInteger<N>::operator/(const cLargeInteger &rhs)
//{
//    tUnsigned32 h=lhsDigits[1];
//    tUnsigned32 l=lhsDigits[0];
//    tUnsigned32 h2=rhsDigits[1];
//    tUnsigned32 l2=rhsDigits[0];
//
//    // shift both down until divisor fits into 32 bits
//    while(h2)
//    {
//        l2>>=1;
//        if(h2&1)
//            l2+=0x80000000;
//        h2>>=1;
//
//        l>>=1;
//        if(h&1)
//            l+=0x80000000;
//        h>>=1;
//    }
//
//    tUnsigned32 remainder;
//    nAssemblyMath::div(h,l,l2,l,remainder);
//    assertD(l<=0x7fffffff);
//    if(sign != rhs.sign)
//        return -static_cast<tSigned32>(l);
//    return l;
//}


//inline void
//multiply(tSigned32 a1, tSigned32 a2, cLargeInteger2& result)
//{
//    tUnsigned32 low;
//    tUnsigned32 high;
//    nAssemblyMath::imul(a1, a2, high, low);
//    result.digits[2] = false;
//    if(high & 0x80000000)
//    {
//        result.digits[2] = true;
//        low = ~low;
//        high = ~high;
//        tUnsigned32 carry;
//        const tUnsigned32 to_add = 1;
//        nAssemblyMath::add(to_add, low, carry);
//        nAssemblyMath::adc(carry, high, carry);
//        assertD(!carry);
//    }
//    result.digits[0] = low;
//    result.digits[1] = high;
//}
inline void
multiply(tSigned32 a1, tSigned32 a2, cLargeInteger2& result)
{
    _asm
    {
        mov eax,a1
        imul a2
//        mov ebx,result
//        mov ebx,[ebx]result.digits
        mov ebx,result
        mov dword ptr [ebx]cLargeInteger2.digits,eax
        mov dword ptr [ebx]cLargeInteger2.digits + 4,edx
        mov dword ptr [ebx]cLargeInteger2.digits + 8,0
        test edx,80000000h
        je asmlabel_not_negative
        mov dword ptr [ebx]cLargeInteger2.digits + 8,1
        not dword ptr [ebx]cLargeInteger2.digits
        not dword ptr [ebx]cLargeInteger2.digits + 4
        add dword ptr [ebx]cLargeInteger2.digits,1
        adc dword ptr [ebx]cLargeInteger2.digits + 4,0
asmlabel_not_negative:
    }
}

//inline void
//multiply(const cLargeInteger2& source1, const cLargeInteger2& source2, cLargeInteger4& dest)
//{
//    tUnsigned32 carry;
//    tUnsigned32 high, low;
//
//    if(source1.digits[1] == 0 && source2.digits[1] == 0)
//    {
//        nAssemblyMath::mul(source1.digits[0], source2.digits[0], dest.digits[1], dest.digits[0]);
//        dest.digits[2] = 0;
//        dest.digits[3] = 0;
//    }
//    else
//    {
//    // multiply by source2.digits[1]
//
//        nAssemblyMath::mul(source1.digits[1], source2.digits[1], dest.digits[3], dest.digits[2]);
//
//        nAssemblyMath::mul(source1.digits[0], source2.digits[1], high, dest.digits[1]);
//        nAssemblyMath::add(high, dest.digits[2], carry);
//        if(carry)
//        {
//            nAssemblyMath::add(carry, dest.digits[3], carry);
//            assertD(!carry); //.... remove this at some point because this is called very often
//        }
//
//    // multiply by source2.digits[0]
//
//        nAssemblyMath::mul(source1.digits[1], source2.digits[0], high, low);
//        nAssemblyMath::add(low, dest.digits[1], carry);
//        nAssemblyMath::adc(high, dest.digits[2], carry);
//        if(carry)
//        {
//            nAssemblyMath::add(carry, dest.digits[3], carry);
//            assertD(!carry); //.... remove this at some point because this is called very often
//        }
//
//        nAssemblyMath::mul(source1.digits[0], source2.digits[0], high, dest.digits[0]);
//        nAssemblyMath::add(high, dest.digits[1], carry);
//        if(carry)
//        {
//            nAssemblyMath::add(carry, dest.digits[2], carry);
//            if(carry)
//            {
//                nAssemblyMath::add(carry, dest.digits[3], carry);
//                assertD(!carry);
//            }
//        }
//    }
//    dest.digits[4] = (source1.digits[2] != source2.digits[2]);
//}
inline void
multiply(const cLargeInteger2& source1, const cLargeInteger2& source2, cLargeInteger4& dest)
{
    typedef cLargeInteger2 tSource;
    typedef cLargeInteger4 tResult;
    _asm
    {
        mov ebx,source1
        mov ecx,source2
        mov eax,dword ptr [ebx]tSource.digits + 4
        or eax,dword ptr [ecx]tSource.digits + 4
        jne label_not_single_lhsDigits

        mov eax,dword ptr [ebx]tSource.digits
        mul dword ptr [ecx]tSource.digits

        mov esi,dest
        mov dword ptr [esi]tResult.digits + 4,edx
        mov dword ptr [esi]tResult.digits,eax
        mov dword ptr [esi]tResult.digits + 8,0
        mov dword ptr [esi]tResult.digits + 12,0

        mov eax,dword ptr [ebx ]tSource.digits + 8
        xor eax,dword ptr [ecx]tSource.digits + 8
        mov dword ptr [esi]tResult.digits + 16,eax
    }
    return;

label_not_single_lhsDigits:

    tUnsigned32 carry;
    tUnsigned32 high, low;

// multiply by source2Digits[1]

    nAssemblyMath::mul(source1.digits[1], source2.digits[1], dest.digits[3], dest.digits[2]);

    nAssemblyMath::mul(source1.digits[0], source2.digits[1], high, dest.digits[1]);
    nAssemblyMath::add(high, dest.digits[2], carry);
    if(carry)
    {
        nAssemblyMath::add(carry, dest.digits[3], carry);
        assertD(!carry); //.... remove this at some point because this is called very often
    }

// multiply by source2.digits[0]

    nAssemblyMath::mul(source1.digits[1], source2.digits[0], high, low);
    nAssemblyMath::add(low, dest.digits[1], carry);
    nAssemblyMath::adc(high, dest.digits[2], carry);
    if(carry)
    {
        nAssemblyMath::add(carry, dest.digits[3], carry);
        assertD(!carry); //.... remove this at some point because this is called very often
    }

    nAssemblyMath::mul(source1.digits[0], source2.digits[0], high, dest.digits[0]);
    nAssemblyMath::add(high, dest.digits[1], carry);
    if(carry)
    {
        nAssemblyMath::add(carry, dest.digits[2], carry);
        if(carry)
        {
            nAssemblyMath::add(carry, dest.digits[3], carry);
            assertD(!carry);
        }
    }
    dest.digits[4] = (source1.digits[2] != source2.digits[2]);
}

inline void
multiply(const cLargeInteger2& source1, tSigned32 source2, cLargeInteger4& dest)
{
    tUnsigned32 carry;
    tUnsigned32 high;

    dest.digits[4] = source1.digits[2];

    if(source2 & 0x80000000)
    {
        source2 = -source2;
        dest.digits[4] = !dest.digits[4];
    }
    tUnsigned32 s2 = static_cast<tUnsigned32>(source2);

    dest.digits[3] = 0;

    nAssemblyMath::mul(source1.digits[1], s2, dest.digits[2], dest.digits[1]);
    nAssemblyMath::mul(source1.digits[0], s2, high, dest.digits[0]);
    nAssemblyMath::add(high, dest.digits[1], carry);
    if(carry)
    {
        nAssemblyMath::add(carry, dest.digits[2], carry);
        // note that this cant carry because s2 is not a full tUnsigned32
        assertD(!carry); //.... remove this at some point because this is called very often
    }
}

// returns true if there is a remainder, otherwise false
inline bool
divide(const int N, const int N2, const tUnsigned32* dividendDigits,
                      const tUnsigned32* divisorDigits,
                      tSigned32& quotient_result)
{
    const tUnsigned32 mask=0x80000000;

// get indices for highest non zero lhsDigits
    int dividend_top=N-1;
    while(dividend_top>=0 && dividendDigits[dividend_top]==0)
        dividend_top--;

    int divisor_top=N2-1;
    while(divisor_top>=0 && divisorDigits[divisor_top]==0)
        divisor_top--;
    
// check for either side being zero
    assertD(divisor_top>=0); // cannot divide by zero
    if(dividend_top<0)
    {
    // dividend is 0
        quotient_result=0;
        return false;
    }

// check for divisor greater than or equal to dividend
    //.... optimise - use top lhsDigits already calculated
    {
        int compare_result=compareMagnitudes(N2, N, divisorDigits, dividendDigits);
        if(compare_result==-1) // divisor is greater than dividend
        {
            quotient_result=0;
            return true;
        }
        if(compare_result==0) // divisor is same as dividend
        {
            quotient_result=1;
            if(divisorDigits[N2] != dividendDigits[N])
                quotient_result=-1;
            return false;
        }
    }

    assertD(divisor_top==dividend_top || divisor_top==dividend_top-1);

// extract top two digits of divisor
    tUnsigned32 divide_by,below;
    divide_by=divisorDigits[divisor_top];
    below=0;
    if(divisor_top)
        below=divisorDigits[divisor_top-1];

// shift up into a single digit
    int shift=0;
    while((divide_by&mask)==0)
    {
        divide_by*=2;
        if(below&mask)
            divide_by++;
        below*=2;
        shift++;
    }

// extract top three digits of dividend
    tUnsigned32 h,l,b;
    h=0;
    if(divisor_top==dividend_top-1)
        h=dividendDigits[dividend_top];
    l=dividendDigits[divisor_top];
    b=0;
    if(divisor_top)
        b=dividendDigits[divisor_top-1];

// shift up by same amount
    {
        int i;
        for(i = 0; i < shift; i++)
        {
            assertD((h&mask)==0);
            h*=2;
            if(l&mask)
                h++;
            l*=2;
            if(b&mask)
                l++;
            b*=2;
        }
    }
    assertD(h || l>=divide_by);

// perform the division

    tUnsigned32 q_top,rem_top;
    assertD(h<divide_by); // this would overflow
    nAssemblyMath::div(h,l,divide_by,q_top,rem_top);

// this partial quotient may be too high

//    //... work out mathematically the maximum possible error in this partial quotient
//    //... if a full unsigned quotient was allowed then this would be >1
//    //... but since quotient must fit in a tSigned32, perhaps this is 1
//    //... then we can mutliply and compare on the fly
//
//    //.. could also
//    // extract tail of divisor
//    // mutiply by q_top
//    // compare with rem_top + tail of dividend
//
//    cLargeInteger<N2+1> multipliedOut = cLargeInteger<N2+1>(divisor);
//    cLargeInteger<1> _q_top;
//    _q_topDigits[0] = q_top;
//    _q_topSign = false;
//    multipliedOut *= _q_top;

    tUnsigned32 multipliedOutDigits[MAXN];
    constructFrom(N2 + 1, N2, multipliedOutDigits, divisorDigits);
    tUnsigned32 qTopDigits[2];
    qTopDigits[0] = q_top;
    qTopDigits[1] = 0;
    multiplyBy(N2 + 1, 1, multipliedOutDigits, qTopDigits);

    int compare_result = compareMagnitudes(N, N2 + 1, dividendDigits, multipliedOutDigits);
    while(compare_result>0)
    {
        subtractFrom(N2 + 1, N2, multipliedOutDigits, divisorDigits);
        assertD(q_top);
        q_top--;
        compare_result = compareMagnitudes(N, N2 + 1, dividendDigits, multipliedOutDigits);
    }

// cast to tSigned32 and sort out sign

    assertD(q_top < 0x80000000); // overflow tSigned32 representation - note negative max is not allowed
    quotient_result = static_cast<tSigned32>(q_top);

    if(isNegative(N2, divisorDigits))
        quotient_result = -quotient_result;
    if(isNegative(N, dividendDigits))
        quotient_result = -quotient_result;

// return true if there was a remainder
    return compare_result<0;
}

} // nLargeIntegers


inline
cLargeInteger2::cLargeInteger2(tSigned32 value)
{
    nLargeIntegers::constructFromLong(2, digits, value);
}
inline
cLargeInteger4::cLargeInteger4(tSigned32 value)
{
    nLargeIntegers::constructFromLong(4, digits, value);
}
inline
cLargeInteger4::cLargeInteger4(const cLargeInteger2& value)
{
    digits[0] = value.digits[0];
    digits[1] = value.digits[1];
    digits[2] = 0;
    digits[3] = 0;
    digits[4] = value.digits[2];
}

inline cLargeInteger2
operator-(const cLargeInteger2& lhs)
{
    cLargeInteger2 result(lhs);
    nLargeIntegers::doNegate(2, result.digits);
    return result;
}

inline void
operator+=(cLargeInteger2& lhs, tSigned32 rhs)
{
    tUnsigned32 rhsDigits[2];
    nLargeIntegers::constructFromLong(1, rhsDigits, rhs);
    nLargeIntegers::addTo(2, 1, lhs.digits, rhsDigits);
}
inline void
operator+=(cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    nLargeIntegers::addTo(2, 2, lhs.digits, rhs.digits);
}
inline void
operator+=(cLargeInteger4& lhs, const cLargeInteger2& rhs)
{
    nLargeIntegers::addTo(4, 2, lhs.digits, rhs.digits);
}
inline void
operator+=(cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    nLargeIntegers::addTo(4, 4, lhs.digits, rhs.digits);
}

inline void
operator-=(cLargeInteger2& lhs, tSigned32 rhs)
{
    tUnsigned32 rhsDigits[2];
    nLargeIntegers::constructFromLong(1, rhsDigits, rhs);
    nLargeIntegers::subtractFrom(2, 1, lhs.digits, rhsDigits);
}
inline void
operator-=(cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    nLargeIntegers::subtractFrom(2, 2, lhs.digits, rhs.digits);
}
inline void
operator-=(cLargeInteger4& lhs, const cLargeInteger2& rhs)
{
    nLargeIntegers::subtractFrom(4, 2, lhs.digits, rhs.digits);
}
inline void
operator-=(cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    nLargeIntegers::subtractFrom(4, 4, lhs.digits, rhs.digits);
}

inline cLargeInteger2
operator-(const cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    cLargeInteger2 result(lhs);
    result -= rhs;
    return result;
}
inline cLargeInteger2
operator-(const cLargeInteger2& lhs, tSigned32 rhs)
{
    return lhs - cLargeInteger2(rhs);
}

inline void
operator*=(cLargeInteger4& lhs, const cLargeInteger2& rhs)
{
    nLargeIntegers::multiplyBy(4, 2, lhs.digits, rhs.digits);
}
inline void
operator*=(cLargeInteger2& lhs, tSigned32 rhs)
{
    nLargeIntegers::multiplyByLong(2, lhs.digits, rhs);
}

inline bool operator==(const cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    return nLargeIntegers::equal(2, 2, lhs.digits, rhs.digits);
}
inline bool operator==(const cLargeInteger4& lhs, const cLargeInteger2& rhs)
{
    return nLargeIntegers::equal(4, 2, lhs.digits, rhs.digits);
}
inline bool operator==(const cLargeInteger2& lhs, const cLargeInteger4& rhs)
{
    return nLargeIntegers::equal(2, 4, lhs.digits, rhs.digits);
}
inline bool operator==(const cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    return nLargeIntegers::equal(4, 4, lhs.digits, rhs.digits);
}

inline bool operator!=(const cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    return !nLargeIntegers::equal(2, 2, lhs.digits, rhs.digits);
}
inline bool operator!=(const cLargeInteger4& lhs, const cLargeInteger2& rhs)
{
    return !nLargeIntegers::equal(4, 2, lhs.digits, rhs.digits);
}
inline bool operator!=(const cLargeInteger2& lhs, const cLargeInteger4& rhs)
{
    return !nLargeIntegers::equal(2, 4, lhs.digits, rhs.digits);
}
inline bool operator!=(const cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    return !nLargeIntegers::equal(4, 4, lhs.digits, rhs.digits);
}

inline bool operator<(const cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    return nLargeIntegers::lessThan(2, 2, lhs.digits, rhs.digits);
}
inline bool operator<(const cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    return nLargeIntegers::lessThan(4, 4, lhs.digits, rhs.digits);
}

inline bool operator<=(const cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    return nLargeIntegers::equal(2, 2, lhs.digits, rhs.digits)
        || nLargeIntegers::lessThan(2, 2, lhs.digits, rhs.digits);
}

inline bool operator>(const cLargeInteger2& lhs, const cLargeInteger2& rhs)
{
    return nLargeIntegers::lessThan(2, 2, rhs.digits, lhs.digits);
}
inline bool operator>(const cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    return nLargeIntegers::lessThan(4, 4, rhs.digits, lhs.digits);
}

inline bool operator>=(const cLargeInteger4& lhs, const cLargeInteger4& rhs)
{
    return !nLargeIntegers::lessThan(4, 4, lhs.digits, rhs.digits);
}

#endif
