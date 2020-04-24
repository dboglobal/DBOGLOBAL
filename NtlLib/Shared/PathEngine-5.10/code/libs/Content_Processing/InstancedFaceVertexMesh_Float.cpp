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
#include "interface/InstancedFaceVertexMesh_Float.h"
#include <math.h>

cInstancedFaceVertexMesh_Float::cInstancedFaceVertexMesh_Float(
        const iFaceVertexMesh* base,
        float rotation,
        float scale,
        tSigned32 translationX, tSigned32 translationY,
        float translationZ,
        tSigned32 sectionID
        )
{
    _base = base;
    _cosOf = cosf(rotation);
    _sinOf = sinf(rotation);
    _scale = scale;
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
    _sectionIDOverride = sectionID;
}

// iFaceVertexMesh interface

tSigned32
cInstancedFaceVertexMesh_Float::faces() const
{
    return _base->faces();
}
tSigned32
cInstancedFaceVertexMesh_Float::vertices() const
{
    return _base->vertices();
}
tSigned32
cInstancedFaceVertexMesh_Float::vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
{
    return _base->vertexIndex(face, vertexInFace);
}
tSigned32
cInstancedFaceVertexMesh_Float::vertexX(tSigned32 vertex) const
{
    return static_cast<tSigned32>((_base->vertexX(vertex) * _cosOf + _base->vertexY(vertex) * _sinOf) * _scale) + _translationX;
}
tSigned32
cInstancedFaceVertexMesh_Float::vertexY(tSigned32 vertex) const
{
    return static_cast<tSigned32>((_base->vertexY(vertex) * _cosOf - _base->vertexX(vertex) * _sinOf) * _scale) + _translationY;
}
float
cInstancedFaceVertexMesh_Float::vertexZ(tSigned32 vertex) const
{
    return _base->vertexZ(vertex) * _scale + _translationZ;
}
tSigned32
cInstancedFaceVertexMesh_Float::faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
{
    if(attributeIndex == PE_FaceAttribute_SectionID && _sectionIDOverride != -1)
    {
        return _sectionIDOverride;
    }
    return _base->faceAttribute(face, attributeIndex);
}
