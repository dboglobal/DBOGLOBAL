//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tLine_Header.h"

tFace
ResolveFaceForIntersection(
                tFace traverseFrom,
                const tLine& traverseLine,
                const tLine& intersectingLine);
