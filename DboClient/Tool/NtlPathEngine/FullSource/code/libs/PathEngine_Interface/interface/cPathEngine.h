//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_PATHENGINE_INCLUDED
#define C_PATHENGINE_INCLUDED

#include "i_pathengine.h"
#include "common/Containers/ReplacementList.h"
#include "common/Handle.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/PathEngine_Interface/interface/cContentChunk.h"
#include "libs/PathEngine_Interface/interface/cMeshFederation.h"
#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

template <class T> class cPool;
class cPath;
class cCollisionInfo;
class cPreprocessShapes;
class cAStarHeapPool;
class cConvexPoly;
class cRandomSequence;
class iRandomSequence;
class cMutex;
template <class T> class cPool;

class cPathEngine : public iPathEngine
{
    cPreprocessShapes* _preprocessShapes;
    cAStarHeapPool* _heapPool;

    cList_NoSize<cShape*> _heldShapes;
    cMutex* _heldShapesMutex;

    cList_NoSize<cMesh*> _heldMeshes;
    cMutex* _heldMeshesMutex;

    cList_NoSize<cContentChunk*> _heldContentChunks;
    cMutex* _heldContentChunksMutex;

    cList_NoSize<cMeshFederation*> _heldFederations;
    cMutex* _heldFederationsMutex;

    cRandomSequence* _randomSequence;
    cMutex* _randomSequenceMutex;

    cMutex* _generalMutex;
    tSigned32 _pathReserveSize;

    cPool<cPath>* _pathPool;
    cPool<cCollisionInfo>* _collisionInfoPool;

public:

    // for construction in Singletons.cpp
    //...... make these private again
    cPathEngine();
    void stage2Initialisation();
    ~cPathEngine();

// iPathEngine

    const char *const* getVersionAttributes() const;
    tSigned32 getInterfaceMajorVersion() const;
    tSigned32 getInterfaceMinorVersion() const;
    void getReleaseNumbers(tSigned32& majorReleaseNumber, tSigned32& minorReleaseNumber, tSigned32& internalReleaseNumber) const;
    void setErrorHandler(iErrorHandler* newHandler);
    iErrorHandler* getErrorHandler();
    void setPathPoolParameters(tSigned32 pathsPooled, tSigned32 pointsPerPath);
    bool shapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData);
    iShape* newShape(tSigned32 numberOfPoints, const tSigned32* coordinateData);
    iMesh* loadMeshFromBuffer(const char* format, const char* dataBuffer, tUnsigned32 bufferSize, const char *const* options);
    iMesh* buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char *const* options);
    void saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char* format, iOutputStream* os);
    tUnsigned32 totalMemoryAllocated();
    tUnsigned32 maximumMemoryAllocated();
    void resetMaximumMemoryAllocated();
    void setRandomSeed(tUnsigned32 value);
    bool collisionPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize);
    bool pathfindPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize);
    void deleteAllObjects();
    iContentChunk* newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes);
    iContentChunk* loadContentChunk(const char* dataBuffer, tUnsigned32 bufferSize);
    iMeshFederation* buildMeshFederation_FromWorldMesh(const iMesh* worldMesh, tSigned32 tileSize, tSigned32 overlap, iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack);
    iMeshFederation* loadFederation(const char* format, const char* dataBuffer, tUnsigned32 bufferSize);
    iMeshFederation* buildMeshFederation_TilingOnly(const cHorizontalRange& worldRange, tSigned32 tileSize, tSigned32 overlap);
    bool largeStaticObstacleShapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData, tSigned32 x, tSigned32 y);
    void operator delete(void* p) {::operator delete(p);}

//=========================

    void
    saveContentData(
            const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects,
            const iAnchorsAndPinnedShapes* anchorsAndShapes,
            const char* format,
            iOutputStream* os
            );

    iMesh* constructMeshFrom3D(
            tMesh_3D& baseMesh,
            std::vector<tSigned32>* surfaceTypes,
            std::vector<tSigned32>* sectionIDs,
            std::vector<tSigned32>* userData,
            const char *const* options
            );

    tSigned32 allocateIndexForPreprocessShape(const cConvexPoly& shape);
    tSigned32 numberOfPreprocessShapes() const;
    const cConvexPoly& refPreprocessShape(tSigned32 index) const;
    void getPreprocessShapeBounds(tSigned32 index, tPoint& boundsMin, tPoint& boundsMax) const;

    void lockHeldShapes() const;
    void unlockHeldShapes() const;
    void lockHeldMeshes() const;
    void unlockHeldMeshes() const;
    void lockHeldContentChunks() const;
    void unlockHeldContentChunks() const;
    void lockHeldFederations() const;
    void unlockHeldFederations() const;

    tSigned32 getRandomSeed() const;
    void setRandomSeed_Long(tSigned32 value);
    tSigned32 random15();
    tSigned32 random31();
    iRandomSequence& lockRandomSequence();
    void releaseRandomSequence();

    cAStarHeapPool& refHeapPool()
    {
        return *_heapPool;
    }
    tSigned32 getPathReserveSize();
    cPool<cPath>& refPathPool()
    {
        return *_pathPool;
    }

    cPool<cCollisionInfo>& refCollisionInfoPool()
    {
        return *_collisionInfoPool;
    }

    iShape* newShape_Internal(const cConvexPoly& cs);

};

inline cPathEngine* FromI(iPathEngine* ptr)
{
    return static_cast<cPathEngine*>(ptr);
}

#endif
