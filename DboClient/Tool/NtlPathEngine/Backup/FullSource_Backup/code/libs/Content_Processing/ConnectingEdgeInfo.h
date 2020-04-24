//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/STL/vector.h"
#include "common/AttributesBuilder.h"

class cConnectingEdgeInfo
{
public:

    tEdge_3D_FaceTracked _edge;
    float _resolveRange;

    cAttributesBuilder _errorAttributes;
    tSigned32 _faceIndex;
};
