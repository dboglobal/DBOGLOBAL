//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__SIDES_FOR_SPLIT_INCLUDED
#define CONTENT_PROCESSING_3D__SIDES_FOR_SPLIT_INCLUDED

#include "common/STL/vector.h"

class cBSPVertices;
class cBSP3DOperand;
class cPlane;
class cBitVector;

void
SidesForSplit(
        const cBSPVertices& vertices,
        const std::vector<cBSP3DOperand*>& operands,
        const cPlane& splitPlane,
        cBitVector& resultBuffer
        );

#endif
