//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MAYA_MESH_WRAPPER_INCLUDED
#define MAYA_MESH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class MFnMesh;
class MDagPath;
class MColorArray;
class MFnDependencyNode;
class MObjectArray;
class MIntArray;

class cMayaMeshWrapper : public iFaceVertexMesh
{
    MFnMesh* _mesh;
    std::vector<tSigned32> _transformedVertexXY;
    std::vector<float> _transformedVertexZ;
    std::vector<tSigned32> _surfaceTypes;
    std::vector<tSigned32> _sectionIDs;
    std::vector<tSigned32> _markerTypes;

    void extractAttributesFromVertexColours(MColorArray&);
    void extractAttributesFromMaterial(const MFnDependencyNode& material, tSigned32 faceIndex);
    void extractAttributesFromShaders(const MObjectArray& shaders, const MIntArray& indices);

public:

    static bool checkCanConstructFrom(const MDagPath& dagPath);

    cMayaMeshWrapper(const MDagPath& dagPath, float scale);
    ~cMayaMeshWrapper();

// iFaceVertexMesh interface

    tSigned32 faces() const;
    tSigned32 vertices() const;

    tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const;
    tSigned32 vertexX(tSigned32 vertex) const;
    tSigned32 vertexY(tSigned32 vertex) const;
    float vertexZ(tSigned32 vertex) const;
    tSigned32 surfaceType(tSigned32 face) const;
    tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const;
};

#endif
