//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IMESH_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IMESH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"
#include "common/STL/list.h"

class iMesh_Wrapper : public iMesh, public iAPIObjectWrapper
{
public:

    typedef iMesh tWrapped;

	iMesh* _target;
    std::list<iAPIObjectWrapper*> _classCallBacks;

    static tSigned32 interfaceIndex()
    {
        return 2;
    }

    void
    destroy();

    tSigned32
    getNumberOf3DFaces() const;

    tSigned32
    get3DFaceAtPosition(const cPosition& position) const;

    void
    get3DFaceVertex(tSigned32 faceIndex, tSigned32 vertexIndexInFace, tSigned32& x, tSigned32& y, float& z) const;

    tSigned32
    get3DFaceConnection(tSigned32 faceIndex, tSigned32 edgeIndexInFace) const;

    void
    get3DFaceNormal(tSigned32 faceIndex, float& resultX, float& resultY, float& resultZ) const;

    tSigned32
    get3DFaceAttribute(tSigned32 faceIndex, tSigned32 attributeIndex) const;

    tSigned32
    getNumberOfSections() const;

    tSigned32
    getSurfaceType(const cPosition& position) const;

    tSigned32
    getSectionID(const cPosition& position) const;

    void
    storeAnchor(const char* id, const cPosition& position, tSigned32 orientation);

    cPosition
    retrieveAnchor(const char* id, tSigned32& orientation) const;

    tSigned32
    getNumberOfAnchors() const;

    void
    retrieveAnchorByIndex(tSigned32 index, cPosition& position, tSigned32& orientation, const char*& id) const;

    void
    storeNamedObstacle(const char* id, iAgent* agent);

    iAgent*
    retrieveNamedObstacle(const char* id) const;

    tSigned32
    getNumberOfNamedObstacles() const;

    void
    retrieveNamedObstacleByIndex(tSigned32 index, iAgent*& agent, const char*& id) const;

    cPosition
    positionFor3DPoint(const tSigned32* point) const;

    cPosition
    positionFor3DPointF(const float* point) const;

    cPosition
    positionNear3DPoint(const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const;

    cPosition
    positionInSection(tSigned32 sectionID, tSigned32 x, tSigned32 y) const;

    cPosition
    positionInSectionFor3DPoint(tSigned32 sectionID, const tSigned32* point) const;

    cPosition
    positionInSectionFor3DPointF(tSigned32 sectionID, const float* point) const;

    cPosition
    positionInSectionNear3DPoint(tSigned32 sectionID, const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const;

    cPosition
    positionFor3DPoint_ExcludeTerrain(const tSigned32* point, tSigned32 numberOfTerrainLayers) const;

    cPosition
    generateRandomPosition() const;

    cPosition
    generateRandomPositionLocally(const cPosition& centre, tSigned32 range) const;

    cPosition
    generateRandomPositionInSection(tSigned32 sectionID) const;

    tSigned32
    heightAtPosition(const cPosition& position) const;

    float
    heightAtPositionF(const cPosition& position) const;

    float
    heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const;

    tSigned32
    getCellForEndOfLine(const cPosition& start, tSigned32 endX, tSigned32 endY) const;

    bool
    positionIsValid(const cPosition& p) const;

    void
    burnContextIntoMesh(iCollisionContext* context);

    void
    saveGround(const char* format, bool includeMapping, iOutputStream* outputStream) const;

    void
    setTerrainCallBack(tSigned32 terrainLayer, iTerrainCallBack* callBack);

    tSigned32
    addEndPoint(const cPosition& position);

    tSigned32
    addOffMeshConnection(tSigned32 fromEndPoint, tSigned32 toEndPoint, tSigned32 penalty);

    tSigned32
    getNumberOfEndPoints() const;

    cPosition
    getEndPoint(tSigned32 index) const;

    tSigned32
    getNumberOfOffMeshConnections() const;

    void
    getOffMeshConnectionInfo(tSigned32 index, tSigned32& fromEndPoint, tSigned32& toEndPoint, tSigned32& penalty) const;

    void
    clearOffMeshConnectionsAndEndPoints();

    void
    generateCollisionPreprocessFor(iShape* shape, const char *const* attributes);

    void
    generatePathfindPreprocessFor(iShape* shape, const char *const* attributes);

    void
    releaseAllPreprocessFor(iShape* shape);

    void
    releasePathfindPreprocessFor(iShape* shape);

    void
    preprocessGenerationCompleted();

    bool
    shapeCanCollide(const iShape* shape) const;

    bool
    shapeCanPathfind(const iShape* shape) const;

    const char *const*
    getCollisionPreprocessAttributes(iShape* shape) const;

    const char *const*
    getPathfindPreprocessAttributes(iShape* shape) const;

    void
    saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const;

    void
    savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const;

    void
    loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize);

    void
    loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize);

    iAgent*
    placeAgent(iShape* shape, const cPosition& position) const;

    iCollisionContext*
    newContext() const;

    iObstacleSet*
    newObstacleSet() const;

    bool
    testPointCollision(iShape* shape, iCollisionContext* context, const cPosition& position) const;

    bool
    testLineCollision(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& end) const;

    bool
    testLineCollision_XY(iShape* shape, iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const;

    iCollisionInfo*
    firstCollision(iShape* shape, iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const;

    cPosition
    findClosestUnobstructedPosition(iShape* shape, iCollisionContext* context, const cPosition& position, tSigned32 maximumDistance) const;

    void
    getAllAgentsOverlapped(iShape* shape, iCollisionContext* context, const cPosition& position, iAgent** resultsBuffer, tSigned32& numberOverlapped) const;

    iPath*
    findShortestPath(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal) const;

    iPath*
    findShortestPath_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack* queryCallBack) const;

    iPath*
    findPathAway(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway) const;

    iPath*
    findPathAway_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const;

    iPath*
    generateCurvedPath(iShape* shape, iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const;

    iPath*
    constructPath(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const;

    iPath*
    constructPath_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const;

    void
    savePath(iPath* path, iOutputStream* os) const;

    iPath*
    loadPath(const char* dataBuffer, tUnsigned32 bufferSize) const;

    void
    renderLineOnGround(const cPosition& start, tSigned32 endX, tSigned32 endY, tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const;

    iObstacleSet*
    newObstacleSet_WithAttributes(const char *const* attributes) const;

    cPosition
    positionInSectionInShape(tSigned32 sectionID, tSigned32 shapeOriginX, tSigned32 shapeOriginY, tSigned32 numberOfPoints, const tSigned32* coordinateData) const;

    iPath*
    generateCurvedPath_WithEndVector(iShape* shape, iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const;

    iAgent*
    placeLargeStaticObstacle(tSigned32 numberOfPoints, const tSigned32* coordinateData, const cPosition& position) const;

    void
    addAnchorsAndShapes(const iAnchorsAndPinnedShapes* anchorsAndShapes, const char* idPrefix, tSigned32 numberOfTerrainLayers);

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
