//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/InsideCorner.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"

bool InsideCorner(const tLine &before, const tLine &after, const tPoint &test)
{
    eDirection direction=before.direction(after);
    if(direction==DIRECTION_LEFT || direction==DIRECTION_FORWARD)
    {
    // concave or inline case
        return before.side(test)==SIDE_RIGHT || after.side(test)==SIDE_RIGHT;
    }
    if(direction==DIRECTION_RIGHT)
    {
    // convex case
        return before.side(test)==SIDE_RIGHT && after.side(test)==SIDE_RIGHT;
    }
    assertD(direction==DIRECTION_BACK);
    return before.direction(test)!=DIRECTION_BACK;
}

bool InsideCorner_Vector(const tLine &before, const tLine &after, const tPoint &test)
{
    eDirection direction=before.direction(after);
    if(direction==DIRECTION_LEFT || direction==DIRECTION_FORWARD)
    {
    // concave or inline case
        return before.axis().side(test)==SIDE_RIGHT || after.axis().side(test)==SIDE_RIGHT;
    }
    if(direction==DIRECTION_RIGHT)
    {
    // convex case
        return before.axis().side(test)==SIDE_RIGHT && after.axis().side(test)==SIDE_RIGHT;
    }
    assertD(direction==DIRECTION_BACK);
    return before.axis().direction(test)!=DIRECTION_BACK;
}

bool InsideCorner(const tLine &before, const tLine &after, const tLine &test)
{
    eDirection direction=before.direction(after);
    if(direction==DIRECTION_LEFT || direction==DIRECTION_FORWARD)
    {
    // concave or inline case
        return before.side(test)==SIDE_RIGHT || after.side(test)==SIDE_RIGHT;
    }
    if(direction==DIRECTION_RIGHT)
    {
    // convex case
        return before.side(test)==SIDE_RIGHT && after.side(test)==SIDE_RIGHT;
    }
    assertD(direction==DIRECTION_BACK);
    return before.direction(test)!=DIRECTION_BACK;
}

bool EntersCorner(const tLine &before, const tLine &after, const tLine &test)
{
    eDirection direction=before.direction(after);
    if(direction==DIRECTION_LEFT || direction==DIRECTION_FORWARD)
    {
    // concave or inline case
    //.. optimise - this is the same as the convex case
        return before.side(test)==SIDE_RIGHT && after.side(test)==SIDE_RIGHT;
    }
    if(direction==DIRECTION_RIGHT)
    {
    // convex case
        return before.side(test)==SIDE_RIGHT && after.side(test)==SIDE_RIGHT;
    }
    assertD(direction==DIRECTION_BACK);
    return before.direction(test)==DIRECTION_FORWARD;
}
