//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Multiplication.h"

inline tSigned32
CompareFractions(const tPoint::tDistance& n1, const tPoint::tDistance& d1,
                 const tPoint::tDistance& n2, const tPoint::tDistance& d2)
{
    tPoint::tMultipliedDistance multiplied1, multiplied2;
    tMultiplier::multiply(n1, d2, multiplied1);
    tMultiplier::multiply(n2, d1, multiplied2);
    return tMultiplier::compare(multiplied1, multiplied2);
}

//......... double check range implications of this
inline tSigned32
CompareFractions(tPoint::tMultipliedDistance n1, const tPoint::tDistance& d1,
                 tPoint::tMultipliedDistance n2, const tPoint::tDistance& d2)
{
    n1 *= d2;
    n2 *= d1;
    return tMultiplier::compare(n1, n2);
}
