//**********************************************************************
//
// Copyright (c) 2002-2007
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_INTERFACE_HAS_BEEN_INCLUDED
#define PATHENGINE_INTERFACE_HAS_BEEN_INCLUDED

#if defined(__GNUC__) && defined(__x86_64__)
typedef int tSigned32;
typedef unsigned int tUnsigned32;
#else
typedef long tSigned32;
typedef unsigned long tUnsigned32;
#endif

const tSigned32 PATHENGINE_INTERFACE_MAJOR_VERSION = 3;
const tSigned32 PATHENGINE_INTERFACE_MINOR_VERSION = 1;

#ifndef PATHENGINE_POD_CLASSES_HAVE_BEEN_DEFINED
#define PATHENGINE_POD_CLASSES_HAVE_BEEN_DEFINED
class cPosition
{
public:
    cPosition() : cell(-1) {}
    tSigned32 x, y, cell;
    bool operator==(const cPosition& rhs) const
    {
        if(cell == -1)
            return cell == rhs.cell;
        return cell == rhs.cell && x == rhs.x && y == rhs.y;
    }
    bool operator!=(const cPosition& rhs) const
    {
        return !((*this) == rhs);
    }
};
class cHorizontalRange
{
public:
    tSigned32 minX, minY, maxX, maxY;
    bool operator==(const cHorizontalRange& rhs) const
    {
        return minX == rhs.minX && minY == rhs.minY && maxX == rhs.maxX && maxY == rhs.maxY;
    }
    bool operator!=(const cHorizontalRange& rhs) const
    {
        return !((*this) == rhs);
    }
};
#endif

class iShape;
class iMesh;
class iAgent;
class iCollisionContext;
class iObstacleSet;
class iPath;
class iCollisionInfo;
class iErrorHandler;
class iResourceLoader;
class iOutputStream;
class iTerrainCallBack;
class iRender3DLinesCallBack;
class iFaceVertexMesh;
class iAnchorsAndPinnedShapes;
class iQueryCallBack;
class iContentChunk;
class iContentChunkInstance;
class iMeshFederation;
class iMeshFederationTileGeneratedCallBack;

class iPathEngine
{
    void operator delete(void* /*voidPointer*/) {}
public:
    virtual const char *const* getVersionAttributes() const = 0;
    virtual tSigned32 getInterfaceMajorVersion() const = 0;
    virtual tSigned32 getInterfaceMinorVersion() const = 0;
    virtual void getReleaseNumbers(tSigned32& majorReleaseNumber, tSigned32& minorReleaseNumber, tSigned32& internalReleaseNumber) const = 0;
    virtual void setErrorHandler(iErrorHandler* newHandler) = 0;
    virtual iErrorHandler* getErrorHandler() = 0;
    virtual void setPathPoolParameters(tSigned32 pathsPooled, tSigned32 pointsPerPath) = 0;
    virtual bool shapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData) = 0;
    virtual iShape* newShape(tSigned32 numberOfPoints, const tSigned32* coordinateData) = 0;
    virtual iMesh* loadMeshFromBuffer(const char* format, const char* dataBuffer, tUnsigned32 bufferSize, const char *const* options) = 0;
    virtual iMesh* buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char *const* options) = 0;
    virtual void saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, tSigned32 numberOfGeometryObjects, const char* format, iOutputStream* os) = 0;
    virtual tUnsigned32 totalMemoryAllocated() = 0;
    virtual tUnsigned32 maximumMemoryAllocated() = 0;
    virtual void resetMaximumMemoryAllocated() = 0;
    virtual void setRandomSeed(tUnsigned32 value) = 0;
    virtual bool collisionPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize) = 0;
    virtual bool pathfindPreprocessVersionIsCompatible(const char* dataBuffer, tUnsigned32 bufferSize) = 0;
    virtual void deleteAllObjects() = 0;
    virtual iContentChunk* newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes) = 0;
    virtual iContentChunk* loadContentChunk(const char* dataBuffer, tUnsigned32 bufferSize) = 0;
    virtual iMeshFederation* buildMeshFederation_FromWorldMesh(const iMesh* worldMesh, tSigned32 tileSize, tSigned32 overlap, iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack) = 0;
    virtual iMeshFederation* loadFederation(const char* format, const char* dataBuffer, tUnsigned32 bufferSize) = 0;
    virtual iMeshFederation* buildMeshFederation_TilingOnly(const cHorizontalRange& worldRange, tSigned32 tileSize, tSigned32 overlap) = 0;
    virtual bool largeStaticObstacleShapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData, tSigned32 x, tSigned32 y) = 0;
};

class iShape
{
public:
    virtual void destroy() = 0;
    virtual tSigned32 size() const = 0;
    virtual void vertex(tSigned32 index, tSigned32& x, tSigned32& y) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iShape* afterCast = static_cast<iShape*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iMesh
{
public:
    virtual void destroy() = 0;
    virtual tSigned32 getNumberOf3DFaces() const = 0;
    virtual tSigned32 get3DFaceAtPosition(const cPosition& position) const = 0;
    virtual void get3DFaceVertex(tSigned32 faceIndex, tSigned32 vertexIndexInFace, tSigned32& x, tSigned32& y, float& z) const = 0;
    virtual tSigned32 get3DFaceConnection(tSigned32 faceIndex, tSigned32 edgeIndexInFace) const = 0;
    virtual void get3DFaceNormal(tSigned32 faceIndex, float& resultX, float& resultY, float& resultZ) const = 0;
    virtual tSigned32 get3DFaceAttribute(tSigned32 faceIndex, tSigned32 attributeIndex) const = 0;
    virtual tSigned32 getNumberOfSections() const = 0;
    virtual tSigned32 getSurfaceType(const cPosition& position) const = 0;
    virtual tSigned32 getSectionID(const cPosition& position) const = 0;
    virtual void storeAnchor(const char* id, const cPosition& position, tSigned32 orientation) = 0;
    virtual cPosition retrieveAnchor(const char* id, tSigned32& orientation) const = 0;
    virtual tSigned32 getNumberOfAnchors() const = 0;
    virtual void retrieveAnchorByIndex(tSigned32 index, cPosition& position, tSigned32& orientation, const char*& id) const = 0;
    virtual void storeNamedObstacle(const char* id, iAgent* agent) = 0;
    virtual iAgent* retrieveNamedObstacle(const char* id) const = 0;
    virtual tSigned32 getNumberOfNamedObstacles() const = 0;
    virtual void retrieveNamedObstacleByIndex(tSigned32 index, iAgent*& agent, const char*& id) const = 0;
    virtual cPosition positionFor3DPoint(const tSigned32* point) const = 0;
    virtual cPosition positionFor3DPointF(const float* point) const = 0;
    virtual cPosition positionNear3DPoint(const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const = 0;
    virtual cPosition positionInSection(tSigned32 sectionID, tSigned32 x, tSigned32 y) const = 0;
    virtual cPosition positionInSectionFor3DPoint(tSigned32 sectionID, const tSigned32* point) const = 0;
    virtual cPosition positionInSectionFor3DPointF(tSigned32 sectionID, const float* point) const = 0;
    virtual cPosition positionInSectionNear3DPoint(tSigned32 sectionID, const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const = 0;
    virtual cPosition positionFor3DPoint_ExcludeTerrain(const tSigned32* point, tSigned32 numberOfTerrainLayers) const = 0;
    virtual cPosition generateRandomPosition() const = 0;
    virtual cPosition generateRandomPositionLocally(const cPosition& centre, tSigned32 range) const = 0;
    virtual cPosition generateRandomPositionInSection(tSigned32 sectionID) const = 0;
    virtual tSigned32 heightAtPosition(const cPosition& position) const = 0;
    virtual float heightAtPositionF(const cPosition& position) const = 0;
    virtual float heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const = 0;
    virtual tSigned32 getCellForEndOfLine(const cPosition& start, tSigned32 endX, tSigned32 endY) const = 0;
    virtual bool positionIsValid(const cPosition& p) const = 0;
    virtual void burnContextIntoMesh(iCollisionContext* context) = 0;
    virtual void saveGround(const char* format, bool includeMapping, iOutputStream* outputStream) const = 0;
    virtual void setTerrainCallBack(tSigned32 terrainLayer, iTerrainCallBack* callBack) = 0;
    virtual tSigned32 addEndPoint(const cPosition& position) = 0;
    virtual tSigned32 addOffMeshConnection(tSigned32 fromEndPoint, tSigned32 toEndPoint, tSigned32 penalty) = 0;
    virtual tSigned32 getNumberOfEndPoints() const = 0;
    virtual cPosition getEndPoint(tSigned32 index) const = 0;
    virtual tSigned32 getNumberOfOffMeshConnections() const = 0;
    virtual void getOffMeshConnectionInfo(tSigned32 index, tSigned32& fromEndPoint, tSigned32& toEndPoint, tSigned32& penalty) const = 0;
    virtual void clearOffMeshConnectionsAndEndPoints() = 0;
    virtual void generateCollisionPreprocessFor(iShape* shape, const char *const* attributes) = 0;
    virtual void generatePathfindPreprocessFor(iShape* shape, const char *const* attributes) = 0;
    virtual void releaseAllPreprocessFor(iShape* shape) = 0;
    virtual void releasePathfindPreprocessFor(iShape* shape) = 0;
    virtual void preprocessGenerationCompleted() = 0;
    virtual bool shapeCanCollide(const iShape* shape) const = 0;
    virtual bool shapeCanPathfind(const iShape* shape) const = 0;
    virtual const char *const* getCollisionPreprocessAttributes(iShape* shape) const = 0;
    virtual const char *const* getPathfindPreprocessAttributes(iShape* shape) const = 0;
    virtual void saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const = 0;
    virtual void savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const = 0;
    virtual void loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize) = 0;
    virtual void loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize) = 0;
    virtual iAgent* placeAgent(iShape* shape, const cPosition& position) const = 0;
    virtual iCollisionContext* newContext() const = 0;
    virtual iObstacleSet* newObstacleSet() const = 0;
    virtual bool testPointCollision(iShape* shape, iCollisionContext* context, const cPosition& position) const = 0;
    virtual bool testLineCollision(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& end) const = 0;
    virtual bool testLineCollision_XY(iShape* shape, iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const = 0;
    virtual iCollisionInfo* firstCollision(iShape* shape, iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const = 0;
    virtual cPosition findClosestUnobstructedPosition(iShape* shape, iCollisionContext* context, const cPosition& position, tSigned32 maximumDistance) const = 0;
    virtual void getAllAgentsOverlapped(iShape* shape, iCollisionContext* context, const cPosition& position, iAgent** resultsBuffer, tSigned32& numberOverlapped) const = 0;
    virtual iPath* findShortestPath(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal) const = 0;
    virtual iPath* findShortestPath_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack* queryCallBack) const = 0;
    virtual iPath* findPathAway(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway) const = 0;
    virtual iPath* findPathAway_WithQueryCallBack(iShape* shape, iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const = 0;
    virtual iPath* generateCurvedPath(iShape* shape, iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const = 0;
    virtual iPath* constructPath(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const = 0;
    virtual iPath* constructPath_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const = 0;
    virtual void savePath(iPath* path, iOutputStream* os) const = 0;
    virtual iPath* loadPath(const char* dataBuffer, tUnsigned32 bufferSize) const = 0;
    virtual void renderLineOnGround(const cPosition& start, tSigned32 endX, tSigned32 endY, tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const = 0;
    virtual iObstacleSet* newObstacleSet_WithAttributes(const char *const* attributes) const = 0;
    virtual cPosition positionInSectionInShape(tSigned32 sectionID, tSigned32 shapeOriginX, tSigned32 shapeOriginY, tSigned32 numberOfPoints, const tSigned32* coordinateData) const = 0;
    virtual iPath* generateCurvedPath_WithEndVector(iShape* shape, iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const = 0;
    virtual iAgent* placeLargeStaticObstacle(tSigned32 numberOfPoints, const tSigned32* coordinateData, const cPosition& position) const = 0;
    virtual void addAnchorsAndShapes(const iAnchorsAndPinnedShapes* anchorsAndShapes, const char* idPrefix, tSigned32 numberOfTerrainLayers) = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iMesh* afterCast = static_cast<iMesh*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iAgent
{
public:
    virtual void destroy() = 0;
    virtual iMesh* getMesh() const = 0;
    virtual iShape* getShape() const = 0;
    virtual void setUserData(void* voidPointer) = 0;
    virtual void* getUserData() const = 0;
    virtual cPosition getPosition() const = 0;
    virtual void moveTo(const cPosition& newPosition) = 0;
    virtual bool canCollide() const = 0;
    virtual bool canPathfind() const = 0;
    virtual bool testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const = 0;
    virtual bool testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const = 0;
    virtual bool testCollisionTo_XY(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const = 0;
    virtual iCollisionInfo* firstCollisionTo(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const = 0;
    virtual cPosition findClosestUnobstructedPosition(iCollisionContext* context, tSigned32 maximumDistance) const = 0;
    virtual void getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, tSigned32& numberOverlapped) const = 0;
    virtual bool testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const = 0;
    virtual iPath* findShortestPathTo(iCollisionContext* context, const cPosition& target) const = 0;
    virtual iPath* findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const = 0;
    virtual iPath* findPathAway(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway) const = 0;
    virtual iPath* findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const = 0;
    virtual iCollisionInfo* advanceAlongPath(iPath* path, float distance, iCollisionContext* context) = 0;
    virtual iCollisionInfo* advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY) = 0;
    virtual iPath* generateCurvedPath(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const = 0;
    virtual void setTraverseCost(float cost) = 0;
    virtual void setTraverseCostDirection(tSigned32 directionVectorX, tSigned32 directionVectorY) = 0;
    virtual float getTraverseCost() const = 0;
    virtual void getTraverseCostDirection(tSigned32& directionVectorX, tSigned32& directionVectorY) const = 0;
    virtual iPath* generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const = 0;
    virtual bool isMoveable() const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iAgent* afterCast = static_cast<iAgent*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iCollisionContext
{
public:
    virtual void destroy() = 0;
    virtual void addAgent(iAgent* agent) = 0;
    virtual void removeAgent(iAgent* agent) = 0;
    virtual tSigned32 getNumberOfAgents() const = 0;
    virtual iAgent* getAgent(tSigned32 index) const = 0;
    virtual bool includes(iAgent* agent) const = 0;
    virtual void addObstacleSet(iObstacleSet* context) = 0;
    virtual void removeObstacleSet(iObstacleSet* context) = 0;
    virtual tSigned32 getNumberOfObstacleSets() const = 0;
    virtual iObstacleSet* getObstacleSet(tSigned32 index) const = 0;
    virtual void updateCollisionPreprocessFor(iShape* shape) const = 0;
    virtual void updatePathfindingPreprocessFor(iShape* shape) const = 0;
    virtual void setSurfaceTypeTraverseCost(tSigned32 surfaceType, float cost) = 0;
    virtual void setSurfaceTypeTraverseCostDirection(tSigned32 surfaceType, tSigned32 directionVectorX, tSigned32 directionVectorY) = 0;
    virtual void setQueryBounds(const cHorizontalRange& bounds) = 0;
    virtual void clearQueryBounds() = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iCollisionContext* afterCast = static_cast<iCollisionContext*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iObstacleSet
{
public:
    virtual void destroy() = 0;
    virtual void addAgent(iAgent* agent) = 0;
    virtual void removeAgent(iAgent* agent) = 0;
    virtual tSigned32 getNumberOfAgents() const = 0;
    virtual iAgent* getAgent(tSigned32 index) const = 0;
    virtual bool includes(iAgent* agent) const = 0;
    virtual tSigned32 size() const = 0;
    virtual bool empty() const = 0;
    virtual void updateCollisionPreprocessFor(iShape* shape) const = 0;
    virtual void updatePathfindingPreprocessFor(iShape* shape) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iObstacleSet* afterCast = static_cast<iObstacleSet*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iPath
{
public:
    virtual void destroy() = 0;
    virtual tSigned32 size() const = 0;
    virtual cPosition position(tSigned32 pointIndex) const = 0;
    virtual tSigned32 connectionIndex(tSigned32 segmentIndex) const = 0;
    virtual const cPosition* getPositionArray() const = 0;
    virtual const tSigned32* getConnectionIndexArray() const = 0;
    virtual tUnsigned32 getLength() const = 0;
    virtual iCollisionInfo* advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY) = 0;
    virtual void renderOnGround(tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iPath* afterCast = static_cast<iPath*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iCollisionInfo
{
public:
    virtual void destroy() = 0;
    virtual void getCollidingLine(tSigned32* coords) const = 0;
    virtual iAgent* getCollidingAgent() const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iCollisionInfo* afterCast = static_cast<iCollisionInfo*>(voidPointer);
            afterCast->destroy();
        }
    }
};

#ifndef I_ERRORHANDLER_HAS_BEEN_INCLUDED
#define I_ERRORHANDLER_HAS_BEEN_INCLUDED
class iErrorHandler
{
public:
    virtual ~iErrorHandler() {}
    enum eAction
    {
        CONTINUE,
        ABORT,
        BREAK,
        CONTINUE_IGNORE_IN_FUTURE,
    };
    virtual eAction handle(const char* type, const char* description, const char *const* attributes) = 0;
};
#endif

#ifndef I_OUTPUTSTREAM_HAS_BEEN_INCLUDED
#define I_OUTPUTSTREAM_HAS_BEEN_INCLUDED
class iOutputStream
{
public:
    virtual ~iOutputStream() {}
    virtual void put(const char* data, tUnsigned32 dataSize) = 0;
};
class cNullOutputStream : public iOutputStream
{
public:
    void put(const char* /*data*/, tUnsigned32 /*dataSize*/) {}
};
#endif

class iTerrainCallBack
{
public:
    virtual ~iTerrainCallBack() {}
    virtual float getHeightAt(tSigned32 x, tSigned32 y) = 0;
    virtual float getHeightAtWithPrecision(tSigned32 x, tSigned32 y, float precisionX, float precisionY) = 0;
};

class iRender3DLinesCallBack
{
public:
    virtual ~iRender3DLinesCallBack() {}
    virtual void startVertex(float x, float y, float z) = 0;
    virtual void vertex(float x, float y, float z) = 0;
};

static const tSigned32 PE_FaceAttribute_SurfaceType = 0;
static const tSigned32 PE_FaceAttribute_SectionID = 1;
static const tSigned32 PE_FaceAttribute_MarksPortal = 2;
static const tSigned32 PE_FaceAttribute_MarksExternalShape = 3;
static const tSigned32 PE_FaceAttribute_MarksConnectingEdge = 4;
static const tSigned32 PE_FaceAttribute_UserData = 5;
static const tSigned32 PE_FaceAttribute_MarksTerrainPortal = 6;
static const tSigned32 PE_FaceAttribute_Top = 6;
class iFaceVertexMesh
{
public:
    virtual ~iFaceVertexMesh() {}
    virtual tSigned32 faces() const = 0;
    virtual tSigned32 vertices() const = 0;
    virtual tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const = 0;
    virtual tSigned32 vertexX(tSigned32 vertex) const = 0;
    virtual tSigned32 vertexY(tSigned32 vertex) const = 0;
    virtual float vertexZ(tSigned32 vertex) const = 0;
    virtual tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const = 0;
};

static const tSigned32 PE_AnchorAttribute_ResolvesToTerrain = 0;
static const tSigned32 PE_AnchorAttribute_Top = 0;
static const tSigned32 PE_PinnedShapeAttribute_Type = 0;
static const tSigned32 PE_PinnedShapeAttribute_Top = 0;
static const tSigned32 PE_PinnedShapeType_BurntIn = 0;
static const tSigned32 PE_PinnedShapeType_NamedObstacle = 1;
static const tSigned32 PE_PinnedShapeType_Top = 1;
class iAnchorsAndPinnedShapes
{
public:
    virtual ~iAnchorsAndPinnedShapes() {}

    virtual tSigned32 numberOfAnchors() const = 0;
    virtual const char* anchor_Name(tSigned32 anchorIndex) const = 0;
    virtual void anchor_HorizontalPosition(tSigned32 anchorIndex, tSigned32& x, tSigned32& y) const = 0;
    virtual void anchor_VerticalRange(tSigned32 anchorIndex, float& zStart, float& zEnd) const = 0;
    virtual tSigned32 anchor_Orientation(tSigned32 anchorIndex) const = 0;
    virtual tSigned32 anchor_Attribute(tSigned32 anchorIndex, tSigned32 attributeIndex) const = 0;

    virtual tSigned32 numberOfPinnedShapes() const = 0;
    virtual const char* pinnedShape_Name(tSigned32 shapeIndex) const = 0;
    virtual tSigned32 pinnedShape_AnchorIndex(tSigned32 shapeIndex) const = 0;
    virtual tSigned32 pinnedShape_NumberOfVertices(tSigned32 shapeIndex) const = 0;
    virtual void pinnedShape_Vertex(tSigned32 shapeIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const = 0;
    virtual tSigned32 pinnedShape_Attribute(tSigned32 shapeIndex, tSigned32 attributeIndex) const = 0;
};

class iSolidObjects
{
public:
    virtual ~iSolidObjects() {}
    virtual tSigned32 numberOfConvexSolids() const = 0;
    virtual tSigned32 convexSolid_Points(tSigned32 convexSolidIndex) const = 0;
    virtual void convexSolid_Point(tSigned32 convexSolidIndex, tSigned32 pointIndex, tSigned32& x, tSigned32& y, tSigned32& z) const = 0;
    virtual tSigned32 convexSolid_Attribute(tSigned32 convexSolidIndex, tSigned32 attributeIndex) const = 0;
};

class iQueryCallBack
{
public:
    virtual ~iQueryCallBack() {}
    virtual tSigned32 desiredCallBackFrequency() = 0;
    virtual bool abort() = 0;
};

class iContentChunk
{
public:
    virtual void destroy() = 0;
    virtual bool preValidate(tSigned32 maximumScale, tSigned32 translationRange) = 0;
    virtual void save(iOutputStream* outputStream) const = 0;
    virtual iContentChunkInstance* instantiate(tSigned32 rotation, tSigned32 scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID) = 0;
    virtual iContentChunkInstance* instantiate_FloatRotateAndScale(float rotation, float scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID) = 0;
    virtual void deleteAllInstances() = 0;
    virtual bool hasGroundComponent() const = 0;
    virtual bool hasAnchorsAndShapesComponent() const = 0;
    virtual tSigned32 numberOfConnectingEdges() const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iContentChunk* afterCast = static_cast<iContentChunk*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iContentChunkInstance
{
public:
    virtual void destroy() = 0;
    virtual iContentChunk* getContentChunk() const = 0;
    virtual const iFaceVertexMesh* getGroundPointer() const = 0;
    virtual const iAnchorsAndPinnedShapes* getAnchorsAndShapesPointer() const = 0;
    virtual void addAnchorsAndShapes(iMesh* mesh, const char* idPrefix, tSigned32 numberOfTerrainLayers) const = 0;
    virtual void getHorizontalRange(cHorizontalRange& result) const = 0;
    virtual tSigned32 numberOfConnectingEdges() const = 0;
    virtual void getConnectingEdgeGeometry(tSigned32 connectingEdgeIndex, tSigned32& startX, tSigned32& startY, float& startZ, tSigned32& endX, tSigned32& endY, float& endZ) const = 0;
    virtual bool edgesCanConnect(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const = 0;
    virtual void makeEdgeConnection(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iContentChunkInstance* afterCast = static_cast<iContentChunkInstance*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iMeshFederation
{
public:
    virtual void destroy() = 0;
    virtual void save(const char* format, iOutputStream* outputStream) const = 0;
    virtual tSigned32 size() const = 0;
    virtual bool coordinatesAreInsideRange(tSigned32 x, tSigned32 y) const = 0;
    virtual tSigned32 tileForQuery(tSigned32 queryStartX, tSigned32 queryStartY) const = 0;
    virtual void getTileCentre(tSigned32 tileIndex, tSigned32& centreX, tSigned32& centreY) const = 0;
    virtual void getHandledRegion_World(tSigned32 tileIndex, cHorizontalRange& result) const = 0;
    virtual void getRepresentedRegion_World(tSigned32 tileIndex, cHorizontalRange& result) const = 0;
    virtual void getHandledRegion_Local(tSigned32 tileIndex, cHorizontalRange& result) const = 0;
    virtual void getRepresentedRegion_Local(tSigned32 tileIndex, cHorizontalRange& result) const = 0;
    virtual cPosition translatePosition(tSigned32 fromTileIndex, const iMesh* fromMesh, tSigned32 toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const = 0;
    virtual void getRepresentedRegionsOverlapped(const cHorizontalRange& queryRegion, tSigned32* resultsBuffer, tSigned32& numberOverlapped) const = 0;
    virtual iMesh* buildTileMeshFromContent(tSigned32 tileIndex, const iContentChunkInstance* const* instancePointers, tSigned32 numberOfInstances, const char *const* options, iOutputStream* os) = 0;
    void operator delete(void* voidPointer)
    {
        if(voidPointer)
        {
            iMeshFederation* afterCast = static_cast<iMeshFederation*>(voidPointer);
            afterCast->destroy();
        }
    }
};

class iMeshFederationTileGeneratedCallBack
{
public:
    virtual ~iMeshFederationTileGeneratedCallBack() {}
    virtual void tileGenerated(tSigned32 tileIndex, iMesh* tileMesh) = 0;
};

class iProgressCallBack
{
public:
    virtual ~iProgressCallBack() {}
    virtual void updateProgress(const char* operationDescription, float percent) = 0;    
    void
    updateProgressL(const char* operationDescription, tSigned32 i, tSigned32 totalI)
    {
        updateProgress(operationDescription, static_cast<float>(i) / static_cast<float>(totalI));
    }
};

#endif // ends file include guard
