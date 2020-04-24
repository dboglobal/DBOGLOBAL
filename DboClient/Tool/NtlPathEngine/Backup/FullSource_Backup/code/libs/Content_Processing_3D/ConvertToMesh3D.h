//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__CONVERT_TO_MESH_3D_INCLUDED
#define CONTENT_PROCESSING_3D__CONVERT_TO_MESH_3D_INCLUDED

#include "tMesh_Dynamic_Header.h"
#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

class cBSPVertices;

void
ConvertToMesh3D(const cBSPVertices& vertices, tMesh_Dynamic& from, tMesh_3D& to);

#endif
