//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BUILD_2D_FACE_PARTITION_INCLUDED
#define BUILD_2D_FACE_PARTITION_INCLUDED

#include "libs/MeshPair/interface/tMesh_TranslationBSP.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

void
Build2DFacePartition(
        tFace f2D,
        tMesh_3D& mesh3D,
        std::vector<tSigned32>& buffer,
        tMesh_3D& tempMesh,
        tMesh_TranslationBSP& result
        );

#endif
