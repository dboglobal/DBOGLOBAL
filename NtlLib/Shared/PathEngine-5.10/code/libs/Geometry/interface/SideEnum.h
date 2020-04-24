//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SIDEENUM_INCLUDED
#define SIDEENUM_INCLUDED

#include "common/interface/Assert.h"

enum eSide
{
    SIDE_CENTRE,
    SIDE_LEFT,
    SIDE_RIGHT,
};

inline eSide OtherSide(eSide side)
{
    switch(side)
    {
    default:
//	invalid();
    case SIDE_CENTRE:
	return SIDE_CENTRE;
    case SIDE_LEFT:
	return SIDE_RIGHT;
    case SIDE_RIGHT:
	return SIDE_LEFT;
    }
}

enum eDirection
{
    DIRECTION_NONE,
    DIRECTION_FORWARD,
    DIRECTION_RIGHT,
    DIRECTION_BACK,
    DIRECTION_LEFT,
};

enum eSideOfPlane
{
    ON_PLANE,
    IN_FRONT,
    BEHIND,
};

inline eSideOfPlane OtherSide(eSideOfPlane side)
{
    switch(side)
    {
    default:
//        invalid();
    case ON_PLANE:
        return ON_PLANE;
    case IN_FRONT:
        return BEHIND;
    case BEHIND:
        return IN_FRONT;
    }
}

#endif
