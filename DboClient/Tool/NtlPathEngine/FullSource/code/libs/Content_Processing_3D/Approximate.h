//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__APPROXIMATE_INCLUDED
#define CONTENT_PROCESSING_3D__APPROXIMATE_INCLUDED

#include "tMesh_Dynamic_Header.h"
#include "common/STL/vector.h"

class cBSPVertices;
class cPlane;

void
ApproximateZsAndCollapseVerticalFaces(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters //.. this is required just for identifying vertical faces
        );

void
ApproximateHorizontalComponents(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh
        );

//void
//SplitVerticalDiscontinuities(
//        tMesh_Dynamic& mesh
//        );

#endif
