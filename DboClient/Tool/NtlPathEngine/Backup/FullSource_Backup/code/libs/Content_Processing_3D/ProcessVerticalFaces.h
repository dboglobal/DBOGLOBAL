//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__PROCESS_VERTICAL_FACES_INCLUDED
#define CONTENT_PROCESSING_3D__PROCESS_VERTICAL_FACES_INCLUDED

#include "tMesh_Dynamic_Header.h"
#include "common/STL/vector.h"

class cBSPVertices;
class cPlane;
class iProgressCallBack;
class cDynamicFraction;

bool
IsVertical(tFace_Dynamic f, const std::vector<cPlane>& splitters);

void
GetVerticalFaceConnections(
        tFace_Dynamic f,
        tEdge_Dynamic& e1,
        tEdge_Dynamic& e2
        );

// assumes that downware facing faces have been removed
// (or at least disconnected)
void
ProcessVerticalFaces(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        const std::vector<int>& projectionTypes,
        const cDynamicFraction& maxStepHeight,
        iProgressCallBack* progressCallBack
        );

#endif
