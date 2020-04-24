//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ELEMENT_COLLISION_INFO_INCLUDED
#define ELEMENT_COLLISION_INFO_INCLUDED

#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tPoint.h"

class cCircuitElement;

class cElementCollisionInfo
{
public:

    cElementCollisionInfo()
    {
        element = 0;
    }

    bool collisionOccurred() const
    {
        return element != 0;
    }

    const cCircuitElement* element;
    tSigned32 faceIndex;
    bool atCorner;
    tPoint::tDistance n, d;

    void getIntersectingLine(const tLine& testLine, tLine& result) const;
    bool isBefore(const cElementCollisionInfo& rhs) const;
};

#endif
