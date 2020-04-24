//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"
#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"
#include "common/STL/string.h"

class cAnchor
{
public:
    std::string _name;
    tSigned32 _orientation;
    tSigned32 _x, _y;
    float _zStart, _zEnd;
    bool _resolvesToTerrain;
};

class cPinnedShape
{
public:
    std::string _name;
    tSigned32 _anchorIndex;
    std::vector<tSigned32> _coords;
    tSigned32 _type;
};

class cAnchorsAndPinnedShapes : public iAnchorsAndPinnedShapes, public iXMLHandler
{
public:

    std::vector<cAnchor> _anchors;
    std::vector<cPinnedShape> _pinnedShapes;

// iAnchorsAndPinnedShapes

    tSigned32 numberOfAnchors() const;
    const char* anchor_Name(tSigned32 anchorIndex) const;
    void anchor_HorizontalPosition(tSigned32 anchorIndex, tSigned32& x, tSigned32& y) const;
    void anchor_VerticalRange(tSigned32 anchorIndex, float& zStart, float& zEnd) const;
    tSigned32 anchor_Orientation(tSigned32 anchorIndex) const;
    tSigned32 anchor_Attribute(tSigned32 anchorIndex, tSigned32 attributeIndex) const;

    tSigned32 numberOfPinnedShapes() const;
    const char* pinnedShape_Name(tSigned32 shapeIndex) const;
    tSigned32 pinnedShape_AnchorIndex(tSigned32 shapeIndex) const;
    tSigned32 pinnedShape_NumberOfVertices(tSigned32 shapeIndex) const;
    void pinnedShape_Vertex(tSigned32 shapeIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const;
    tSigned32 pinnedShape_Attribute(tSigned32 shapeIndex, tSigned32 attributeIndex) const;

// iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attr);

};
