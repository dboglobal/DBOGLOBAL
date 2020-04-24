//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/AnchorsAndPinnedShapes.h"
#include "libs/Geometry/interface/GeometryPersistence.h"
#include "common/interface/Attributes.h"

// iAnchorsAndPinnedShapes

tSigned32
cAnchorsAndPinnedShapes::numberOfAnchors() const
{
    return SizeL(_anchors);
}
const char*
cAnchorsAndPinnedShapes::anchor_Name(tSigned32 anchorIndex) const
{
    const cAnchor& anchor = _anchors[anchorIndex];
    return anchor._name.c_str();    
}
void
cAnchorsAndPinnedShapes::anchor_HorizontalPosition(tSigned32 anchorIndex, tSigned32& x, tSigned32& y) const
{
    const cAnchor& anchor = _anchors[anchorIndex];
    x = anchor._x;
    y = anchor._y;
}
void
cAnchorsAndPinnedShapes::anchor_VerticalRange(tSigned32 anchorIndex, float& zStart, float& zEnd) const
{
    const cAnchor& anchor = _anchors[anchorIndex];
    zStart = anchor._zStart;
    zEnd = anchor._zEnd;
}
tSigned32
cAnchorsAndPinnedShapes::anchor_Orientation(tSigned32 anchorIndex) const
{
    const cAnchor& anchor = _anchors[anchorIndex];
    return anchor._orientation;
}
tSigned32
cAnchorsAndPinnedShapes::anchor_Attribute(tSigned32 anchorIndex, tSigned32 attributeIndex) const
{
    if(attributeIndex == PE_AnchorAttribute_ResolvesToTerrain)
    {
        const cAnchor& anchor = _anchors[anchorIndex];
        if(anchor._resolvesToTerrain)
        {
            return 1;
        }
        return 0;
    }
    return -1;
}
tSigned32
cAnchorsAndPinnedShapes::numberOfPinnedShapes() const
{
    return SizeL(_pinnedShapes);
}
const char*
cAnchorsAndPinnedShapes::pinnedShape_Name(tSigned32 shapeIndex) const
{
    const cPinnedShape& shape = _pinnedShapes[shapeIndex];
    return shape._name.c_str();    
}
tSigned32
cAnchorsAndPinnedShapes::pinnedShape_AnchorIndex(tSigned32 shapeIndex) const
{
    const cPinnedShape& shape = _pinnedShapes[shapeIndex];
    return shape._anchorIndex;
}
tSigned32
cAnchorsAndPinnedShapes::pinnedShape_NumberOfVertices(tSigned32 shapeIndex) const
{
    const cPinnedShape& shape = _pinnedShapes[shapeIndex];
    return SizeL(shape._coords) / 2;
}
void
cAnchorsAndPinnedShapes::pinnedShape_Vertex(tSigned32 shapeIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const
{
    const cPinnedShape& shape = _pinnedShapes[shapeIndex];
    x = shape._coords[vertexIndex * 2];
    y = shape._coords[vertexIndex * 2 + 1];
}
tSigned32
cAnchorsAndPinnedShapes::pinnedShape_Attribute(tSigned32 shapeIndex, tSigned32 attributeIndex) const
{
    if(attributeIndex == PE_PinnedShapeAttribute_Type)
    {
        const cPinnedShape& shape = _pinnedShapes[shapeIndex];
        return shape._type;
    }
    return -1;
}

// iXMLHandler

iXMLHandler*
cAnchorsAndPinnedShapes::element(const char* el, const char *const* attr)
{
    if(getDepth() == 0)
    {
        if(strcmp(el, "anchors") == 0 || strcmp(el, "pinnedShapes") == 0)
        {
            return this;
        }
        return 0;
    }
    if(strcmp(el, "anchor") == 0)
    {
        _anchors.push_back(cAnchor());
        if(GetValueForAttribute(attr, "name"))
        {
            _anchors.back()._name = GetValueForAttribute(attr, "name");
        }
        _anchors.back()._x = ValueAsLong(attr, "x");
        _anchors.back()._y = ValueAsLong(attr, "y");
        _anchors.back()._zStart = ValueAsPersistentFloat(attr, "zStart");
        _anchors.back()._zEnd = ValueAsPersistentFloat(attr, "zEnd");
        _anchors.back()._orientation = ValueAsLong(attr, "orientation");
        _anchors.back()._resolvesToTerrain = (ValueAsLongWithDefault(attr, "_resolvesToTerrain", 0) == 1);
    }
    else if(strcmp(el, "pinnedShape") == 0)
    {
        _pinnedShapes.push_back(cPinnedShape());
        if(GetValueForAttribute(attr, "name"))
        {
            _pinnedShapes.back()._name = GetValueForAttribute(attr, "name");
        }
        _pinnedShapes.back()._anchorIndex = ValueAsLong(attr, "anchorIndex");
        VerticesFromString(GetValueForAttribute(attr, "vertices"), _pinnedShapes.back()._coords);
        _pinnedShapes.back()._type = ValueAsLongWithDefault(attr, "_type", 0);
    }
    return 0;
}
