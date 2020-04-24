//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_3D_WRAPPER_INCLUDED
#define MESH_3D_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cMesh3DWrapper : public iFaceVertexMesh
{
    tMesh_3D_FaceTracked _mesh;
    std::vector<tSigned32> _surfaceTypes;
    std::vector<tSigned32> _sectionIDs;
    std::vector<tSigned32> _userData;
    std::vector<tSigned32> _attributesMapping;

public:

    // the mesh passed in is copied and vertical discontinuities split
    // the resulting wrapper is then a stand-alone object
    cMesh3DWrapper(tMesh_3D& meshToWrap,
            const std::vector<tSigned32>* surfaceTypes,
            const std::vector<tSigned32>* sectionIDs,
            const std::vector<tSigned32>* userData
            );

// iFaceVertexMesh interface

    tSigned32 faces() const
    {
        return _mesh.faces();
    }
    tSigned32 vertices() const
    {
        return _mesh.vertices();
    }
    tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
    {
        assertD(vertexInFace >= 0 && vertexInFace < 3);
        tFace_3D_FaceTracked f = _mesh.faceAt(face);
        tEdge_3D_FaceTracked e = tMesh_3D_FaceTracked::beginEdge(f);
        while(vertexInFace--)
        {
            ++e;
        }
        return e.vertex().index();
    }
    tSigned32 vertexX(tSigned32 vertex) const
    {
        tVertex_3D_FaceTracked v = _mesh.vertAt(vertex);
        return GetVertexPoint(v).getX();
    }
    tSigned32 vertexY(tSigned32 vertex) const
    {
        tVertex_3D_FaceTracked v = _mesh.vertAt(vertex);
        return GetVertexPoint(v).getY();
    }
    float vertexZ(tSigned32 vertex) const
    {
        tVertex_3D_FaceTracked v = _mesh.vertAt(vertex);
        tEdge_3D_FaceTracked e = tMesh_3D_FaceTracked::firstEdge(v);
        assertD(e.valid());
        return e->getZ();
    }
    tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const;
};

#endif
