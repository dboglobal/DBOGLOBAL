//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"

class cInstancedAnchorsAndShapes : public iAnchorsAndPinnedShapes
{
    const iAnchorsAndPinnedShapes* _base;
    tSigned32 _rotationAsPathEngineAngle;
    tSigned32 _sinOf, _cosOf;
    tSigned32 _scale;
    float _scaleF;
    tSigned32 _translationX, _translationY;
    float _translationZ;

public:

    cInstancedAnchorsAndShapes(
        const iAnchorsAndPinnedShapes* base,
        tSigned32 rotation,
        tSigned32 scale,
        tSigned32 translationX, tSigned32 translationY,
        float translationZ
        );

// iAnchorsAndPinnedShapes interface

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
};
