//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class MDagPath;
class cAnchorsAndPinnedShapes;

void
BuildAnchorsAndShapesFromMaya(
    const std::vector<MDagPath>& toProcess,
    float scale,
    cAnchorsAndPinnedShapes& result
    );
