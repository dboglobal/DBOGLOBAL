//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/STL/vector.h"

template <class T> class cCollapsedVectorVector;

void
CopyMesh3DPart(
        tMesh_3D& mesh3D,
        const cCollapsedVectorVector<tSigned32>& subsetsLookup,
        tSigned32 subsetIndex,
        tMesh_3D& mesh3DResult,
        std::vector<tSigned32>& faceRemap // this vector must be presized to the number of internal faces in mesh3D, but its contents do not need to be initialised
        );

void
AddFaceToCopiedMesh3DPart(
        tMesh_3D& mesh3D,
        tSigned32 fIndex,
        tMesh_3D& mesh3DResult,
        std::vector<tSigned32>& faceRemap
        );

cMeshElement<tMesh_3D>
TranslateToCopiedMesh3DPart(
        const cMeshElement<tMesh_3D>& toTranslate,
        const std::vector<tSigned32>& faceRemap,
        tMesh_3D& originalMesh,
        tMesh_3D& part
        );

