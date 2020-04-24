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
#include "interface/LoadedFaceVertexMesh.h"
#include "common/interface/Attributes.h"
#include "common/CLib/string.h"

tSigned32
cLoadedFaceVertexMesh::addVertex(tSigned32 x, tSigned32 y, float z)
{
    tSigned32 result = vertices();
    _vertexXYs.push_back(x);
    _vertexXYs.push_back(y);
    _vertexZs.push_back(z);
    return result;
}
tSigned32
cLoadedFaceVertexMesh::addFace(tSigned32 v0, tSigned32 v1, tSigned32 v2)
{
    assertD(v0 >= 0 && v0 < vertices());
    assertD(v1 >= 0 && v1 < vertices());
    assertD(v2 >= 0 && v2 < vertices());
    assertD(v0 != v1 && v0 != v2 && v1 != v2);
    tSigned32 result = faces();
    _faceVerts.push_back(v0);
    _faceVerts.push_back(v1);
    _faceVerts.push_back(v2);
    assertC(PE_FaceAttribute_Top == 6);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(-1);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    _faceAttributes.push_back(0);
    return result;
}
void
cLoadedFaceVertexMesh::setAttribute(tSigned32 face, tSigned32 attribute, tSigned32 value)
{
    assertD(face >= 0 && face < faces());
    assertD(attribute >= 0 && attribute < PE_FaceAttribute_Top);
    _faceAttributes[face * (PE_FaceAttribute_Top + 1) + attribute] = value;
}

iXMLHandler*
cLoadedFaceVertexMesh::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "vertex") == 0)
    {
        _vertexXYs.push_back(ValueAsLong(attr, "x"));
        _vertexXYs.push_back(ValueAsLong(attr, "y"));
        _vertexZs.push_back(ValueAsPersistentFloat(attr, "z"));
        return 0;
    }
    if(strcmp(el, "face") == 0)
    {
        _faceVerts.push_back(ValueAsLong(attr, "v1"));
        _faceVerts.push_back(ValueAsLong(attr, "v2"));
        _faceVerts.push_back(ValueAsLong(attr, "v3"));

        assertC(PE_FaceAttribute_Top == 6);
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_surfaceType", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_sectionID", -1));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksPortal", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksExternalShape", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksConnectingEdge", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_userData", 0));
        _faceAttributes.push_back(ValueAsLongWithDefault(attr, "a_marksTerrainPortal", 0));
        return 0;
    }
    return 0;
}

tSigned32
cLoadedFaceVertexMesh::faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
{
    if(attributeIndex <= PE_FaceAttribute_Top)
    {
        return _faceAttributes[face * (PE_FaceAttribute_Top + 1) + attributeIndex];
    }
    return -1;
}
