//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Iterators.h"
#include "libs/Geometry/interface/tPoint_Header.h"

class cConvexPoly;

bool
GetPointInsideShapeEntry(tEdge_3D e3D, const tPoint& origin, const cConvexPoly& cs, tPoint& result);
