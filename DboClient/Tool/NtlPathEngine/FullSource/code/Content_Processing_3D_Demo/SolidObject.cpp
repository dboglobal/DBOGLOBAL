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
#include "SolidObject.h"
#include "SolidObjects.h"
#include "LoadOperands.h"
#include "libs/TestBed_Interface/interface/MeshRenderInfo.h"
#include "libs/Content_Processing_3D/interface/GenerateHullsForVisualisation.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "i_pathengine.h"
#include <math.h>

static void
TransformPoint(
        double cosOfRotation, double sinOfRotation,
        float scale,
        tSigned32 translationX, tSigned32 translationY, tSigned32 translationZ,
        const tSigned32* from,
        tSigned32* to
        )
{
    to[0] = static_cast<tSigned32>((from[0] * cosOfRotation + from[1] * sinOfRotation) * scale) + translationX;
    to[1] = static_cast<tSigned32>((from[1] * cosOfRotation - from[0] * sinOfRotation) * scale) + translationY;
    to[2] = static_cast<tSigned32>(from[2] * scale) + translationZ;
}

void
cSolidObject::transformOperand(tSigned32 i)
{
    tSigned32 numberOfPoints = SizeL(_operands[i]) / 3;
    tSigned32 j;
    for(j = 0; j != numberOfPoints; ++j)
    {
        TransformPoint(
                _cosOfRotation, _sinOfRotation,
                _scale,
                _translationX, _translationY, _translationZ, 
                &_operands[i][j * 3],
                &_cachedTransformedOperands._data[i][j * 3]
                );
    }
}

void
cSolidObject::invalidateCachedData()
{
    delete _cachedRenderInfo;
    _cachedRenderInfo = 0;
}
void
cSolidObject::generateCachedDataIfNotValid()
{
    if(_cachedRenderInfo)
    {
        return;
    }
    _cosOfRotation = cos(_rotation);
    _sinOfRotation = sin(_rotation);
    tSigned32 i;
    for(i = 0; i != _operands.size(); ++i)
    {
        transformOperand(i);
    }
    tMesh_3D hullsMesh3D;
    GenerateHullsForVisualisation(_cachedTransformedOperands, hullsMesh3D);
    _cachedRenderInfo = new cMeshRenderInfo;
    _cachedRenderInfo->build(hullsMesh3D);
}

cSolidObject::cSolidObject(const char *const* attributes) :
 bSceneObject(attributes)
{
    LoadOperands(_type.c_str(), _operands);
    _cachedTransformedOperands._data.resize(_operands.size());
    tSigned32 i;
    for(i = 0; i != SizeL(_operands); ++i)
    {
        assertR(SizeL(_operands[i]) >= 3);
        tSigned32 numberOfPoints = SizeL(_operands[i]) / 3;
        assertR(SizeL(_operands[i]) == numberOfPoints * 3);
        _cachedTransformedOperands._data[i].resize(_operands[i].size());
    }
    _cachedRenderInfo = 0;
}

cSolidObject::~cSolidObject()
{
    delete _cachedRenderInfo;
}

void
cSolidObject::renderEdges()
{
    generateCachedDataIfNotValid();
    _cachedRenderInfo->drawEdges();
}
void
cSolidObject::renderFaces()
{
    generateCachedDataIfNotValid();
    _cachedRenderInfo->drawFaces();
}

void
cSolidObject::addTransformedOperands(cSolidObjects& solidObjects)
{
    generateCachedDataIfNotValid();
    tSigned32 i;
    for(i = 0; i != SizeL(_operands); ++i)
    {
        solidObjects._data.push_back(_cachedTransformedOperands._data[i]);
    }
}

void
cSolidObject::addAllPoints(std::vector<tPoint3>& addTo)
{
    generateCachedDataIfNotValid();
    tSigned32 i;
    for(i = 0; i != SizeL(_operands); ++i)
    {
        tSigned32 j;
        for(j = 0; j != SizeL(_operands[i]) / 3; ++j)
        {
            tSigned32 *ptr = &_cachedTransformedOperands._data[i][j * 3];
            tSigned32 x = *ptr++;
            tSigned32 y = *ptr++;
            tSigned32 z = *ptr++;
            if(x < -WORLD_RANGE || x > WORLD_RANGE)
            {
                continue;
            }
            if(y < -WORLD_RANGE || y > WORLD_RANGE)
            {
                continue;
            }
            if(z < -WORLD_RANGE || z > WORLD_RANGE)
            {
                continue;
            }
            addTo.push_back(tPoint3(x, y, z, WORLD_RANGE));            
        }
    }
}
