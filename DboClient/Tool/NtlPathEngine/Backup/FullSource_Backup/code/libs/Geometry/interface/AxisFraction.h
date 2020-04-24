//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CORE_AXIS_FRACTION_INCLUDED
#define CORE_AXIS_FRACTION_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine_Header.h"

class cAxisFraction
{
public:

    cAxisFraction()
    {
    }
    cAxisFraction(const tPoint::tDistance& n, const tPoint::tDistance& d) :
     n(n),
     d(d)
    {
    }
    cAxisFraction(const tLine& axisLine, const tLine& intersector);

    tPoint::tDistance n, d;

    bool operator<(const cAxisFraction& rhs) const;
    bool operator>(const cAxisFraction& rhs) const;
    bool operator<=(const cAxisFraction& rhs) const;
    bool operator>=(const cAxisFraction& rhs) const;
    int compare(const cAxisFraction& rhs) const;

    double asDistance(const tLine& axisLine) const;

    float asFloat() const;

    tSigned32 multiplyLongByThis(tSigned32 toMultiply) const;
};

#endif
