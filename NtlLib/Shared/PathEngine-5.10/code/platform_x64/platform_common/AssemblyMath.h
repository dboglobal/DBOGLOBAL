//**********************************************************************
//
// Copyright (c) 2000-2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

namespace nAssemblyMath
{

inline void add(tUnsigned32 source, tUnsigned32& dest, tUnsigned32& carry)
{
    tUnsigned32 result = dest + source;
    carry = 0;
    if(result < dest)
    {
        carry = 1;
    }
    dest = result;
}
inline void adc(tUnsigned32 source, tUnsigned32& dest, tUnsigned32& carry)
{
    tUnsigned32 result = dest + carry;
    carry = 0;
    if(result < dest)
    {
        carry = 1;
    }
    result += source;
    if(result < source)
    {
        carry = 1;
    }
    dest = result;
}
inline void sub(tUnsigned32 source, tUnsigned32& dest, tUnsigned32& borrow)
{
    tUnsigned32 result = dest - source;
    borrow = 0;
    if(result > dest)
    {
        borrow = 1;
    }
    dest = result;
}
inline void sbb(tUnsigned32 source, tUnsigned32& dest, tUnsigned32& borrow)
{
    tUnsigned32 result = dest - source;
    if(result > dest)
    {
        result -= borrow;
        borrow = 1;
    }
    else if(result == 0)
    {
        result -= borrow;
    }
    else
    {
        result -= borrow;
        borrow = 0;
    }
    dest = result;
}

inline void
mul(tUnsigned32 source1, 
                tUnsigned32 source2, 
                tUnsigned32& destHigh, 
                tUnsigned32& destLow)
{
    tSigned64 result = source1;
    result *= source2;
    destLow = static_cast<tUnsigned32>(result);
    result >>= 32;
    destHigh = static_cast<tUnsigned32>(result);
}
inline void
imul(tSigned32 source1, 
                tSigned32 source2, 
                tUnsigned32& destHigh, 
                tUnsigned32& destLow)
{
    tSigned64 result = source1;
    result *= source2;
    destLow = static_cast<tUnsigned32>(result);
    result >>= 32;
    destHigh = static_cast<tUnsigned32>(result);
}
inline void
div(tUnsigned32 sourceHigh, 
                tUnsigned32 sourceLow,
                tUnsigned32 divisor, 
                tUnsigned32& dest,
                tUnsigned32& remainder
                )
{
    tUnsigned64 asI64;
    asI64 = sourceHigh;
    asI64 <<= 32;
    asI64 += sourceLow;
    tUnsigned64 result = asI64 / divisor;
    tUnsigned64 remultiplied = result * divisor;
    remainder = static_cast<tUnsigned32>(asI64 - remultiplied);
    dest = static_cast<tUnsigned32>(result);
}
inline int indexForTopBit(tUnsigned32 v)
{
    static const int MultiplyDeBruijnBitPosition[32] = 
    {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    v |= v >> 1; // first round down to power of 2 
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v = (v >> 1) + 1;
    return MultiplyDeBruijnBitPosition[(v * 0x077CB531UL) >> 27];
}

}
