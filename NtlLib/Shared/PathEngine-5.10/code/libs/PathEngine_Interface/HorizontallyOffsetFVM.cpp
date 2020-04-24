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
#include "HorizontallyOffsetFVM.h"

cHorizontallyOffsetFVM::cHorizontallyOffsetFVM(const iFaceVertexMesh* base, tSigned32 offsetX, tSigned32 offsetY) :
 _base(base),
 _offsetX(offsetX),
 _offsetY(offsetY)
{
}

tSigned32
cHorizontallyOffsetFVM::faces() const
{
    return _base->faces();
}
tSigned32
cHorizontallyOffsetFVM::vertices() const
{
    return _base->vertices();
}
tSigned32
cHorizontallyOffsetFVM::vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
{
    return _base->vertexIndex(face, vertexInFace);
}
tSigned32
cHorizontallyOffsetFVM::vertexX(tSigned32 vertex) const
{
    return _base->vertexX(vertex) + _offsetX;
}
tSigned32
cHorizontallyOffsetFVM::vertexY(tSigned32 vertex) const
{
    return _base->vertexY(vertex) + _offsetY;
}
float
cHorizontallyOffsetFVM::vertexZ(tSigned32 vertex) const
{
    return _base->vertexZ(vertex);
}
tSigned32
cHorizontallyOffsetFVM::faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
{
    return _base->faceAttribute(face, attributeIndex);
}
