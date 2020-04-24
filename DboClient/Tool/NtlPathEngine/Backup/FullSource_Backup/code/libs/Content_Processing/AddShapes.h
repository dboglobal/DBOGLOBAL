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

class iAnchorsAndPinnedShapes;
class iMesh;
class cPosition;
class iPathEngine;

bool
PrevalidatePinnedShapes(
        iPathEngine& pathEngine,
        const iAnchorsAndPinnedShapes& anchorsAndShapes
        );

void
AddPinnedShapes(
        iPathEngine& pathEngine,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        const std::vector<cPosition>& anchorPositions,
        iMesh* mesh
        );

