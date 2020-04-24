//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SideOfProjectedTo2D.h"
#include "DynamicPoint3.h"

static void
Project(const cDynamicPoint3& v, int projectionType, const cDynamicFraction*& x, const cDynamicFraction*& y)
{
    switch(projectionType)
    {
    default:
        invalid();
    case 0:
        x = &v.x();
        y = &v.y();
        break;
    case 1:
        x = &v.x();
        y = &v.z();
        break;
    case 2:
        x = &v.y();
        y = &v.z();
        break;
    }
}

int
GetProjectionType(const cDynamicPoint3& planeNormal)
{
    if(planeNormal.x().isZero())
    {
        if(planeNormal.y().isZero())
        {
          // planeNormal must go along the z axis
            if(planeNormal.z().isNegative())
            {
                return 3;
            }
            return 0;
        }
        // must be visible from along the y axis
        if(planeNormal.y().isNegative())
        {
            return 1;
        }
        return 4;
    }
    // must be visible from along the x axis
    if(planeNormal.x().isNegative())
    {
        return 5;
    }
    return 2;
}

static int
ProjectedTriHasZeroLengthEdge(
    const cDynamicFraction& x1,
    const cDynamicFraction& y1,
    const cDynamicFraction& x2,
    const cDynamicFraction& y2,
    const cDynamicFraction& x3,
    const cDynamicFraction& y3
    )
{
    if(x1 == x2 && y1 == y2)
    {
        return true;
    }
    if(x1 == x3 && y1 == y3)
    {
        return true;
    }
    if(x2 == x3 && y2 == y3)
    {
        return true;
    }
    return false;
}

static eSide
SideOf(const cDynamicFraction& axisX, const cDynamicFraction& axisY, const cDynamicFraction& testX, const cDynamicFraction& testY)
{
    cDynamicFraction lhs = testX;
    lhs *= axisY;
    cDynamicFraction rhs = testY;
    rhs *= axisX;
    int comparison = lhs.compare(rhs);
    if(comparison == 1)
    {
        return SIDE_LEFT;
    }
    if(comparison == 0)
    {
        return SIDE_CENTRE;
    }
    return SIDE_RIGHT;

}

int
GetProjectionType(const cDynamicPoint3& v1, const cDynamicPoint3& v2, const cDynamicPoint3& v3)
{
    const cDynamicFraction* x1;
    const cDynamicFraction* y1;
    const cDynamicFraction* x2;
    const cDynamicFraction* y2;
    const cDynamicFraction* x3;
    const cDynamicFraction* y3;
    int projectionType;
    for(projectionType = 0; projectionType != 3; ++projectionType)
    {
        Project(v1, projectionType, x1, y1);
        Project(v2, projectionType, x2, y2);
        Project(v3, projectionType, x3, y3);
        if(ProjectedTriHasZeroLengthEdge(*x1, *y1, *x2, *y2, *x3, *y3))
        {
            continue;
        }
        cDynamicFraction axisX, axisY, testX, testY;
        axisX = *x2 - *x1;
        axisY = *y2 - *y1;
        testX = *x3 - *x1;
        testY = *y3 - *y1;
        eSide side = SideOf(axisX, axisY, testX, testY);
        if(side == SIDE_CENTRE)
        {
            continue;
        }
        if(side == SIDE_LEFT)
        {
            projectionType += 3;
        }
        return projectionType;
    }
    invalid();
    return 0;
}

eSide
SideOfProjectedTo2D(int projectionType, const cDynamicPoint3& v1, const cDynamicPoint3& v2, const cDynamicPoint3& v3)
{
    const cDynamicFraction* x1;
    const cDynamicFraction* y1;
    const cDynamicFraction* x2;
    const cDynamicFraction* y2;
    const cDynamicFraction* x3;
    const cDynamicFraction* y3;
    bool reverse = false;
    if(projectionType >= 3)
    {
        projectionType -= 3;
        reverse = true;
    }
    Project(v1, projectionType, x1, y1);
    Project(v2, projectionType, x2, y2);
    Project(v3, projectionType, x3, y3);
    cDynamicFraction axisX, axisY, testX, testY;
    axisX = *x2 - *x1;
    axisY = *y2 - *y1;
    assertD(!axisX.isZero() || !axisY.isZero());
    testX = *x3 - *x1;
    testY = *y3 - *y1;
    eSide side = SideOf(axisX, axisY, testX, testY);
    if(reverse)
    {
        side = OtherSide(side);
    }
    return side;
}
