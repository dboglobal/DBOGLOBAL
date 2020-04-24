//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_INTERFACE__C_AGENT_INCLUDED
#define PATHENGINE_INTERFACE__C_AGENT_INCLUDED

#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/PathEngine_Core/interface/ShapeCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/ShapePathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/ShapeRenderPreprocess.h"
#include "common/interface/Assert.h"
#include "common/Containers/SwapOutMirror.h"
#include "common/Handle.h"
#include "common/ModifiedStateTracker.h"
#include "platform_common/Mutex.h"
#include "common/STL/vector.h"
#include "i_pathengine.h"

class cMesh;
class cConvexPoly;
class cShapeCollisionPreprocess;
class cShapePathfindPreprocess;
class cPathfindPreprocess;
class iAPIObjectWrapper;

class cAgent : public iAgent
{
  // immutable
    cMesh* _mesh;
    cHandle<cShape> _shape;
    cConvexPoly _placedShape; // agents placed as 'large static obstacles' will have a placed shape, but no cShape ref
    tPoint _shapeBoundsMin, _shapeBoundsMax;
    tSigned32 _indexInMesh;

    cMutex _stateMutex; // applied on external interface to synchronise access to the following state information
    cModifiedStateTracker _stateTracker;
    cPosition _position;
    cInternalPosition _internalPosition;
    void* _userData;
    float _costToTraverse;
    tPoint _directionForCostToTraverse;
    cHandle<cShapeCollisionPreprocess> _unexpandedPreprocess;
    std::vector<cHandle<cShapeCollisionPreprocess> > _collisionPreprocess;
    std::vector<cHandle<cShapePathfindPreprocess> > _pathfindPreprocess;
    std::vector<cHandle<cShapeRenderPreprocess> > _renderPreprocess;

    // protected by iMesh::_agentsAndObstacleSetsMutex
    std::vector<tSigned32> _indexInObstacleSetVector;

    void setPosition(const cPosition& value);

public:

    iAPIObjectWrapper* _wrapper;

    cAgent(cMesh* mesh, cShape* shape, const cPosition& position);
    cAgent(cMesh* mesh, const cConvexPoly& largeStaticShape, const cPosition& rootPosition);
    void setIndexInMesh(tSigned32 value)  // must be called directly post construction
    {
        _indexInMesh = value;
    }

    ~cAgent();

// iAgent

    void destroy();
    iMesh* getMesh() const;
    iShape* getShape() const;
    void setUserData(void* voidPointer);
    void* getUserData() const;
    cPosition getPosition() const;
    void moveTo(const cPosition& newPosition);
    bool canCollide() const;
    bool canPathfind() const;
    bool testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const;
    bool testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const;
    bool testCollisionTo_XY(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const;
    iCollisionInfo* firstCollisionTo(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const;
    cPosition findClosestUnobstructedPosition(iCollisionContext* context, tSigned32 maximumDistance) const;
    void getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, tSigned32& numberOverlapped) const;
    bool testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const;
    iPath* findShortestPathTo(iCollisionContext* context, const cPosition& target) const;
    iPath* findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const;
    iPath* findPathAway(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway) const;
    iPath* findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const;
    iCollisionInfo* advanceAlongPath(iPath* path, float distance, iCollisionContext* context);
    iCollisionInfo* advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY);
    iPath* generateCurvedPath(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const;
    void setTraverseCost(float cost);
    void setTraverseCostDirection(tSigned32 directionVectorX, tSigned32 directionVectorY);
    float getTraverseCost() const;
    void getTraverseCostDirection(tSigned32& directionVectorX, tSigned32& directionVectorY) const;
    iPath* generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const;
    bool isMoveable() const;
    void operator delete(void* p) {::operator delete(p);}

//=================

    bool isSoftObstacle() const;
    float getCostToTraverse() const;
    tPoint getDirectionForCostToTraverse() const;

    // these must only be called under protection of _mesh->_agentsAndObstacleSetsMutex
    void notifyAddedToObstacleSet(tSigned32 setIndex, tSigned32 indexInSet);
    tSigned32 notifyRemovedFromObstacleSet(tSigned32 setIndex);
    void notifyIndexInObstacleSetChanged(tSigned32 setIndex, tSigned32 newIndexInSet);
    bool isInObstacleSet(tSigned32 setIndex);

    cHandle<cShapeCollisionPreprocess> getUnexpandedPreprocess(cQueryContext& qc);
    cHandle<cShapeCollisionPreprocess> getCollisionPreprocess(cQueryContext& qc, tSigned32 expandShape);
    cHandle<cShapePathfindPreprocess> getPathfindPreprocess(cQueryContext& qc, const cPathfindPreprocess& cp, tSigned32 expandShape);
    cHandle<cShapeRenderPreprocess> getRenderPreprocess(tSigned32 expandShape);

    bool
    expansionPotentiallyOverlapsBounds(
            tSigned32 expandShape,
            const tPoint& boundsMin,
            const tPoint& boundsMax
            );

    cShape& refShape()
    {
        assertD(_shape.isSet());
        return* _shape;
    }
//    cHandle<cShape> getShapeHandle()
//    {
//        return _shape;
//    }

    const cConvexPoly&
    refPlacedShape() const
    {
        return _placedShape;
    }

    cMesh& refMesh()
    {
        assertD(_mesh);
        return* _mesh;
    }

    cInternalPosition getInternalPosition() const
    {
        return _internalPosition;
    }

    tSigned32 getIndexInMesh() const
    {
        return _indexInMesh;
    }
};

inline cAgent* FromI(iAgent* ptr)
{
    return static_cast<cAgent*>(ptr);
}

#endif
