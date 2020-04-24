//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IPATHENGINE_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IPATHENGINE_WRAPPER_INCLUDED

#include "i_pathengine.h"

class iPathEngine_Wrapper : public iPathEngine
{
public:

    typedef iPathEngine tWrapped;

	iPathEngine* _target;

    static tSigned32 interfaceIndex()
    {
        return 0;
    }

    const char *const*
    getVersionAttributes() const;

    tSigned32
    getInterfaceMajorVersion() const;

    tSigned32
    getInterfaceMinorVersion() const;

    void
    getReleaseNumbers(tSigned32& majorReleaseNumber, tSigned32& minorReleaseNumber, tSigned32& internalReleaseNumber) const;

    void
    setErrorHandler(iErrorHandler* newHandler);

    iErrorHandler*
    getErrorHandler();

    void
    setPathPoolParameters(tSigned32 pathsPooled, tSigned32 pointsPerPath);

    bool
    shapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData);

    iShape*
    newShape(tSigned32 numberOfPoints, const tSigned32* coordinateData);

    iMesh*
    loadMeshFromBuffer(const char* format, const char* dataBuffer, tUnsigned32 bufferSize, const char *const* options);

    iMesh*
    buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char *const* options);

    void
    saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char* format, iOutputStream* os);

    tUnsigned32
    totalMemoryAllocated();

    tUnsigned32
    maximumMemoryAllocated();

    void
    resetMaximumMemoryAllocated();

    void
    setRandomSeed(tUnsigned32 value);

    bool
    collisionPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize);

    bool
    pathfindPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize);

    void
    deleteAllObjects();

    iContentChunk*
    newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes);

    iContentChunk*
    loadContentChunk(const char* dataBuffer, tUnsigned32 bufferSize);

    iMeshFederation*
    buildMeshFederation_FromWorldMesh(const iMesh* worldMesh, tSigned32 tileSize, tSigned32 overlap, iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack);

    iMeshFederation*
    loadFederation(const char* format, const char* dataBuffer, tUnsigned32 bufferSize);

    iMeshFederation*
    buildMeshFederation_TilingOnly(const cHorizontalRange& worldRange, tSigned32 tileSize, tSigned32 overlap);

    bool
    largeStaticObstacleShapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData, tSigned32 x, tSigned32 y);
};

#endif
