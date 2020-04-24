//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "BridgingModifier.h"

cBridgingModifier::cBridgingModifier(const iFaceVertexMesh* base, const std::vector<tSigned32>& connectingEdgeInfo) :
 _base(base),
 _baseVerts(connectingEdgeInfo.size()),
 _baseFaces(connectingEdgeInfo.size() / 2),
 _extendedVertHorizontalCoords(connectingEdgeInfo.size() * 2),
 _extendedVertZs(connectingEdgeInfo.size())
{
    tSigned32 i;
    for(i = 0; i != SizeL(connectingEdgeInfo) / 2; ++i)
    {       
        tSigned32 face = connectingEdgeInfo[i * 2];
        tSigned32 vInFace = connectingEdgeInfo[i * 2 + 1];
        tSigned32 nextVInFace = (vInFace + 1) % 3;
        _baseVerts[i * 2] = _base->vertexIndex(face, vInFace);
        _baseVerts[i * 2 + 1] = _base->vertexIndex(face, nextVInFace);
        _baseFaces[i] = face;
        _extendedVertHorizontalCoords[i * 4] = _base->vertexX(_baseVerts[i * 2]);
        _extendedVertHorizontalCoords[i * 4 + 1] = _base->vertexY(_baseVerts[i * 2]);
        _extendedVertZs[i * 2] = _base->vertexZ(_baseVerts[i * 2]);
        _extendedVertHorizontalCoords[i * 4 + 2] = _base->vertexX(_baseVerts[i * 2 + 1]);
        _extendedVertHorizontalCoords[i * 4 + 3] = _base->vertexY(_baseVerts[i * 2 + 1]);
        _extendedVertZs[i * 2 + 1] = _base->vertexZ(_baseVerts[i * 2 + 1]);
    }
}

void
cBridgingModifier::setConnectionTarget(tSigned32 i, tSigned32 startX, tSigned32 startY, float startZ, tSigned32 endX, tSigned32 endY, float endZ)
{
    _extendedVertHorizontalCoords[i * 4] = endX;
    _extendedVertHorizontalCoords[i * 4 + 1] = endY;
    _extendedVertZs[i * 2] = endZ;
    _extendedVertHorizontalCoords[i * 4 + 2] = startX;
    _extendedVertHorizontalCoords[i * 4 + 3] = startY;
    _extendedVertZs[i * 2 + 1] = startZ;
}
