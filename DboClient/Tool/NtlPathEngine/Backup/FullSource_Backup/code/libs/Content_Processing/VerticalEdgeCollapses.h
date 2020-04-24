//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VERTICAL_EDGE_COLLAPSES_INCLUDED
#define VERTICAL_EDGE_COLLAPSES_INCLUDED

#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;

class cVerticalEdgeCollapses
{
    std::vector<tSigned32> _vertexCollapseTable;
    cBitVector _faceWasCollapsed;
    std::vector<float> _zRanges;

    bool collapseEdgeIfVertical(const iFaceVertexMesh& mesh, tSigned32 v0, tSigned32 v1);

public:

    cVerticalEdgeCollapses(const iFaceVertexMesh& mesh, cBitVector& faceSkipFlags);

    tSigned32 getCollapsedIndex(tSigned32 vertex) const
    {
        return _vertexCollapseTable[vertex];
    }
    bool faceWasCollapsed(tSigned32 faceIndex) const
    {
        return _faceWasCollapsed[faceIndex];
    }
    void vertexZRange(tSigned32 vertex, float& minZ, float& maxZ) const
    {
        minZ = _zRanges[vertex * 2];
        maxZ = _zRanges[vertex * 2 + 1];
    }
};

#endif
