//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class iMesh;
class iAnchorsAndPinnedShapes;
class cPosition;

void
ResolveAnchors(
        const iMesh& mesh,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        tSigned32 numberOfTerrainLayers,
        std::vector<cPosition>& anchorPositions
        );
