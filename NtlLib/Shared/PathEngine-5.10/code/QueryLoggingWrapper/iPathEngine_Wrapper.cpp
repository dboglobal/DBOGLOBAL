//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iPathEngine_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
const char *const*
iPathEngine_Wrapper::getVersionAttributes() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 0, (void*)this);
    gLogger.methodEntry_Completed();
    const char *const* result = _target->getVersionAttributes();
    gLogger.methodExit(0, 0, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iPathEngine_Wrapper::getInterfaceMajorVersion() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 1, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getInterfaceMajorVersion();
    gLogger.methodExit(0, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tSigned32
iPathEngine_Wrapper::getInterfaceMinorVersion() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 2, (void*)this);
    gLogger.methodEntry_Completed();
    tSigned32 result = _target->getInterfaceMinorVersion();
    gLogger.methodExit(0, 2, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::getReleaseNumbers(tSigned32& majorReleaseNumber, tSigned32& minorReleaseNumber, tSigned32& internalReleaseNumber) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 3, (void*)this);
    gLogger.methodEntry_Completed();
    _target->getReleaseNumbers(majorReleaseNumber, minorReleaseNumber, internalReleaseNumber);
    gLogger.methodExit(0, 3, queryCount, (void*)this);
    gLogger.log(majorReleaseNumber);
    gLogger.log(minorReleaseNumber);
    gLogger.log(internalReleaseNumber);
    gLogger.methodExit_Completed();
}

void
iPathEngine_Wrapper::setErrorHandler(iErrorHandler* newHandler)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 4, (void*)this);
    gLogger.methodEntry_Completed();
    _target->setErrorHandler(newHandler);
    gLogger.methodExit(0, 4, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iErrorHandler*
iPathEngine_Wrapper::getErrorHandler()
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 5, (void*)this);
    gLogger.methodEntry_Completed();
    iErrorHandler* result = _target->getErrorHandler();
    gLogger.methodExit(0, 5, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::setPathPoolParameters(tSigned32 pathsPooled, tSigned32 pointsPerPath)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 6, (void*)this);
    gLogger.log(pathsPooled);
    gLogger.log(pointsPerPath);
    gLogger.methodEntry_Completed();
    _target->setPathPoolParameters(pathsPooled, pointsPerPath);
    gLogger.methodExit(0, 6, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iPathEngine_Wrapper::shapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 7, (void*)this);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.methodEntry_Completed();
    bool result = _target->shapeIsValid(numberOfPoints, coordinateData);
    gLogger.methodExit(0, 7, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iShape*
iPathEngine_Wrapper::newShape(tSigned32 numberOfPoints, const tSigned32* coordinateData)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 8, (void*)this);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.methodEntry_Completed();
    iShape* result = _target->newShape(numberOfPoints, coordinateData);
    result = WrapNew<iShape_Wrapper>(result);
    gLogger.methodExit(0, 8, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMesh*
iPathEngine_Wrapper::loadMeshFromBuffer(const char* format, const char* dataBuffer, tUnsigned32 bufferSize, const char *const* options)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 9, (void*)this);
    gLogger.log(format);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.log(options);
    gLogger.methodEntry_Completed();
    iMesh* result = _target->loadMeshFromBuffer(format, dataBuffer, bufferSize, options);
    result = WrapNew<iMesh_Wrapper>(result);
    gLogger.methodExit(0, 9, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMesh*
iPathEngine_Wrapper::buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char *const* options)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 10, (void*)this);
    gLogger.logSourceContent(geometryObjectPointers, numberOfGeometryObjects);
    gLogger.log(options);
    gLogger.methodEntry_Completed();
    iMesh* result = _target->buildMeshFromContent(geometryObjectPointers, numberOfGeometryObjects, options);
    result = WrapNew<iMesh_Wrapper>(result);
    gLogger.methodExit(0, 10, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char* format, iOutputStream* os)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 11, (void*)this);
    gLogger.logSourceContent(geometryObjectPointers, numberOfGeometryObjects);
    gLogger.log(format);
    gLogger.methodEntry_Completed();
    _target->saveContentData(geometryObjectPointers, numberOfGeometryObjects, format, os);
    gLogger.methodExit(0, 11, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

tUnsigned32
iPathEngine_Wrapper::totalMemoryAllocated()
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 12, (void*)this);
    gLogger.methodEntry_Completed();
    tUnsigned32 result = _target->totalMemoryAllocated();
    gLogger.methodExit(0, 12, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

tUnsigned32
iPathEngine_Wrapper::maximumMemoryAllocated()
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 13, (void*)this);
    gLogger.methodEntry_Completed();
    tUnsigned32 result = _target->maximumMemoryAllocated();
    gLogger.methodExit(0, 13, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::resetMaximumMemoryAllocated()
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 14, (void*)this);
    gLogger.methodEntry_Completed();
    _target->resetMaximumMemoryAllocated();
    gLogger.methodExit(0, 14, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iPathEngine_Wrapper::setRandomSeed(tUnsigned32 value)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 15, (void*)this);
    gLogger.log(value);
    gLogger.methodEntry_Completed();
    _target->setRandomSeed(value);
    gLogger.methodExit(0, 15, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iPathEngine_Wrapper::collisionPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 16, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    bool result = _target->collisionPreprocessVersionIsCompatible(dataBuffer, bufferSize);
    gLogger.methodExit(0, 16, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iPathEngine_Wrapper::pathfindPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 17, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    bool result = _target->pathfindPreprocessVersionIsCompatible(dataBuffer, bufferSize);
    gLogger.methodExit(0, 17, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::deleteAllObjects()
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 18, (void*)this);
    gLogger.methodEntry_Completed();
    _target->deleteAllObjects();
    gLogger.methodExit(0, 18, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iContentChunk*
iPathEngine_Wrapper::newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 19, (void*)this);
    gLogger.log(attributes);
    gLogger.methodEntry_Completed();
    iContentChunk* result = _target->newContentChunk(ground, anchorsAndShapes, attributes);
    result = WrapNew<iContentChunk_Wrapper>(result);
    gLogger.methodExit(0, 19, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iContentChunk*
iPathEngine_Wrapper::loadContentChunk(const char* dataBuffer, tUnsigned32 bufferSize)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 20, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    iContentChunk* result = _target->loadContentChunk(dataBuffer, bufferSize);
    result = WrapNew<iContentChunk_Wrapper>(result);
    gLogger.methodExit(0, 20, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMeshFederation*
iPathEngine_Wrapper::buildMeshFederation_FromWorldMesh(const iMesh* worldMesh, tSigned32 tileSize, tSigned32 overlap, iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 21, (void*)this);
    gLogger.log((void*)worldMesh);
    gLogger.log(tileSize);
    gLogger.log(overlap);
    gLogger.methodEntry_Completed();
    iMeshFederation* result = _target->buildMeshFederation_FromWorldMesh(ExtractWrapped<const iMesh_Wrapper>(worldMesh), tileSize, overlap, tileGeneratedCallBack);
    result = WrapNew<iMeshFederation_Wrapper>(result);
    gLogger.methodExit(0, 21, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMeshFederation*
iPathEngine_Wrapper::loadFederation(const char* format, const char* dataBuffer, tUnsigned32 bufferSize)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 22, (void*)this);
    gLogger.log(format);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    iMeshFederation* result = _target->loadFederation(format, dataBuffer, bufferSize);
    result = WrapNew<iMeshFederation_Wrapper>(result);
    gLogger.methodExit(0, 22, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMeshFederation*
iPathEngine_Wrapper::buildMeshFederation_TilingOnly(const cHorizontalRange& worldRange, tSigned32 tileSize, tSigned32 overlap)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 23, (void*)this);
    gLogger.log(worldRange);
    gLogger.log(tileSize);
    gLogger.log(overlap);
    gLogger.methodEntry_Completed();
    iMeshFederation* result = _target->buildMeshFederation_TilingOnly(worldRange, tileSize, overlap);
    result = WrapNew<iMeshFederation_Wrapper>(result);
    gLogger.methodExit(0, 23, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iPathEngine_Wrapper::largeStaticObstacleShapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData, tSigned32 x, tSigned32 y)
{
    tUnsigned32 queryCount = gLogger.methodEntry(0, 24, (void*)this);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    bool result = _target->largeStaticObstacleShapeIsValid(numberOfPoints, coordinateData, x, y);
    gLogger.methodExit(0, 24, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}
