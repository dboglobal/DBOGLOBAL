//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "tMesh_Simple3D_Header.h"
#include "common/STL/vector.h"

//void
//MoveVerticesDown(std::vector<tVertex_Simple3D>& vertices, tSigned32 amount);

void
SplitHullForExtrusion(tMesh_Simple3D& hull, std::vector<tVertex_Simple3D>& verticesBelow);

void
Extrude(tMesh_Simple3D& hull, tSigned32 amount);
