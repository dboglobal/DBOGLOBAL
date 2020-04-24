//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INTERNAL_POSITION_INCLUDED
#define INTERNAL_POSITION_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tPoint.h"

class cInternalPosition
{
public:

    cInternalPosition()
    {
    }
    cInternalPosition(tFace f, tPoint p) :
      f(f), p(p)
    {
    }
//    cInternalPosition(tFace f); // used to construct as invalid

    bool valid() const;
    void setInvalid();

    bool operator==(const cInternalPosition& rhs) const
    {
        return f == rhs.f && p == rhs.p;
    }
    bool operator!=(const cInternalPosition& rhs) const
    {
        return f != rhs.f || p != rhs.p;
    }

    tSigned32 cell() const;

    tFace f;
    tPoint p;
};

#endif
