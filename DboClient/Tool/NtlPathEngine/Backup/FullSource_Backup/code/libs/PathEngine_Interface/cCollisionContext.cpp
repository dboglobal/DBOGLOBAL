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
#include "interface/cCollisionContext.h"
#include "SurfaceTypeCosts.h"
#include "interface/cAgent.h"
#include "interface/cObstacleSet.h"
#include "libs/PathEngine_Core/interface/ContextCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/CompoundContextCollisionPreprocess.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShapeSet.h"
#include "libs/PathEngine_Core/interface/ContextPathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/CornerProvider.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "interface/cPathEngine.h"
#include "interface/cShape.h"
#include "interface/Singletons.h"
#include "common/MutexLock.h"
#include "common/interface/Error.h"
#include "common/Containers/SwapOut.h"
#include "platform_common/CoordinateRanges.h"
#include "iAPIObjectWrapper.h"

void
cCollisionContext::invalidatePathfindingPreprocess() const
{
    cMutexLock lock(_preprocessMutex);
    if(!_preprocessValid)
    {
        return;
    }
    size_t i;
    for(i = 0; i < _pathfindPreprocess.size(); i++)
    {
        _pathfindPreprocess[i] = 0;
    }
    _preprocessValid = false;
}

void
cCollisionContext::doSwapOutObstacleSet(tSigned32 i) const
{
    if(_preprocessedSet == i)
    {
        _preprocessedSet = -1;
    }
    if(SwapOut(_obstacleSets, i))
    {
        if(_preprocessedSet == SizeL(_obstacleSets))
        {
            _preprocessedSet = i;
        }
    }
    invalidatePathfindingPreprocess();
}
void
cCollisionContext::reapObstacleSets() const
{
    tSigned32 i;
    for(i = 0; i != _obstacleSets.size();)
    {
        if(_obstacleSets[i]->hasBeenDestroyed())
        {
            doSwapOutObstacleSet(i);
            continue;
        }
        ++i;
    }
}

void
cCollisionContext::getObstacleSetStateHandles() const
{
    _obstacleSetStateHandles.resize(_obstacleSets.size());
    tSigned32 i;
    for(i = 0; i != _obstacleSets.size(); ++i)
    {
        _obstacleSetStateHandles[i] = _obstacleSets[i]->getPathfindingStateHandle();
    }
}

bool
cCollisionContext::obstacleSetStateChanged() const
{
    assertD(_preprocessValid);
    assertD(_obstacleSets.size() == _obstacleSetStateHandles.size());
    tSigned32 i;
    for(i = 0; i != _obstacleSets.size(); ++i)
    {
        if(i == _preprocessedSet)
        {
         // this set does not contribute to dynamic pathfinding state
            continue;
        }
        if(_obstacleSets[i]->pathfindingStateHasBeenModifiedSince(_obstacleSetStateHandles[i]))
        {
            return true;
        }
    }
    return false;
}

cCollisionContext::cCollisionContext(cMesh* mesh) :
 _obstacleSets(1),
 _pathfindPreprocess(nSingletons::pathEngine().numberOfPreprocessShapes(), static_cast<cContextPathfindPreprocess*>(0))
{
    _wrapper = 0;
    _surfaceTypeCosts = new cSurfaceTypeCosts(mesh->getSurfaceTypesVector(), mesh->getTopSurfaceType());
    _boundsValid = false;
    _mesh = mesh;
    _indexInMesh = -1;
    _preprocessValid = false;
    _obstacleSets[0] = static_cast<cObstacleSet*>(_mesh->newObstacleSet());
    _preprocessedSet = -1;
}

void
cCollisionContext::setIndexInMesh(tSigned32 value)
{
    assertD(_indexInMesh == -1);
    _indexInMesh = value;
}

cCollisionContext::~cCollisionContext()
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    INTERFACE_ASSERT(_indexInMesh != -1);
    _obstacleSets[0]->destroy();
    delete _surfaceTypeCosts;
    _mesh->notifyContextDestroyed(_indexInMesh);
}

// iCollisionContext

void cCollisionContext::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    delete this;
}

void
cCollisionContext::addAgent(iAgent *agent)
{
    INTERFACE_CHECK(!includes(agent), "iCollisionContext::addAgent() - agent is already in context");
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    _obstacleSets[0]->addAgent(agent);
}

void
cCollisionContext::removeAgent(iAgent *agent)
{
    INTERFACE_CHECK(includes(agent), "iCollisionContext::removeAgent() - agent is not in context");
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    _obstacleSets[0]->removeAgent(agent);
}

tSigned32
cCollisionContext::getNumberOfAgents() const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    return _obstacleSets[0]->size();
}

iAgent* cCollisionContext::getAgent(tSigned32 index) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    return _obstacleSets[0]->getAgent(index);
}

bool cCollisionContext::includes(iAgent *agent) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    return _obstacleSets[0]->includes(agent);
}

void
cCollisionContext::addObstacleSet(iObstacleSet* set)
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    cObstacleSet* c = static_cast<cObstacleSet*>(set);
    if(c->isMarkedForPreprocessing())
    {
        if(_preprocessedSet != -1)
        {
            Error("NonFatal", "iCollisionContext::addObstacleSet - This collision context already has one obstacle set that is marked for preprocessing. "
                "Only one such set is permitted per collision context. "
                "The set will not be added."
                );
            return;
        }
    }
    _preprocessedSet = SizeL(_obstacleSets);
    _obstacleSets.push_back(c);
    invalidatePathfindingPreprocess();
}
void
cCollisionContext::removeObstacleSet(iObstacleSet* set)
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    cObstacleSet* c = static_cast<cObstacleSet*>(set);
    tSigned32 i;
    for(i = 0; i != SizeL(_obstacleSets); ++i)
    {
        if(_obstacleSets[i].get() == c)
        {
            break;
        }
    }
    INTERFACE_CHECK(i != SizeL(_obstacleSets), "iObstacleSet::removeObstacleSet() - The set passed in is not added to this context.");
    assertD(i); // set at index zero is for internal use only
    doSwapOutObstacleSet(i);
}
tSigned32
cCollisionContext::getNumberOfObstacleSets() const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    reapObstacleSets();
    return SizeL(_obstacleSets) - 1;
}
iObstacleSet*
cCollisionContext::getObstacleSet(tSigned32 index) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex()); // to protect the _obstacleSets vector
    reapObstacleSets();
    return _obstacleSets[index + 1].get();
}

void cCollisionContext::updateCollisionPreprocessFor(iShape *shape) const
{
    INTERFACE_ASSERT(_mesh->shapeCanCollide(shape));
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    reapObstacleSets();
    tSigned32 i;
    for(i = 0; i != SizeL(_obstacleSets); ++i)
    {
        _obstacleSets[i]->updateCollisionPreprocessFor(shape);
    }
}

void
cCollisionContext::updatePathfindingPreprocessFor(iShape* shape) const
{
    INTERFACE_ASSERT(_mesh->shapeCanPathfind(shape));
//    updateCollisionPreprocessFor(shape);
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();
    cQueryContext& qc = _mesh->enterQuery();
//    getPathfindPreprocessFor(qc, i);
    {
        cCompoundContextCollisionPreprocess addTo;
        cHandle<cPathfindPreprocess> base;
        cHandle<cContextPathfindPreprocess> dynamic;
        getPathfindingPreprocessesFor(qc, i, addTo, base, dynamic);
    }
    _mesh->exitQuery(qc);
}

void
cCollisionContext::setSurfaceTypeTraverseCost(tSigned32 surfaceType, float cost)
{
    INTERFACE_CHECK(surfaceType >= 0, "iCollisionContext::setSurfaceTypeTraverseCost() - invalid surface type.");
    INTERFACE_CHECK(cost >= 0.f || cost == -1.f, "iCollisionContext::setSurfaceTypeTraverseCost() - cost must be == -1.f or >= 0.f.");
    {
        cMutexLock lock(_internalMutex);
        _surfaceTypeCosts->setCost(surfaceType, cost);
    }
    invalidatePathfindingPreprocess();
}
void
cCollisionContext::setSurfaceTypeTraverseCostDirection(tSigned32 surfaceType, tSigned32 directionVectorX, tSigned32 directionVectorY)
{
    INTERFACE_CHECK(directionVectorX >= -DIRECTION_VECTOR_RANGE && directionVectorX <= DIRECTION_VECTOR_RANGE, "iCollisionContext::setSurfaceTypeTraverseCostDirection() called with vector x component outside allowed range.");
    INTERFACE_CHECK(directionVectorY >= -DIRECTION_VECTOR_RANGE && directionVectorY <= DIRECTION_VECTOR_RANGE, "iCollisionContext::setSurfaceTypeTraverseCostDirection() called with vector y component outside allowed range.");
    INTERFACE_CHECK(surfaceType >= 0, "iCollisionContext::setSurfaceTypeTraverseCostDirection() - invalid surface type.");
    {
        cMutexLock lock(_internalMutex);
        _surfaceTypeCosts->setCostDirection(surfaceType, tPoint(directionVectorX, directionVectorY, DIRECTION_VECTOR_RANGE));
    }
    invalidatePathfindingPreprocess();
}

void
cCollisionContext::setQueryBounds(const cHorizontalRange& bounds)
{
    INTERFACE_CHECK(bounds.maxX >= bounds.minX, "iCollisionContext::setQueryBounds() - endX must be >= startX.");
    INTERFACE_CHECK(bounds.maxY >= bounds.minY, "iCollisionContext::setQueryBounds() - endY must be >= startY.");
    INTERFACE_ASSERT(bounds.minX >= -WORLD_RANGE && bounds.maxX <= WORLD_RANGE);
    INTERFACE_ASSERT(bounds.minY >= -WORLD_RANGE && bounds.maxY <= WORLD_RANGE);
    {
        cMutexLock lock(_internalMutex);
        _boundsMin = tPoint(bounds.minX, bounds.minY, WORLD_RANGE);
        _boundsMax = tPoint(bounds.maxX, bounds.maxY, WORLD_RANGE);
        _boundsValid = true;
    }
    invalidatePathfindingPreprocess();
}
void
cCollisionContext::clearQueryBounds()
{
    {
        cMutexLock lock(_internalMutex);
        _boundsValid = false;
    }
    invalidatePathfindingPreprocess();
}

//=================

void
cCollisionContext::addUnexpandedPreprocess(cQueryContext& qc, cCompoundContextCollisionPreprocess& addTo) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    tSigned32 i;
    for(i = 0; i != SizeL(_obstacleSets); ++i)
    {
        addTo.addPreprocess(_obstacleSets[i]->getUnexpandedPreprocess(qc));
    }
}

void
cCollisionContext::addCollisionPreprocessFor(cQueryContext& qc, tSigned32 index, cCompoundContextCollisionPreprocess& addTo) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    tSigned32 i;
    for(i = 0; i != SizeL(_obstacleSets); ++i)
    {
        addTo.addPreprocess(_obstacleSets[i]->getCollisionPreprocessFor(qc, index));
    }
}

void
cCollisionContext::getPathfindingPreprocessesFor(
            cQueryContext& qc, tSigned32 i,
            cCompoundContextCollisionPreprocess& addCollisionPreprocessTo,
            cHandle<cPathfindPreprocess>& basePathfindPreprocess,
            cHandle<cContextPathfindPreprocess>& dynamicPathfindPreprocess
            ) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    addCollisionPreprocessFor(qc, i, addCollisionPreprocessTo);
    if(_preprocessedSet == -1)
    {
      // base preprocess comes from the mesh
        basePathfindPreprocess = _mesh->getPathfindPreprocessFor(i);
    }
    else
    {
      // base preprocess comes from an obstacle set
        basePathfindPreprocess = _obstacleSets[_preprocessedSet]->getPathfindPreprocessFor(qc, i);
    }
    cMutexLock lock2(_preprocessMutex);
    reapObstacleSets();
    if(_preprocessValid)
    {
        if(obstacleSetStateChanged())
        {
            invalidatePathfindingPreprocess();
        }
    }

    if(_preprocessValid)
    {
        dynamicPathfindPreprocess = _pathfindPreprocess[i];
        return;
    }

    getObstacleSetStateHandles();

    _pathfindPreprocess.resize(nSingletons::pathEngine().numberOfPreprocessShapes(), 0);

    if(!_pathfindPreprocess[i].isSet())
    {
        _pathfindPreprocess[i] = new cContextPathfindPreprocess(qc);
    }
    else if(_pathfindPreprocess[i]->hasMoreThanOneRef())
    {
    // we can't reuse the existing preprocess object because it is still in use by another thread
        _pathfindPreprocess[i] = new cContextPathfindPreprocess(qc);
    }
    else
    {

    }

    cHandle<cCollisionPreprocess> cp = _mesh->getCollisionPreprocessFor(i);
    cHandle<cPathfindPreprocess> pp = _mesh->getPathfindPreprocessFor(i);

    _pathfindPreprocess[i]->clear();

    cMutexLock lock3(_internalMutex);

    if(_boundsValid)
    {
        _pathfindPreprocess[i]->setQueryBounds(_boundsMin, _boundsMax);
    }

    tSigned32 j;
    for(j = 0; j != _obstacleSets.size(); ++j)
    {
        if(j == _preprocessedSet)
        {
         // this set does not contribute to dynamic pathfinding state
            continue;
        }
        _obstacleSets[j]->addAgentsToPathfindPreprocess(qc, i, *cp, *pp, _boundsValid, _boundsMin, _boundsMax, *_pathfindPreprocess[i]);
    }

    if(!_surfaceTypeCosts->empty())
    {
        iMovementCostCalculator* costCalculator;
        cCornerProvider cornersToAdd(&_mesh->ref2D());
        _surfaceTypeCosts->buildCornersAndCostCalculator(_mesh->ref3D(), _mesh->refTranslation(), costCalculator, cornersToAdd);
        _pathfindPreprocess[i]->setMesh3DCostCalculator(costCalculator);
        _pathfindPreprocess[i]->addMesh3DSoftCornersAndFinalise(cornersToAdd);
    }
    else
    {
        _pathfindPreprocess[i]->finalise();
    }

    dynamicPathfindPreprocess = _pathfindPreprocess[i];
}

bool
cCollisionContext::surfaceTypeCostsEmpty() const
{
    return _surfaceTypeCosts->empty();
}
cSurfaceTypeCosts*
cCollisionContext::cloneSurfaceTypeCosts() const
{
    return new cSurfaceTypeCosts(*_surfaceTypeCosts);
}

