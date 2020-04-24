//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/Side_Directions.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"

//eSide
//Side_Directions(eDirection base, eDirection test)
//{
//    assertD(base != test);
//    assertD(base != DIRECTION_NONE);
//    assertD(test != DIRECTION_NONE);
//    int baseOrder = int(base);
//    if(baseOrder == 1)
//    {
//        baseOrder = 5;
//    }
//    int testOrder = int(test);
//    if(testOrder == 1)
//    {
//        testOrder = 5;
//    }
//    if(testOrder > baseOrder)
//    {
//        return SIDE_RIGHT;
//    }
//    else
//    {
//        return SIDE_LEFT;
//    }
//}

eSide
Side_Directions(eDirection base, eDirection test)
{
    assertD(base != test);
    assertD(base != DIRECTION_NONE);
    assertD(test != DIRECTION_NONE);
    int baseOrder = int(base);
    int testOrder = int(test);
    if(testOrder > baseOrder)
    {
        return SIDE_RIGHT;
    }
    else
    {
        return SIDE_LEFT;
    }
}

eSide
Side_Directions(const tPoint& base, const tPoint& lhs, const tPoint& rhs)
{
    eDirection lhsDirection = base.direction(lhs);
    eDirection rhsDirection = base.direction(rhs);
    if(lhsDirection == rhsDirection)
    {
        return lhs.side(rhs);
    }
    else
    {
        return Side_Directions(lhsDirection, rhsDirection);
    }
}

eSide
Side_Directions_BaseAtEnd(eDirection base, eDirection test)
{
    assertD(base != test);
    assertD(base != DIRECTION_NONE);
    assertD(test != DIRECTION_NONE);
    int baseOrder = int(base);
    if(baseOrder == 1)
    {
        baseOrder = 5;
    }
    int testOrder = int(test);
    if(testOrder == 1)
    {
        testOrder = 5;
    }
    if(testOrder > baseOrder)
    {
        return SIDE_RIGHT;
    }
    else
    {
        return SIDE_LEFT;
    }
}

eSide
Side_Directions_BaseAtEnd(const tPoint& base, const tPoint& lhs, const tPoint& rhs)
{
    eDirection lhsDirection = base.direction(lhs);
    eDirection rhsDirection = base.direction(rhs);
    if(lhsDirection == rhsDirection)
    {
        return lhs.side(rhs);
    }
    else
    {
        return Side_Directions_BaseAtEnd(lhsDirection, rhsDirection);
    }
}
