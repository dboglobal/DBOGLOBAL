//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class cMesh;
class cShape;
class cCollisionContext;
class cInternalPosition;

void
GenerateCurvedPath(
        cMesh* mesh, cShape* shape, cCollisionContext* context,
        const std::vector<cInternalPosition>& path, const std::vector<tSigned32>* connectionIndices, tSigned32 startOffset,
        tSigned32 startVectorX, tSigned32 startVectorY,
        tSigned32 endVectorX, tSigned32 endVectorY,
        tSigned32 sectionLength,
        float turnRatio1, float turnRatio2,
        std::vector<cInternalPosition>& pathResult, std::vector<tSigned32>*& connectionIndicesResult
        );
