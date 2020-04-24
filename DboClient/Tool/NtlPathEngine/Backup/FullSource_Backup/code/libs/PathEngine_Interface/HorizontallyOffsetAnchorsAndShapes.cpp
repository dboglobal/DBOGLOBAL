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
#include "HorizontallyOffsetAnchorsAndShapes.h"

cHorizontallyOffsetAnchorsAndShapes::cHorizontallyOffsetAnchorsAndShapes(const iAnchorsAndPinnedShapes* base, tSigned32 offsetX, tSigned32 offsetY) :
 _base(base),
 _offsetX(offsetX),
 _offsetY(offsetY)
{
}

tSigned32
cHorizontallyOffsetAnchorsAndShapes::numberOfAnchors() const
{
    return _base->numberOfAnchors();
}
const char*
cHorizontallyOffsetAnchorsAndShapes::anchor_Name(tSigned32 anchorIndex) const
{
    return _base->anchor_Name(anchorIndex);
}
void
cHorizontallyOffsetAnchorsAndShapes::anchor_HorizontalPosition(tSigned32 anchorIndex, tSigned32& x, tSigned32& y) const
{
    tSigned32 baseX, baseY;
    _base->anchor_HorizontalPosition(anchorIndex, baseX, baseY);
    x = baseX + _offsetX;
    y = baseY + _offsetY;
}
void
cHorizontallyOffsetAnchorsAndShapes::anchor_VerticalRange(tSigned32 anchorIndex, float& zStart, float& zEnd) const
{
    _base->anchor_VerticalRange(anchorIndex, zStart, zEnd);
}
tSigned32
cHorizontallyOffsetAnchorsAndShapes::anchor_Orientation(tSigned32 anchorIndex) const
{
    return _base->anchor_Orientation(anchorIndex);
}
tSigned32
cHorizontallyOffsetAnchorsAndShapes::anchor_Attribute(tSigned32 anchorIndex, tSigned32 attributeIndex) const
{
    return _base->anchor_Attribute(anchorIndex, attributeIndex);
}

tSigned32
cHorizontallyOffsetAnchorsAndShapes::numberOfPinnedShapes() const
{
    return _base->numberOfPinnedShapes();
}
const char*
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_Name(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_Name(shapeIndex);
}
tSigned32
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_AnchorIndex(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_AnchorIndex(shapeIndex);
}
tSigned32
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_NumberOfVertices(tSigned32 shapeIndex) const
{
    return _base->pinnedShape_NumberOfVertices(shapeIndex);
}
void
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_Vertex(tSigned32 shapeIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const
{
    _base->pinnedShape_Vertex(shapeIndex, vertexIndex, x, y);
}
tSigned32
cHorizontallyOffsetAnchorsAndShapes::pinnedShape_Attribute(tSigned32 shapeIndex, tSigned32 attributeIndex) const
{
    return _base->pinnedShape_Attribute(shapeIndex, attributeIndex);
}
