//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "GetOriginAndNormal.h"
#include "tMesh_Simple3D.h"
#include "Plane.h"
#include "DynamicPoint3.h"
#include "libs/Geometry/interface/tPoint3.h"

void
GetOriginAndNormal(tFace_Simple3D f, cPlane& result)
{
    tEdge_Simple3D e = tMesh_Simple3D::beginEdge(f);
    result._origin = GetVertexPoint3(e);
    tPoint3 axis1 = GetVertexPoint3(e.next()) - result._origin;
    tPoint3 axis2 = GetVertexPoint3(e.prev()) - result._origin;
    cDynamicPoint3(axis2).crossProduct(axis1, result._normal);
    result._normal.fastNormalise();
}

