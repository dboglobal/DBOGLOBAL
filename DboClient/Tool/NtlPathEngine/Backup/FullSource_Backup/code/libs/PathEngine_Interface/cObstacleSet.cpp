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
#include "interface/cObstacleSet.h"
#include "interface/cAgent.h"
#include "interface/cPathEngine.h"
#include "interface/cShape.h"
#include "interface/Singletons.h"
#include "libs/PathEngine_Core/interface/ContextCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/ContextPathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/CompoundContextCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h"
#include "libs/PathEngine_Core/interface/ManagedSoftObstacleExpansions.h"
#include "libs/PathEngine_Core/interface/QueryContext.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/MutexLock.h"
#include "common/interface/Error.h"
#include "common/interface/Attributes.h"
#include "common/Containers/SwapOut.h"
#include "platform_common/CoordinateRanges.h"
#include "iAPIObjectWrapper.h"


void
cObstacleSet::collisionAndPathfindingStateModified()
{
    cMutexLock lock(_preprocessMutex);
    size_t i;
    if(_unexpandedPreprocess.isSet())
    {
        _unexpandedPreprocess->setInvalid();
    }
    for(i = 0; i < _collisionPreprocess.size(); i++)
    {
        if(_collisionPreprocess[i].isSet())
        {
            _collisionPreprocess[i]->setInvalid();
        }
    }
    _pathfindingStateTracker.notifyStateModified();
    _pathfindPreprocess.clear();
}

void
cObstacleSet::pathfindingStateOnlyModified()
{
    cMutexLock lock(_preprocessMutex);
    _pathfindingStateTracker.notifyStateModified();    
    _pathfindPreprocess.clear();
}

cObstacleSet::cObstacleSet(cMesh* mesh, const char *const* attributes) :
 _unexpandedPreprocess(static_cast<cContextCollisionPreprocess*>(0)),
 _collisionPreprocess(nSingletons::pathEngine().numberOfPreprocessShapes(), static_cast<cContextCollisionPreprocess*>(0)),
 _pathfindPreprocess(nSingletons::pathEngine().numberOfPreprocessShapes(), static_cast<cPathfindPreprocess*>(0))
{
    _wrapper = 0;
    _mesh = mesh;
    _indexInMesh = -1;
    _markedForPreprocessing = ValueAsBoolWithDefault(attributes, "markForPreprocessing", false);
}
void
cObstacleSet::setIndexInMesh(tSigned32 value)
{
    assertD(_indexInMesh == -1);
    _indexInMesh = value;
    _hasBeenDestroyed = false;
}

cObstacleSet::~cObstacleSet()
{
    assertD(_hasBeenDestroyed);

}

// iObstacleSet

void cObstacleSet::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}

    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());

// collision contexts are always held by their mesh
    INTERFACE_ASSERT(_indexInMesh != -1);

// empty all held objects before removing from parent
    while(!_agentVector.empty())
    {
        removeAgent(_agentVector.back());
    }

    _hasBeenDestroyed = true;
    _mesh->notifyObstacleSetDestroyed(_indexInMesh); // mesh held handle will be released here, and delete may therefore be triggered during this call
}

void
cObstacleSet::addAgent(iAgent *agent)
{
    INTERFACE_CHECK(!includes(agent), "iCollisionContext::addAgent() - agent is already in context");
    _mesh->lockAgentsAndObstacleSets();
    cAgent* a = static_cast<cAgent*>(agent);
    a->notifyAddedToObstacleSet(_indexInMesh, SizeL(_agentVector));
    _agentVector.push_back(a);
    _mesh->unlockAgentsAndObstacleSets();

  // the following can be done outside of the above mutex protection because preprocess readers don't actually access the agent vector directly
  // (so it's ok for preprocess state to be different from agents in sets state momentarily)
    collisionAndPathfindingStateModified();
}

void
cObstacleSet::removeAgent(iAgent *agent)
{
    INTERFACE_CHECK(includes(agent), "iCollisionContext::removeAgent() - agent is not in context");
    _mesh->lockAgentsAndObstacleSets();
    cAgent* a = static_cast<cAgent*>(agent);
    tSigned32 indexInAgentVector = a->notifyRemovedFromObstacleSet(_indexInMesh);
    if(SwapOut(_agentVector, indexInAgentVector))
    {
        // the swapped down agent needs to update it's index in this set
        _agentVector[indexInAgentVector]->notifyIndexInObstacleSetChanged(_indexInMesh, indexInAgentVector);
    }
    _mesh->unlockAgentsAndObstacleSets();

  // the following can be done outside of the above mutex protection because preprocess readers don't actually access the agent vector directly
  // (so it's ok for preprocess state to be different from agents in sets state momentarily)
    collisionAndPathfindingStateModified();
}

tSigned32
cObstacleSet::getNumberOfAgents() const
{
    return size();
}

bool cObstacleSet::includes(iAgent *agent) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    cAgent* a = static_cast<cAgent*>(agent);
    bool result = a->isInObstacleSet(_indexInMesh);
    return result;
}

void cObstacleSet::updateCollisionPreprocessFor(iShape *shape) const
{
    cShape *cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();
    cQueryContext& qc = _mesh->enterQuery();
    getCollisionPreprocessFor(qc, i);
    _mesh->exitQuery(qc);
}

void
cObstacleSet::updatePathfindingPreprocessFor(iShape* shape) const
{
    INTERFACE_ASSERT(_mesh->shapeCanPathfind(shape));
    cShape *cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();
    cQueryContext& qc = _mesh->enterQuery();
    getCollisionPreprocessFor(qc, i);
    getPathfindPreprocessFor(qc, i);
    _mesh->exitQuery(qc);
}

iAgent*
cObstacleSet::getAgent(tSigned32 index) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    iAgent* result = _agentVector[index];
    return result;
}
tSigned32
cObstacleSet::size() const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    tSigned32 result = SizeL(_agentVector);
    return result;
}

bool
cObstacleSet::empty() const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    bool result = _agentVector.empty();
    return result;
}

//==============

cHandle<cContextCollisionPreprocess>
cObstacleSet::getUnexpandedPreprocess(cQueryContext& qc)
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    {
        cMutexLock lock(_preprocessMutex);
        if(!_unexpandedPreprocess.isSet())
        {
            _unexpandedPreprocess = new cContextCollisionPreprocess(_mesh->ref2D().faces());
        }
        if(_unexpandedPreprocess->isValid())
        {
            return _unexpandedPreprocess;
        }

        if(_unexpandedPreprocess->hasMoreThanOneRef())
        {
        // we can't reuse the existing preprocess object because it is still in use by another thread
            _unexpandedPreprocess = new cContextCollisionPreprocess(_mesh->ref2D().faces());
        }

        _unexpandedPreprocess->clear();
        tSigned32 j;
        for(j = 0; j < SizeL(_agentVector); ++j)
        {
            if(!_agentVector[j]->isSoftObstacle())
            {
                _unexpandedPreprocess->push_back(_agentVector[j], _agentVector[j]->getUnexpandedPreprocess(qc));
            }
        }
        _unexpandedPreprocess->setCompleted();
        _unexpandedPreprocess->setValid();
        return _unexpandedPreprocess;
    }
}
cHandle<cContextCollisionPreprocess>
cObstacleSet::getCollisionPreprocessFor(cQueryContext& qc, tSigned32 i) const
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    {
        cMutexLock lock(_preprocessMutex);
        _collisionPreprocess.resize(nSingletons::pathEngine().numberOfPreprocessShapes(), 0);

        if(!_collisionPreprocess[i].isSet())
        {
            _collisionPreprocess[i] = new cContextCollisionPreprocess(_mesh->ref2D().faces());
        }
        if(_collisionPreprocess[i]->isValid())
        {
            return _collisionPreprocess[i];
        }

        if(_collisionPreprocess[i]->hasMoreThanOneRef())
        {
        // we can't reuse the existing preprocess object because it is still in use by another thread
            _collisionPreprocess[i] = new cContextCollisionPreprocess(_mesh->ref2D().faces());
        }

        {
            _collisionPreprocess[i]->clear();
            tSigned32 j;
            tSigned32 size = SizeL(_agentVector);
            for(j = 0; j < size; ++j)
            {
                cAgent* agent = _agentVector[j];
                if(!agent->isSoftObstacle())
                {
                    _collisionPreprocess[i]->push_back(agent, agent->getCollisionPreprocess(qc, i));
                }
            }
            _collisionPreprocess[i]->setCompleted();
        }
        _collisionPreprocess[i]->setValid();
        return _collisionPreprocess[i];
    }
}

cHandle<cPathfindPreprocess>
cObstacleSet::getPathfindPreprocessFor(cQueryContext& qc, tSigned32 i) const
{
    INTERFACE_ASSERT(_mesh->shapeCanPathfind(i));
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    {
        cMutexLock lock(_preprocessMutex);
        _pathfindPreprocess.resize(nSingletons::pathEngine().numberOfPreprocessShapes(), 0);

        if(_pathfindPreprocess[i].isSet())
        {
            return _pathfindPreprocess[i];
        }

        cHandle<cManagedShapeSet> baseObstacleExpansions;
        baseObstacleExpansions = new cManagedShapeSet(qc.getNumberOfInternalFaces());

        const std::vector<cObstacleInfo>& baseObstacles = _mesh->refBaseObstacles();
        const std::vector<cConvexPoly>& baseObstacleShapes = _mesh->refBaseObstacleShapes();
        const cConvexPoly& expandShape = nSingletons::pathEngine().refPreprocessShape(i);

        tSigned32 j;
        for(j = 0; j != SizeL(baseObstacles); ++j)
        {
            const cObstacleInfo& info = baseObstacles[j];
            if(info._costToTraverse != -1.f)
            {
                continue;
            }
            cConvexPoly placedShape;
            baseObstacleShapes[j].offset(info._position.p, placedShape);
            cConvexPoly combinedShape;
            placedShape.minkowskiExpand(expandShape, combinedShape);
            cLocalisedConvexShape* lcs = new cLocalisedConvexShape(qc, combinedShape, info._position);
            baseObstacleExpansions->push_back(lcs);
        }
        for(j = 0; j < SizeL(_agentVector); ++j)
        {
            cAgent* agent = _agentVector[j];
            if(agent->isSoftObstacle())
            {
                continue;
            }
            cHandle<cShapeCollisionPreprocess> scp = agent->getCollisionPreprocess(qc, i);
            baseObstacleExpansions->push_back(scp);
        }
        baseObstacleExpansions->setCompleted();
        
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions = new cManagedSoftObstacleExpansions();
        softObstacleExpansions->add(qc, baseObstacles, baseObstacleShapes, expandShape);
        for(j = 0; j < SizeL(_agentVector); ++j)
        {
            cAgent* agent = _agentVector[j];
            if(!agent->isSoftObstacle())
            {
                continue;
            }
            cHandle<cShapeCollisionPreprocess> scp = agent->getCollisionPreprocess(qc, i);
            tSigned32 x, y;
            agent->getTraverseCostDirection(x, y);
            tPoint directionForCostToTraverse(x, y, DIRECTION_VECTOR_RANGE);
            softObstacleExpansions->add(qc, scp, agent->getTraverseCost(), directionForCostToTraverse);
        }
        softObstacleExpansions->setCompleted();

        _pathfindPreprocess[i] = new cPathfindPreprocess(
                qc,
                _mesh->refCollisionPreprocess(i).getEdgeExpansion(),
                baseObstacleExpansions,
                _mesh->refCollisionPreprocess(i).getAttributes(),
                _mesh->refOffMeshConnections(),
                softObstacleExpansions,
                *_mesh->getMesh3DCorners(),
                _mesh->refMesh3DCostCalculators(),
                0, false, false  // no attributes, dont keep partitioned mesh
                );

        return _pathfindPreprocess[i];
    }
}

void
cObstacleSet::addAgentsToPathfindPreprocess(
        cQueryContext& qc, 
        tSigned32 expansionShapeIndex,
        const cCollisionPreprocess& cp, const cPathfindPreprocess& pp,
        bool boundsValid,
        const tPoint& boundsMin, const tPoint& boundsMax,
        cContextPathfindPreprocess& addTo
        )
{
    tSigned32 i;
    for(i = 0; i < SizeL(_agentVector); ++i)
    {
        if(boundsValid && !_agentVector[i]->expansionPotentiallyOverlapsBounds(expansionShapeIndex, boundsMin, boundsMax))
        {
            continue;
        }
        if(_agentVector[i]->isSoftObstacle())
        {
            addTo.addAgentExpansion_SoftObstacle(
                qc, cp,
                _agentVector[i]->getCollisionPreprocess(qc, expansionShapeIndex),
                _agentVector[i]->getCostToTraverse(),
                _agentVector[i]->getDirectionForCostToTraverse()
                );
        }
        else
        {
            addTo.addAgentExpansion(
                qc, cp, pp,
                _agentVector[i]->getCollisionPreprocess(qc, expansionShapeIndex),
                _agentVector[i]->getPathfindPreprocess(qc, pp, expansionShapeIndex)
                );
        }
    }
}

void
cObstacleSet::notifyAgentChangeAffectingCollisionAndPathfindingState()
{
    collisionAndPathfindingStateModified();
}
void
cObstacleSet::notifyAgentChangeAffectingPathfindingStateOnly()
{
    pathfindingStateOnlyModified();
}

void
cObstacleSet::addUnexpandedPreprocess(cQueryContext& qc, cCompoundContextCollisionPreprocess& addTo)
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    addTo.addPreprocess(getUnexpandedPreprocess(qc));
}
void
cObstacleSet::addCollisionPreprocessFor(cQueryContext& qc, tSigned32 index, cCompoundContextCollisionPreprocess& addTo)
{
    cMutexLock lock(_mesh->refAgentsAndObstacleSetsMutex());
    addTo.addPreprocess(getCollisionPreprocessFor(qc, index));
}


tUnsigned32
cObstacleSet::getPathfindingStateHandle()
{
    cMutexLock lock(_preprocessMutex);
    return _pathfindingStateTracker.getStateHandle();
}
bool
cObstacleSet::pathfindingStateHasBeenModifiedSince(tUnsigned32 stateHandle) const
{
    return _pathfindingStateTracker.hasBeenModifiedSince(stateHandle);
}
