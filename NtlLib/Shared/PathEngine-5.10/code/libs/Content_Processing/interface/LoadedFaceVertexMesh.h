//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOADED_FACE_VERTEX_MESH_INCLUDED
#define LOADED_FACE_VERTEX_MESH_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "i_pathengine.h"
#include "common/AttributesBuilder.h"
#include "common/STL/vector.h"

class cLoadedFaceVertexMesh : public iXMLHandler, public iFaceVertexMesh
{
    std::vector<tSigned32> _vertexXYs;
    std::vector<float> _vertexZs;
    std::vector<tSigned32> _faceVerts;
    std::vector<tSigned32> _faceAttributes;

public:

// direct construction

    tSigned32 addVertex(tSigned32 x, tSigned32 y, float z);
    tSigned32 addFace(tSigned32 v0, tSigned32 v1, tSigned32 v2);
    void setAttribute(tSigned32 face, tSigned32 atttribute, tSigned32 value);

// iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attr);

// iFaceVertexMesh interface

    tSigned32 faces() const
    {
        return SizeL(_faceVerts) / 3;
    }
    tSigned32 vertices() const
    {
        return SizeL(_vertexZs);
    }
    tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
    {
        return _faceVerts[face * 3 + vertexInFace];
    }
    tSigned32 vertexX(tSigned32 vertex) const
    {
        return _vertexXYs[vertex * 2 + 0];
    }
    tSigned32 vertexY(tSigned32 vertex) const
    {
        return _vertexXYs[vertex * 2 + 1];
    }
    float vertexZ(tSigned32 vertex) const
    {
        return _vertexZs[vertex];
    }
    tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const;
};

#endif
