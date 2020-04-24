//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPORTER_COMMON__COMBINED_FACE_VERTEX_MESHES_INCLUDED
#define EXPORTER_COMMON__COMBINED_FACE_VERTEX_MESHES_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class cCombinedFaceVertexMeshes : public iFaceVertexMesh
{
    const std::vector<iFaceVertexMesh*>& _baseMeshes;
    std::vector<tSigned32> _baseMeshForFace;
    std::vector<tSigned32> _baseMeshForVertex;
    std::vector<tSigned32> _startFace;
    std::vector<tSigned32> _startVertex;

public:

    // base meshes and vector passed in here must remain valid (and unmodified) for the lifetime of this object
    cCombinedFaceVertexMeshes(const std::vector<iFaceVertexMesh*>& baseMeshes);

    void getOriginalMeshAndIndexForFace(tSigned32 face, tSigned32& originalMesh, tSigned32& originalIndex) const
    {
        originalMesh = _baseMeshForFace[face];
        originalIndex = face - _startFace[originalMesh];
    }
    void getOriginalMeshAndIndexForVertex(tSigned32 vertex, tSigned32& originalMesh, tSigned32& originalIndex) const
    {
        originalMesh = _baseMeshForVertex[vertex];
        originalIndex = vertex - _startVertex[originalMesh];
    }

// iFaceVertexMesh interface

    tSigned32 faces() const
    {
        return SizeL(_baseMeshForFace);
    }
    tSigned32 vertices() const
    {
        return SizeL(_baseMeshForVertex);
    }
    tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
    {
        tSigned32 baseMesh = _baseMeshForFace[face];
        tSigned32 baseFace = face - _startFace[baseMesh];
        return _baseMeshes[baseMesh]->vertexIndex(baseFace, vertexInFace) + _startVertex[baseMesh];
    }
    tSigned32 vertexX(tSigned32 vertex) const
    {
        tSigned32 baseMesh = _baseMeshForVertex[vertex];
        tSigned32 baseVertex = vertex - _startVertex[baseMesh];
        return _baseMeshes[baseMesh]->vertexX(baseVertex);
    }
    tSigned32 vertexY(tSigned32 vertex) const
    {
        tSigned32 baseMesh = _baseMeshForVertex[vertex];
        tSigned32 baseVertex = vertex - _startVertex[baseMesh];
        return _baseMeshes[baseMesh]->vertexY(baseVertex);
    }
    float vertexZ(tSigned32 vertex) const
    {
        tSigned32 baseMesh = _baseMeshForVertex[vertex];
        tSigned32 baseVertex = vertex - _startVertex[baseMesh];
        return _baseMeshes[baseMesh]->vertexZ(baseVertex);
    }
    tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
    {
        tSigned32 baseMesh = _baseMeshForFace[face];
        tSigned32 baseFace = face - _startFace[baseMesh];
        return _baseMeshes[baseMesh]->faceAttribute(baseFace, attributeIndex);
    }
};

#endif
