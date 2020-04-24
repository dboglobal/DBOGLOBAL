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

class cCollapsedVertexFilter;
class cFaceVertexEdge;
class cExternalShapeInfo;
class cConnectingEdgeInfo;
class iFaceVertexMesh;

float
ResolveRangeForConnectingEdge(const cCollapsedVertexFilter& mesh, const cFaceVertexEdge& edgeFV);

void
FaceVertexToMesh3D(const iFaceVertexMesh& faceVertex,
                   bool filterBySlope, float maximumSlope,
                   tMesh_3D_FaceTracked& result,
                   tSigned32 part, std::vector<std::pair<tSigned32, tSigned32> >& faceMapping,
                   std::vector<cExternalShapeInfo*>& buildingConnections,
                   std::vector<cConnectingEdgeInfo*>& connectingEdges,
                   const char *const* errorAttributes
                   );
