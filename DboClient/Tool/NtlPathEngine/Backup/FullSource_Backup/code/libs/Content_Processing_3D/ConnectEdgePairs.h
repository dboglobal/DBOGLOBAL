//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__CONNECT_EDGE_PAIRS_INCLUDED
#define CONTENT_PROCESSING_3D__CONNECT_EDGE_PAIRS_INCLUDED

#include "tMesh_Dynamic_Header.h"

class cBSPVertices;
class iProgressCallBack;

void
ConnectEdgePairs(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        iProgressCallBack* progressCallBack
        );

#endif
