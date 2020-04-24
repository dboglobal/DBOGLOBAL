//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CombinedFaceVertexMeshes.h"

cCombinedFaceVertexMeshes::cCombinedFaceVertexMeshes(const std::vector<iFaceVertexMesh*>& baseMeshes) :
    _baseMeshes(baseMeshes),
    _startFace(baseMeshes.size()),
    _startVertex(baseMeshes.size())
{
    tSigned32 i;
    for(i = 0; i != SizeL(_baseMeshes); ++i)
    {
        _startFace[i] = SizeL(_baseMeshForFace);
        _startVertex[i] = SizeL(_baseMeshForVertex);
        if(!_baseMeshes[i])
        {
            continue;
        }
        _baseMeshForFace.resize(SizeL(_baseMeshForFace) + _baseMeshes[i]->faces(), i);
        _baseMeshForVertex.resize(SizeL(_baseMeshForVertex) + _baseMeshes[i]->vertices(), i);
    }
}
