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
#include "InstancedAnchorsAndShapes_Float.h"
#include <math.h>

cInstancedAnchorsAndShapes_Float::cInstancedAnchorsAndShapes_Float(
        const iAnchorsAndPinnedShapes* base,
        float rotation,
        float scale,
        tSigned32 translationX, tSigned32 translationY,
        float translationZ
        )
{
    _base = base;
    _rotationAsPathEngineAngle = static_cast<tSigned32>(rotation * 1048576.f / 6.283185f);
    _cosOf = cosf(rotation);
    _sinOf = sinf(rotation);
    _scale = scale;
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
}

// iAnchorsAndPinnedShapes interface

tSigned32
cInstancedAnchorsAndShapes_Float::numberOfAnchors() const
{
    return _base->numberOfAnchors();
}
const char*
cInstancedAnchorsAndShapes_Float::anchor_Name(tSigned32 anchorIndex) const
{
    return _base->anchor_Name(anchorIndex);
}
void
cInstancedAnchorsAndShapes_Float::anchor_HorizontalPosition(tSigned32 anchorIndex, tSigned32& x, tSigned32& y) const
{
    tSigned32 baseX, baseY;
    _base->anchor_HorizontalPosition(anchorIndex, baseX, baseY);
    x = static_cast<tSigned32>((baseX * _cosOf + baseY * _sinOf) * _scale) + _translationX;
    y = static_cast<tSigned32>((baseY * _cosOf - baseX * _sinOf) * _scale) + _translationY;
}
void
cInstancedAnchorsAndShapes_Float::anchor_VerticalRange(tSigned32 anchorIndex, float& zStart, float& zEnd) const
{
    _base->anchor_VerticalRange(anchorIndex, zStart, zEnd);
    zStart = zStart * _scale + _translationZ;
    zEnd = zEnd * _scale + _translationZ;
}
tSigned32
cInstancedAnchorsAndShapes_Float::anchor_Orientation(tSigned32 anchorIndex) const
{
    return (_base->anchor_Orientation(anchorIndex) + _rotationAsPathEngineAngle) & 1048575;
}
tSigned32
cInstancedAnchorsAndShapes_Float::anchor_Attribute(tSigned32 anchorIndex, tSigned32 attributeIndex) const
{
    return _base->anchor_Attribute(anchorIndex, attributeIndex);
}

tSigned32
cInstancedAnchorsAndShapes_Float::numberOfPinnedShapes() const
{
    return _base->numberOfPinnedShapes();
}
const char*
cInstancedAnchorsAndShapes_Float::pinnedShape_Name(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_Name(shapeIndex);
}
tSigned32
cInstancedAnchorsAndShapes_Float::pinnedShape_AnchorIndex(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_AnchorIndex(shapeIndex);
}
tSigned32
cInstancedAnchorsAndShapes_Float::pinnedShape_NumberOfVertices(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_NumberOfVertices(shapeIndex);
}
void
cInstancedAnchorsAndShapes_Float::pinnedShape_Vertex(tSigned32 shapeIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const
{
    tSigned32 baseX, baseY;
    _base->pinnedShape_Vertex(shapeIndex, vertexIndex, baseX, baseY);
    x = static_cast<tSigned32>((baseX * _cosOf + baseY * _sinOf) * _scale);
    y = static_cast<tSigned32>((baseY * _cosOf - baseX * _sinOf) * _scale);
}
tSigned32
cInstancedAnchorsAndShapes_Float::pinnedShape_Attribute(tSigned32 shapeIndex, tSigned32 attributeIndex) const
{
    return _base->pinnedShape_Attribute(shapeIndex, attributeIndex);
}
