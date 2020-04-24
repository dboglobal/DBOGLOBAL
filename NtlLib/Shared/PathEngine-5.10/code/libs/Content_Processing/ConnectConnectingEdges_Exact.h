//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

class cConnectingEdgeInfo;

// baseMesh should have external faces stripped before calling this
// no errors are reported in case of failure by this version
void
ConnectConnectingEdges_Exact(
        tMesh_3D_FaceTracked& baseMesh,
        const std::vector<cConnectingEdgeInfo*>& connectingEdges
        );
