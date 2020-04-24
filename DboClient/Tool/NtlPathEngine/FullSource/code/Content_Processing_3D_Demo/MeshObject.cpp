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
#include "MeshObject.h"
#include "LoadBaseMesh.h"
#include "libs/TestBed_Interface/interface/MeshRenderInfo.h"
#include "libs/Content_Processing/interface/InstancedFaceVertexMesh_Float.h"
#include "libs/Content_Processing/interface/InstancedFaceVertexMesh_Float.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/CoordinateRanges.h"

void
cMeshObject::invalidateCachedData()
{
    delete _cachedInstance;
    _cachedInstance = 0;
    delete _cachedRenderInfo;
    _cachedRenderInfo = 0;
}
void
cMeshObject::generateCachedDataIfNotValid()
{
    if(_cachedInstance)
    {
        return;
    }

    _cachedInstance = new cInstancedFaceVertexMesh_Float(
            _base,
            _rotation,
            _scale,
            _translationX, _translationY, static_cast<float>(_translationZ)
            );

    _cachedRenderInfo = new cMeshRenderInfo;
    _cachedRenderInfo->build(*_cachedInstance);
}

cMeshObject::cMeshObject(const char *const* attributes) :
 bSceneObject(attributes)
{
    _base = LoadBaseMesh(_type.c_str());
    _cachedInstance = 0;
    _cachedRenderInfo = 0;
}

cMeshObject::~cMeshObject()
{
    delete _cachedInstance;
    delete _cachedRenderInfo;
    delete _base;
}

void
cMeshObject::renderEdges()
{
    generateCachedDataIfNotValid();
    _cachedRenderInfo->drawEdges();
}
void
cMeshObject::renderFaces()
{
    generateCachedDataIfNotValid();
    _cachedRenderInfo->drawFaces();
}
iFaceVertexMesh*
cMeshObject::getCachedInstance()
{
    generateCachedDataIfNotValid();
    return _cachedInstance;
}
void
cMeshObject::addAllPoints(std::vector<tPoint3>& addTo)
{
    iFaceVertexMesh& mesh = *getCachedInstance();
    tSigned32 i;
    for(i = 0; i != mesh.vertices(); ++i)
    {
        tSigned32 x = mesh.vertexX(i);
        if(x < -WORLD_RANGE || x > WORLD_RANGE)
        {
            continue;
        }
        tSigned32 y = mesh.vertexY(i);
        if(y < -WORLD_RANGE || y > WORLD_RANGE)
        {
            continue;
        }
        tSigned32 z = static_cast<tSigned32>(mesh.vertexZ(i));
        if(z < -WORLD_RANGE || z > WORLD_RANGE)
        {
            continue;
        }
        addTo.push_back(tPoint3(x, y, z, WORLD_RANGE));
    }
}
