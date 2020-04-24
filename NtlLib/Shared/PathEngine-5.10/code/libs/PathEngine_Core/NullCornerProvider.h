//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef NULL_CORNER_PROVIDER_INCLUDED
#define NULL_CORNER_PROVIDER_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/tMesh.h"

class cNullCornerProvider
{
public:

// interface to support the state space

    static tSigned32
    size()
    {
        return 0;
    }
    static tFace cornerFace(tSigned32 index)
    {
        invalid();
        return tMesh::endFace();
    }
    static tPoint
    cornerPoint(tSigned32 index)
    {
        invalid();
        return tPoint(0,0,0);
    }
    static tPoint
    axisBefore(tSigned32 index)
    {
        invalid();
        return tPoint(0,0,0);
    }
    static tPoint
    axisAfter(tSigned32 index)
    {
        invalid();
        return tPoint(0,0,0);
    }
};

#endif
