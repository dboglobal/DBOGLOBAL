//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef HORIZONTALLY_OFFSET_FVM__INCLUDED
#define HORIZONTALLY_OFFSET_FVM__INCLUDED

#include "i_pathengine.h"

class cHorizontallyOffsetFVM : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    tSigned32 _offsetX, _offsetY;

public:

    cHorizontallyOffsetFVM(const iFaceVertexMesh* base, tSigned32 offsetX, tSigned32 offsetY);

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
