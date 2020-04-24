//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifdef ASSERTIONS_ON
#ifndef CONTENT_PROCESSING_3D__VALIDATE_FACE_DYNAMIC_INCLUDED
#define CONTENT_PROCESSING_3D__VALIDATE_FACE_DYNAMIC_INCLUDED

#include "tMesh_Dynamic_Iterators.h"
#include "common/STL/vector.h"

class cBSPVertices;
class cPlane;

bool
ValidateFaceDynamic(
        const cBSPVertices& vertices,
        tFace_Dynamic f
        );

bool
ValidateFaceDynamic(
        const cBSPVertices& vertices,
        const cPlane& splitter,
        tFace_Dynamic f
        );

bool
ValidateFaceDynamic_Ground(
        const cBSPVertices& vertices,
        tFace_Dynamic f
        );

#endif
#endif
