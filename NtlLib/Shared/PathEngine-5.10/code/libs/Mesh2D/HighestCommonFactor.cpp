//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

//Note: Discovered by J. Stein in 1967. Another source says discovered by R. Silver and J. Tersian in 1962 and published by G. Stein in 1967. 
//
//The algorithm uses the following observations. 
//If u and v are both even, gcd(u,v) = 2 gcd(u/2, v/2). 
//If u is even and v is odd, gcd(u,v) = gcd(u/2, v). 
//Otherwise both are odd, and gcd(u,v) = gcd(|u-v|/2, v). (Euclid's algorithm with a division by 2 since the difference of two odd numbers is even.) 
//
//Here is the algorithm. It is especially efficient for operations on binary representations. 
//
//g = 1 
//while u is even and v is even 
// u = u/2 (right shift) 
// v = v/2 
// g = 2*g (left shift) 
//now u or v (or both) are odd 
//while u > 0 
// if u is even, u = u/2 
// else if v is even, v = v/2 
// else 
//  t = |u-v|/2 
//  if u < v, then v = t else u = t 
//return g*v 
//
//Called Algorithm B in [Knuth98, vol. 2, Sect. 4.5.2].

#include "platform_common/Header.h"
#include "HighestCommonFactor.h"
#include "common/interface/Assert.h"

tUnsigned32
HighestCommonFactor(tUnsigned32 u, tUnsigned32 v)
{
    if(u == 0)
    {
        assertD(v != 0);
        return v;
    }
    if(v == 0)
    {
        assertD(u != 0);
        return u;
    }

    tUnsigned32 g = 1;
    while((u & 1) == 0 && (v & 1) == 0)
    {
        u /= 2;
        v /= 2;
        g *= 2;
    }

    while(u > 0)
    {
        if((u & 1) == 0)
        {
            u = u/2;
        }
        else if((v & 1) == 0)
        {
            v = v/2;
        }
        else if(u < v)
        {
            v -= u;
            v /= 2;
        }
        else
        {
            u -= v;
            u /= 2;
        }
    }

    return v * g;
}

tSigned32
HighestCommonFactor(tSigned32 u, tSigned32 v)
{
    if(u < 0)
    {
        u = -u;
    }
    if(v < 0)
    {
        v = -v;
    }
    return static_cast<tSigned32>(HighestCommonFactor(static_cast<tUnsigned32>(u), static_cast<tUnsigned32>(v)));
}
