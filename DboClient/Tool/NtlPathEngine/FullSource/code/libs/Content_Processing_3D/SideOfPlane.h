//**********************************************************************
//
// Copyright (c) 2005-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SIDE_OF_PLANE_INCLUDED
#define SIDE_OF_PLANE_INCLUDED

#include "Plane.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/PointMultiplication.h"

inline eSideOfPlane
SideOfPlane(
        const cPlane& plane,
        const tPoint3& p
        )
{
    tPoint3 offset = p - plane._origin;
    cDynamicFraction dp(plane._normal.dotProduct(offset));
    if(dp.isNegative())
    {
        return BEHIND;
    }
    if(dp.isZero())
    {
        return ON_PLANE;
    }
    return IN_FRONT;
}
inline eSideOfPlane
SideOfPlane(
            const cPlane& plane,
            const cDynamicPoint3& p
            )
{
    cDynamicPoint3 offset(p);
    offset -= cDynamicPoint3(plane._origin);

    cDynamicFraction dp(offset.dotProduct(plane._normal));

//    cDynamicFraction result(_x);
//    result *= rhs._x;
//    cDynamicFraction product1(_y);
//    product1 *= rhs._y;
//    cDynamicFraction product2(_z);
//    product2 *= rhs._z;
//    result += product1;
//    result += product2;
//    return result;

    if(dp.isNegative())
    {
        return BEHIND;
    }
    if(dp.isZero())
    {
        return ON_PLANE;
    }
    return IN_FRONT;
}

inline void
Intersection(
            const cPlane& plane,
            const cDynamicPoint3& lineStart,
            const cDynamicPoint3& lineAxis,
            cDynamicPoint3& result
            )
{
    cDynamicFraction d = lineAxis.dotProduct(plane._normal);
    assertD(!d.isZero());
    cDynamicPoint3 offsetToPlaneOrigin(plane._origin);
    offsetToPlaneOrigin -= lineStart;
    cDynamicFraction n = offsetToPlaneOrigin.dotProduct(plane._normal);
    result = lineAxis;
    result *= n;
    result /= d;

    assertD(lineAxis._x.isZero() || n.isZero() || result._y / result._x == lineAxis._y / lineAxis._x);
    assertD(lineAxis._x.isZero() || n.isZero() || result._z / result._x == lineAxis._z / lineAxis._x);

    result += lineStart;

    assertD(SideOfPlane(plane, result) == ON_PLANE);
}

inline int
GetNZP(const cDynamicFraction& value)
{
    if(value.isNegative())
    {
        return -1;
    }
    if(value.isZero())
    {
        return 0;
    }
    return 1;
}
inline int
OppositeNZP(int nzp)
{
    return -nzp;
}

inline eSideOfPlane
SideOfPlane_1D(tSigned32 origin, const cDynamicFraction& normal, const cDynamicFraction& testPoint)
{
    assertD(!normal.isZero());
    cDynamicFraction originDF(origin);
    int comparison = originDF.compare(testPoint);
    if(normal.isNegative())
    {
        comparison *= -1;
    }
    if(comparison == 0)
    {
        return ON_PLANE;
    }
    if(comparison == 1)
    {
        return IN_FRONT;
    }
    return BEHIND;
}

inline eSideOfPlane
SideOfPlane_2D(
        tSigned32 originX, tSigned32 originY,
        const cDynamicFraction& normalX,
        const cDynamicFraction& normalY,
        const cDynamicFraction& testPointX,
        const cDynamicFraction& testPointY
        )
{
    cDynamicFraction offsetX(testPointX);
    offsetX -= originX;
    cDynamicFraction offsetY(testPointY);
    offsetY -= originY;
    cDynamicFraction dp(normalX);
    dp *= offsetX;
    cDynamicFraction temp(normalY);
    temp *= offsetY;
    dp += temp;
    if(dp.isNegative())
    {
        return BEHIND;
    }
    if(dp.isZero())
    {
        return ON_PLANE;
    }
    return IN_FRONT;
}

template <int xNZP, int yNZP, int zNZP> eSideOfPlane
SideOfPlane_TemplatedSpecialCases(
        const cPlane& plane,
        const cDynamicPoint3& p
        )
{
    if(xNZP == 0)
    {
        if(yNZP == 0)
        {
            return SideOfPlane_1D(plane._origin.getZ(), plane._normal.z(), p.z());
        }
        if(zNZP == 0)
        {
            return SideOfPlane_1D(plane._origin.getY(), plane._normal.y(), p.y());
        }
        return SideOfPlane_2D(plane._origin.getY(), plane._origin.getZ(), plane._normal.y(), plane._normal.z(), p.y(), p.z());
    }
    if(yNZP == 0)
    {
        if(xNZP == 0)
        {
            return SideOfPlane_1D(plane._origin.getZ(), plane._normal.z(), p.z());
        }
        if(zNZP == 0)
        {
            return SideOfPlane_1D(plane._origin.getX(), plane._normal.x(), p.x());
        }
        return SideOfPlane_2D(plane._origin.getX(), plane._origin.getZ(), plane._normal.x(), plane._normal.z(), p.x(), p.z());
    }
    if(zNZP == 0)
    {
        if(xNZP == 0)
        {
            return SideOfPlane_1D(plane._origin.getY(), plane._normal.y(), p.y());
        }
        if(yNZP == 0)
        {
            return SideOfPlane_1D(plane._origin.getX(), plane._normal.x(), p.x());
        }
        return SideOfPlane_2D(plane._origin.getX(), plane._origin.getY(), plane._normal.x(), plane._normal.y(), p.x(), p.y());
    }
    cDynamicPoint3 offset = p;
    offset -= plane._origin;
    int p_xNZP = GetNZP(offset.x());
    int p_yNZP = GetNZP(offset.y());
    int p_zNZP = GetNZP(offset.z());
    if(p_xNZP == xNZP && p_yNZP == yNZP && p_zNZP == zNZP)
    {
        return IN_FRONT;
    }
    if(p_xNZP == OppositeNZP(xNZP) && p_yNZP == OppositeNZP(yNZP) && p_zNZP == OppositeNZP(zNZP))
    {
        return BEHIND;
    }

    cDynamicFraction partialDP(offset.x());
    partialDP *= plane._normal.x();
    cDynamicFraction yProduct(offset.y());
    yProduct *= plane._normal.y();
    partialDP += yProduct;
    cDynamicFraction zProduct(offset.z());
    zProduct *= plane._normal.z();

    // figure out sign of full dotProduct (partialDP + zProduct), without actually doing the add

    if(partialDP.isNegative())
    {
        if(zProduct.isNegative() || zProduct.isZero())
        {
            return BEHIND;
        }
        partialDP.negate();
        int comparison = partialDP.compare(zProduct);
        if(comparison == 0)
        {
            return ON_PLANE;
        }
        if(comparison == 1)
        {
            return IN_FRONT;
        }
        return BEHIND;
    }

    if(partialDP.isZero())
    {
        if(zProduct.isNegative())
        {
            return BEHIND;
        }
        if(zProduct.isZero())
        {
            return ON_PLANE;
        }
        return IN_FRONT;
    }

    if(!zProduct.isNegative())
    {
        return IN_FRONT;
    }
    zProduct.negate();
    int comparison = partialDP.compare(zProduct);
    if(comparison == 0)
    {
        return ON_PLANE;
    }
    if(comparison == 1)
    {
        return BEHIND;
    }
    return IN_FRONT;
}

//template <int xNZP, int yNZP> eSideOfPlane
//SideOfPlane_TestTemplatedSpecialCases_2(
//        const cPlane& plane,
//        const cDynamicPoint3& p
//        )
//{
//    int zNZP = GetNZP(plane._normal.z());
//    if(zNZP == -1)
//    {
//        return SideOfPlane_TemplatedSpecialCases<xNZP, yNZP, -1>(plane, p);
//    }
//    else if(zNZP == 0)
//    {
//        return SideOfPlane_TemplatedSpecialCases<xNZP, yNZP, 0>(plane, p);
//    }
//    else
//    {
//        return SideOfPlane_TemplatedSpecialCases<xNZP, yNZP, 1>(plane, p);
//    }
//}
//template <int xNZP> eSideOfPlane
//SideOfPlane_TestTemplatedSpecialCases_1(
//        const cPlane& plane,
//        const cDynamicPoint3& p
//        )
//{
//    int yNZP = GetNZP(plane._normal.y());
//    if(yNZP == -1)
//    {
//        return SideOfPlane_TestTemplatedSpecialCases_2<xNZP, -1>(plane, p);
//    }
//    else if(yNZP == 0)
//    {
//        return SideOfPlane_TestTemplatedSpecialCases_2<xNZP, 0>(plane, p);
//    }
//    else
//    {
//        return SideOfPlane_TestTemplatedSpecialCases_2<xNZP, 1>(plane, p);
//    }
//}
//inline eSideOfPlane
//SideOfPlane_TestTemplatedSpecialCases(
//        const cPlane& plane,
//        const cDynamicPoint3& p
//        )
//{
//    int xNZP = GetNZP(plane._normal.x());
//    eSideOfPlane result;
//    if(xNZP == -1)
//    {
//        result = SideOfPlane_TestTemplatedSpecialCases_1<-1>(plane, p);
//    }
//    else if(xNZP == 0)
//    {
//        result = SideOfPlane_TestTemplatedSpecialCases_1<0>(plane, p);
//    }
//    else
//    {
//        result = SideOfPlane_TestTemplatedSpecialCases_1<1>(plane, p);
//    }
//    eSideOfPlane checkResult = SideOfPlane(plane, p);
//    assertR(checkResult == result);
//    return result;
//}

inline bool
PlanesAreEqual(const cPlane& p1, const cPlane& p2)
{
// assumes that both planes normal have been normalised, so that that can be compared without scaling
    if(p1._normal != p2._normal)
    {
        return false;
    }
    return SideOfPlane(p1, p2._origin) == ON_PLANE;
}

#endif
