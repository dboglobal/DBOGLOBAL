//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "libs/Geometry/interface/tIntersection.h"

cAxisFraction::cAxisFraction(const tLine& axisLine, const tLine& intersector)
{
    tIntersection intersection(axisLine, intersector);
    intersection.getAxisFraction(n, d);
}

bool
cAxisFraction::operator<(const cAxisFraction& rhs) const
{
    return CompareFractions(rhs.n, rhs.d, n, d) == -1;
}
bool
cAxisFraction::operator>(const cAxisFraction& rhs) const
{
    return CompareFractions(rhs.n, rhs.d, n, d) == 1;
}
bool
cAxisFraction::operator<=(const cAxisFraction& rhs) const
{
    return CompareFractions(rhs.n, rhs.d, n, d) != 1;
}
bool
cAxisFraction::operator>=(const cAxisFraction& rhs) const
{
    return CompareFractions(rhs.n, rhs.d, n, d) != -1;
}
int
cAxisFraction::compare(const cAxisFraction& rhs) const
{
    return CompareFractions(n, d, rhs.n, rhs.d);
}


double
cAxisFraction::asDistance(const tLine& axisLine) const
{
    double result = tMultiplier::asDouble(n);
    result *= Length_AsDouble(axisLine.axis());
    result /= tPoint::tMultiplier::asDouble(d);
    return result;
}

float
cAxisFraction::asFloat() const
{
    double result = tMultiplier::asDouble(n);
    result /= tPoint::tMultiplier::asDouble(d);
    return static_cast<float>(result);
}

tSigned32
cAxisFraction::multiplyLongByThis(tSigned32 toMultiply) const
{
    tPoint::tMultipliedDistance multipliedN;
    tMultiplier::multiply(n, toMultiply, multipliedN);
    tSigned32 result;
    tMultiplier::divide(multipliedN, d, result);
    return result;
}
