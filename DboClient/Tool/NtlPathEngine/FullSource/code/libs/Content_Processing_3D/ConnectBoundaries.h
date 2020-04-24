//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__CONNECT_BOUNDARIES_INCLUDED
#define CONTENT_PROCESSING_3D__CONNECT_BOUNDARIES_INCLUDED

#include "tMesh_Dynamic_Header.h"
#include "common/STL/vector.h"

class cBSPVertices;
class cPlane;
class iProgressCallBack;

bool
ConnectBoundaries(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        iProgressCallBack* progressCallBack
        );

#endif
