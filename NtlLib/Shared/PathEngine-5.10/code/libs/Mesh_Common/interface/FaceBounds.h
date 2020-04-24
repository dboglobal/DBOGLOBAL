//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FACE_BOUNDS_INCLUDED
#define FACE_BOUNDS_INCLUDED

#include "libs/Geometry/interface/tPoint.h"

template <class tMesh> void
GetFaceBounds(typename tMesh::face f, tPoint& bottomLeft, tPoint& topRight)
{
    typename tMesh::edge e = tMesh::beginEdge(f);
    bottomLeft = GetVertexPoint(e);
    topRight = GetVertexPoint(e);
    while(e.toNextInFace())
    {
        bottomLeft.updateAsMinimumBound(GetVertexPoint(e));
        topRight.updateAsMaximumBound(GetVertexPoint(e));
    }
}       

#endif
