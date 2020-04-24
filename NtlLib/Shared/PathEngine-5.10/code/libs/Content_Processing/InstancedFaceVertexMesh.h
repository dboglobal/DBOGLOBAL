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

class cInstancedFaceVertexMesh : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    tSigned32 _sinOf, _cosOf;
    tSigned32 _scale;
    float _scaleF;
    tSigned32 _translationX, _translationY;
    float _translationZ;
    tSigned32 _sectionIDOverride;

public:

    cInstancedFaceVertexMesh(
        const iFaceVertexMesh* base,
        tSigned32 rotation,
        tSigned32 scale,
        tSigned32 translationX, tSigned32 translationY,
        float translationZ,
        tSigned32 sectionID = -1
        );

// iFaceVertexMesh interface

    tSigned32 faces() const;
    tSigned32 vertices() const;
    tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const;
    tSigned32 vertexX(tSigned32 vertex) const;
    tSigned32 vertexY(tSigned32 vertex) const;
    float vertexZ(tSigned32 vertex) const;
    tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const;
};
