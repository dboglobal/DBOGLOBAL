//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_DEMO__MESH_OBJECT_INCLUDED
#define CONTENT_PROCESSING_3D_DEMO__MESH_OBJECT_INCLUDED

#include "SceneObject.h"
#include "libs/Geometry/interface/tPoint3_Header.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;
class cMeshRenderInfo;

class cMeshObject : public bSceneObject
{
    iFaceVertexMesh* _base;
    
    iFaceVertexMesh* _cachedInstance;
    cMeshRenderInfo* _cachedRenderInfo;

    void invalidateCachedData();
    void generateCachedDataIfNotValid();

public:

    cMeshObject(const char *const* attributes);
    ~cMeshObject();

    void renderEdges();
    void renderFaces();
    iFaceVertexMesh* getCachedInstance();
    void addAllPoints(std::vector<tPoint3>& addTo);
};

#endif
