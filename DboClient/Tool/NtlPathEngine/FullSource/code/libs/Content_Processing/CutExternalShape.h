// **********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
// **********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

class cExternalShapeInfo;
class iFaceIsOnTerrainCallBack;

bool
CutExternalShape(
        tMesh_3D_FaceTracked& mesh,
        cExternalShapeInfo& shape,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack
        );
