//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iMesh_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iMesh_Wrapper::destroy()
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(2, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

tSigned32
iMesh_Wrapper::getNumberOf3DFaces() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 1, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getNumberOf3DFaces();
    gLogger.methodExit(2, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::get3DFaceAtPosition(const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 2, (void*)this);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->get3DFaceAtPosition(position);
    gLogger.methodExit(2, 2, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::get3DFaceVertex(tSigned32 faceIndex, tSigned32 vertexIndexInFace, tSigned32& x, tSigned32& y, float& z) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 3, (void*)this);
    gLogger.log(faceIndex);
    gLogger.log(vertexIndexInFace);
    gLogger.methodEntry_Completed();
    _target->get3DFaceVertex(faceIndex, vertexIndexInFace, x, y, z);
    gLogger.methodExit(2, 3, queryCount, (void*)this);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.log(z);
    gLogger.methodExit_Completed();
}

tSigned32
iMesh_Wrapper::get3DFaceConnection(tSigned32 faceIndex, tSigned32 edgeIndexInFace) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 4, (void*)this);
    gLogger.log(faceIndex);
    gLogger.log(edgeIndexInFace);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->get3DFaceConnection(faceIndex, edgeIndexInFace);
    gLogger.methodExit(2, 4, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::get3DFaceNormal(tSigned32 faceIndex, float& resultX, float& resultY, float& resultZ) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 5, (void*)this);
    gLogger.log(faceIndex);
    gLogger.methodEntry_Completed();
    _target->get3DFaceNormal(faceIndex, resultX, resultY, resultZ);
    gLogger.methodExit(2, 5, queryCount, (void*)this);
    gLogger.log(resultX);
    gLogger.log(resultY);
    gLogger.log(resultZ);
    gLogger.methodExit_Completed();
}

tSigned32
iMesh_Wrapper::get3DFaceAttribute(tSigned32 faceIndex, tSigned32 attributeIndex) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 6, (void*)this);
    gLogger.log(faceIndex);
    gLogger.log(attributeIndex);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->get3DFaceAttribute(faceIndex, attributeIndex);
    gLogger.methodExit(2, 6, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getNumberOfSections() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 7, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getNumberOfSections();
    gLogger.methodExit(2, 7, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getSurfaceType(const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 8, (void*)this);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getSurfaceType(position);
    gLogger.methodExit(2, 8, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getSectionID(const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 9, (void*)this);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getSectionID(position);
    gLogger.methodExit(2, 9, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::storeAnchor(const char* id, const cPosition& position, tSigned32 orientation)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 10, (void*)this);
    gLogger.log(id);
    gLogger.log(position);
    gLogger.log(orientation);
    gLogger.methodEntry_Completed();
    _target->storeAnchor(id, position, orientation);
    gLogger.methodExit(2, 10, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

cPosition
iMesh_Wrapper::retrieveAnchor(const char* id, tSigned32& orientation) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 11, (void*)this);
    gLogger.log(id);
    gLogger.methodEntry_Completed();
    cPosition result = _target->retrieveAnchor(id, orientation);
    gLogger.methodExit(2, 11, queryCount, (void*)this);
    gLogger.log(orientation);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getNumberOfAnchors() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 12, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getNumberOfAnchors();
    gLogger.methodExit(2, 12, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::retrieveAnchorByIndex(tSigned32 index, cPosition& position, tSigned32& orientation, const char*& id) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 13, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    _target->retrieveAnchorByIndex(index, position, orientation, id);
    gLogger.methodExit(2, 13, queryCount, (void*)this);
    gLogger.log(position);
    gLogger.log(orientation);
    gLogger.log(id);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::storeNamedObstacle(const char* id, iAgent* agent)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 14, (void*)this);
    gLogger.log(id);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    _target->storeNamedObstacle(id, ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(2, 14, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iAgent*
iMesh_Wrapper::retrieveNamedObstacle(const char* id) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 15, (void*)this);
    gLogger.log(id);
    gLogger.methodEntry_Completed();
    iAgent* result = _target->retrieveNamedObstacle(id);
    ExtractWrapper<iAgent_Wrapper>(result);
    gLogger.methodExit(2, 15, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getNumberOfNamedObstacles() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 16, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getNumberOfNamedObstacles();
    gLogger.methodExit(2, 16, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::retrieveNamedObstacleByIndex(tSigned32 index, iAgent*& agent, const char*& id) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 17, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    _target->retrieveNamedObstacleByIndex(index, agent, id);
    ExtractWrapper<iAgent_Wrapper>(agent);
    gLogger.methodExit(2, 17, queryCount, (void*)this);
    gLogger.log((void*)agent);
    gLogger.log(id);
    gLogger.methodExit_Completed();
}

cPosition
iMesh_Wrapper::positionFor3DPoint(const tSigned32* point) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 18, (void*)this);
    gLogger.log(point);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionFor3DPoint(point);
    gLogger.methodExit(2, 18, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionFor3DPointF(const float* point) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 19, (void*)this);
    gLogger.log(point);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionFor3DPointF(point);
    gLogger.methodExit(2, 19, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionNear3DPoint(const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 20, (void*)this);
    gLogger.log(point);
    gLogger.log(horizontalRange);
    gLogger.log(verticalRange);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionNear3DPoint(point, horizontalRange, verticalRange);
    gLogger.methodExit(2, 20, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionInSection(tSigned32 sectionID, tSigned32 x, tSigned32 y) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 21, (void*)this);
    gLogger.log(sectionID);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionInSection(sectionID, x, y);
    gLogger.methodExit(2, 21, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionInSectionFor3DPoint(tSigned32 sectionID, const tSigned32* point) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 22, (void*)this);
    gLogger.log(sectionID);
    gLogger.log(point);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionInSectionFor3DPoint(sectionID, point);
    gLogger.methodExit(2, 22, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionInSectionFor3DPointF(tSigned32 sectionID, const float* point) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 23, (void*)this);
    gLogger.log(sectionID);
    gLogger.log(point);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionInSectionFor3DPointF(sectionID, point);
    gLogger.methodExit(2, 23, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionInSectionNear3DPoint(tSigned32 sectionID, const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 24, (void*)this);
    gLogger.log(sectionID);
    gLogger.log(point);
    gLogger.log(horizontalRange);
    gLogger.log(verticalRange);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionInSectionNear3DPoint(sectionID, point, horizontalRange, verticalRange);
    gLogger.methodExit(2, 24, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionFor3DPoint_ExcludeTerrain(const tSigned32* point, tSigned32 numberOfTerrainLayers) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 25, (void*)this);
    gLogger.log(point);
    gLogger.log(numberOfTerrainLayers);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionFor3DPoint_ExcludeTerrain(point, numberOfTerrainLayers);
    gLogger.methodExit(2, 25, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::generateRandomPosition() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 26, (void*)this);
    gLogger.methodEntry_Completed();
    cPosition result = _target->generateRandomPosition();
    gLogger.methodExit(2, 26, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::generateRandomPositionLocally(const cPosition& centre, tSigned32 range) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 27, (void*)this);
    gLogger.log(centre);
    gLogger.log(range);
    gLogger.methodEntry_Completed();
    cPosition result = _target->generateRandomPositionLocally(centre, range);
    gLogger.methodExit(2, 27, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::generateRandomPositionInSection(tSigned32 sectionID) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 28, (void*)this);
    gLogger.log(sectionID);
    gLogger.methodEntry_Completed();
    cPosition result = _target->generateRandomPositionInSection(sectionID);
    gLogger.methodExit(2, 28, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::heightAtPosition(const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 29, (void*)this);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->heightAtPosition(position);
    gLogger.methodExit(2, 29, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

float
iMesh_Wrapper::heightAtPositionF(const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 30, (void*)this);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    float result = _target->heightAtPositionF(position);
    gLogger.methodExit(2, 30, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

float
iMesh_Wrapper::heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 31, (void*)this);
    gLogger.log(position);
    gLogger.log(precisionX);
    gLogger.log(precisionY);
    gLogger.methodEntry_Completed();
    float result = _target->heightAtPositionWithPrecision(position, precisionX, precisionY);
    gLogger.methodExit(2, 31, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getCellForEndOfLine(const cPosition& start, tSigned32 endX, tSigned32 endY) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 32, (void*)this);
    gLogger.log(start);
    gLogger.log(endX);
    gLogger.log(endY);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getCellForEndOfLine(start, endX, endY);
    gLogger.methodExit(2, 32, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iMesh_Wrapper::positionIsValid(const cPosition& p) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 33, (void*)this);
    gLogger.log(p);
    gLogger.methodEntry_Completed();
    bool result = _target->positionIsValid(p);
    gLogger.methodExit(2, 33, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::burnContextIntoMesh(iCollisionContext* context)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 34, (void*)this);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    _target->burnContextIntoMesh(ExtractWrapped<iCollisionContext_Wrapper>(context));
    gLogger.methodExit(2, 34, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::saveGround(const char* format, bool includeMapping, iOutputStream* outputStream) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 35, (void*)this);
    gLogger.log(format);
    gLogger.log(includeMapping);
    gLogger.methodEntry_Completed();
    _target->saveGround(format, includeMapping, outputStream);
    gLogger.methodExit(2, 35, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::setTerrainCallBack(tSigned32 terrainLayer, iTerrainCallBack* callBack)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 36, (void*)this);
    gLogger.log(terrainLayer);
    gLogger.log((void*)callBack);
    gLogger.methodEntry_Completed();
    iTerrainCallBack_Wrapper* callBack_Wrapper = new iTerrainCallBack_Wrapper;
    _classCallBacks.push_back(callBack_Wrapper);
    callBack_Wrapper->_target = callBack;
    _target->setTerrainCallBack(terrainLayer, callBack_Wrapper);
    gLogger.methodExit(2, 36, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

tSigned32
iMesh_Wrapper::addEndPoint(const cPosition& position)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 37, (void*)this);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->addEndPoint(position);
    gLogger.methodExit(2, 37, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::addOffMeshConnection(tSigned32 fromEndPoint, tSigned32 toEndPoint, tSigned32 penalty)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 38, (void*)this);
    gLogger.log(fromEndPoint);
    gLogger.log(toEndPoint);
    gLogger.log(penalty);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->addOffMeshConnection(fromEndPoint, toEndPoint, penalty);
    gLogger.methodExit(2, 38, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getNumberOfEndPoints() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 39, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getNumberOfEndPoints();
    gLogger.methodExit(2, 39, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::getEndPoint(tSigned32 index) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 40, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    cPosition result = _target->getEndPoint(index);
    gLogger.methodExit(2, 40, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iMesh_Wrapper::getNumberOfOffMeshConnections() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 41, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getNumberOfOffMeshConnections();
    gLogger.methodExit(2, 41, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::getOffMeshConnectionInfo(tSigned32 index, tSigned32& fromEndPoint, tSigned32& toEndPoint, tSigned32& penalty) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 42, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    _target->getOffMeshConnectionInfo(index, fromEndPoint, toEndPoint, penalty);
    gLogger.methodExit(2, 42, queryCount, (void*)this);
    gLogger.log(fromEndPoint);
    gLogger.log(toEndPoint);
    gLogger.log(penalty);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::clearOffMeshConnectionsAndEndPoints()
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 43, (void*)this);
    gLogger.methodEntry_Completed();
    _target->clearOffMeshConnectionsAndEndPoints();
    gLogger.methodExit(2, 43, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::generateCollisionPreprocessFor(iShape* shape, const char *const* attributes)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 44, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log(attributes);
    gLogger.methodEntry_Completed();
    _target->generateCollisionPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape), attributes);
    gLogger.methodExit(2, 44, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::generatePathfindPreprocessFor(iShape* shape, const char *const* attributes)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 45, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log(attributes);
    gLogger.methodEntry_Completed();
    _target->generatePathfindPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape), attributes);
    gLogger.methodExit(2, 45, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::releaseAllPreprocessFor(iShape* shape)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 46, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->releaseAllPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(2, 46, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::releasePathfindPreprocessFor(iShape* shape)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 47, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->releasePathfindPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(2, 47, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::preprocessGenerationCompleted()
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 48, (void*)this);
    gLogger.methodEntry_Completed();
    _target->preprocessGenerationCompleted();
    gLogger.methodExit(2, 48, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iMesh_Wrapper::shapeCanCollide(const iShape* shape) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 49, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    bool result = _target->shapeCanCollide(ExtractWrapped<const iShape_Wrapper>(shape));
    gLogger.methodExit(2, 49, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iMesh_Wrapper::shapeCanPathfind(const iShape* shape) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 50, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    bool result = _target->shapeCanPathfind(ExtractWrapped<const iShape_Wrapper>(shape));
    gLogger.methodExit(2, 50, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

const char *const*
iMesh_Wrapper::getCollisionPreprocessAttributes(iShape* shape) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 51, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    const char *const* result = _target->getCollisionPreprocessAttributes(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(2, 51, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

const char *const*
iMesh_Wrapper::getPathfindPreprocessAttributes(iShape* shape) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 52, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    const char *const* result = _target->getPathfindPreprocessAttributes(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(2, 52, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 53, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->saveCollisionPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape), os);
    gLogger.methodExit(2, 53, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 54, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->savePathfindPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape), os);
    gLogger.methodExit(2, 54, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 55, (void*)this);
    gLogger.log((void*)shape);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    _target->loadCollisionPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape), dataBuffer, bufferSize);
    gLogger.methodExit(2, 55, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMesh_Wrapper::loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 56, (void*)this);
    gLogger.log((void*)shape);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    _target->loadPathfindPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape), dataBuffer, bufferSize);
    gLogger.methodExit(2, 56, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iAgent*
iMesh_Wrapper::placeAgent(iShape* shape, const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 57, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    iAgent* result = _target->placeAgent(ExtractWrapped<iShape_Wrapper>(shape), position);
    result = WrapNew<iAgent_Wrapper>(result);
    gLogger.methodExit(2, 57, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iCollisionContext*
iMesh_Wrapper::newContext() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 58, (void*)this);
    gLogger.methodEntry_Completed();
    iCollisionContext* result = _target->newContext();
    result = WrapNew<iCollisionContext_Wrapper>(result);
    gLogger.methodExit(2, 58, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iObstacleSet*
iMesh_Wrapper::newObstacleSet() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 59, (void*)this);
    gLogger.methodEntry_Completed();
    iObstacleSet* result = _target->newObstacleSet();
    result = WrapNew<iObstacleSet_Wrapper>(result);
    gLogger.methodExit(2, 59, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iMesh_Wrapper::testPointCollision(iShape* shape, iCollisionContext* context, const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 60, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    bool result = _target->testPointCollision(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), position);
    gLogger.methodExit(2, 60, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iMesh_Wrapper::testLineCollision(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& end) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 61, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(end);
    gLogger.methodEntry_Completed();
    bool result = _target->testLineCollision(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, end);
    gLogger.methodExit(2, 61, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iMesh_Wrapper::testLineCollision_XY(iShape* shape, iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 62, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    bool result = _target->testLineCollision_XY(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, x, y, cell);
    gLogger.methodExit(2, 62, queryCount, (void*)this);
    gLogger.log(cell);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iCollisionInfo*
iMesh_Wrapper::firstCollision(iShape* shape, iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 63, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    iCollisionInfo* result = _target->firstCollision(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, x, y, cell);
    result = WrapNew<iCollisionInfo_Wrapper>(result);
    gLogger.methodExit(2, 63, queryCount, (void*)this);
    gLogger.log(cell);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::findClosestUnobstructedPosition(iShape* shape, iCollisionContext* context, const cPosition& position, tSigned32 maximumDistance) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 64, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(position);
    gLogger.log(maximumDistance);
    gLogger.methodEntry_Completed();
    cPosition result = _target->findClosestUnobstructedPosition(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), position, maximumDistance);
    gLogger.methodExit(2, 64, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::getAllAgentsOverlapped(iShape* shape, iCollisionContext* context, const cPosition& position, iAgent** resultsBuffer, tSigned32& numberOverlapped) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 65, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    _target->getAllAgentsOverlapped(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), position, resultsBuffer, numberOverlapped);
    ExtractWrappers(resultsBuffer, numberOverlapped);
    gLogger.methodExit(2, 65, queryCount, (void*)this);
    gLogger.logAgentVector(resultsBuffer, numberOverlapped);
    gLogger.methodExit_Completed();
}

iPath*
iMesh_Wrapper::findShortestPath(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 66, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(goal);
    gLogger.methodEntry_Completed();
    iPath* result = _target->findShortestPath(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, goal);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 66, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::findShortestPath_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack* queryCallBack) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 67, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(goal);
    gLogger.log((void*)queryCallBack);
    gLogger.methodEntry_Completed();
    iQueryCallBack_Wrapper queryCallBack_Wrapper;
    queryCallBack_Wrapper._target = queryCallBack;
    iPath* result = _target->findShortestPath_WithQueryCallBack(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, goal, &queryCallBack_Wrapper);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 67, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::findPathAway(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 68, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(awayFrom);
    gLogger.log(distanceAway);
    gLogger.methodEntry_Completed();
    iPath* result = _target->findPathAway(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, awayFrom, distanceAway);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 68, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::findPathAway_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 69, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)context);
    gLogger.log(start);
    gLogger.log(awayFrom);
    gLogger.log(distanceAway);
    gLogger.log((void*)queryCallBack);
    gLogger.methodEntry_Completed();
    iQueryCallBack_Wrapper queryCallBack_Wrapper;
    queryCallBack_Wrapper._target = queryCallBack;
    iPath* result = _target->findPathAway_WithQueryCallBack(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iCollisionContext_Wrapper>(context), start, awayFrom, distanceAway, &queryCallBack_Wrapper);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 69, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::generateCurvedPath(iShape* shape, iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 70, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)basePath);
    gLogger.log((void*)context);
    gLogger.log(startVectorX);
    gLogger.log(startVectorY);
    gLogger.log(sectionLength);
    gLogger.log(turnRatio1);
    gLogger.log(turnRatio2);
    gLogger.methodEntry_Completed();
    iPath* result = _target->generateCurvedPath(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iPath_Wrapper>(basePath), ExtractWrapped<iCollisionContext_Wrapper>(context), startVectorX, startVectorY, sectionLength, turnRatio1, turnRatio2);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 70, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::constructPath(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 71, (void*)this);
    gLogger.logPathBuffers(positionsBuffer, connectionIndicesBuffer, pathLength);
    gLogger.methodEntry_Completed();
    iPath* result = _target->constructPath(positionsBuffer, connectionIndicesBuffer, pathLength);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 71, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::constructPath_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 72, (void*)this);
    gLogger.logPathBuffers(positionsBuffer, connectionIndicesBuffer, pathLength);
    gLogger.methodEntry_Completed();
    iPath* result = _target->constructPath_Reversed(positionsBuffer, connectionIndicesBuffer, pathLength);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 72, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::savePath(iPath* path, iOutputStream* os) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 73, (void*)this);
    gLogger.log((void*)path);
    gLogger.methodEntry_Completed();
    _target->savePath(ExtractWrapped<iPath_Wrapper>(path), os);
    gLogger.methodExit(2, 73, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iPath*
iMesh_Wrapper::loadPath(const char* dataBuffer, tUnsigned32 bufferSize) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 74, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    iPath* result = _target->loadPath(dataBuffer, bufferSize);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 74, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::renderLineOnGround(const cPosition& start, tSigned32 endX, tSigned32 endY, tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 75, (void*)this);
    gLogger.log(start);
    gLogger.log(endX);
    gLogger.log(endY);
    gLogger.log(originX);
    gLogger.log(originY);
    gLogger.methodEntry_Completed();
    _target->renderLineOnGround(start, endX, endY, originX, originY, callBack);
    gLogger.methodExit(2, 75, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iObstacleSet*
iMesh_Wrapper::newObstacleSet_WithAttributes(const char *const* attributes) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 76, (void*)this);
    gLogger.log(attributes);
    gLogger.methodEntry_Completed();
    iObstacleSet* result = _target->newObstacleSet_WithAttributes(attributes);
    result = WrapNew<iObstacleSet_Wrapper>(result);
    gLogger.methodExit(2, 76, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iMesh_Wrapper::positionInSectionInShape(tSigned32 sectionID, tSigned32 shapeOriginX, tSigned32 shapeOriginY, tSigned32 numberOfPoints, const tSigned32* coordinateData) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 77, (void*)this);
    gLogger.log(sectionID);
    gLogger.log(shapeOriginX);
    gLogger.log(shapeOriginY);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.methodEntry_Completed();
    cPosition result = _target->positionInSectionInShape(sectionID, shapeOriginX, shapeOriginY, numberOfPoints, coordinateData);
    gLogger.methodExit(2, 77, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iMesh_Wrapper::generateCurvedPath_WithEndVector(iShape* shape, iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 78, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log((void*)basePath);
    gLogger.log((void*)context);
    gLogger.log(startVectorX);
    gLogger.log(startVectorY);
    gLogger.log(endVectorX);
    gLogger.log(endVectorY);
    gLogger.log(sectionLength);
    gLogger.log(turnRatio1);
    gLogger.log(turnRatio2);
    gLogger.methodEntry_Completed();
    iPath* result = _target->generateCurvedPath_WithEndVector(ExtractWrapped<iShape_Wrapper>(shape), ExtractWrapped<iPath_Wrapper>(basePath), ExtractWrapped<iCollisionContext_Wrapper>(context), startVectorX, startVectorY, endVectorX, endVectorY, sectionLength, turnRatio1, turnRatio2);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(2, 78, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iAgent*
iMesh_Wrapper::placeLargeStaticObstacle(tSigned32 numberOfPoints, const tSigned32* coordinateData, const cPosition& position) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 79, (void*)this);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.log(position);
    gLogger.methodEntry_Completed();
    iAgent* result = _target->placeLargeStaticObstacle(numberOfPoints, coordinateData, position);
    result = WrapNew<iAgent_Wrapper>(result);
    gLogger.methodExit(2, 79, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMesh_Wrapper::addAnchorsAndShapes(const iAnchorsAndPinnedShapes* anchorsAndShapes, const char* idPrefix, tSigned32 numberOfTerrainLayers)
{
    tUnsigned32 queryCount = gLogger.methodEntry(2, 80, (void*)this);
    gLogger.log(idPrefix);
    gLogger.log(numberOfTerrainLayers);
    gLogger.methodEntry_Completed();
    _target->addAnchorsAndShapes(anchorsAndShapes, idPrefix, numberOfTerrainLayers);
    gLogger.methodExit(2, 80, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}
void iMesh_Wrapper::destroy_APIObjectWrapper()
{
    while(!_classCallBacks.empty())
    {
        _classCallBacks.front()->destroy_APIObjectWrapper();
        _classCallBacks.pop_front();
    }
    delete this;
}
