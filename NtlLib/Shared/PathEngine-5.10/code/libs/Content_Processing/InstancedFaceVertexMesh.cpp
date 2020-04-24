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
#include "InstancedFaceVertexMesh.h"
#include "QuantisedRotation.h"

cInstancedFaceVertexMesh::cInstancedFaceVertexMesh(
        const iFaceVertexMesh* base,
        tSigned32 rotation,
        tSigned32 scale,
        tSigned32 translationX, tSigned32 translationY,
        float translationZ,
        tSigned32 sectionID
        )
{
    _base = base;
    QuantisedRotationCoefficients(rotation, _sinOf, _cosOf);
    _scale = scale;
    _scaleF = static_cast<float>(scale);
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
    _sectionIDOverride = sectionID;
}

// iFaceVertexMesh interface

tSigned32
cInstancedFaceVertexMesh::faces() const
{
    return _base->faces();
}
tSigned32
cInstancedFaceVertexMesh::vertices() const
{
    return _base->vertices();
}
tSigned32
cInstancedFaceVertexMesh::vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
{
    return _base->vertexIndex(face, vertexInFace);
}
tSigned32
cInstancedFaceVertexMesh::vertexX(tSigned32 vertex) const
{
    return (_base->vertexX(vertex) * _cosOf + _base->vertexY(vertex) * _sinOf) / QuantisedRotationDivisor() * _scale + _translationX;
}
tSigned32
cInstancedFaceVertexMesh::vertexY(tSigned32 vertex) const
{
    return (_base->vertexY(vertex) * _cosOf - _base->vertexX(vertex) * _sinOf) / QuantisedRotationDivisor() * _scale + _translationY;
}
float
cInstancedFaceVertexMesh::vertexZ(tSigned32 vertex) const
{
    return _base->vertexZ(vertex) * _scaleF + _translationZ;
}
tSigned32
cInstancedFaceVertexMesh::faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
{
    if(attributeIndex == PE_FaceAttribute_SectionID && _sectionIDOverride != -1)
    {
        return _sectionIDOverride;
    }
    return _base->faceAttribute(face, attributeIndex);
}
