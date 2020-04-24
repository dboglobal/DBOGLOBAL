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
#include "interface/cAgent.h"
#include "interface/Singletons.h"
#include "interface/cPathEngine.h"
#include "interface/cPath.h"
#include "interface/cObstacleSet.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h"
#include "libs/PathEngine_Core/interface/PointIsInsideCircuits.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Geometry/interface/Bounds.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Error.h"
#include "common/Containers/SwapOut.h"
#include "common/MutexLock.h"
#include "iAPIObjectWrapper.h"

void
cAgent::setPosition(const cPosition& value)
{
    assertD(_shape.isSet());
    _position = value;
    _mesh->convertPosition(_position, _internalPosition);
//    _placedShape = _shape->refCS() + _internalPosition.p;
    _shape->refCS().offset(_internalPosition.p, _placedShape);
}

cAgent::cAgent(cMesh* mesh, cShape* shape, const cPosition& position) :
 _directionForCostToTraverse(0,0,0)
{
    _wrapper = 0;
    _mesh = mesh;
    _userData = 0;
    _indexInMesh = -1;
    _shape = shape;
    _shape->refCS().getBounds(_shapeBoundsMin, _shapeBoundsMax);
    setPosition(position);
    _costToTraverse = -1;
}

cAgent::cAgent(cMesh* mesh, const cConvexPoly& largeStaticShape, const cPosition& rootPosition) :
 _directionForCostToTraverse(0,0,0)
{
    _wrapper = 0;
    _mesh = mesh;
    _userData = 0;
    _indexInMesh = -1;
    _placedShape = largeStaticShape;
    _placedShape.getBounds(_shapeBoundsMin, _shapeBoundsMax);
//    setPosition(rootPosition);
    _position = rootPosition;
    _mesh->convertPosition(_position, _internalPosition);
    _costToTraverse = -1;
}

cAgent::~cAgent()
{
    assertD(_indexInMesh == -1);
    tSigned32 i;
    for(i = 0; i < SizeL(_indexInObstacleSetVector); i++)
    {
        if(_indexInObstacleSetVector[i] == -1)
        {
            continue;
        }
        cObstacleSet& os = _mesh->refObstacleSet(i);
        os.removeAgent(this);
        assertD(_indexInObstacleSetVector[i] == -1);
    }
}

// iAgent

void
cAgent::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    _mesh->notifyAgentDestroyed(_indexInMesh);
    _indexInMesh = -1;
    delete this;
}

iMesh*
cAgent::getMesh() const
{
    return _mesh;
}
iShape*
cAgent::getShape() const
{
    return _shape.get();
}

void
cAgent::setUserData(void* void_ptr)
{
    cMutexLock lock(_stateMutex);
    _userData = void_ptr;
}
void*
cAgent::getUserData() const
{
    cMutexLock lock(_stateMutex);
    return _userData;
}

cPosition
cAgent::getPosition() const
{
    return _position;
}

void
cAgent::moveTo(const cPosition& newPosition)
{
    INTERFACE_ASSERT(isMoveable());
	INTERFACE_CHECK(_mesh->positionIsValid(newPosition), "iAgent::moveTo() - invalid position");

    assertD(newPosition.cell != -1);
    {
        cMutexLock lock(_stateMutex);
        setPosition(newPosition);
    }

// notify all contexts containing this agent
    {
        _mesh->lockAgentsAndObstacleSets();
        tSigned32 i;
        for(i = 0; i < SizeL(_indexInObstacleSetVector); i++)
        {
            if(_indexInObstacleSetVector[i] != -1)
            {
                if(isSoftObstacle())
                {
                    _mesh->refObstacleSet(i).notifyAgentChangeAffectingPathfindingStateOnly();
                }
                else
                {
                    _mesh->refObstacleSet(i).notifyAgentChangeAffectingCollisionAndPathfindingState();
                }
            }
        }
        _mesh->unlockAgentsAndObstacleSets();
    }
}

bool
cAgent::canCollide() const
{
    return _shape.isSet() && _mesh->shapeCanCollide(_shape.get());
}
bool
cAgent::canPathfind() const
{
    return _shape.isSet() && _mesh->shapeCanPathfind(_shape.get());
}

bool
cAgent::testCollisionAt(iCollisionContext* context, const cPosition& position) const
{
    INTERFACE_ASSERT(isMoveable());
    INTERFACE_CHECK(_mesh->positionIsValid(position),"invalid position passed in to iAgent::testCollisionAt()");
    return _mesh->testPointCollision_IgnoreOneAgent(_shape.get(), context, position, this);
    {
        return true;
    }
}

bool
cAgent::testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const
{
    INTERFACE_ASSERT(isMoveable());
	INTERFACE_CHECK(_mesh->positionIsValid(newPosition), "iAgent::testCollisionTo() - invalid position");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->testLineCollision_IgnoreOneAgent(_shape.get(), context, positionCopy, newPosition, this);
}
bool
cAgent::testCollisionTo_XY(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32 &cell) const
{
    INTERFACE_ASSERT(isMoveable());
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->testLineCollision_XY_IgnoreOneAgent(_shape.get(), context, positionCopy, x, y, cell, this);
}

iCollisionInfo*
cAgent::firstCollisionTo(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32 &cell) const
{
    INTERFACE_CHECK(canCollide(), "iAgent::firstCollisionTo() cannot be called on this agent - agent is non-moveable or collision preprocess has not been generated for this agent's shape");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->firstCollision(_shape.get(), context, positionCopy, x, y, cell);
}

cPosition
cAgent::findClosestUnobstructedPosition(iCollisionContext* context, tSigned32 maximumdistance) const
{
    INTERFACE_CHECK(canCollide(), "iAgent::findClosestUnobstructedPoint() cannot be called on this agent - agent is non-moveable or collision preprocess has not been generated for this agent's shape");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->findClosestUnobstructedPosition(_shape.get(), context, positionCopy, maximumdistance);
}

void
cAgent::getAllAgentsOverlapped(
                        iCollisionContext* context,
                        iAgent** results_buffer,
                        tSigned32& number_overlapped
                        ) const
{
    INTERFACE_ASSERT(isMoveable());
    cMutexLock lock(_stateMutex);
    _mesh->getAllAgentsOverlapped(_shape.get(), context, _position, results_buffer, number_overlapped);
}

bool
cAgent::testCollisionDirectlyAgainstPlacedShape(iShape* shapeI, const cPosition& shapePosition) const
{
    INTERFACE_ASSERT(isMoveable());
    cShape* shapeC = static_cast<cShape*>(shapeI);

    cInternalPosition p;
    _mesh->convertPosition(shapePosition, p);
    std::list<tFace> containedExternalFaces;
    tMeshCircuits expandedCircuits;

    cConvexPoly placedShape;
    shapeC->refCS().offset(p.p, placedShape);
    cConvexPoly combinedShape;
    placedShape.minkowskiExpand(_shape->refCS(), combinedShape);
    CircuitsForOverlappingPolygon(
            p.f,
            combinedShape,
            containedExternalFaces,
            expandedCircuits
            );

    cInternalPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _internalPosition;
    }
    return PointIsInsideCircuits(expandedCircuits, containedExternalFaces, positionCopy);
}

iPath*
cAgent::findShortestPathTo(iCollisionContext* context, const cPosition& target) const
{
    INTERFACE_CHECK(canPathfind(),"iAgent::findShortestPathTo() cannot be called on this agent - agent is non-moveable or pathfind preprocess has not been generated for this agent's shape");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->findShortestPath(_shape.get(), context, positionCopy, target);
}
iPath*
cAgent::findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const
{
    INTERFACE_CHECK(canPathfind(),"iAgent::findShortestPathTo_WithQueryCallBack() cannot be called on this agent - agent is non-moveable or pathfind preprocess has not been generated for this agent's shape");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->findShortestPath_WithQueryCallBack(_shape.get(), context, positionCopy, target, queryCallBack);
}

iPath*
cAgent::findPathAway(iCollisionContext* context, const cPosition& awayfrom, tSigned32 distanceaway) const
{
    INTERFACE_CHECK(canPathfind(),"iAgent::findPathAway() cannot be called on this agent - agent is non-moveable or pathfind preprocess has not been generated for this agent's shape");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->findPathAway(_shape.get(), context, positionCopy, awayfrom, distanceaway);
}
iPath*
cAgent::findPathAway_WithQueryCallBack(
        iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack
        ) const
{
    INTERFACE_CHECK(canPathfind(),"iAgent::findPathAway_WithQueryCallBack() cannot be called on this agent - agent is non-moveable or pathfind preprocess has not been generated for this agent's shape");
    cPosition positionCopy;
    {
        cMutexLock lock(_stateMutex);
        positionCopy = _position;
    }
    return _mesh->findPathAway_WithQueryCallBack(_shape.get(), context, positionCopy, awayFrom, distanceAway, queryCallBack);
}

iCollisionInfo*
cAgent::advanceAlongPath(iPath* path, float distance, iCollisionContext* context)
{
    float precisionX = 0.0f;
    float precisionY = 0.0f;
    return advanceAlongPathWithPrecision(path, distance, context, precisionX, precisionY);
}

iCollisionInfo*
cAgent::advanceAlongPathWithPrecision(
        iPath* path, float distance, iCollisionContext* context,
        float& precisionX, float& precisionY
        )
{
    INTERFACE_ASSERT(isMoveable());
    if(!path)
    {
        return 0;
    }
    cMutexLock lock(_stateMutex);
    cPath* pathC = static_cast<cPath*>(path);
    iCollisionInfo* info = pathC->advanceAlong(_shape.get(), distance, context, precisionX, precisionY);
    if(getPosition() != pathC->position(0))
    {
        moveTo(pathC->position(0));
    }
    return info;
}

iPath*
cAgent::generateCurvedPath(
        iPath* basePath, 
        iCollisionContext* context,
        tSigned32 startVectorX, tSigned32 startVectorY,
        tSigned32 sectionLength,
        float turnRatio1, float turnRatio2
        ) const
{
    return _mesh->generateCurvedPath_WithEndVector(
            _shape.get(), basePath, context,
            startVectorX, startVectorY,
            0, 0,
            sectionLength,
            turnRatio1, turnRatio2
            );
}

void
cAgent::setTraverseCost(float cost)
{
    INTERFACE_CHECK(cost >= 0 || cost == -1.f, "iAgent::setCostToTraverse() called with invalid value for cost.");
    cMutexLock lock(_stateMutex);
    if(cost == _costToTraverse)
    {
        return;
    }
    float costBefore = _costToTraverse;
    _costToTraverse = cost;
// notify all contexts containing this agent
    {
        _mesh->lockAgentsAndObstacleSets();
        tSigned32 i;
        for(i = 0; i < SizeL(_indexInObstacleSetVector); i++)
        {
            if(_indexInObstacleSetVector[i] != -1)
            {
                if(cost == -1 || costBefore == -1)
                {
                    _mesh->refObstacleSet(i).notifyAgentChangeAffectingCollisionAndPathfindingState();
                }
                else
                {
                    _mesh->refObstacleSet(i).notifyAgentChangeAffectingPathfindingStateOnly();
                }
            }
        }
        _mesh->unlockAgentsAndObstacleSets();
    }
}

void
cAgent::setTraverseCostDirection(tSigned32 directionVectorX, tSigned32 directionVectorY)
{
    INTERFACE_CHECK(directionVectorX >= -DIRECTION_VECTOR_RANGE && directionVectorX <= DIRECTION_VECTOR_RANGE, "iAgent::setDirectionForCostToTraverse() called with vector x component outside allowed range.");
    INTERFACE_CHECK(directionVectorY >= -DIRECTION_VECTOR_RANGE && directionVectorY <= DIRECTION_VECTOR_RANGE, "iAgent::setDirectionForCostToTraverse() called with vector y component outside allowed range.");
    cMutexLock lock(_stateMutex);
    tPoint newDirection = tPoint(directionVectorX, directionVectorY, DIRECTION_VECTOR_RANGE);
    if(newDirection == _directionForCostToTraverse)
    {
        return;
    }
    _directionForCostToTraverse = newDirection;
    if(isSoftObstacle())
    {
        _mesh->lockAgentsAndObstacleSets();
        tSigned32 i;
        for(i = 0; i < SizeL(_indexInObstacleSetVector); i++)
        {
            if(_indexInObstacleSetVector[i] != -1)
            {
                _mesh->refObstacleSet(i).notifyAgentChangeAffectingPathfindingStateOnly();
            }
        }
        _mesh->unlockAgentsAndObstacleSets();
    }
}

float
cAgent::getTraverseCost() const
{
    cMutexLock lock(_stateMutex);
    return _costToTraverse;
}
void
cAgent::getTraverseCostDirection(tSigned32& directionVectorX, tSigned32& directionVectorY) const
{
    cMutexLock lock(_stateMutex);
    directionVectorX = _directionForCostToTraverse.getX();
    directionVectorY = _directionForCostToTraverse.getY();
}

iPath*
cAgent::generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const
{
    INTERFACE_ASSERT(isMoveable());
    return _mesh->generateCurvedPath_WithEndVector(
        _shape.get(), basePath, context,
        startVectorX, startVectorY,
        endVectorX, endVectorY,
        sectionLength,
        turnRatio1, turnRatio2
        );
}

bool
cAgent::isMoveable() const
{
    return _shape.isSet();
}

//=================

bool
cAgent::isSoftObstacle() const
{
    PROFILER_SCOPE("cAgent::isSoftObstacle");
    cMutexLock lock(_stateMutex);
    return _costToTraverse >= 0.f;
}
float
cAgent::getCostToTraverse() const
{
    cMutexLock lock(_stateMutex);
    return _costToTraverse;
}
tPoint
cAgent::getDirectionForCostToTraverse() const
{
    cMutexLock lock(_stateMutex);
    return _directionForCostToTraverse;
}

void
cAgent::notifyAddedToObstacleSet(tSigned32 setIndex, tSigned32 indexInObstacleSet)
{
    if(setIndex >= SizeL(_indexInObstacleSetVector))
    {
        _indexInObstacleSetVector.resize(setIndex + 1, -1);
    }
    assertD(_indexInObstacleSetVector[setIndex] == -1);
    _indexInObstacleSetVector[setIndex] = indexInObstacleSet;
}
tSigned32
cAgent::notifyRemovedFromObstacleSet(tSigned32 setIndex)
{
    assertD(setIndex >= 0 && setIndex < SizeL(_indexInObstacleSetVector));
    assertD(_indexInObstacleSetVector[setIndex] != -1);
    tSigned32 result = _indexInObstacleSetVector[setIndex];
    _indexInObstacleSetVector[setIndex] = -1;
    return result;
}
void
cAgent::notifyIndexInObstacleSetChanged(tSigned32 setIndex, tSigned32 newIndexInObstacleSet)
{
    assertD(setIndex >= 0 && setIndex < SizeL(_indexInObstacleSetVector));
    assertD(_indexInObstacleSetVector[setIndex] != -1);
    _indexInObstacleSetVector[setIndex] = newIndexInObstacleSet;
}
bool
cAgent::isInObstacleSet(tSigned32 index)
{
    if(index >= SizeL(_indexInObstacleSetVector))
    {
        return false;
    }
    return _indexInObstacleSetVector[index] != -1;
}

cHandle<cShapeCollisionPreprocess>
cAgent::getUnexpandedPreprocess(cQueryContext& qc)
{
    cMutexLock lock(_stateMutex);
    if(!_unexpandedPreprocess.isSet())
    {
        _unexpandedPreprocess = new cShapeCollisionPreprocess();
        _unexpandedPreprocess->initOrReinit(
                qc,
                _placedShape,
                _internalPosition
                );
        return _unexpandedPreprocess;
    }
    if(_unexpandedPreprocess->isValidFor(_internalPosition))
    {
        return _unexpandedPreprocess;
    }
    if(_unexpandedPreprocess->hasMoreThanOneRef())
    {
        _unexpandedPreprocess = new cShapeCollisionPreprocess();
        _unexpandedPreprocess->initOrReinit(
                qc,
                _placedShape,
                _internalPosition
                );
    }
    else
    {
        _unexpandedPreprocess->initOrReinit(
                qc,
                _placedShape,
                _internalPosition
                );
    }
    return _unexpandedPreprocess;
}


cHandle<cShapeCollisionPreprocess>
cAgent::getCollisionPreprocess(cQueryContext& qc, tSigned32 expandShape)
{
    PROFILER_SCOPE("cAgent::getCollisionPreprocess");
    cMutexLock lock(_stateMutex);
    _collisionPreprocess.resize(nSingletons::pathEngine().numberOfPreprocessShapes(), 0);
    cHandle<cShapeCollisionPreprocess> cpHandle = _collisionPreprocess[expandShape];
    if(!cpHandle.isSet())
    {
        _collisionPreprocess[expandShape] = new cShapeCollisionPreprocess();
        _collisionPreprocess[expandShape]->initOrReinit(
                qc,
                _placedShape,
                _internalPosition,
                nSingletons::pathEngine().refPreprocessShape(expandShape)
                );
        return _collisionPreprocess[expandShape];
    }
    if(cpHandle->isValidFor(_internalPosition))
    {
        return cpHandle;
    }
    if(cpHandle->hasMoreThanOneRef())
    {
        _collisionPreprocess[expandShape] = new cShapeCollisionPreprocess();
        _collisionPreprocess[expandShape]->initOrReinit(
                qc,
                _placedShape,
                _internalPosition,
                nSingletons::pathEngine().refPreprocessShape(expandShape)
                );
    }
    else
    {
        cpHandle->initOrReinit(
                qc,
                _placedShape,
                _internalPosition,
                nSingletons::pathEngine().refPreprocessShape(expandShape)
                );
    }
    return _collisionPreprocess[expandShape];
}

cHandle<cShapePathfindPreprocess>
cAgent::getPathfindPreprocess(
        cQueryContext& qc,
        const cPathfindPreprocess& pp,
        tSigned32 expandShape
        )
{
    cMutexLock lock(_stateMutex);
    _pathfindPreprocess.resize(nSingletons::pathEngine().numberOfPreprocessShapes(), 0);
    if(!_pathfindPreprocess[expandShape].isSet())
    {
        _pathfindPreprocess[expandShape] = new cShapePathfindPreprocess();
        _pathfindPreprocess[expandShape]->initOrReinit(
                _internalPosition,
                pp,
                *getCollisionPreprocess(qc, expandShape)
                );
        return _pathfindPreprocess[expandShape];
    }
    if(_pathfindPreprocess[expandShape]->isValidFor(_internalPosition))
    {
        return _pathfindPreprocess[expandShape];
    }
    if(_pathfindPreprocess[expandShape]->hasMoreThanOneRef())
    {
        _pathfindPreprocess[expandShape] = new cShapePathfindPreprocess();
        _pathfindPreprocess[expandShape]->initOrReinit(
                _internalPosition,
                pp,
                *getCollisionPreprocess(qc, expandShape)
                );
    }
    else
    {
        _pathfindPreprocess[expandShape]->initOrReinit(
                _internalPosition,
                pp,
                *getCollisionPreprocess(qc, expandShape)
                );
    }
    return _pathfindPreprocess[expandShape];
}
cHandle<cShapeRenderPreprocess>
cAgent::getRenderPreprocess(tSigned32 expandShape)
{
    cMutexLock lock(_stateMutex);
    _renderPreprocess.resize(nSingletons::pathEngine().numberOfPreprocessShapes(), 0);
    if(!_renderPreprocess[expandShape].isSet())
    {
        _renderPreprocess[expandShape] = new cShapeRenderPreprocess();
        _renderPreprocess[expandShape]->initOrReinit(
                _internalPosition,
                _placedShape,
                nSingletons::pathEngine().refPreprocessShape(expandShape)
                );
        return _renderPreprocess[expandShape];
    }
    if(_renderPreprocess[expandShape]->isValidFor(_internalPosition))
    {
        return _renderPreprocess[expandShape];
    }
    if(_renderPreprocess[expandShape]->hasMoreThanOneRef())
    {
        _renderPreprocess[expandShape] = new cShapeRenderPreprocess();
        _renderPreprocess[expandShape]->initOrReinit(
                _internalPosition,
                _placedShape,
                nSingletons::pathEngine().refPreprocessShape(expandShape)
                );
    }
    else
    {
        _renderPreprocess[expandShape]->initOrReinit(
                _internalPosition,
                _placedShape,
                nSingletons::pathEngine().refPreprocessShape(expandShape)
                );
    }
    return _renderPreprocess[expandShape];
}

bool
cAgent::expansionPotentiallyOverlapsBounds(
        tSigned32 expandShape,
        const tPoint& boundsMin,
        const tPoint& boundsMax
        )
{
    cMutexLock lock(_stateMutex);
    tPoint expandShapeMin;
    tPoint expandShapeMax;
    nSingletons::pathEngine().getPreprocessShapeBounds(expandShape, expandShapeMin, expandShapeMax);
    tPoint expandedMin;
    tPoint expandedMax;
    if(_shape.isSet())
    {
        expandedMin = _internalPosition.p + _shapeBoundsMin + expandShapeMin;
        expandedMax = _internalPosition.p + _shapeBoundsMax + expandShapeMax;
    }
    else
    {
        expandedMin = _shapeBoundsMin + expandShapeMin;
        expandedMax = _shapeBoundsMax + expandShapeMax;
    }
    return BoundsOverlap(boundsMin, boundsMax, expandedMin, expandedMax);
}

//const cPlacedConvexShape&
//cAgent::refPlacedShape() const
//{
//    assertD(_largeStaticShape);
//    return *_largeStaticShape;
//}
