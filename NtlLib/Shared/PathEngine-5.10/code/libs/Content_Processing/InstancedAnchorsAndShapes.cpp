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
#include "InstancedAnchorsAndShapes.h"
#include "QuantisedRotation.h"

cInstancedAnchorsAndShapes::cInstancedAnchorsAndShapes(
        const iAnchorsAndPinnedShapes* base,
        tSigned32 rotation,
        tSigned32 scale,
        tSigned32 translationX, tSigned32 translationY,
        float translationZ
        )
{
    _base = base;
    _rotationAsPathEngineAngle = QuantisedRotationAsPathEngineAngle(rotation);
    QuantisedRotationCoefficients(rotation, _sinOf, _cosOf);
    _scale = scale;
    _scaleF = static_cast<float>(scale);
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
}

// iAnchorsAndPinnedShapes interface

tSigned32
cInstancedAnchorsAndShapes::numberOfAnchors() const
{
    return _base->numberOfAnchors();
}
const char*
cInstancedAnchorsAndShapes::anchor_Name(tSigned32 anchorIndex) const
{
    return _base->anchor_Name(anchorIndex);
}
void
cInstancedAnchorsAndShapes::anchor_HorizontalPosition(tSigned32 anchorIndex, tSigned32& x, tSigned32& y) const
{
    tSigned32 baseX, baseY;
    _base->anchor_HorizontalPosition(anchorIndex, baseX, baseY);
    x = (baseX * _cosOf + baseY * _sinOf) / QuantisedRotationDivisor() * _scale + _translationX;
    y = (baseY * _cosOf - baseX * _sinOf) / QuantisedRotationDivisor() * _scale + _translationY;
}
void
cInstancedAnchorsAndShapes::anchor_VerticalRange(tSigned32 anchorIndex, float& zStart, float& zEnd) const
{
    _base->anchor_VerticalRange(anchorIndex, zStart, zEnd);
    zStart = zStart * _scaleF + _translationZ;
    zEnd = zEnd * _scaleF + _translationZ;
}
tSigned32
cInstancedAnchorsAndShapes::anchor_Orientation(tSigned32 anchorIndex) const
{
    return (_base->anchor_Orientation(anchorIndex) + _rotationAsPathEngineAngle) & 1048575;
}
tSigned32
cInstancedAnchorsAndShapes::anchor_Attribute(tSigned32 anchorIndex, tSigned32 attributeIndex) const
{
    return _base->anchor_Attribute(anchorIndex, attributeIndex);
}

tSigned32
cInstancedAnchorsAndShapes::numberOfPinnedShapes() const
{
    return _base->numberOfPinnedShapes();
}
const char*
cInstancedAnchorsAndShapes::pinnedShape_Name(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_Name(shapeIndex);
}
tSigned32
cInstancedAnchorsAndShapes::pinnedShape_AnchorIndex(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_AnchorIndex(shapeIndex);
}
tSigned32
cInstancedAnchorsAndShapes::pinnedShape_NumberOfVertices(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_NumberOfVertices(shapeIndex);
}
void
cInstancedAnchorsAndShapes::pinnedShape_Vertex(tSigned32 shapeIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const
{
    tSigned32 baseX, baseY;
    _base->pinnedShape_Vertex(shapeIndex, vertexIndex, baseX, baseY);
    x = (baseX * _cosOf + baseY * _sinOf) / QuantisedRotationDivisor() * _scale;
    y = (baseY * _cosOf - baseX * _sinOf) / QuantisedRotationDivisor() * _scale;
}
tSigned32
cInstancedAnchorsAndShapes::pinnedShape_Attribute(tSigned32 shapeIndex, tSigned32 attributeIndex) const
{
    return _base->pinnedShape_Attribute(shapeIndex, attributeIndex);
}
