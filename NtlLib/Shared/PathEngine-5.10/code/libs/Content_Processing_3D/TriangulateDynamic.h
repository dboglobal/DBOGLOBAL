//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRIANGULATE_DYNAMIC_INCLUDED
#define TRIANGULATE_DYNAMIC_INCLUDED

#include "tMesh_Dynamic.h"
#include "common/STL/vector.h"

class cBSPVertices;
class cPlane;
class iProgressCallBack;

void
RemoveInternalVertex_Dynamic(tMesh_Dynamic& mesh, tVertex_Dynamic v, const cBSPVertices& vertices, int projectionType);

void
RemoveInlineEdgeVertex_NoExternalFaces_Dynamic(tMesh_Dynamic& mesh, tVertex_Dynamic v, const cBSPVertices& vertices, int projectionType);

void
RemoveInlineBoundaryVertex_Dynamic(tMesh_Dynamic& mesh, tEdge_Dynamic eBoundary1, tEdge_Dynamic eBoundary2, const cBSPVertices& vertices, int projectionType1, int projectionType2);

void
TriangulateFace_ConvexWithInlineEdges_Dynamic(
        tMesh_Dynamic& mesh,
        tFace_Dynamic& f,
        const cBSPVertices& vertices, int projectionType
        );

void
TriangulateMesh_Dynamic(tMesh_Dynamic& mesh, const cBSPVertices& vertices, const std::vector<int>& projectionTypes, iProgressCallBack* progressCallBack);

#endif
