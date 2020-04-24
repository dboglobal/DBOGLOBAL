//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

class iFaceIsOnTerrainCallBack;
class cExternalShapeInfo;

void
ConnectPortals(
        tMesh_3D_FaceTracked& baseMesh,
        iFaceIsOnTerrainCallBack& faceIsOnTerrain0CallBack,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
        const std::vector<cExternalShapeInfo*>& externalShapes
        );
