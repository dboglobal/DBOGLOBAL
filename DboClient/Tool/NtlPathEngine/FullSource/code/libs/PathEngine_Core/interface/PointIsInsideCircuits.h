//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include <common/STL/list.h>

class cInternalPosition;

bool PointIsInsideCircuits(const tMeshCircuits& circuits, const cInternalPosition& ip);
bool PointIsInsideCircuits(const tMeshCircuits& circuits, const std::list<tFace>& containedExternalFaces, const cInternalPosition& ip);
bool PointIsInsideCircuit(const tMeshCutSequence& circuit, tFace f, tIntersection p);
