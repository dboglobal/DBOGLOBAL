//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "libs/Geometry/interface/tPoint3_Header.h"
#include "common/STL/vector.h"

void
ZoomExtents(const std::vector<tPoint3>& points, tSigned32 windowWidth, tSigned32 windowHeight);

void
ZoomExtents(tMesh_3D& mesh, tSigned32 windowWidth, tSigned32 windowHeight);

void
ZoomExtents(const std::vector<tPoint3>& points);