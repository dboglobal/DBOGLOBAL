//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "tMesh_Simple3D_Header.h"
#include "libs/Geometry/interface/tPoint3_Header.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;

void
ExtrudedOperandsFromFaceVertex(
        const iFaceVertexMesh& faceVertex, const tPoint3& extrudeOffset,
        tSigned32 sourceMeshIndex,
        std::vector<tMesh_Simple3D*>& appendTo,
        std::vector<tSigned32>& meshAndFaceLookup
        );
