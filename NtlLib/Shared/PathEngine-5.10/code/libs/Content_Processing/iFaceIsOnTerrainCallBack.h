//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_FACE_IS_ON_TERRAIN_CALL_BACK__INCLUDED
#define I_FACE_IS_ON_TERRAIN_CALL_BACK__INCLUDED

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

class iFaceIsOnTerrainCallBack
{
public:
    virtual bool faceIsOnTerrain(tFace_3D_FaceTracked f) = 0;
};

#endif
