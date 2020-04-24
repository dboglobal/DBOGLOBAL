//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/MeshRenderInfo.h"
#include "cTestBed.h"
#include "interface/Singletons_TestBed.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tPoint3.h"

cMeshRenderInfo::~cMeshRenderInfo()
{
    clear();
}

void
cMeshRenderInfo::build(tMesh_3D& mesh3D)
{
    cTestBed& testBed = nSingletons::testBed();
    iGraphicsInterface* gip = testBed.getGIP();
    build(mesh3D, gip);
}

void
cMeshRenderInfo::build(const iFaceVertexMesh& meshFaceVertex)
{
    cTestBed& testBed = nSingletons::testBed();
    _gip = testBed.getGIP();

    clear();

    _geometrySet = _gip->newGeometrySet();

    tSigned32 f;
    for(f = 0; f != meshFaceVertex.faces(); ++f)
    {
        float pointArray[12];
        tSigned32 i;
        for(i = 0; i != 3; ++i)
        {
            tSigned32 v = meshFaceVertex.vertexIndex(f, i);
            pointArray[i * 3 + 0] = static_cast<float>(meshFaceVertex.vertexX(v));
            pointArray[i * 3 + 1] = static_cast<float>(meshFaceVertex.vertexY(v));
            pointArray[i * 3 + 2] = meshFaceVertex.vertexZ(v);
        }
        _faceElements.push_back(_gip->newPolygon(3, pointArray, _geometrySet));
        pointArray[i * 3 + 0] = pointArray[0];
        pointArray[i * 3 + 1] = pointArray[1];
        pointArray[i * 3 + 2] = pointArray[2];
        for(i = 0; i != 3; ++i)
        {
            _edgeElements.push_back(_gip->newLine(pointArray + i * 3, _geometrySet));
        }
    }
}

void cMeshRenderInfo::clear()
{
    _faceElements.clear();
    _edgeElements.clear();
    if(_gip)
    {
        _gip->deleteGeometrySet(_geometrySet);
    }
    _geometrySet = 0;
}

void
cMeshRenderInfo::drawFaces() const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_faceElements); i++)
    {
        if(_faceElements[i])
        {
            _gip->render(_faceElements[i]);
        }
    }
}
void cMeshRenderInfo::drawEdges() const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_edgeElements); i++)
    {
        _gip->render(_edgeElements[i]);
    }
}
void cMeshRenderInfo::drawFace(tSigned32 index) const
{
    if(_faceElements[index])
    {
        _gip->render(_faceElements[index]);
    }
}
void cMeshRenderInfo::drawEdge(tSigned32 index) const
{
    _gip->render(_edgeElements[index]);
}


