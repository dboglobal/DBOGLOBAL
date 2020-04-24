//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class iFaceVertexMesh;

void
ExtractConnectingEdgeAndTerrainPortalInfo(
        const iFaceVertexMesh& ground,
        std::vector<tSigned32>& connectingEdgeVertices,
        std::vector<float>& connectingEdgeResolveRanges,
        std::vector<tSigned32>& terrainPortalInfo
        );
