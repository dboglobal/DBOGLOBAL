//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/STL/vector.h"

template <class T> class cMeshWithTrackedTransform;
//class cObstacleInfo;
class cInternalPosition;

void
AddInternalVertex(cMeshWithTrackedTransform<tMesh>& mesh, tFace f, const tPoint& p);

void
AddInternalVertices(cMeshWithTrackedTransform<tMesh>& mesh,
                  const tMeshCircuits& circuits, const int frequency);

//void
//AddInternalVertices(cMeshWithTrackedTransform<tMesh>& mesh,
//                  const std::vector<cObstacleInfo>& obstacles,
//                  const int frequency);

void
AddInternalVerticesInsideObstructions(
                cMeshWithTrackedTransform<tMesh>& mesh,
                const tMeshCircuits& circuits, const int frequency
                );

void
AddInternalVertices(cMeshWithTrackedTransform<tMesh>& mesh,
                  const std::vector<cInternalPosition>& positions,
                  const std::vector<tMesh::tVertexData>& vertexData
                  );
