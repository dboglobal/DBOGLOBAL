//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/STL/vector.h"

class cTranslationToOverlapped;
class cCornerProvider;

void
AddMesh3DHardCorners(
        tMesh_3D& mesh3D,
        const cTranslationToOverlapped& translation,
        const std::vector<tSigned32>& surfaceTypes,
        const std::vector<float>& costs,
        cCornerProvider& addTo);

void
AddMesh3DSoftCorners(
        tMesh_3D& mesh3D,
        const cTranslationToOverlapped& translation,
        const std::vector<tSigned32>& surfaceTypes,
        const std::vector<float>& costs,
        const std::vector<tPoint>& costDirections,
        cCornerProvider& addTo);

