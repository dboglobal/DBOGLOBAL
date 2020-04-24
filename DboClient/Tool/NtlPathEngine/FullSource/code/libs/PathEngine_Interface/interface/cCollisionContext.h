//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_COLLISION_CONTEXT_INCLUDED
#define C_COLLISION_CONTEXT_INCLUDED

#include "i_pathengine.h"
#include "libs/Geometry/interface/tPoint.h"
#include "common/Handle.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "platform_common/Mutex.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class cQueryContext;
class cMesh;
class cObstacleSet;
class cAgent;
class cCollisionPreprocess;
class cPathfindPreprocess;
class cContextCollisionPreprocess;
class cContextPathfindPreprocess;
class cSurfaceTypeCosts;
class cCompoundContextCollisionPreprocess;
class iAPIObjectWrapper;

class cCollisionContext : public iCollisionContext
{
  // immutable
    cMesh* _mesh;
    tSigned32 _indexInMesh;

  // protected by _mesh->_agentsAndContextsMutex
    mutable std::vector<cHandle<cObstacleSet> > _obstacleSets;
    mutable tSigned32 _preprocessedSet;

    cMutex _internalMutex;
    cSurfaceTypeCosts* _surfaceTypeCosts;
    tPoint _boundsMin, _boundsMax;
    bool _boundsValid;

    cMutex _preprocessMutex;
    mutable bool _preprocessValid; // when this is false, _pathfindPreprocess should all be zero
    mutable std::vector<cHandle<cContextPathfindPreprocess> > _pathfindPreprocess; // per preprocess shape
    mutable std::vector<tUnsigned32> _obstacleSetStateHandles; // per obstacle set

    void invalidatePathfindingPreprocess() const;

    // must be called under _mesh->_agentsAndObstacleSetsMutex
    void doSwapOutObstacleSet(tSigned32 i) const;
    void reapObstacleSets() const;

    // must be called under _mesh->_agentsAndObstacleSetsMutex and _preprocessMutex
    void getObstacleSetStateHandles() const;
    bool obstacleSetStateChanged() const;

public:

    iAPIObjectWrapper* _wrapper;

    cCollisionContext(cMesh* mesh);
    void setIndexInMesh(tSigned32 value); // must be called post construction

    ~cCollisionContext();

// iCollisionContext

    void destroy();
    void addAgent(iAgent* agent);
    void removeAgent(iAgent* agent);
    tSigned32 getNumberOfAgents() const;
    iAgent* getAgent(tSigned32 index) const;
    bool includes(iAgent* agent) const;
    void addObstacleSet(iObstacleSet* context);
    void removeObstacleSet(iObstacleSet* context);
    tSigned32 getNumberOfObstacleSets() const;
    iObstacleSet* getObstacleSet(tSigned32 index) const;
    void updateCollisionPreprocessFor(iShape* shape) const;
    void updatePathfindingPreprocessFor(iShape* shape) const;
    void setSurfaceTypeTraverseCost(tSigned32 surfaceType, float cost);
    void setSurfaceTypeTraverseCostDirection(tSigned32 surfaceType, tSigned32 directionVectorX, tSigned32 directionVectorY);
    void setQueryBounds(const cHorizontalRange& bounds);
    void clearQueryBounds();
    void operator delete(void* p) {::operator delete(p);}

//=================

    void addUnexpandedPreprocess(cQueryContext& qc, cCompoundContextCollisionPreprocess& addTo) const;
    void addCollisionPreprocessFor(cQueryContext& qc, tSigned32 i, cCompoundContextCollisionPreprocess& addTo) const;

    void
    getPathfindingPreprocessesFor(
            cQueryContext& qc, tSigned32 i,
            cCompoundContextCollisionPreprocess& addCollisionPreprocessTo,
            cHandle<cPathfindPreprocess>& basePathfindPreprocess,
            cHandle<cContextPathfindPreprocess>& dynamicPathfindPreprocess
            ) const;

    cMesh& refMesh()
    {
        return* _mesh;
    }

    bool surfaceTypeCostsEmpty() const;
    cSurfaceTypeCosts* cloneSurfaceTypeCosts() const;
};

inline cCollisionContext* FromI(iCollisionContext* ptr)
{
    return static_cast<cCollisionContext*>(ptr);
}

#endif
