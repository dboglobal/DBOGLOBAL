//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/STL/vector.h"

class Interface;
class iAnchorsAndPinnedShapes;
class iFaceVertexMesh;
class cMesh;

INode*
ImportMesh(
        Interface* ip,
        const iFaceVertexMesh& mesh,
        float scale
        );

void
ImportAnchorsAndPinnedShapes(
        Interface* ip,
        INode* parent,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        float anchorHeight,
        float scale
        );
