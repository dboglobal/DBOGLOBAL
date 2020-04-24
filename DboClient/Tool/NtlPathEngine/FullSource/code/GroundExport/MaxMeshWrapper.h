//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MAX_MESH_WRAPPER_INCLUDED
#define MAX_MESH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"
class Mesh;
class Matrix3;

class cMaxMeshWrapper : public iFaceVertexMesh
{
    const Mesh& _mesh;
    std::vector<tSigned32> _transformedVertexXY;
    std::vector<float> _transformedVertexZ;
    std::vector<tSigned32> _surfaceTypes;
    std::vector<tSigned32> _sectionIDs;
    std::vector<tSigned32> _markerTypes;
    bool _negateY;

public:

    cMaxMeshWrapper(const Mesh& mesh, const Matrix3& matrix,
     bool useMaterialIDs, bool setTerrainLayer, bool externalShapeAndPortalMarkers,
     float scale, bool negateY);

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
