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
#include "libs/PathEngine_Interface/interface/cContentChunkInstance.h"
#include "BridgingModifier.h"
#include "interface/Export.h"
#include "interface/LoadedFaceVertexMesh.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/PathEngine_Interface/interface/cContentChunk.h"
#include "libs/Geometry/interface/tLine.h"
#include "iAPIObjectWrapper.h"
#include "platform_common/CoordinateRanges.h"

#include "common/interface/Assert.h"

static bool
OutsideWorldRange(tSigned32 x, tSigned32 y)
{
    return x < -WORLD_RANGE || x > WORLD_RANGE || y < -WORLD_RANGE || y > WORLD_RANGE;
}

static bool
StartConnects(tSigned32 startX, tSigned32 startY, tSigned32 endX, tSigned32 endY, tSigned32 targetX, tSigned32 targetY)
{
    tPoint start(startX, startY, WORLD_RANGE);
    tPoint target(targetX, targetY, WORLD_RANGE);
    if(start == target)
    {
        return true;
    }
    tPoint end(endX, endY, WORLD_RANGE);
    tLine l(end, start);
    return l.side(target) == SIDE_RIGHT;
}

static void
UpdateRange(tSigned32 x, tSigned32 y, cHorizontalRange& range, bool& rangeValid)
{
    if(!rangeValid)
    {
        range.minX = x;
        range.minY = y;
        range.maxX = x;
        range.maxY = y;
        rangeValid = true;
        return;
    }
    if(x < range.minX)
    {
        range.minX = x;
    }
    else
    if(x > range.maxX)
    {
        range.maxX = x;
    }
    if(y < range.minY)
    {
        range.minY = y;
    }
    else
    if(y > range.maxY)
    {
        range.maxY = y;
    }
}

cContentChunkInstance::cContentChunkInstance(
            cContentChunk* base, tSigned32 indexInBase,
            const iFaceVertexMesh* faceVertex,
            const std::vector<tSigned32>& connectingEdgeInfo,
            const iAnchorsAndPinnedShapes* anchorsAndShapes,
            tSigned32 sectionID
            ) :
 _base(base),
 _indexInBase(indexInBase),
 _faceVertex(faceVertex),
 _bridgingModifier(0),
 _beforeBridgingModifier(0),
 _anchorsAndShapes(anchorsAndShapes),
 _sectionID(sectionID)
{
    _wrapper = 0;
    bool rangeValid = false;
    bool footprintRangeValid = false;
    tSigned32 i;
    if(_faceVertex)
    {
        for(i = 0; i != _faceVertex->vertices(); ++i)
        {
            UpdateRange(_faceVertex->vertexX(i), _faceVertex->vertexY(i), _horizontalRange, rangeValid);
        }
        for(i = 0; i != _faceVertex->faces(); ++i)
        {
            if(_faceVertex->faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) != 1)
            {
                continue;
            }
            tSigned32 j;
            for(j = 0; j != 3; ++j)
            {
                tSigned32 v = _faceVertex->vertexIndex(i, j);
                UpdateRange(_faceVertex->vertexX(v), _faceVertex->vertexY(v), _footprintRange, footprintRangeValid);
            }
        }
    }
    if(_anchorsAndShapes)
    {
        for(i = 0; i != _anchorsAndShapes->numberOfAnchors(); ++i)
        {
            tSigned32 x, y;
            _anchorsAndShapes->anchor_HorizontalPosition(i, x, y);
            UpdateRange(x, y, _horizontalRange, rangeValid);
            if(_anchorsAndShapes->anchor_Attribute(i, PE_AnchorAttribute_ResolvesToTerrain) == 1)
            {
                UpdateRange(x, y, _footprintRange, footprintRangeValid);
            }
        }
    }
    assertD(rangeValid);
    _hasGroundFootprint = footprintRangeValid;
    if(!connectingEdgeInfo.empty())
    {
        _beforeBridgingModifier = _faceVertex;
        _bridgingModifier = new cBridgingModifier(_beforeBridgingModifier, connectingEdgeInfo);
        _faceVertex = _bridgingModifier;
    }
}

cContentChunkInstance::~cContentChunkInstance()
{
    delete _faceVertex;
    delete _beforeBridgingModifier;
    delete _anchorsAndShapes;
    _base->releaseInstance(_indexInBase);
}

void
cContentChunkInstance::getGroundFootprintRange(cHorizontalRange& result) const
{
    result = _footprintRange;
}

void
cContentChunkInstance::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    delete this;
}

iContentChunk*
cContentChunkInstance::getContentChunk() const
{
    return _base;
}
const iFaceVertexMesh*
cContentChunkInstance::getGroundPointer() const
{
    return _faceVertex;
}
const iAnchorsAndPinnedShapes*
cContentChunkInstance::getAnchorsAndShapesPointer() const
{
    return _anchorsAndShapes;
}
void
cContentChunkInstance::addAnchorsAndShapes(iMesh* mesh, const char* idPrefix, tSigned32 numberOfTerrainLayers) const
{
    if(_anchorsAndShapes)
    {
        AddAnchorsAndPinnedShapes(nSingletons::pathEngine(), mesh, *_anchorsAndShapes, idPrefix, numberOfTerrainLayers);
    }
}
void
cContentChunkInstance::getHorizontalRange(cHorizontalRange& result) const
{
    result = _horizontalRange;
}
tSigned32
cContentChunkInstance::numberOfConnectingEdges() const
{
    return _base->numberOfConnectingEdges();
}

void
cContentChunkInstance::getConnectingEdgeGeometry(tSigned32 connectingEdgeIndex, tSigned32& startX, tSigned32& startY, float& startZ, tSigned32& endX, tSigned32& endY, float& endZ) const
{
    INTERFACE_ASSERT(connectingEdgeIndex >= 0 && connectingEdgeIndex < numberOfConnectingEdges());
    tSigned32 face, vInFace;
    _base->getConnectingEdgeInfo(connectingEdgeIndex, face, vInFace);
    tSigned32 nextVInFace = (vInFace + 1) % 3;
    assertD(_faceVertex);
    tSigned32 v = _faceVertex->vertexIndex(face, vInFace);
    tSigned32 nextV = _faceVertex->vertexIndex(face, nextVInFace);
    startX = _faceVertex->vertexX(v);
    startY = _faceVertex->vertexY(v);
    startZ = _faceVertex->vertexZ(v);
    endX = _faceVertex->vertexX(nextV);
    endY = _faceVertex->vertexY(nextV);
    endZ = _faceVertex->vertexZ(nextV);
}
bool
cContentChunkInstance::edgesCanConnect(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const
{
//.. move this logic into cBridgingModifier
    INTERFACE_ASSERT(targetInstance);
    tSigned32 startX, startY, endX, endY;
    {
        float startZ_Ignored, endZ_Ignored;
        getConnectingEdgeGeometry(connectingEdgeOnThis, startX, startY, startZ_Ignored, endX, endY, endZ_Ignored);
    }
    tSigned32 targetStartX, targetStartY, targetEndX, targetEndY;
    {
        float targetStartZ_Ignored, targetEndZ_Ignored;
        targetInstance->getConnectingEdgeGeometry(connectingEdgeOnTarget, targetStartX, targetStartY, targetStartZ_Ignored, targetEndX, targetEndY, targetEndZ_Ignored);
    }
    if(OutsideWorldRange(startX, startY) || OutsideWorldRange(endX, endY))
    {
        return false;
    }
    return StartConnects(startX, startY, endX, endY, targetEndX, targetEndY)
        && StartConnects(targetStartX, targetStartY, targetEndX, targetEndY, endX, endY);
}
void
cContentChunkInstance::makeEdgeConnection(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const
{
    INTERFACE_ASSERT(edgesCanConnect(connectingEdgeOnThis, targetInstance, connectingEdgeOnTarget));
    INTERFACE_ASSERT(connectingEdgeOnThis >= 0 && connectingEdgeOnThis < numberOfConnectingEdges());
    tSigned32 startX, startY, endX, endY;
    float startZ, endZ;
    targetInstance->getConnectingEdgeGeometry(connectingEdgeOnTarget, startX, startY, startZ, endX, endY, endZ);
    _bridgingModifier->setConnectionTarget(connectingEdgeOnThis, startX, startY, startZ, endX, endY, endZ);
}
