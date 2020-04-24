//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING__INSTANCED_FACE_VERTEX_MESH__FLOAT_INCLUDED
#define CONTENT_PROCESSING__INSTANCED_FACE_VERTEX_MESH__FLOAT_INCLUDED

#include "i_pathengine.h"

class cInstancedFaceVertexMesh_Float : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    float _cosOf, _sinOf;
    float _scale;
    tSigned32 _translationX, _translationY;
    float _translationZ;
    tSigned32 _sectionIDOverride;

public:

    cInstancedFaceVertexMesh_Float(
        const iFaceVertexMesh* base,
        float rotation,
        float scale,
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

#endif
